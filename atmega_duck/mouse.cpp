/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "mouse.h"

#ifdef ENABLE_MOUSE

#include <HID.h>

namespace mouse {
    // ====== PRIVATE ====== //
    
    // Current button state
    static uint8_t current_buttons = 0;
    
    // Mouse HID descriptor (Report ID 0x04)
    const uint8_t mouseDescriptor[] PROGMEM = {
        // Mouse
        0x05, 0x01,        // Usage Page (Generic Desktop)
        0x09, 0x02,        // Usage (Mouse)
        0xa1, 0x01,        // Collection (Application)
        0x09, 0x01,        //   Usage (Pointer)
        0xa1, 0x00,        //   Collection (Physical)
        0x85, 0x04,        //     Report ID (4)
        0x05, 0x09,        //     Usage Page (Button)
        0x19, 0x01,        //     Usage Minimum (Button 1)
        0x29, 0x03,        //     Usage Maximum (Button 3)
        0x15, 0x00,        //     Logical Minimum (0)
        0x25, 0x01,        //     Logical Maximum (1)
        0x95, 0x03,        //     Report Count (3)
        0x75, 0x01,        //     Report Size (1)
        0x81, 0x02,        //     Input (Data,Var,Abs)
        0x95, 0x01,        //     Report Count (1)
        0x75, 0x05,        //     Report Size (5)
        0x81, 0x03,        //     Input (Const,Var,Abs) - padding
        0x05, 0x01,        //     Usage Page (Generic Desktop)
        0x09, 0x30,        //     Usage (X)
        0x09, 0x31,        //     Usage (Y)
        0x09, 0x38,        //     Usage (Wheel)
        0x15, 0x81,        //     Logical Minimum (-127)
        0x25, 0x7f,        //     Logical Maximum (127)
        0x75, 0x08,        //     Report Size (8)
        0x95, 0x03,        //     Report Count (3)
        0x81, 0x06,        //     Input (Data,Var,Rel)
        0xc0,              //   End Collection
        0xc0               // End Collection
    };
    
    // Mouse report structure
    struct mouse_report {
        uint8_t buttons;    // Button states
        int8_t  x;          // X movement
        int8_t  y;          // Y movement
        int8_t  wheel;      // Wheel movement
    };
    
    // Send mouse report to host
    void send_mouse_report(uint8_t buttons, int8_t x, int8_t y, int8_t wheel) {
        mouse_report report;
        report.buttons = buttons;
        report.x = x;
        report.y = y;
        report.wheel = wheel;
        
        HID().SendReport(4, (uint8_t*)&report, sizeof(report));
    }
    
    // ====== PUBLIC ====== //
    
    void mouse_begin() {
        static HIDSubDescriptor node(mouseDescriptor, sizeof(mouseDescriptor));
        HID().AppendDescriptor(&node);
        current_buttons = 0;
    }
    
    void mouse_move(int8_t dx, int8_t dy) {
        send_mouse_report(current_buttons, dx, dy, 0);
    }
    
    void mouse_scroll(int8_t distance) {
        send_mouse_report(current_buttons, 0, 0, distance);
    }
    
    void mouse_press(uint8_t button) {
        current_buttons |= button;
        send_mouse_report(current_buttons, 0, 0, 0);
    }
    
    void mouse_release(uint8_t button) {
        current_buttons &= ~button;
        send_mouse_report(current_buttons, 0, 0, 0);
    }
    
    void mouse_click(uint8_t button) {
        mouse_press(button);
        mouse_release(button);
    }
}

#else // !ENABLE_MOUSE

// Stub implementations when ENABLE_MOUSE is not defined
namespace mouse {
    void mouse_begin() {}
    void mouse_move(int8_t dx, int8_t dy) {}
    void mouse_scroll(int8_t distance) {}
    void mouse_press(uint8_t button) {}
    void mouse_release(uint8_t button) {}
    void mouse_click(uint8_t button) {}
}

#endif // ENABLE_MOUSE
