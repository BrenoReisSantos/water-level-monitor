#include "UltrassonicSensor.hpp"

UltrassonicSensor::UltrassonicSensor(int triggerPin, int echoPin)
{
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
};

float UltrassonicSensor::medeDistanciaEmCentimetros()
{
    // Clears the trigPin condition
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(triggerPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    return (float)duration * 0.034f / (float)2;
};
