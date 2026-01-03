/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#define VERSION "1.2.0-esp32s3"

/*! ===== DEBUG Settings ===== */
// #define ENABLE_DEBUG
// #define DEBUG_PORT Serial
// #define DEBUG_BAUD 115200

/*! ===== Communication Settings ===== */
// #define ENABLE_SERIAL
#define SERIAL_PORT Serial
#define SERIAL_BAUD 9600

// #define ENABLE_I2C
#define I2C_ADDR 0x31
// Default I2C pins for ESP32-S3: SDA=8, SCL=9
// Adjust based on your board variant
#define I2C_SDA 8
#define I2C_SCL 9
#define I2C_CLOCK_SPEED 100000L

#define BUFFER_SIZE 256
#define PACKET_SIZE 32

#define MSG_CONNECTED "LED 0 0 25\n"
#define MSG_STARTED "LED 0 25 0\n"

/*! ===== Preferences Settings (replaces EEPROM on ESP32) ===== */
#define PREFERENCES_NAMESPACE "wifiduck"
#define BOOT_MAGIC_NUM    1234567890

/*! ===== WiFi Settings ===== */
#define WIFI_SSID "wifiduck"
#define WIFI_PASSWORD "wifiduck"
#define WIFI_CHANNEL "1"

#define HOSTNAME "wifiduck"
#define URL "wifi.duck"

/*! ========== Safety checks ========== */
#if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL)
  #define ENABLE_I2C
  #ifndef I2C_SDA
    #define I2C_SDA 8
  #endif
  #ifndef I2C_SCL
    #define I2C_SCL 9
  #endif
#endif /* if !defined(ENABLE_I2C) || !defined(ENABLE_SERIAL) */

#if !defined(ESP32)
#error You are compiling for the wrong board! Select an ESP32-S3 board.
#endif /* ifndef ESP32 */

#if defined(ENABLE_DEBUG) && defined(ENABLE_SERIAL) && DEBUG_PORT == SERIAL_PORT
#error Using same serial port for debugging and Communication!\
    Use I2C instead or disable debug.
#endif /* if DEBUG_PORT == SERIAL_PORT */

#if defined(ENABLE_I2C) && I2C_SDA==I2C_SCL
#error SDA pin equals to SCL pin
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_I2C) */

#if defined(ENABLE_I2C) && defined(ENABLE_SERIAL) && (I2C_SDA==1 || I2C_SDA==3 || I2C_SCL==1 || I2C_SCL==3)
#error I2C pins overlap with RX and TX pins. Disable serial debugging or change the I2C pins.
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_I2C) */
