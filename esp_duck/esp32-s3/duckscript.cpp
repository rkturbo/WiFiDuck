/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "duckscript.h"

#include "config.h"
#include "debug.h"

#include "com.h"
#include "spiffs.h"

namespace duckscript {
    // ===== PRIVATE ===== //
    File f;

    char * prevMessage    { NULL };
    size_t prevMessageLen { 0 };

    bool running { false };

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

            // UTF-8 multi-byte character detection
            if((b & 0x80) == 0x80) {
                uint8_t sequence_length = 1; // Default to 1 byte (continuation byte)
            
                // 2-byte sequence: 110xxxxx
                if((b & 0xE0) == 0xC0) {
                    sequence_length = 2;
                }
                // 3-byte sequence: 1110xxxx
                else if((b & 0xF0) == 0xE0) {
                    sequence_length = 3;
                }
                // 4-byte sequence: 11110xxx
                else if((b & 0xF8) == 0xF0) {
                    sequence_length = 4;
                }

                // Check if complete UTF-8 sequence fits into buffer
                if ((buf_i + sequence_length) > BUFFER_SIZE) break;
            }
            
            eol        = (b == '\n');
            buf[buf_i] = f.read();
            ++buf_i;
            // debug(char(b));
        }

        if (!eol) debugln();

        if (strncmp((char*)buf, "REPEAT", _min(buf_i, 6)) != 0) {
            if (prevMessage) free(prevMessage);
            prevMessageLen = buf_i;
            prevMessage    = (char*)malloc(prevMessageLen + 1);
            memcpy(prevMessage, buf, buf_i);
            prevMessage[buf_i] = '\0';
        }

        com::send(buf, buf_i);
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