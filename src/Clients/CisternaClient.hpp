#include <iostream>

#include <Arduino.h>

#include <HTTPClient.h>

using namespace std;

class CisternaClient
{
    HTTPClient http;

    const char *statePath = "/state";
    const char *healthPath = "/ping";

    char *baseUrl;

public:
    CisternaClient(char *caixaIP);

    bool checaVidaDaCaixa();
    EstadoDoNivelDeAgua checaEstadoDaCaixa();
};
