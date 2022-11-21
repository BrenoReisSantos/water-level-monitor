#include "WaterMonitor.hpp"
#include <stdio.h>

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

std::string WaterMonitor::toString()
{
    char texto[100];

    const char *textoDoEstado;
    if (estado == Cheio)
        textoDoEstado = "Cheio";
    else if (estado == Mediano)
        textoDoEstado = "Mediano";
    else
        textoDoEstado = "Vazio";

    sprintf(texto, "ALTURA SENSOR: %f | ALTURA CHEIO: %f | ALTURA VAZIO: %f | LOCK: %s | NÍVEL: %d\% | ESTADO: %s",
            alturaDoSensor,
            alturaQuandoCheio,
            alturaQuandoVazio,
            trava ? "true" : "false",
            porcentagemDoNivelDeAgua,
            textoDoEstado);
    return std::string(texto);
};
