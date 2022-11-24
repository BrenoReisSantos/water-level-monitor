#include <Arduino.h>

class UltrassonicSensor
{
    int triggerPin;
    int echoPin;

public:
    UltrassonicSensor(int triggerPin, int echoPin);
    float medeDistanciaEmCentimetros();
};
