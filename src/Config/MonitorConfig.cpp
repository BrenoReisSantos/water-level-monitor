#include "MonitorConfig.hpp"

MonitorConfig::MonitorConfig(float alturaDoSensor,
                             float alturaQuandoCheio,
                             float alturaQuandoVazio,
                             TipoRecipiente tipoReservatorio,
                             char *ipOutroReservatorio)
{
    this->alturaDoSensor = alturaDoSensor;
    this->alturaQuandoCheio = alturaQuandoCheio;
    this->alturaQuandoVazio = alturaQuandoVazio;
    this->tipoReservatorio = tipoReservatorio;
    this->ipOutroReservatorio = ipOutroReservatorio;
};

float MonitorConfig::getAlturaDoSendor()
{
    return alturaDoSensor;
};

float MonitorConfig::getalturaQuandoCheio()
{
    return alturaQuandoCheio;
};

float MonitorConfig::getalturaQuandoVazio()
{
    return alturaQuandoVazio;
};

TipoRecipiente MonitorConfig::getTipoReservatorio()
{
    return tipoReservatorio;
};

char *MonitorConfig::getIpOutroReservatorio()
{
    return ipOutroReservatorio;
};
