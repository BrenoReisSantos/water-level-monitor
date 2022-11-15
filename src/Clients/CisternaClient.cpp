#include <iostream>

#include <Arduino.h>

#include "CisternaClient.hpp"
#include "../Monitors/WaterMonitor.hpp"

#include <HTTPClient.h>

using namespace std;

bool CisternaClient::checaVidaDaCaixa()
{
    int httpResponseCode = http.GET();
    bool response = false;
    if (httpResponseCode > 0)
    {
        String responseText = http.getString();
        if (responseText == "pong" && httpResponseCode == 200)
            response = true;
    }
    else
    {
        response = false;
    }
    // Serial.println(response);
    return response;
};

CisternaClient::CisternaClient(char *caixaIP)
{
    baseUrl = strcat("http://", caixaIP);
};

EstadoDoNivelDeAgua CisternaClient::checaEstadoDaCaixa()
{
    String url(strcat(baseUrl, statePath));

    http.begin(url);
    int httpResponseCode = http.GET();
    String response = "";
    if (httpResponseCode > 0)
    {
        response = http.getString(); // Get the response to the request
    }
    else
    {
        Serial.print("Error on sending POST: ");
    }

    if (response == "CHEIO")
        return Cheio;
    if (response == "MEDIANO")
        return Mediano;
    if (response == "VAZIO")
        return Vazio;
    return Cheio;
}
