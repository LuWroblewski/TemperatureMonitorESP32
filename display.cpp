#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay()
{
    pinMode(2, OUTPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Starting...");
    display.display();
    delay(2000);
}

void updateDisplay(float temperature, float humidity, unsigned long secondsRemaining)
{
    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("Temperatura ");
    display.setCursor(0, 8);
    display.print("em Celsius ");

    display.setCursor(70, 0);
    display.print("Umidade ");
    display.setCursor(70, 8);
    display.print("em % ");

    display.setTextSize(2);
    display.setCursor(0, 17);
    display.print(temperature);

    display.setCursor(65, 17);
    display.print(humidity);

    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("Prox. verif.: ");
    display.print(secondsRemaining);
    display.println(" seg");

    display.display();
}
