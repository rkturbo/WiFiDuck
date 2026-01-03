/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "duckscript.h"

#include "config.h"
#include "debug.h"

#include "com.h"
#include "spiffs.h"

#include <limits.h>  // For INT_MAX

namespace duckscript {
    // ===== PRIVATE ===== //
    File f;

    char * prevMessage    { NULL };
    size_t prevMessageLen { 0 };

    // Line history for multi-line REPEAT
    #define MAX_HISTORY_LINES 256
    struct LineHistory {
        char* line;
        size_t len;
    };
    LineHistory lineHistory[MAX_HISTORY_LINES];
    int historyCount = 0;
    int historyIndex = 0;

    // State for ESP-side multi-line REPEAT
    bool inEspRepeat = false;
    int espRepeatTimes = 0;
    int espRepeatLines = 0;
    int espRepeatCurrentTime = 0;
    int espRepeatCurrentLine = 0;

    bool running { false };

    // Helper function to add line to history
    void addToHistory(const char* buf, size_t len) {
        // Free the oldest line if we're at capacity
        if (lineHistory[historyIndex].line) {
            free(lineHistory[historyIndex].line);
            lineHistory[historyIndex].line = NULL;
        }
        
        // Store the new line
        lineHistory[historyIndex].line = (char*)malloc(len + 1);
        if (lineHistory[historyIndex].line) {
            memcpy(lineHistory[historyIndex].line, buf, len);
            lineHistory[historyIndex].line[len] = '\0';
            lineHistory[historyIndex].len = len;
            
            // Move to next slot in circular buffer
            historyIndex = (historyIndex + 1) % MAX_HISTORY_LINES;
            if (historyCount < MAX_HISTORY_LINES) historyCount++;
        } else {
            // Malloc failed - log and continue without storing
            debugln("Warning: Failed to allocate memory for line history");
        }
    }

    // Helper function to parse REPEAT command arguments
    // Returns: 0 = not a REPEAT, 1 = single-arg REPEAT, 2 = two-arg REPEAT
    // Sets times and lines output parameters
    int parseRepeatCommand(const char* buf, size_t len, int* times, int* lines) {
        if (len < 6 || strncmp(buf, "REPEAT", 6) != 0) return 0;
        
        // Skip "REPEAT" and any spaces
        size_t i = 6;
        while (i < len && (buf[i] == ' ' || buf[i] == '\t')) i++;
        
        if (i >= len || buf[i] == '\n' || buf[i] == '\r') {
            // No arguments
            return 0;
        }
        
        // Parse first argument (times) with overflow protection
        *times = 0;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            int digit = buf[i] - '0';
            // Check for overflow before multiplication
            if (*times > (INT_MAX - digit) / 10) {
                // Overflow would occur, cap at reasonable maximum
                *times = 1000;  // Cap at 1000 repetitions
                break;
            }
            *times = *times * 10 + digit;
            i++;
        }
        
        // Skip spaces
        while (i < len && (buf[i] == ' ' || buf[i] == '\t')) i++;
        
        // Check if there's a second argument
        if (i >= len || buf[i] == '\n' || buf[i] == '\r') {
            // Single argument
            return 1;
        }
        
        // Parse second argument (lines) with overflow protection
        *lines = 0;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            int digit = buf[i] - '0';
            // Check for overflow before multiplication
            if (*lines > (INT_MAX - digit) / 10) {
                // Overflow would occur, cap at MAX_HISTORY_LINES
                *lines = MAX_HISTORY_LINES;
                break;
            }
            *lines = *lines * 10 + digit;
            i++;
        }
        
        // Two arguments
        return 2;
    }

    // Helper function to send next line in ESP-side repeat sequence
    void sendNextEspRepeatLine() {
        if (!inEspRepeat) return;
        
        // Check if we've completed all repetitions
        if (espRepeatCurrentTime >= espRepeatTimes) {
            inEspRepeat = false;
            espRepeatTimes = 0;
            espRepeatLines = 0;
            espRepeatCurrentTime = 0;
            espRepeatCurrentLine = 0;
            debugln("ESP-side REPEAT completed");
            nextLine();  // Continue with next line from file
            return;
        }
        
        // Additional safety check - make sure we still have enough history
        if (espRepeatLines > historyCount) {
            debugln("ESP-side REPEAT aborted - insufficient history");
            inEspRepeat = false;
            nextLine();
            return;
        }
        
        // Send the next line in the current repetition
        // Calculate the actual index correctly based on history size
        int actualStart;
        if (historyCount < MAX_HISTORY_LINES) {
            // History not yet full, start from beginning
            actualStart = historyCount - espRepeatLines;
            // Safety check for negative index
            if (actualStart < 0) actualStart = 0;
        } else {
            // History is full, use circular buffer calculation
            actualStart = (historyIndex - espRepeatLines + MAX_HISTORY_LINES) % MAX_HISTORY_LINES;
        }
        
        int idx;
        if (historyCount < MAX_HISTORY_LINES) {
            idx = actualStart + espRepeatCurrentLine;
        } else {
            idx = (actualStart + espRepeatCurrentLine) % MAX_HISTORY_LINES;
        }
        
        if (idx >= 0 && idx < MAX_HISTORY_LINES && lineHistory[idx].line) {
            debugf("ESP REPEAT [%d/%d] line [%d/%d]\n", 
                   espRepeatCurrentTime + 1, espRepeatTimes, 
                   espRepeatCurrentLine + 1, espRepeatLines);
            com::send(lineHistory[idx].line, lineHistory[idx].len);
        }
        
        // Move to next line
        espRepeatCurrentLine++;
        
        // If we've sent all lines in this repetition, move to next repetition
        if (espRepeatCurrentLine >= espRepeatLines) {
            espRepeatCurrentLine = 0;
            espRepeatCurrentTime++;
        }
    }

    // ===== PUBLIC ===== //
    void run(String fileName) {
        if (fileName.length() > 0) {
            debugf("Run file %s\n", fileName.c_str());
            f       = spiffs::open(fileName);
            running = true;
            nextLine();
        }
    }

    void nextLine() {
        if (!running) return;
        
        // If we're in the middle of an ESP-side repeat, continue that
        if (inEspRepeat) {
            sendNextEspRepeatLine();
            return;
        }

        if (!f) {
            debugln("File error");
            stopAll();
            return;
        }

        if (!f.available()) {
            debugln("Reached end of file");
            stopAll();
            return;
        }

        char buf[BUFFER_SIZE];
        unsigned int buf_i =  0;
        bool eol           =  false; // End of line

        while (f.available() && !eol && buf_i < BUFFER_SIZE) {
            uint8_t b = f.peek();

            //utf8
            if((b & 0x80) == 0x80) {
                uint8_t extra_chars = 0;
            
                if((b & 0xC0) == 0xC0) {
                    extra_chars = 2;
                } else if((b & 0xE0) == 0xC0) {
                    extra_chars = 3;
                } else if((b & 0xF0) == 0xC0) {
                    extra_chars = 4;
                }

                // utf8 char doesn't fit into buffer
                if ((buf_i + extra_chars) > BUFFER_SIZE) break;
            }
            
            eol        = (b == '\n');
            buf[buf_i] = f.read();
            ++buf_i;
            // debug(char(b));
        }

        if (!eol) debugln();

        // Parse REPEAT command
        int times = 0, lines = 0;
        int repeatType = parseRepeatCommand(buf, buf_i, &times, &lines);
        
        if (repeatType == 2) {
            // Two-argument REPEAT: handle on ESP side
            debugf("ESP-side REPEAT %d lines %d times\n", lines, times);
            
            // Validate arguments
            if (lines > 0 && times > 0 && lines <= historyCount) {
                // Start ESP-side repeat sequence
                inEspRepeat = true;
                espRepeatTimes = times;
                espRepeatLines = lines;
                espRepeatCurrentTime = 0;
                espRepeatCurrentLine = 0;
                
                // Send the first line
                sendNextEspRepeatLine();
            } else {
                // Invalid arguments, skip this command and continue
                debugln("Invalid ESP-side REPEAT arguments");
                nextLine();
            }
        } else {
            // Not a two-arg REPEAT or single-arg REPEAT
            // Store in history (excluding REPEAT commands)
            if (strncmp((char*)buf, "REPEAT", _min(buf_i, 6)) != 0) {
                addToHistory(buf, buf_i);
                
                // Also keep prevMessage for backward compatibility with single-arg REPEAT
                if (prevMessage) free(prevMessage);
                prevMessageLen = buf_i;
                prevMessage    = (char*)malloc(prevMessageLen + 1);
                if (prevMessage) {
                    memcpy(prevMessage, buf, buf_i);
                    prevMessage[buf_i] = '\0';
                } else {
                    debugln("Warning: Failed to allocate memory for prevMessage");
                    prevMessageLen = 0;
                }
            }
            
            // Send the command to ATmega
            com::send(buf, buf_i);
        }
    }

    void repeat() {
        if (!prevMessage) {
            stopAll();
        } else {
            debugln("Repeating last message");
            com::send(prevMessage, prevMessageLen);
        }
    }

    void stopAll() {
        if (running) {
            if (f) f.close();
            running = false;
            debugln("Stopped script");
        }
        
        // Reset ESP-side repeat state
        inEspRepeat = false;
        espRepeatTimes = 0;
        espRepeatLines = 0;
        espRepeatCurrentTime = 0;
        espRepeatCurrentLine = 0;
        
        // Free line history
        for (int i = 0; i < MAX_HISTORY_LINES; i++) {
            if (lineHistory[i].line) {
                free(lineHistory[i].line);
                lineHistory[i].line = NULL;
                lineHistory[i].len = 0;
            }
        }
        historyCount = 0;
        historyIndex = 0;
    }

    void stop(String fileName) {
        if (fileName.length() == 0) stopAll();
        else {
            if (running && f && (fileName == currentScript())) {
                f.close();
                running = false;
                debugln("Stopped script");
            }
        }
    }

    bool isRunning() {
        return running;
    }

    String currentScript() {
        if (!running) return String();
        return String(f.name());
    }
}