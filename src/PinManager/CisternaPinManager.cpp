#include "CisternaPinManager.hpp"
#include <Arduino.h>

CisternaPinManager::CisternaPinManager(int releBombaPin, CisternaMonitor *monitor)
{
    this->releBombaPin = releBombaPin;
    this->monitor = monitor;

    pinMode(releBombaPin, OUTPUT);
};

void CisternaPinManager::atualizaPinoDoRele()
{
    digitalWrite(releBombaPin, monitor->getEstadoBomba() ? HIGH : LOW);
};
