// #include "TipoRecipiente.hpp"

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
        char *ipOutroReservatorio);

    float getAlturaDoSendor();
    float getalturaQuandoCheio();
    float getalturaQuandoVazio();
    TipoReservatorio getTipoReservatorio();
    char *getIpOutroReservatorio();

private:
    float alturaDoSensor;
    float alturaQuandoCheio;
    float alturaQuandoVazio;
    TipoReservatorio tipoReservatorio;
    char *ipOutroReservatorio;
};
