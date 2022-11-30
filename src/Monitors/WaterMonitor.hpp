#ifndef WATERMONITOR_H
#define WATERMONITOR_H

#include "EstadoDoNivelDeAgua.hpp"
#include "../DataStructures/MonitorLevelQueue.hpp"
#include <string>

class WaterMonitor
{
protected:
    float alturaDoSensor;
    float alturaQuandoCheio;
    float alturaQuandoVazio;

    int porcentagemQuandoVazio;
    int porcentagemQuandoCheio;

    bool trava;

    MonitorLevelQueue *levelQueue;

    int porcentagemDoNivelDeAgua;

    EstadoDoNivelDeAgua estado;

    void atualizaEstado();

public:
    void atualizaNivel(float distanciaEmCm);
    int getNivel();
    bool getTrava();
    EstadoDoNivelDeAgua getEstado();
    void controlaTrava(bool travar);
    virtual void gerenciaReservatorio();
    virtual std::string toString();
};

#endif
