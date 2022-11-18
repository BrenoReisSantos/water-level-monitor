#include "BaseApi.hpp"
#include <ESPAsyncWebServer.h>

void BaseApi::getNivel()
{
    webServer.on("/level", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
        char* responseText;
        sprintf("%d" , responseText, monitor->getNivel());
        request->send(200, "text/plain", responseText); });
};

void BaseApi::getEstado()
{
    webServer.on("/state", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
                    
        char* responseText;
        switch (monitor->getEstado())
        {
        case Vazio:
            responseText = "VAZIO";
            break;
        case Cheio:
            responseText = "CHEIO";
            break;
        case Mediano:
            responseText = "MEDIANO";
            break;
        }
        request->send(200, "text/plain", responseText); });
};

void BaseApi::postTravaSistema()
{
    webServer.on("/lock", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
        if (!request->hasParam("mode"))
        {
            request->send(400, "text/plain", "parâmetro \"mode\" necessário");
            return;
        }
        String modo = request->getParam("mode")->value();
        if (modo != "on" && modo != "off")
        {
            request->send(400, "text/plain", "valores aceitos pelo parâmetro \"mode\" são: \"on\" - para ligar e \"off\" para desligar");
            return;
        }
        bool trava = (modo == "on" ? true : modo == "off" ? false : monitor->getTrava());
        monitor->controlaTrava(trava);
        const char *responseText = monitor->getTrava() ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });
};

void BaseApi::getEstadoDaTrava()
{
    webServer.on("/isLocked", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
        const char *responseText = monitor->getTrava() ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });
};

void BaseApi::ping()
{
    webServer.on("/ping", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
        char *responseText = "pong";
        request->send(200, "text/plain", responseText); });
};

void BaseApi::configuraRotas()
{
    getEstado();
    getNivel();
    postTravaSistema();
    getEstadoDaTrava();
    ping();
};
