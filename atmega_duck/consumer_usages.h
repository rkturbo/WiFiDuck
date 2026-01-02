/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#ifndef CONSUMER_USAGES_H
#define CONSUMER_USAGES_H

// Consumer Control Usage IDs (HID Usage Tables 1.12, Section 15)
// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf

// Consumer Control Page (0x0C)
#define HID_CONSUMER_POWER              0x0030
#define HID_CONSUMER_SLEEP              0x0032
#define HID_CONSUMER_MENU               0x0040

// Media Control
#define HID_CONSUMER_PLAY_PAUSE         0x00CD
#define HID_CONSUMER_STOP               0x00B7
#define HID_CONSUMER_NEXT_TRACK         0x00B5
#define HID_CONSUMER_PREV_TRACK         0x00B6
#define HID_CONSUMER_EJECT              0x00B8

// Volume Control
#define HID_CONSUMER_VOLUME_UP          0x00E9
#define HID_CONSUMER_VOLUME_DOWN        0x00EA
#define HID_CONSUMER_MUTE               0x00E2

// Application Launch
#define HID_CONSUMER_EMAIL              0x018A
#define HID_CONSUMER_CALCULATOR         0x0192
#define HID_CONSUMER_BROWSER            0x0194
#define HID_CONSUMER_SEARCH             0x0221
#define HID_CONSUMER_HOME               0x0223

// System Control
#define HID_CONSUMER_REBOOT             0x00B9  // Note: may not be supported on all systems

// Fn/Globe key (macOS-specific, placeholder usage)
// This usage may vary by implementation. macOS uses a vendor-specific usage for the Globe key.
// For general compatibility, we use a placeholder. Actual HID usage may require vendor-specific descriptor.
#define HID_CONSUMER_FN_GLOBE           0x0000  // Placeholder - actual usage is vendor-specific

#endif // CONSUMER_USAGES_H
