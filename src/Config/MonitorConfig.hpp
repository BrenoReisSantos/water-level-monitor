#include "TipoRecipiente.hpp"

class MonitorConfig
{
    float alturaDoSensor;
    float alturaQuandoCheio;
    float alturaQuandoVazio;
    TipoReservatorio tipoReservatorio;
    char *ipOutroReservatorio;

public:
    MonitorConfig(
        float alturaDoSensor,
        float alturaQuandoCheio,
        float alturaQuandoVazio,
        TipoReservatorio tipoReservatorio,
        char *ipOutroReservatorio);

    float getAlturaDoSendor();
    float getalturaQuandoCheio();
    float getalturaQuandoVazio();
    TipoReservatorio getTipoReservatorio();
    char *getIpOutroReservatorio();
};
