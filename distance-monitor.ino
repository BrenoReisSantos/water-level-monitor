#include <DS1307RTC.h>
#include <HCSR04.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <SPI.h>
#include "time.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include <string>

#include "src/Factory/WaterMonitorFactory.hpp"
#include "src/Factory/ApiFactory.hpp"
#include "src/Config/MonitorConfig.hpp"
#include "src/PinManager/UltrassonicSensor.hpp"
#include "src/PinManager/CisternaPinManager.hpp"

const String CAIXA_IP = "192.168.68.106";
const String CISTERNA_IP = "192.168.68.109";

// Configuração do WIFI
const char *WIFI_SSID = "BRENAN";
const char *WIFI_PASSWORD = "25602874";

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configuração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;

const int PINO_BOMBEAR = 5; // Pino que vai controlar o relé e ligar a bomba

MonitorConfig *config;

WaterMonitor *monitor;
BaseApi *api;

UltrassonicSensor sensor = UltrassonicSensor(TRIGGER_PIN, ECHO_PIN);

CisternaPinManager *cisternaPinManager;

void setup()
{
    Serial.begin(115200);

    configuraFusoUTC();
    conectaAoWifi();

    float alturadoSensor = 100;
    float alturaQuandoCheio = 80;
    float alturaQuandoVazio = 40;
    MonitorConfig::TipoReservatorio tipoReservatorio = MonitorConfig::Cisterna;
    std::string ipOutroReservatorio = "192.168.0.59";

    config = new MonitorConfig(
        alturadoSensor,
        alturaQuandoCheio,
        alturaQuandoVazio,
        tipoReservatorio,
        ipOutroReservatorio);

    WaterMonitorFactory monitorFactory = WaterMonitorFactory();
    ApiFactory apiFactory = ApiFactory();

    monitor = monitorFactory.criaInstancia(config);

    api = apiFactory.criaInstancia(config->getTipoReservatorio(), monitor);
    api->configuraRotas();

    if (config->getTipoReservatorio() == MonitorConfig::Cisterna)
    {
        cisternaPinManager = new CisternaPinManager(PINO_BOMBEAR, (CisternaMonitor *)monitor);
    }
}

void loop()
{
    float distanciaMedidaEmCentimetros = sensor.medeDistanciaEmCentimetros();
    Serial.printf("[Distância medida: %f]\n", distanciaMedidaEmCentimetros);

    monitor->atualizaNivel(distanciaMedidaEmCentimetros);

    monitor->gerenciaReservatorio();

    if (config->getTipoReservatorio() == MonitorConfig::Cisterna)
    {
        cisternaPinManager->atualizaPinoDoRele();
    }

    printaStatus();

    delay(1000);
}

void printaStatus()
{
    Serial.printf("%d/%d/%d %d:%d:%d\n", year(), month(), day(), hour(), minute(), second());
    Serial.printf("%s\n", monitor->toString().c_str());

    Serial.println();
}

// Conecta o ESP32 no WiFi
void conectaAoWifi()
{
    Serial.printf("Conectando a: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.printf("SSID: %s | IP: %s\n", WiFi.SSID(), WiFi.localIP().toString().c_str());
}

void configuraFusoUTC()
{
    conectaAoWifi();
    configTime(BRAZIL_TIME_ZONE, 0, TIME_SERVER);
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    Serial.println("Time got with success");
    Serial.println("Disconnecting WiFi");
    WiFi.disconnect();
}
