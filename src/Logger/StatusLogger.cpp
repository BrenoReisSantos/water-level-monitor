#include "StatusLogger.hpp"

StatusLogger::StatusLogger(){};

void StatusLogger::logStatus(
    int nivelDeAguaDoReservatorio,
    EstadoDoNivelDeAgua estado,
    int ano,
    int mes,
    int dia,
    int hora,
    int minuto,
    int segundo)
{
    SD.begin();
};
