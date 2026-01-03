#include "eeprom.h"

#include "config.h"

// Used to verify memory
typedef struct boot {
    unsigned int magic_num : 32;
    unsigned int boot_num  : 8;
} boot;

namespace eeprom {
    Preferences prefs;

    void begin() {
        // ESP32 uses Preferences instead of EEPROM
        // No initialization needed here, done per-operation
    }

    void end() {
        // No-op for ESP32 Preferences
    }

    bool checkBootNum() {
        boot b;
        
        prefs.begin(PREFERENCES_NAMESPACE, false);
        prefs.getBytes("boot", &b, sizeof(boot));

        if ((b.magic_num == BOOT_MAGIC_NUM) && (b.boot_num < 3)) {
            b.boot_num++;
            prefs.putBytes("boot", &b, sizeof(boot));
            prefs.end();
            return true;
        }
        
        prefs.end();
        return false;
    }

    void resetBootNum() {
        boot b = { BOOT_MAGIC_NUM, 1 };
        prefs.begin(PREFERENCES_NAMESPACE, false);
        prefs.putBytes("boot", &b, sizeof(boot));
        prefs.end();
    }
}
