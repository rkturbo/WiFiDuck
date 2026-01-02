/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "keyboard.h"
#include "debug.h"

#ifdef ENABLE_CONSUMER
#include "consumer_usages.h"
#endif

#ifdef ENABLE_MOUSE
#include "mouse.h"
#endif

namespace keyboard {
    // ====== PRIVATE ====== //
    hid_locale_t* locale      { &locale_us };
    report prev_report = report { KEY_NONE, KEY_NONE, { KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE } };

    const uint8_t keyboardDescriptor[] PROGMEM {
        //  Keyboard
        0x05, 0x01, //   USAGE_PAGE (Generic Desktop)  // 47
        0x09, 0x06, //   USAGE (Keyboard)
        0xa1, 0x01, //   COLLECTION (Application)
        0x85, 0x02, //   REPORT_ID (2)
        0x05, 0x07, //   USAGE_PAGE (Keyboard)

        0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x01, //   LOGICAL_MAXIMUM (1)
        0x75, 0x01, //   REPORT_SIZE (1)

        0x95, 0x08, //   REPORT_COUNT (8)
        0x81, 0x02, //   INPUT (Data,Var,Abs)
        0x95, 0x01, //   REPORT_COUNT (1)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x81, 0x03, //   INPUT (Cnst,Var,Abs)

        0x95, 0x06, //   REPORT_COUNT (6)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x73, //   LOGICAL_MAXIMUM (115)
        0x05, 0x07, //   USAGE_PAGE (Keyboard)

        0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x73, //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00, //   INPUT (Data,Ary,Abs)
        0xc0,       //   END_COLLECTION
    };

#ifdef ENABLE_CONSUMER
    const uint8_t consumerDescriptor[] PROGMEM {
        // Consumer Control
        0x05, 0x0C,        // Usage Page (Consumer)
        0x09, 0x01,        // Usage (Consumer Control)
        0xA1, 0x01,        // Collection (Application)
        0x85, 0x03,        //   Report ID (3)
        0x19, 0x00,        //   Usage Minimum (0)
        0x2A, 0x3C, 0x02,  //   Usage Maximum (0x023C)
        0x15, 0x00,        //   Logical Minimum (0)
        0x26, 0x3C, 0x02,  //   Logical Maximum (0x023C)
        0x75, 0x10,        //   Report Size (16)
        0x95, 0x01,        //   Report Count (1)
        0x81, 0x00,        //   Input (Data,Array,Abs)
        0xC0               // End Collection
    };
#endif

    report makeReport(uint8_t modifiers = 0, uint8_t key1 = 0, uint8_t key2 = 0, uint8_t key3 = 0, uint8_t key4 = 0, uint8_t key5 = 0, uint8_t key6 = 0);

    report makeReport(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
        report k;

        k.modifiers = modifiers;

        k.reserved = 0x00;

        k.keys[0] = key1;
        k.keys[1] = key2;
        k.keys[2] = key3;
        k.keys[3] = key4;
        k.keys[4] = key5;
        k.keys[5] = key6;

        return k;
    }

    // ====== PUBLIC ====== //
    void begin() {
        static HIDSubDescriptor node(keyboardDescriptor, sizeof(keyboardDescriptor));

        HID().AppendDescriptor(&node);

#ifdef ENABLE_CONSUMER
        static HIDSubDescriptor consumerNode(consumerDescriptor, sizeof(consumerDescriptor));
        HID().AppendDescriptor(&consumerNode);
#endif

#ifdef ENABLE_MOUSE
        mouse::mouse_begin();
#endif
    }

    void setLocale(hid_locale_t* locale) {
        keyboard::locale = locale;
    }

    void send(report* k) {
#ifdef ENABLE_DEBUG
        debug("Sending Report [");
        for (uint8_t i = 0; i<6; ++i) {
            debug(String(prev_report.keys[i], HEX));
            debug(",");
        }
        debug("#");
        debug(String(prev_report.modifiers, HEX));
        debugln("]");
#endif // ENABLE_DEBUG
        HID().SendReport(2, (uint8_t*)k, sizeof(report));
    }

    void release() {
        prev_report = makeReport();
        send(&prev_report);
    }

    void pressKey(uint8_t key, uint8_t modifiers) {
        for (uint8_t i = 0; i<6; ++i) {
            if (prev_report.keys[i] == KEY_NONE) {
                prev_report.modifiers |= modifiers;
                prev_report.keys[i]    = key;
                send(&prev_report);
                return;
            }
        }
    }

    void pressModifier(uint8_t key) {
        prev_report.modifiers |= key;

        send(&prev_report);
    }

    uint8_t press(const char* strPtr) {
        // Convert string pointer into a byte pointer
        uint8_t* b = (uint8_t*)strPtr;

        // Key combinations (accent keys)
        // We have to check them first, because sometimes ASCII keys are in here
        for (uint8_t i = 0; i<locale->combinations_len; ++i) {
            uint8_t res = 0;

            // Read utf8 code and match it with the given data
            for (uint8_t j = 0; j<4; ++j) {
                uint8_t key_code = pgm_read_byte(locale->combinations + (i * 8) + j);

                if (key_code == 0) {
                    break;
                }

                if (key_code == b[j]) {
                    ++res;
                } else {
                    res = 0;
                    break;
                }
            }

            // If a match was found, read out the data and type it
            if (res > 0) {
                uint8_t comboModifiers = pgm_read_byte(locale->combinations + (i * 8) + 4);
                uint8_t comboKey       = pgm_read_byte(locale->combinations + (i * 8) + 5);

                uint8_t modifiers = pgm_read_byte(locale->combinations + (i * 8) + 6);
                uint8_t key       = pgm_read_byte(locale->combinations + (i * 8) + 7);

                pressKey(comboKey, comboModifiers);
                release();
                pressKey(key, modifiers);
                release();

                // Return the number of extra bytes we used from the string pointer
                return res-1;
            }
        }

        // ASCII
        if (b[0] < locale->ascii_len) {
            uint8_t modifiers = pgm_read_byte(locale->ascii + (b[0] * 2) + 0);
            uint8_t key       = pgm_read_byte(locale->ascii + (b[0] * 2) + 1);

            pressKey(key, modifiers);

            return 0;
        }
        
        // UTF8
        for (size_t i = 0; i<locale->utf8_len; ++i) {
            uint8_t res = 0;

            // Read utf8 code and match it with the given data
            for (uint8_t j = 0; j<4; ++j) {
                uint8_t key_code = pgm_read_byte(locale->utf8 + (i * 6) + j);

                if (key_code == 0) {
                    break;
                }

                if (key_code == b[j]) {
                    ++res;
                } else {
                    res = 0;
                    break;
                }
            }

            // If a match was found, read out the data and type it
            if (res > 0) {
                uint8_t modifiers = pgm_read_byte(locale->utf8 + (i * 6) + 4);
                uint8_t key       = pgm_read_byte(locale->utf8 + (i * 6) + 5);

                pressKey(key, modifiers);

                // Return the number of extra bytes we used from the string pointer
                return res-1;
            }
        }

        return 0;
    }

    uint8_t write(const char* c) {
        uint8_t res = press(c);

        release();

        return res;
    }

    void write(const char* str, size_t len) {
        for (size_t i = 0; i<len; ++i) {
            i += write(&str[i]);
        }
    }

    void releaseKey(uint8_t key) {
        bool changed = false;
        
        // Find and remove the key from the report
        for (uint8_t i = 0; i < 6; ++i) {
            if (prev_report.keys[i] == key) {
                // Shift remaining keys down to compact the array
                for (uint8_t j = i; j < 5; ++j) {
                    prev_report.keys[j] = prev_report.keys[j + 1];
                }
                prev_report.keys[5] = KEY_NONE;
                changed = true;
                break;
            }
        }
        
        // Only send report if we actually removed a key
        if (changed) {
            send(&prev_report);
        }
    }

    void releaseModifier(uint8_t modifier) {
        uint8_t old_modifiers = prev_report.modifiers;
        prev_report.modifiers &= ~modifier;
        
        // Only send report if modifiers actually changed
        if (old_modifiers != prev_report.modifiers) {
            send(&prev_report);
        }
    }

#ifdef ENABLE_CONSUMER
    void sendConsumer(uint16_t usage) {
        uint8_t report[2];
        report[0] = usage & 0xFF;         // Low byte
        report[1] = (usage >> 8) & 0xFF;  // High byte
        
        HID().SendReport(3, report, 2);
    }

    void consumerClick(uint16_t usage) {
        sendConsumer(usage);
        // Send a release report (0x0000)
        sendConsumer(0x0000);
    }
#endif
}
