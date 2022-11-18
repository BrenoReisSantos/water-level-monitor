#include <iostream>

#include <Arduino.h>

#include <HTTPClient.h>

using namespace std;

class CaixaClient
{
    HTTPClient http;

    const char *pumpPath = "/pump?mode=";

    char *baseUrl;

public:
    CaixaClient(char *cisternaIP);

    void controlaBomba(bool deveLigar);
};
