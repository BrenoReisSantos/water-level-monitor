#include "WaterMonitor.hpp"

void WaterMonitor::atualizaEstado()
{
    if (porcentagemDoNivelDeAgua <= porcentagemQuandoVazio)
        estado = Vazio;
    else if (porcentagemDoNivelDeAgua >= porcentagemQuandoCheio)
        estado = Cheio;
    else
        estado = Mediano;
};

void WaterMonitor::atualizaNivel(float distanciaEmCm)
{
    int porcentagemDoNivelDeAguaAux = (int)((1 - (distanciaEmCm / alturaDoSensor)) * 100);

    if (porcentagemDoNivelDeAguaAux > 100)
        porcentagemDoNivelDeAgua = 100;
    else if (porcentagemDoNivelDeAguaAux < 0)
        porcentagemDoNivelDeAgua = 0;
    else
        porcentagemDoNivelDeAgua = porcentagemDoNivelDeAguaAux;

    atualizaEstado();
};

int WaterMonitor::getNivel()
{
    return porcentagemDoNivelDeAgua;
};

bool WaterMonitor::getTrava()
{
    return trava;
};

EstadoDoNivelDeAgua WaterMonitor::getEstado()
{
    return estado;
};

void WaterMonitor::controlaTrava(bool travar)
{
    trava = travar;
};
