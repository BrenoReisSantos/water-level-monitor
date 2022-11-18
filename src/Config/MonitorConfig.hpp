#include "TipoRecipiente.hpp"

class MonitorConfig
{
    float alturaDoSensor;
    float alturaQuandoCheio;
    float alturaQuandoVazio;
    TipoRecipiente tipoReservatorio;
    char *ipOutroReservatorio;

public:
    MonitorConfig(
        float alturaDoSensor,
        float alturaQuandoCheio,
        float alturaQuandoVazio,
        TipoRecipiente tipoReservatorio,
        char *ipOutroReservatorio);

    float getAlturaDoSendor();
    float getalturaQuandoCheio();
    float getalturaQuandoVazio();
    TipoRecipiente getTipoReservatorio();
    char *getIpOutroReservatorio();
};
