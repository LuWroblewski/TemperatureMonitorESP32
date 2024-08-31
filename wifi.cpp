#include <Arduino.h>
#include <WiFi.h>
#include "wificonnect.h"
#include "config.h"

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
