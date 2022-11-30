#ifndef MONITORCONFIG_H
#define MONITORCONFIG_H

#include <string>

class MonitorConfig
{
public:
    enum TipoReservatorio
    {
        Caixa,
        Cisterna
    };

    MonitorConfig(
        float alturaDoSensor,
        float alturaQuandoCheio,
        float alturaQuandoVazio,
        TipoReservatorio tipoReservatorio,
        std::string ipOutroReservatorio);

    float getAlturaDoSendor();
    float getalturaQuandoCheio();
    float getalturaQuandoVazio();
    TipoReservatorio getTipoReservatorio();
    std::string getIpOutroReservatorio();

private:
    float alturaDoSensor;
    float alturaQuandoCheio;
    float alturaQuandoVazio;
    TipoReservatorio tipoReservatorio;
    std::string ipOutroReservatorio;
};

#endif
