#ifndef CAIXACLIENT_H
#define CAIXACLIENT_H

#include <iostream>

#include <Arduino.h>

#include <HTTPClient.h>

class CaixaClient
{
    HTTPClient http;

    const std::string pumpPath = "/pump?mode=";

    std::string baseUrl;

public:
    CaixaClient(std::string cisternaIP);

    void controlaBomba(bool deveLigar);
};

#endif
