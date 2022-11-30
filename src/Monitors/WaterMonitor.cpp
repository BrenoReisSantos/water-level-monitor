#include "WaterMonitor.hpp"
#include <stdio.h>

void WaterMonitor::atualizaEstado()
{
    if (getNivel() <= porcentagemQuandoVazio)
        estado = Vazio;
    else if (getNivel() >= porcentagemQuandoCheio)
        estado = Cheio;
    else
        estado = Mediano;
};

void WaterMonitor::atualizaNivel(float distanciaEmCm)
{
    int porcentagemDoNivelDeAguaAux = (int)((1 - (distanciaEmCm / alturaDoSensor)) * 100);

    if (porcentagemDoNivelDeAguaAux > 100)
        levelQueue->queue(100);
    else if (porcentagemDoNivelDeAguaAux < 0)
        levelQueue->queue(0);
    else
        levelQueue->queue(porcentagemDoNivelDeAguaAux);

    atualizaEstado();
};

int WaterMonitor::getNivel()
{
    return (int)levelQueue->average();
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

std::string WaterMonitor::toString()
{
    char texto[200];

    const char *textoDoEstado;
    if (estado == Cheio)
        textoDoEstado = "Cheio";
    else if (estado == Mediano)
        textoDoEstado = "Mediano";
    else
        textoDoEstado = "Vazio";

    sprintf(texto, "ALTURA SENSOR: %.1f | ALTURA CHEIO: %.1f | ALTURA VAZIO: %.1f | LOCK: %s | NÍVEL: %d%% | ESTADO: %s",
            alturaDoSensor,
            alturaQuandoCheio,
            alturaQuandoVazio,
            trava ? "true" : "false",
            getNivel(),
            textoDoEstado);
    return std::string(texto);
};
