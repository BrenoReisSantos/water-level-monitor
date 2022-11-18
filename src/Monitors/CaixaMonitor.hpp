#include <Arduino.h>
#include "WaterMonitor.hpp"
#include "../Clients/CaixaClient.hpp"

class CaixaMonitor : WaterMonitor
{
    CaixaClient *client;

public:
    CaixaMonitor(float alturaDoSensor, float alturaQuandoCheio, float alturaQuandoVazio, CaixaClient *client);
    void gerenciaNivelDaCaixa();
};
