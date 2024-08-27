#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

String ordem = "";
unsigned long ultimoTempo = 0;
const unsigned long intervalo = 60000; // 60 seconds

float temperatura = 0.0;
float umidade = 0.0;

void setup()
{
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    dht.begin();

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

void loop()
{
    if (Serial.available() > 0)
    {
        ordem = Serial.readString();
        ordem.trim();
    }

    unsigned long tempoAtual = millis();
    unsigned long tempoRestante = intervalo - (tempoAtual - ultimoTempo);
    unsigned long segundosRestantes = tempoRestante / 1000;

    digitalWrite(2, HIGH);

    if (ordem.equals("temperatura") || (tempoAtual - ultimoTempo >= intervalo))
    {
        digitalWrite(2, LOW);

        temperatura = dht.readTemperature();
        umidade = dht.readHumidity();

        if (isnan(temperatura) || isnan(umidade))
        {
            Serial.println("Falha ao ler do sensor DHT22");
        }
        else
        {
            Serial.print("Temperatura ");
            Serial.print(temperatura);
            Serial.println("\xF8C");

            Serial.print("Umidade ");
            Serial.print(umidade);

            ultimoTempo = tempoAtual;
        }

        ordem = "";
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
    display.print(temperatura);

    display.setCursor(65, 17);
    display.print(umidade);

    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("Prox. verif.: ");
    display.print(segundosRestantes);
    display.println(" seg");

    display.display();
}