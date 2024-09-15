#include "CisternaMonitor.hpp"
#include "../Clients/CisternaClient.hpp"

CisternaMonitor::CisternaMonitor(float alturaDoSensor, float alturaQuandoCheio, float alturaQuandoVazio, CisternaClient *client)
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

    bombaLigada = false;
};

void CisternaMonitor::controlaBomba(bool ligar)
{
    bombaLigada = ligar;
};

void CisternaMonitor::gerenciaReservatorio()
{
    if (!bombaLigada)
        return;

    if (!client->checaVidaDaCaixa())
        bombaLigada = false;

    EstadoDoNivelDeAgua estadoDaCaixa = client->checaEstadoDaCaixa();
    if (estadoDaCaixa == Cheio || estado == Vazio || trava)
        bombaLigada = false;
};

bool CisternaMonitor::getEstadoBomba()
{
    return bombaLigada;
};

std::string CisternaMonitor::toString()
{
    char texto[200];

    sprintf(texto, "| BOMBA LIGADA: %s", bombaLigada ? "true" : "false");
    return WaterMonitor::toString() + texto;
};
