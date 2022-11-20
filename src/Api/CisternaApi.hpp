#ifndef CISTERNAAPI_H
#define CISTERNAAPI_H

#include "BaseApi.hpp"
#include "../Monitors/CisternaMonitor.hpp"

class CisternaApi : public BaseApi
{
    void controlaBomba();
    void getEstadoBomba();

public:
    CisternaApi(WaterMonitor *cisterna);
    void configuraRotas() override;
};

#endif
