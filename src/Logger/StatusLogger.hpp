#include <SD.h>
#include "../Monitors/EstadoDoNivelDeAgua.hpp"

class StatusLogger
{
public:
    StatusLogger();
    void logStatus(
        int nivelDeAguaDoReservatorio,
        EstadoDoNivelDeAgua estado,
        int ano,
        int mes,
        int dia,
        int hora,
        int minuto,
        int segundo);
};
