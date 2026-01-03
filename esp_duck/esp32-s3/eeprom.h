#pragma once

#include <Preferences.h>
#include "config.h"

namespace eeprom {
    void begin();
    void end();

    bool checkBootNum();
    void resetBootNum();

    template<typename T>
    void saveObject(const char* key, const T& t) {
        Preferences prefs;
        prefs.begin(PREFERENCES_NAMESPACE, false);
        prefs.putBytes(key, &t, sizeof(T));
        prefs.end();
    }

    template<typename T>
    void getObject(const char* key, T& t) {
        Preferences prefs;
        prefs.begin(PREFERENCES_NAMESPACE, true);
        prefs.getBytes(key, &t, sizeof(T));
        prefs.end();
    }
}
