# TemperatureMonitorESP32

Uses an ESP32 to monitor temperature and humidity in the room, all datas stored in database.
Automatically, the temperature and humidity are refreshed every minute. However, if you send a command via the serial monitor, the data will refresh immediately as well.

If you are using Arduino IDE you need change to other branch "CodeOneFolder", IDE don't support multiples files.

## What do i need?

- ESP 32
- DHT 22 or DHT 11
- display oled
- protoboard
- Wifi

## How does it work??

1. you need configure your board, i'm using ESP32 Dev Module.
2. download libs ( see next topic)
3. configure your config.h using wifi password and it's name.
4. connect wires
5. upload project using .ino

## which libraries used?

- DHT sensor library
- Adafruit Unified Sensor
- Adafruit GFX Library
- Adafruit BusIO
- Adafruit SSD1306
- ArduinoHttpClient
