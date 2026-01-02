/**
 * Source: https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2
 *
 * USB HID Keyboard scan codes as per USB spec 1.11
 * plus some additional codes
 *
 * Created by MightyPork, 2016
 * Public domain
 *
 * Adapted from:
 * https://source.android.com/devices/input/keyboard-devices.html
 */

#ifndef USB_HID_KEYS
#define USB_HID_KEYS

/**
 * Modifier masks - used for the first byte in the HID report.
 * NOTE: The second byte in the report is reserved, 0x00
 */
#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

/**
 * Scan codes - last N slots in the HID report (usually 6).
 * 0x00 if no key pressed.
 *
 * If more than N keys are pressed, the HID reports
 * KEY_ERR_OVF in all slots to indicate this condition.
 */

#define KEY_NONE 0x00       // No key pressed
#define KEY_ERR_OVF 0x01    //  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
// 0x02 //  Keyboard POST Fail
// 0x03 //  Keyboard Error Undefined
#define KEY_A 0x04          // Keyboard a and A
#define KEY_B 0x05          // Keyboard b and B
#define KEY_C 0x06          // Keyboard c and C
#define KEY_D 0x07          // Keyboard d and D
#define KEY_E 0x08          // Keyboard e and E
#define KEY_F 0x09          // Keyboard f and F
#define KEY_G 0x0a          // Keyboard g and G
#define KEY_H 0x0b          // Keyboard h and H
#define KEY_I 0x0c          // Keyboard i and I
#define KEY_J 0x0d          // Keyboard j and J
#define KEY_K 0x0e          // Keyboard k and K
#define KEY_L 0x0f          // Keyboard l and L
#define KEY_M 0x10          // Keyboard m and M
#define KEY_N 0x11          // Keyboard n and N
#define KEY_O 0x12          // Keyboard o and O
#define KEY_P 0x13          // Keyboard p and P
#define KEY_Q 0x14          // Keyboard q and Q
#define KEY_R 0x15          // Keyboard r and R
#define KEY_S 0x16          // Keyboard s and S
#define KEY_T 0x17          // Keyboard t and T
#define KEY_U 0x18          // Keyboard u and U
#define KEY_V 0x19          // Keyboard v and V
#define KEY_W 0x1a          // Keyboard w and W
#define KEY_X 0x1b          // Keyboard x and X
#define KEY_Y 0x1c          // Keyboard y and Y
#define KEY_Z 0x1d          // Keyboard z and Z

#define KEY_1 0x1e          // Keyboard 1 and !
#define KEY_2 0x1f          // Keyboard 2 and @
#define KEY_3 0x20          // Keyboard 3 and #
#define KEY_4 0x21          // Keyboard 4 and $
#define KEY_5 0x22          // Keyboard 5 and %
#define KEY_6 0x23          // Keyboard 6 and ^
#define KEY_7 0x24          // Keyboard 7 and &
#define KEY_8 0x25          // Keyboard 8 and *
#define KEY_9 0x26          // Keyboard 9 and (
#define KEY_0 0x27          // Keyboard 0 and )

#define KEY_ENTER 0x28      // Keyboard Return (ENTER)
#define KEY_ESC 0x29        // Keyboard ESCAPE
#define KEY_BACKSPACE 0x2a  // Keyboard DELETE (Backspace)
#define KEY_TAB 0x2b        // Keyboard Tab
#define KEY_SPACE 0x2c      // Keyboard Spacebar
#define KEY_MINUS 0x2d      // Keyboard - and _
#define KEY_EQUAL 0x2e      // Keyboard = and +
#define KEY_LEFTBRACE 0x2f  // Keyboard [ and {
#define KEY_RIGHTBRACE 0x30 // Keyboard ] and }
#define KEY_BACKSLASH 0x31  // Keyboard \ and |
#define KEY_HASHTILDE 0x32  // Keyboard Non-US # and ~
#define KEY_SEMICOLON 0x33  // Keyboard ; and :
#define KEY_APOSTROPHE 0x34 // Keyboard ' and "
#define KEY_GRAVE 0x35      // Keyboard ` and ~
#define KEY_COMMA 0x36      // Keyboard , and <
#define KEY_DOT 0x37        // Keyboard . and >
#define KEY_SLASH 0x38      // Keyboard / and ?
#define KEY_CAPSLOCK 0x39   // Keyboard Caps Lock

#define KEY_F1 0x3a         // Keyboard F1
#define KEY_F2 0x3b         // Keyboard F2
#define KEY_F3 0x3c         // Keyboard F3
#define KEY_F4 0x3d         // Keyboard F4
#define KEY_F5 0x3e         // Keyboard F5
#define KEY_F6 0x3f         // Keyboard F6
#define KEY_F7 0x40         // Keyboard F7
#define KEY_F8 0x41         // Keyboard F8
#define KEY_F9 0x42         // Keyboard F9
#define KEY_F10 0x43        // Keyboard F10
#define KEY_F11 0x44        // Keyboard F11
#define KEY_F12 0x45        // Keyboard F12

#define KEY_SYSRQ 0x46      // Keyboard Print Screen
#define KEY_SCROLLLOCK 0x47 // Keyboard Scroll Lock
#define KEY_PAUSE 0x48      // Keyboard Pause
#define KEY_INSERT 0x49     // Keyboard Insert
#define KEY_HOME 0x4a       // Keyboard Home
#define KEY_PAGEUP 0x4b     // Keyboard Page Up
#define KEY_DELETE 0x4c     // Keyboard Delete Forward
#define KEY_END 0x4d        // Keyboard End
#define KEY_PAGEDOWN 0x4e   // Keyboard Page Down
#define KEY_RIGHT 0x4f      // Keyboard Right Arrow
#define KEY_LEFT 0x50       // Keyboard Left Arrow
#define KEY_DOWN 0x51       // Keyboard Down Arrow
#define KEY_UP 0x52         // Keyboard Up Arrow

#define KEY_NUMLOCK 0x53    // Keyboard Num Lock and Clear
#define KEY_KPSLASH 0x54    // Keypad /
#define KEY_KPASTERISK 0x55 // Keypad *
#define KEY_KPMINUS 0x56    // Keypad -
#define KEY_KPPLUS 0x57     // Keypad +
#define KEY_KPENTER 0x58    // Keypad ENTER
#define KEY_KP1 0x59        // Keypad 1 and End
#define KEY_KP2 0x5a        // Keypad 2 and Down Arrow
#define KEY_KP3 0x5b        // Keypad 3 and PageDn
#define KEY_KP4 0x5c        // Keypad 4 and Left Arrow
#define KEY_KP5 0x5d        // Keypad 5
#define KEY_KP6 0x5e        // Keypad 6 and Right Arrow
#define KEY_KP7 0x5f        // Keypad 7 and Home
#define KEY_KP8 0x60        // Keypad 8 and Up Arrow
#define KEY_KP9 0x61        // Keypad 9 and Page Up
#define KEY_KP0 0x62        // Keypad 0 and Insert
#define KEY_KPDOT 0x63      // Keypad . and Delete

#define KEY_102ND 0x64      // Keyboard Non-US \ and |
#define KEY_COMPOSE 0x65    // Keyboard Application
#define KEY_POWER 0x66      // Keyboard Power
#define KEY_KPEQUAL 0x67    // Keypad =

#define KEY_F13 0x68        // Keyboard F13
#define KEY_F14 0x69        // Keyboard F14
#define KEY_F15 0x6a        // Keyboard F15
#define KEY_F16 0x6b        // Keyboard F16
#define KEY_F17 0x6c        // Keyboard F17
#define KEY_F18 0x6d        // Keyboard F18
#define KEY_F19 0x6e        // Keyboard F19
#define KEY_F20 0x6f        // Keyboard F20
#define KEY_F21 0x70        // Keyboard F21
#define KEY_F22 0x71        // Keyboard F22
#define KEY_F23 0x72        // Keyboard F23
#define KEY_F24 0x73        // Keyboard F24

#define KEY_OPEN 0x74       // Keyboard Execute
#define KEY_HELP 0x75       // Keyboard Help
#define KEY_PROPS 0x76      // Keyboard Menu
#define KEY_FRONT 0x77      // Keyboard Select
#define KEY_STOP 0x78       // Keyboard Stop
#define KEY_AGAIN 0x79      // Keyboard Again
#define KEY_UNDO 0x7a       // Keyboard Undo
#define KEY_CUT 0x7b        // Keyboard Cut
#define KEY_COPY 0x7c       // Keyboard Copy
#define KEY_PASTE 0x7d      // Keyboard Paste
#define KEY_FIND 0x7e       // Keyboard Find
#define KEY_MUTE 0x7f       // Keyboard Mute
#define KEY_VOLUMEUP 0x80   // Keyboard Volume Up
#define KEY_VOLUMEDOWN 0x81 // Keyboard Volume Down
// 0x82  Keyboard Locking Caps Lock
// 0x83  Keyboard Locking Num Lock
// 0x84  Keyboard Locking Scroll Lock
#define KEY_KPCOMMA 0x85          // Keypad Comma
// 0x86  Keypad Equal Sign
#define KEY_RO 0x87               // Keyboard International1
#define KEY_KATAKANAHIRAGANA 0x88 // Keyboard International2
#define KEY_YEN 0x89              // Keyboard International3
#define KEY_HENKAN 0x8a           // Keyboard International4
#define KEY_MUHENKAN 0x8b         // Keyboard International5
#define KEY_KPJPCOMMA 0x8c        // Keyboard International6
// 0x8d  Keyboard International7
// 0x8e  Keyboard International8
// 0x8f  Keyboard International9
#define KEY_HANGEUL 0x90        // Keyboard LANG1
#define KEY_HANJA 0x91          // Keyboard LANG2
#define KEY_KATAKANA 0x92       // Keyboard LANG3
#define KEY_HIRAGANA 0x93       // Keyboard LANG4
#define KEY_ZENKAKUHANKAKU 0x94 // Keyboard LANG5
// 0x95  Keyboard LANG6
// 0x96  Keyboard LANG7
// 0x97  Keyboard LANG8
// 0x98  Keyboard LANG9
// 0x99  Keyboard Alternate Erase
// 0x9a  Keyboard SysReq/Attention
// 0x9b  Keyboard Cancel
// 0x9c  Keyboard Clear
// 0x9d  Keyboard Prior
// 0x9e  Keyboard Return
// 0x9f  Keyboard Separator
// 0xa0  Keyboard Out
// 0xa1  Keyboard Oper
// 0xa2  Keyboard Clear/Again
// 0xa3  Keyboard CrSel/Props
// 0xa4  Keyboard ExSel

// 0xb0  Keypad 00
// 0xb1  Keypad 000
// 0xb2  Thousands Separator
// 0xb3  Decimal Separator
// 0xb4  Currency Unit
// 0xb5  Currency Sub-unit
#define KEY_KPLEFTPAREN 0xb6  // Keypad (
#define KEY_KPRIGHTPAREN 0xb7 // Keypad )
// 0xb8  Keypad {
// 0xb9  Keypad }
// 0xba  Keypad Tab
// 0xbb  Keypad Backspace
// 0xbc  Keypad A
// 0xbd  Keypad B
// 0xbe  Keypad C
// 0xbf  Keypad D
// 0xc0  Keypad E
// 0xc1  Keypad F
// 0xc2  Keypad XOR
// 0xc3  Keypad ^
// 0xc4  Keypad %
// 0xc5  Keypad <
// 0xc6  Keypad >
// 0xc7  Keypad &
// 0xc8  Keypad &&
// 0xc9  Keypad |
// 0xca  Keypad ||
// 0xcb  Keypad :
// 0xcc  Keypad #
// 0xcd  Keypad Space
// 0xce  Keypad @
// 0xcf  Keypad !
// 0xd0  Keypad Memory Store
// 0xd1  Keypad Memory Recall
// 0xd2  Keypad Memory Clear
// 0xd3  Keypad Memory Add
// 0xd4  Keypad Memory Subtract
// 0xd5  Keypad Memory Multiply
// 0xd6  Keypad Memory Divide
// 0xd7  Keypad +/-
// 0xd8  Keypad Clear
// 0xd9  Keypad Clear Entry
// 0xda  Keypad Binary
// 0xdb  Keypad Octal
// 0xdc  Keypad Decimal
// 0xdd  Keypad Hexadecimal

#define KEY_LEFTCTRL 0xe0   // Keyboard Left Control
#define KEY_LEFTSHIFT 0xe1  // Keyboard Left Shift
#define KEY_LEFTALT 0xe2    // Keyboard Left Alt
#define KEY_LEFTMETA 0xe3   // Keyboard Left GUI
#define KEY_RIGHTCTRL 0xe4  // Keyboard Right Control
#define KEY_RIGHTSHIFT 0xe5 // Keyboard Right Shift
#define KEY_RIGHTALT 0xe6   // Keyboard Right Alt
#define KEY_RIGHTMETA 0xe7  // Keyboard Right GUI

#define KEY_MEDIA_PLAYPAUSE 0xe8
#define KEY_MEDIA_STOPCD 0xe9
#define KEY_MEDIA_PREVIOUSSONG 0xea
#define KEY_MEDIA_NEXTSONG 0xeb
#define KEY_MEDIA_EJECTCD 0xec
#define KEY_MEDIA_VOLUMEUP 0xed
#define KEY_MEDIA_VOLUMEDOWN 0xee
#define KEY_MEDIA_MUTE 0xef
#define KEY_MEDIA_WWW 0xf0
#define KEY_MEDIA_BACK 0xf1
#define KEY_MEDIA_FORWARD 0xf2
#define KEY_MEDIA_STOP 0xf3
#define KEY_MEDIA_FIND 0xf4
#define KEY_MEDIA_SCROLLUP 0xf5
#define KEY_MEDIA_SCROLLDOWN 0xf6
#define KEY_MEDIA_EDIT 0xf7
#define KEY_MEDIA_SLEEP 0xf8
#define KEY_MEDIA_COFFEE 0xf9
#define KEY_MEDIA_REFRESH 0xfa
#define KEY_MEDIA_CALC 0xfb

// Consumer Control Usage IDs (for Consumer HID descriptor)
// Usage page 0x0C (Consumer)
#define CONSUMER_POWER 0x0030
#define CONSUMER_SLEEP 0x0032
#define CONSUMER_MENU 0x0040
#define CONSUMER_SELECTION 0x0080
#define CONSUMER_ASSIGN_SEL 0x0081
#define CONSUMER_MODE_STEP 0x0082
#define CONSUMER_MEDIA_SELECT_COMPUTER 0x0088
#define CONSUMER_MEDIA_SELECT_TV 0x0089
#define CONSUMER_MEDIA_SELECT_WWW 0x008A
#define CONSUMER_MEDIA_SELECT_DVD 0x008B
#define CONSUMER_MEDIA_SELECT_TELEPHONE 0x008C
#define CONSUMER_MEDIA_SELECT_PROGRAM_GUIDE 0x008D
#define CONSUMER_MEDIA_SELECT_VIDEO_PHONE 0x008E
#define CONSUMER_MEDIA_SELECT_GAMES 0x008F
#define CONSUMER_MEDIA_SELECT_MESSAGES 0x0090
#define CONSUMER_MEDIA_SELECT_CD 0x0091
#define CONSUMER_MEDIA_SELECT_VCR 0x0092
#define CONSUMER_MEDIA_SELECT_TUNER 0x0093
#define CONSUMER_QUIT 0x0094
#define CONSUMER_HELP 0x0095
#define CONSUMER_MEDIA_SELECT_TAPE 0x0096
#define CONSUMER_MEDIA_SELECT_CABLE 0x0097
#define CONSUMER_MEDIA_SELECT_SATELLITE 0x0098
#define CONSUMER_MEDIA_SELECT_SECURITY 0x0099
#define CONSUMER_MEDIA_SELECT_HOME 0x009A
#define CONSUMER_MEDIA_SELECT_CALL 0x009B
#define CONSUMER_CHANNEL_INCREMENT 0x009C
#define CONSUMER_CHANNEL_DECREMENT 0x009D
#define CONSUMER_MEDIA_SELECT_SAP 0x009E
#define CONSUMER_VCR_PLUS 0x00A0
#define CONSUMER_ONCE 0x00A1
#define CONSUMER_DAILY 0x00A2
#define CONSUMER_WEEKLY 0x00A3
#define CONSUMER_MONTHLY 0x00A4
#define CONSUMER_PLAY 0x00B0
#define CONSUMER_PAUSE 0x00B1
#define CONSUMER_RECORD 0x00B2
#define CONSUMER_FAST_FORWARD 0x00B3
#define CONSUMER_REWIND 0x00B4
#define CONSUMER_SCAN_NEXT_TRACK 0x00B5
#define CONSUMER_SCAN_PREVIOUS_TRACK 0x00B6
#define CONSUMER_STOP 0x00B7
#define CONSUMER_EJECT 0x00B8
#define CONSUMER_RANDOM_PLAY 0x00B9
#define CONSUMER_SELECT_DISC 0x00BA
#define CONSUMER_ENTER_DISC 0x00BB
#define CONSUMER_REPEAT 0x00BC
#define CONSUMER_TRACKING 0x00BD
#define CONSUMER_TRACK_NORMAL 0x00BE
#define CONSUMER_SLOW_TRACKING 0x00BF
#define CONSUMER_FRAME_FORWARD 0x00C0
#define CONSUMER_FRAME_BACK 0x00C1
#define CONSUMER_MARK 0x00C2
#define CONSUMER_CLEAR_MARK 0x00C3
#define CONSUMER_REPEAT_FROM_MARK 0x00C4
#define CONSUMER_RETURN_TO_MARK 0x00C5
#define CONSUMER_SEARCH_MARK_FORWARD 0x00C6
#define CONSUMER_SEARCH_MARK_BACKWARDS 0x00C7
#define CONSUMER_COUNTER_RESET 0x00C8
#define CONSUMER_SHOW_COUNTER 0x00C9
#define CONSUMER_TRACKING_INCREMENT 0x00CA
#define CONSUMER_TRACKING_DECREMENT 0x00CB
#define CONSUMER_STOP_EJECT 0x00CC
#define CONSUMER_PLAY_PAUSE 0x00CD
#define CONSUMER_PLAY_SKIP 0x00CE
#define CONSUMER_VOICE_COMMAND 0x00CF
#define CONSUMER_VOLUME 0x00E0
#define CONSUMER_BALANCE 0x00E1
#define CONSUMER_MUTE 0x00E2
#define CONSUMER_BASS 0x00E3
#define CONSUMER_TREBLE 0x00E4
#define CONSUMER_BASS_BOOST 0x00E5
#define CONSUMER_SURROUND_MODE 0x00E6
#define CONSUMER_LOUDNESS 0x00E7
#define CONSUMER_MPX 0x00E8
#define CONSUMER_VOLUME_INCREMENT 0x00E9
#define CONSUMER_VOLUME_DECREMENT 0x00EA
#define CONSUMER_SPEED_SELECT 0x00F1
#define CONSUMER_PLAYBACK_SPEED 0x00F2
#define CONSUMER_STANDARD_PLAY 0x00F3
#define CONSUMER_LONG_PLAY 0x00F4
#define CONSUMER_EXTENDED_PLAY 0x00F5
#define CONSUMER_SLOW 0x00F6
#define CONSUMER_FAN_ENABLE 0x0100
#define CONSUMER_FAN_SPEED 0x0101
#define CONSUMER_LIGHT_ENABLE 0x0102
#define CONSUMER_LIGHT_ILLUMINATION_LEVEL 0x0103
#define CONSUMER_CLIMATE_CONTROL_ENABLE 0x0104
#define CONSUMER_ROOM_TEMPERATURE 0x0105
#define CONSUMER_SECURITY_ENABLE 0x0106
#define CONSUMER_FIRE_ALARM 0x0107
#define CONSUMER_POLICE_ALARM 0x0108
#define CONSUMER_PROXIMITY 0x0109
#define CONSUMER_MOTION 0x010A
#define CONSUMER_DURESS_ALARM 0x010B
#define CONSUMER_HOLDUP_ALARM 0x010C
#define CONSUMER_MEDICAL_ALARM 0x010D
#define CONSUMER_BALANCE_RIGHT 0x0150
#define CONSUMER_BALANCE_LEFT 0x0151
#define CONSUMER_BASS_INCREMENT 0x0152
#define CONSUMER_BASS_DECREMENT 0x0153
#define CONSUMER_TREBLE_INCREMENT 0x0154
#define CONSUMER_TREBLE_DECREMENT 0x0155
#define CONSUMER_SPEAKER_SYSTEM 0x0160
#define CONSUMER_CHANNEL_LEFT 0x0161
#define CONSUMER_CHANNEL_RIGHT 0x0162
#define CONSUMER_CHANNEL_CENTER 0x0163
#define CONSUMER_CHANNEL_FRONT 0x0164
#define CONSUMER_CHANNEL_CENTER_FRONT 0x0165
#define CONSUMER_CHANNEL_SIDE 0x0166
#define CONSUMER_CHANNEL_SURROUND 0x0167
#define CONSUMER_CHANNEL_LOW_FREQ_ENH 0x0168
#define CONSUMER_CHANNEL_TOP 0x0169
#define CONSUMER_CHANNEL_UNKNOWN 0x016A
#define CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION 0x0183
#define CONSUMER_AL_EMAIL_READER 0x018A
#define CONSUMER_AL_CALCULATOR 0x0192
#define CONSUMER_AL_LOCAL_BROWSER 0x0194
#define CONSUMER_AC_SEARCH 0x0221
#define CONSUMER_AC_GOTO 0x0222
#define CONSUMER_AC_HOME 0x0223
#define CONSUMER_AC_BACK 0x0224
#define CONSUMER_AC_FORWARD 0x0225
#define CONSUMER_AC_STOP 0x0226
#define CONSUMER_AC_REFRESH 0x0227
#define CONSUMER_AC_PREVIOUS_LINK 0x0228
#define CONSUMER_AC_NEXT_LINK 0x0229
#define CONSUMER_AC_BOOKMARKS 0x022A
#define CONSUMER_AC_HISTORY 0x022B
#define CONSUMER_AC_SUBSCRIPTIONS 0x022C
#define CONSUMER_AC_ZOOM_IN 0x022D
#define CONSUMER_AC_ZOOM_OUT 0x022E
#define CONSUMER_AC_ZOOM 0x022F
#define CONSUMER_AC_FULL_SCREEN_VIEW 0x0230
#define CONSUMER_AC_NORMAL_VIEW 0x0231
#define CONSUMER_AC_VIEW_TOGGLE 0x0232
#define CONSUMER_AC_SCROLL_UP 0x0233
#define CONSUMER_AC_SCROLL_DOWN 0x0234
#define CONSUMER_AC_SCROLL 0x0235
#define CONSUMER_AC_PAN_LEFT 0x0236
#define CONSUMER_AC_PAN_RIGHT 0x0237

#define KEY_GLOBE 0xe8  // Globe/Fn key (same as GUI on some systems)

#endif // USB_HID_KEYS