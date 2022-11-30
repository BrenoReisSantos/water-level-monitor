#include "CaixaApi.hpp"

CaixaApi::CaixaApi(WaterMonitor *caixa) : BaseApi(caixa)
{
    monitor = caixa;
};

void CaixaApi::configuraRotas()
{
    BaseApi::configuraRotas();
};
