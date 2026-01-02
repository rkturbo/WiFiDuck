# HOLD/RELEASE and MEDIA/GLOBE Support

This document describes the new HOLD/RELEASE commands and Consumer HID (media key) support for WiFiDuck ATmega target.

## Overview

The WiFiDuck now supports:
1. **HOLD/RELEASE commands** - Allow holding keys down without automatic release
2. **MEDIA commands** - Consumer HID keys for media playback and system controls
3. **GLOBE command** - Globe/Function key support

All Consumer HID features are gated behind the `ENABLE_CONSUMER` compile-time flag.

## Enabling Consumer HID Support

To enable Consumer HID (MEDIA/GLOBE) support:

1. Edit `atmega_duck/config.h`
2. Uncomment the line:
   ```c
   #define ENABLE_CONSUMER
   ```
3. Recompile and upload the firmware

## HOLD Command

The `HOLD` command presses keys without releasing them automatically. This is useful for:
- Holding modifier keys (Ctrl, Shift, Alt, GUI)
- Creating custom key combinations
- Multi-step selections

### Syntax
```
HOLD <key>
```

### Examples
```
HOLD CTRL
STRING c
RELEASE

HOLD SHIFT
HOLD CTRL
RIGHT
RIGHT
RIGHT
RELEASE
```

## RELEASE Command

The `RELEASE` command releases all currently held keys.

### Syntax
```
RELEASE
```

### Examples
```
HOLD ALT
TAB
TAB
RELEASE
```

## MEDIA Command

The `MEDIA` command sends Consumer HID control codes for media playback, volume control, and application launching.

**Note:** Requires `ENABLE_CONSUMER` to be defined in config.h

### Syntax
```
MEDIA <action>
```

### Supported Actions

#### Playback Controls
- `PLAY` or `PLAYPAUSE` - Play/Pause toggle
- `PAUSE` - Pause playback
- `STOP` - Stop playback
- `NEXT` or `NEXTTRACK` - Next track
- `PREV`, `PREVIOUS`, or `PREVIOUSTRACK` - Previous track
- `FASTFORWARD` or `FF` - Fast forward
- `REWIND` - Rewind
- `RECORD` - Start recording
- `EJECT` - Eject media

#### Volume Controls
- `VOLUMEUP` or `VOLUP` - Increase volume
- `VOLUMEDOWN` or `VOLDOWN` - Decrease volume
- `MUTE` - Toggle mute

#### Application Launchers
- `CALCULATOR` or `CALC` - Open calculator
- `EMAIL` - Open email client
- `BROWSER` or `WWW` - Open web browser

#### Browser Controls
- `SEARCH` - Browser search
- `HOME` - Browser home
- `BACK` - Browser back
- `FORWARD` - Browser forward
- `REFRESH` - Browser refresh
- `BOOKMARKS` - Browser bookmarks

#### System Controls
- `SLEEP` - Put system to sleep

### Examples
```
REM Increase volume twice
MEDIA VOLUMEUP
MEDIA VOLUMEUP

REM Open calculator
MEDIA CALCULATOR

REM Control media playback
MEDIA PLAY
DELAY 5000
MEDIA PAUSE
```

## GLOBE Command

The `GLOBE` command presses the Globe/Function key (similar to Windows/GUI key on some systems).

**Note:** Requires `ENABLE_CONSUMER` to be defined in config.h

### Syntax
```
GLOBE
```

### Examples
```
REM Press Globe key alone
GLOBE

REM Globe key combination
HOLD GLOBE
STRING a
RELEASE
```

## Complete Example Scripts

### Example 1: Copy and Paste with HOLD/RELEASE
```
REM Select all text and copy
HOLD CTRL
STRING a
RELEASE
DELAY 100
HOLD CTRL
STRING c
RELEASE
```

### Example 2: Media Player Control
```
REM Control media playback
MEDIA PLAY
DELAY 5000
MEDIA VOLUMEUP
MEDIA VOLUMEUP
DELAY 2000
MEDIA NEXT
```

### Example 3: Window Switching with HOLD
```
REM Switch between windows
HOLD ALT
TAB
DELAY 200
TAB
DELAY 200
RELEASE
```

## Implementation Details

### Consumer HID Descriptor

When `ENABLE_CONSUMER` is enabled, a Consumer Control HID descriptor is added with Report ID 3. This descriptor supports Consumer usage page (0x0C) with usage IDs up to 0x023C (AC Format).

### Key Codes

Consumer HID usage IDs are defined in `usb_hid_keys.h` with the prefix `CONSUMER_`. For example:
- `CONSUMER_PLAY_PAUSE` (0x00CD)
- `CONSUMER_VOLUME_INCREMENT` (0x00E9)
- `CONSUMER_MUTE` (0x00E2)

### Backward Compatibility

The HOLD/RELEASE commands are always available and work with the standard keyboard HID descriptor.

The MEDIA and GLOBE commands are only compiled when `ENABLE_CONSUMER` is defined, maintaining backward compatibility with systems that don't need Consumer HID support.

## Testing

Test scripts are provided:
- `test_hold_release.script` - Tests HOLD and RELEASE functionality
- `test_media_globe.script` - Tests MEDIA and GLOBE commands (requires ENABLE_CONSUMER)

## Troubleshooting

### MEDIA commands not working
1. Ensure `ENABLE_CONSUMER` is defined in `config.h`
2. Recompile and upload the firmware
3. Some systems may not support all Consumer HID codes

### GLOBE key not working
The Globe/Function key may behave differently on different operating systems:
- macOS: May act as Fn key
- Windows: May act as Windows key
- Linux: Behavior depends on desktop environment

### Keys stuck after HOLD
Always ensure you call `RELEASE` after using `HOLD`. If keys get stuck:
1. Press and release the stuck key manually
2. Fix your script to include proper `RELEASE` calls
