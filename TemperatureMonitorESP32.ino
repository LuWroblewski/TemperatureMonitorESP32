#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "display.h"
#include "sensor.h"
#include "wificonnect.h"
#include "apiRequest.h"

String command = "";
unsigned long lastTime = 0;
const unsigned long interval = 60000; // 60 seconds

float temperature = 0.0;
float humidity = 0.0;

void setup()
{
    Serial.begin(9600);
    setupDisplay();
    setupSensor();
    setupWiFi();
}

void loop()
{
    if (Serial.available() > 0)
    {
        command = Serial.readString();
        command.trim();
    }

    unsigned long currentTime = millis();
    unsigned long timeRemaining = interval - (currentTime - lastTime);
    unsigned long secondsRemaining = timeRemaining / 1000;

    digitalWrite(2, HIGH);

    if (command.equals("temperatura") || (currentTime - lastTime >= interval))
    {
        digitalWrite(2, LOW);

        temperature = readTemperature();
        humidity = readHumidity();

        if (isnan(temperature) || isnan(humidity))
        {
            Serial.println("Failed to read from DHT22 sensor");
        }
        else
        {
            Serial.print("Temperature ");
            Serial.print(temperature);
            Serial.println("\xF8C");

            Serial.print("Humidity ");
            Serial.print(humidity);

            lastTime = currentTime;
        }
        sendDataToServer(temperature, humidity);

        command = "";
    }

    updateDisplay(temperature, humidity, secondsRemaining);
}
