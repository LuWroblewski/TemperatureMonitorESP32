#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SSID "your_SSID"
#define PASSWORD "your_PASSWORD"

String command = "";
unsigned long lastTime = 0;
const unsigned long interval = 60000;

float temperature = 0.0;
float humidity = 0.0;

void setupWiFi()
{
    Serial.begin(115200);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print("Connecting to WiFi network");
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void sendDataToServer(float temperature, float humidity)
{
    HTTPClient http;
    http.begin("https://temperature-monitor-system.vercel.app/api/temperature");
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + "}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println("POST sent, server response: " + response);
    }
    else
    {
        Serial.println("Error sending POST, response code: " + String(httpResponseCode));
    }

    http.end();
}

void setupDisplay()
{
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
    display.print("Iniciando...");
    display.display();
    delay(2000);
}

void setup()
{
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    dht.begin();
    setupDisplay();
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

        temperature = dht.readTemperature();
        humidity = dht.readHumidity();

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
