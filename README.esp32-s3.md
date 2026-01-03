# ESP32-S3 WiFi Duck - Build and Flash Guide

This guide explains how to build and flash the ESP32-S3 firmware for WiFi Duck using the Arduino IDE.

## Hardware Requirements

- **ESP32-S3** development board (ESP32-S3 DevKit, ESP32-S3-WROOM, or similar)
- **ATmega32u4** board (Arduino Leonardo, Pro Micro, etc.) - unchanged from original WiFi Duck
- Jumper wires for connections
- USB cables for both boards

## Pin Connections

Connect the ESP32-S3 to the ATmega32u4 as follows:

| ESP32-S3 Pin | ATmega32u4 Pin | Notes |
|--------------|----------------|-------|
| GPIO 8 (SDA) | Pin 2 (SDA)    | I2C Data |
| GPIO 9 (SCL) | Pin 3 (SCL)    | I2C Clock |
| GND          | GND            | Ground |
| 5V or VIN    | RAW, 5V, or VIN | Power (optional, if powering ESP32-S3 from ATmega32u4) |

> **Note:** GPIO 8 and GPIO 9 are the default I2C pins for many ESP32-S3 boards. If your board uses different pins, update `I2C_SDA` and `I2C_SCL` in `config.h`.

## Arduino IDE Setup

### 1. Install Arduino IDE

Download and install the [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.19 or later, or Arduino IDE 2.x).

### 2. Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. In the "Additional Board Manager URLs" field, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools** → **Board** → **Boards Manager**
6. Search for "esp32"
7. Install **"esp32 by Espressif Systems"** (version 2.0.11 or later recommended)
8. Click **Close**

### 3. Install Required Libraries

Go to **Sketch** → **Include Library** → **Manage Libraries** and install:

- **ESPAsyncWebServer** (by me-no-dev) - Search for "ESPAsyncWebServer"
  - This may also require **AsyncTCP** (automatically installed with ESPAsyncWebServer for ESP32)
- **SimpleCLI** (by Spacehuhn) - Search for "SimpleCLI"

> **Note:** You may need to manually install these libraries from GitHub if they're not available in the Library Manager:
> - ESPAsyncWebServer: https://github.com/me-no-dev/ESPAsyncWebServer
> - AsyncTCP: https://github.com/me-no-dev/AsyncTCP

### 4. Open the ESP32-S3 Firmware

1. Navigate to the `esp_duck/esp32-s3/` folder in this repository
2. Open `esp32-s3.ino` in Arduino IDE

### 5. Configure Board Settings

1. Go to **Tools** → **Board** → **ESP32 Arduino** → **ESP32S3 Dev Module**
   (or select your specific ESP32-S3 board if listed)

2. Configure the following settings under **Tools**:
   - **USB CDC On Boot**: "Enabled" (for Serial debugging)
   - **Flash Mode**: "QIO" (or "DIO" if QIO doesn't work)
   - **Flash Size**: Select based on your board (e.g., "4MB", "8MB", "16MB")
   - **Partition Scheme**: "Default 4MB with spiffs" (or similar with SPIFFS support)
   - **PSRAM**: "Disabled" (or "QSPI" if your board has PSRAM and you want to use it)
   - **Upload Speed**: "921600" (or lower if upload fails)
   - **Core Debug Level**: "None" (or "Info" for debugging)

3. Select your **Port** under **Tools** → **Port**

### 6. Compile and Upload

1. Click the **Verify** button (checkmark) to compile the code
2. If compilation succeeds, click the **Upload** button (right arrow) to flash the firmware
3. Wait for the upload to complete

> **Troubleshooting:** If upload fails, press and hold the **BOOT** button on the ESP32-S3 board while clicking upload, then release when "Connecting..." appears.

## Flashing the ATmega32u4

The ATmega32u4 firmware is **unchanged** from the original WiFi Duck. Follow the standard flashing procedure:

1. Open `atmega_duck/atmega_duck.ino` in Arduino IDE
2. Select your ATmega32u4 board under **Tools** → **Board** (e.g., "Arduino Leonardo" or "Sparkfun Pro Micro")
3. Select the correct **Port**
4. Click **Upload**

For detailed instructions, see the main [README.md](../README.md#flash-atmega32u4).

## Configuration (Optional)

You can customize the following settings in `esp_duck/esp32-s3/config.h`:

- **I2C Pins**: Change `I2C_SDA` and `I2C_SCL` if your board uses different pins
- **WiFi Settings**: Change default SSID/password (`WIFI_SSID`, `WIFI_PASSWORD`)
- **Debug**: Uncomment `#define ENABLE_DEBUG` to enable serial debugging
- **Communication**: Switch between I2C and Serial by uncommenting `#define ENABLE_I2C` or `#define ENABLE_SERIAL`

## Smoke Test Checklist

After building and flashing both firmwares, perform these tests:

### 1. Power On and Boot
- [ ] Connect both boards via USB
- [ ] ESP32-S3 powers on and shows activity on built-in LED (if present)
- [ ] ATmega32u4 is recognized as a USB keyboard by your computer

### 2. WiFi Access Point
- [ ] ESP32-S3 creates WiFi network named "wifiduck" (or your configured SSID)
- [ ] You can connect to the WiFi network with password "wifiduck" (or your configured password)

### 3. Web Interface
- [ ] Open browser and navigate to `http://192.168.4.1`
- [ ] Web interface loads successfully
- [ ] You can see the file list and terminal

### 4. Serial Communication (Optional)
If you enabled serial debugging:
- [ ] Open Serial Monitor at 115200 baud (or your configured `DEBUG_BAUD`)
- [ ] You see startup message: `[~~~ WiFi Duck v1.2.0-esp32s3 Started! ~~~]`
- [ ] You see I2C or Serial connection status: `"I2C Connection OK"` or `"Serial Connection OK"`

### 5. ESP ↔ ATmega Communication
- [ ] In Serial Monitor, check for `MSG_STARTED` sent to ATmega
- [ ] LED on ATmega should turn green (if LED is configured)
- [ ] Serial Monitor shows successful communication status

### 6. Script Execution - Basic Test
- [ ] In the web interface, create a new script named `test.txt`
- [ ] Enter the following script:
  ```
  REM Test script
  DELAY 1000
  STRING Hello World
  ENTER
  ```
- [ ] Save the script
- [ ] Open a text editor (Notepad, TextEdit, etc.) on your computer
- [ ] Click "Run" on the `test.txt` script in the web interface
- [ ] Verify that "Hello World" is typed into the text editor

### 7. REPEAT Command - Single Argument (Existing Behavior)
- [ ] Create a script:
  ```
  STRING A
  REPEAT 4
  ```
- [ ] Run the script
- [ ] Verify output: `AAAAA` (original line + 4 repeats = 5 A's total)

### 8. File Management
- [ ] Create a file via web interface
- [ ] Edit the file content
- [ ] Delete the file
- [ ] Verify all operations work correctly

### 9. Settings
- [ ] Open Settings in web interface
- [ ] Change SSID (optional)
- [ ] Change password (optional)
- [ ] Verify settings are saved after reboot

## Troubleshooting

### ESP32-S3 doesn't create WiFi network
- Check Serial Monitor for error messages
- Verify ESP32-S3 board is powered
- Try resetting the ESP32-S3
- Check if `SPIFFS.begin()` succeeded in Serial Monitor

### No communication between ESP32-S3 and ATmega32u4
- Verify I2C connections (SDA to SDA, SCL to SCL, GND to GND)
- Check I2C pin configuration in `config.h` matches your board
- Ensure both boards share a common ground
- Check Serial Monitor for I2C errors
- Try using Serial communication instead (update `config.h`)

### Upload fails
- Press and hold BOOT button during upload
- Lower upload speed in Tools menu
- Check USB cable and port
- Try a different USB port

### Web interface doesn't load
- Verify you're connected to the WiFi Duck network
- Try accessing `http://192.168.4.1` directly
- Clear browser cache
- Check Serial Monitor for webserver startup messages

### Scripts don't execute
- Verify ATmega32u4 firmware is flashed correctly
- Check communication status in Serial Monitor
- Ensure script syntax is correct (Ducky Script format)
- Try a simpler script first (just `STRING test`)

## Next Steps

Once the basic functionality is verified:

1. **Test more complex scripts**: Try scripts with multiple commands, loops, and delays
2. **Test STRINGLN and STRING_DELAY_RANDOM**: If these features were ported
3. **Test CLI commands**: Use the terminal in web interface to test commands like `ls`, `run`, `settings`
4. **OTA Updates**: Test over-the-air firmware updates via web interface or Arduino IDE
5. **Customize**: Modify `config.h` for your specific use case

## Known Limitations / Future Work

- **Token protocol**: Not yet implemented for ESP32-S3 (planned for future PRs)
- **Extended testing**: More extensive runtime testing needed
- **Performance**: I2C communication performance may differ from ESP8266
- **Power consumption**: ESP32-S3 power usage not yet optimized

## Contributing

If you encounter issues or have improvements, please open an issue or pull request on the GitHub repository.

## License

This software is licensed under the MIT License. See the [LICENSE](../LICENSE) file for details.
