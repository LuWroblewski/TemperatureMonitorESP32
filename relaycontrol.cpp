#include <Arduino.h>
#include "relaycontrol.h"

const int relayPin = 5;

void setupRelay()
{
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
}

void activateRelay()
{
    digitalWrite(relayPin, HIGH);
    delay(30000);
    digitalWrite(relayPin, LOW);
}
