#ifndef CISTERNACLIENT_H
#define CISTERNACLIENT_H

#include <iostream>

#include <Arduino.h>

#include <HTTPClient.h>

#include "../Monitors/EstadoDoNivelDeAgua.hpp"

using namespace std;

class CisternaClient
{
    HTTPClient http;

    const string statePath = "/state";
    const string healthPath = "/ping";

    string baseUrl;

public:
    CisternaClient(string caixaIP);

    bool checaVidaDaCaixa();
    EstadoDoNivelDeAgua checaEstadoDaCaixa();
};

#endif
