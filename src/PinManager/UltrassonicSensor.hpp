#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrassonicSensor
{
    int triggerPin;
    int echoPin;

public:
    UltrassonicSensor(int triggerPin, int echoPin);
    float medeDistanciaEmCentimetros();
};

#endif
