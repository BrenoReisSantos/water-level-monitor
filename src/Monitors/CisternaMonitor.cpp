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

    trava = false;

    bombaLigada = false;
};

void CisternaMonitor::controlaBomba(bool ligar)
{
    bombaLigada = ligar;
};

void CisternaMonitor::gerenciaBomba()
{
    if (!client->checaVidaDaCaixa())
        bombaLigada = false;

    EstadoDoNivelDeAgua estadoDaCaixa = client->checaEstadoDaCaixa();
    if (estadoDaCaixa == Cheio || estado == Vazio || trava)
        bombaLigada = false;
};
