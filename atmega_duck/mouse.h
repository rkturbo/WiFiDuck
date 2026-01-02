/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

// Mouse button constants
#define MOUSE_LEFT    0x01
#define MOUSE_RIGHT   0x02
#define MOUSE_MIDDLE  0x04

namespace mouse {
    // Initialize mouse HID functionality
    void mouse_begin();
    
    // Move mouse cursor (dx, dy are signed 8-bit values: -128 to 127)
    void mouse_move(int8_t dx, int8_t dy);
    
    // Scroll mouse wheel (distance is signed 8-bit: -128 to 127)
    void mouse_scroll(int8_t distance);
    
    // Press a mouse button (use MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
    void mouse_press(uint8_t button);
    
    // Release a mouse button (use MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE)
    void mouse_release(uint8_t button);
    
    // Click a mouse button (press and release)
    void mouse_click(uint8_t button);
}

#endif // MOUSE_H
