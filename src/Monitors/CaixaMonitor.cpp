#include "CaixaMonitor.hpp"

CaixaMonitor::CaixaMonitor(float alturaDoSensor, float alturaQuandoCheio, float alturaQuandoVazio, CaixaClient *client)
{

    this->client = client;

    this->alturaDoSensor = alturaDoSensor;
    this->alturaQuandoCheio = alturaQuandoCheio;
    this->alturaQuandoVazio = alturaQuandoVazio;

    porcentagemDoNivelDeAgua = 100;
    estado = Cheio;

    porcentagemQuandoCheio = (int)((alturaQuandoCheio / alturaDoSensor) * 100);
    porcentagemQuandoVazio = (int)((alturaQuandoVazio / alturaDoSensor) * 100);

    levelQueue = new MonitorLevelQueue(30);

    trava = false;
};

void CaixaMonitor::gerenciaReservatorio()
{
    if (estado == Vazio)
        client->controlaBomba(true);
};

std::string CaixaMonitor::toString()
{
    return WaterMonitor::toString();
};
