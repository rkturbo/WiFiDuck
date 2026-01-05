#include <Arduino.h>
#include <FS.h>
#include <LITTLEFS.h> // LittleFS for ESP32 (lorol's library)

namespace spiffs { // keep namespace name for compatibility with rest of code

void begin() {
    // Format on first mount failure
    if (!LITTLEFS.begin(true)) {
        Serial.println("LittleFS mount failed, attempting format...");
        LITTLEFS.format();
        if (!LITTLEFS.begin()) {
            Serial.println("LittleFS format+mount failed");
        } else {
            Serial.println("LittleFS formatted and mounted");
        }
    } else {
        Serial.println("LittleFS mounted");
    }
}

void format() {
    LITTLEFS.format();
}

size_t size() {
    return LITTLEFS.totalBytes();
}

size_t usedBytes() {
    return LITTLEFS.usedBytes();
}

size_t freeBytes() {
    return LITTLEFS.totalBytes() - LITTLEFS.usedBytes();
}

size_t size(const String &fileName) {
    if (!LITTLEFS.exists(fileName)) return 0;
    File f = LITTLEFS.open(fileName, FILE_READ);
    if (!f) return 0;
    size_t s = f.size();
    f.close();
    return s;
}

bool exists(const String &fileName) {
    return LITTLEFS.exists(fileName);
}

fs::File open(const String &fileName, const char *mode = "a+") {
    if (strcmp(mode, "r") == 0) return LITTLEFS.open(fileName, FILE_READ);
    if (strchr(mode, 'w')) return LITTLEFS.open(fileName, FILE_WRITE);
    // fallback: open for append/create
    return LITTLEFS.open(fileName, FILE_APPEND);
}

void create(const String &fileName) {
    File f = LITTLEFS.open(fileName, FILE_WRITE);
    if (f) f.close();
}

void remove(const String &fileName) {
    LITTLEFS.remove(fileName);
}

void rename(const String &oldName, const String &newName) {
    LITTLEFS.rename(oldName, newName);
}

String listDir(const String &dirName = "/") {
    String out;
    File root = LITTLEFS.open(dirName);
    if (!root) return out;
    if (!root.isDirectory()) {
        root.close();
        return out;
    }
    File file = root.openNextFile();
    while (file) {
        out += String(file.name()) + "," + String(file.size()) + "\n";
        file = root.openNextFile();
    }
    root.close();
    return out;
}

} // namespace spiffs
