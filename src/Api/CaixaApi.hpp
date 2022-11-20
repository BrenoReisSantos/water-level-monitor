#ifndef CAIXAAPI_H
#define CAIXAAPI_H

#include "BaseApi.hpp"
#include "../Monitors/CaixaMonitor.hpp"

class CaixaApi : public BaseApi
{

public:
    CaixaApi(WaterMonitor *caixa);
    void configuraRotas() override;
};

#endif
