#ifndef CAIXAMONITOR_H
#define CAIXAMONITOR_H

#include "WaterMonitor.hpp"
#include "../Clients/CaixaClient.hpp"

class CaixaMonitor : public WaterMonitor
{
    CaixaClient *client;

public:
    CaixaMonitor(float alturaDoSensor, float alturaQuandoCheio, float alturaQuandoVazio, CaixaClient *client);
    void gerenciaReservatorio();
    std::string toString() override;
};

#endif
