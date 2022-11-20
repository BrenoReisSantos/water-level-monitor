#include "CisternaApi.hpp"

CisternaApi::CisternaApi(WaterMonitor *cisterna) : BaseApi(cisterna)
{
    monitor = cisterna;
};

void CisternaApi::configuraRotas()
{
    BaseApi::configuraRotas();
    controlaBomba();
    getEstadoBomba();
};

void CisternaApi::controlaBomba()
{
    webServer.on("/pump", HTTP_POST, [this](AsyncWebServerRequest *request)
                 {
        if (!request->hasParam("mode")) {
            request->send(400, "text/plain", "parâmetro \"mode\" necessário");
            return;
        }
        String modo = request->getParam("mode")->value();
        if (modo != "on" && modo != "off") {
            request->send(400, "text/plain", "valores aceitos pelo parâmetro \"mode\" são: \"on\" - para ligar e \"off\" para desligar");
            return;
        }
        if (modo == "on" && (((CisternaMonitor *)monitor)->getEstado() == Vazio))
        {
            request->send(400, "text/plain", "Cisterna vazia. Não será possível ligar a Bomba");
            return;
        }
        if (modo == "on" && monitor->getTrava())
        {
            request->send(400, "text/plain", "O Sistema está bloqueado. Não será possível ligar a Bomba. Desbloqueie o sistema e tente novamente");
            return;
        }
        bool bombaDeveLigar = modo == "on" ? true : modo == "off" ? false
                                                                  : ((CisternaMonitor *)monitor)->getEstadoBomba();
        ((CisternaMonitor *)monitor)->controlaBomba(bombaDeveLigar);
        std::string responseText = ((CisternaMonitor *)monitor)->getEstadoBomba() ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText.c_str()); });
};

void CisternaApi::getEstadoBomba()
{
    webServer.on("/isPumping", HTTP_GET, [this](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /isPumping");
        std::string responseText = ((CisternaMonitor *)monitor)->getEstadoBomba() ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText.c_str()); });
};
