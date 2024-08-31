#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "apiRequest.h"

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
