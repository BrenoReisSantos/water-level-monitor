#include <DS1307RTC.h>
#include <HCSR04.h>
#include <WiFi.h>
// #include <SPI.h>
#include "time.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// Configuração do WIFI
const char *WIFI_SSID = "BRENAN";
const char *WIFI_PASSWORD = "25602874";

AsyncWebServer webServer(80);

const char *CAIXA_HOST_NAME = "CAIXA_ESP32";
const char *CISTERNA_HOST_NAME = "CISTERNA_ESP32";

const IPAddress SUBNET = IPAddress(255, 255, 255, 0);
const IPAddress GATEWAY = IPAddress(192, 168, 0, 1);
const IPAddress DNS(181, 213, 132, 2);
const IPAddress DNS1(8, 8, 8, 8);
const IPAddress DNS2(8, 8, 4, 4);

const IPAddress CAIXA_STATIC_IP = IPAddress(192, 168, 1, 10);
const IPAddress CISTERNA_STATIC_IP = IPAddress(192, 168, 1, 11);

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;
UltraSonicDistanceSensor distanceSensor(TRIGGER_PIN, ECHO_PIN);

// Configuração da dos alertas de Vazio e Cheio
const float MAX_DISTANCE_IN_CM = 30.f;
const float FULL_LEVEL_PERCENTAGE = 85.f;
const float EMPTY_LEVEL_PERCENTAGE = 50.f;

bool isFull = false;
bool isEmpty = false;
bool lock = false;
bool waterOut = false;
bool waterIn = false;

// Nível da Água
float waterLevelPercentage = 0.f;

// Configuração dos pinos de Vazio e Cheio
const int FULL_PIN = 2;
const int EMPTY_PIN = 4;

void setup()
{
    Serial.begin(115200);
    pinMode(FULL_PIN, OUTPUT);
    pinMode(EMPTY_PIN, OUTPUT);

    configUTCTime();
    connectToWifiWithStaticIP();
    configureWebServer();
}

void loop()
{

    // Serial.print("Printando a distancia: ");
    float distanceCm = distanceSensor.measureDistanceCm();
    float levelPercentage = (1 - (distanceCm / MAX_DISTANCE_IN_CM)) * 100;
    levelPercentage = limitLevelPercentage(levelPercentage);
    waterLevelPercentage = levelPercentage;

    // Serial.print(distance_cm);

    // printLevelInPercentage(levelPercentage);

    updateLevelPins(levelPercentage);

    registerLevel(levelPercentage);

    // Serial.println();
    delay(1000);
}

float limitLevelPercentage(float levelPercentage)
{
    if (levelPercentage > 100.f)
        levelPercentage = 100.f;
    if (levelPercentage < 0.f)
        levelPercentage = 0.f;
    return levelPercentage;
}

// Por enquanto só printando
void registerLevel(float levelPercentage)
{
    Serial.print(year());
    Serial.print("-");
    Serial.print(month());
    Serial.print("-");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" ");
    Serial.print(levelPercentage);
    Serial.print("%");
    Serial.println();
}

void printLevelInPercentage(float levelPercentage)
{
    Serial.print("Distance %: ");
    Serial.print(levelPercentage);
    Serial.println();
}

void updateLevelPins(float levelPercentage)
{
    if (levelPercentage > 100.f)
        levelPercentage = 100.f;
    if (levelPercentage <= EMPTY_LEVEL_PERCENTAGE)
    {
        digitalWrite(FULL_PIN, LOW);
        digitalWrite(EMPTY_PIN, HIGH);
    }
    else if (levelPercentage >= FULL_LEVEL_PERCENTAGE)
    {
        digitalWrite(EMPTY_PIN, LOW);
        digitalWrite(FULL_PIN, HIGH);
    }
    else
    {
        digitalWrite(EMPTY_PIN, LOW);
        digitalWrite(FULL_PIN, LOW);
    }
}

void connectToWifi()
{
    Serial.print("Conectando a:");
    Serial.print(WIFI_SSID);
    Serial.println();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("SSID: ");
    Serial.print(WiFi.SSID());
    Serial.print(" IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" SUBNET: ");
    Serial.print(WiFi.subnetMask());
    Serial.print(" GATEWAY: ");
    Serial.print(WiFi.gatewayIP());
    Serial.print(" DNS: ");
    Serial.print(WiFi.dnsIP());
    Serial.println("\nWiFi conectado.");
}

void configUTCTime()
{
    connectToWifi();
    // Não funciona com ip estático...
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

void connectToWifiWithStaticIP()
{
    Serial.println("Connecting to WiFi with static IP");
    WiFi.setHostname(CAIXA_HOST_NAME);
    WiFi.config(CAIXA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
    connectToWifi();
}

void configureWebServer()
{
    // Também não funciona com o IP estático.
    Serial.println("Configurando o WebServer...");
    Serial.println("Configurando rotas");
    webServer.on("/stats/fullness", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /stats/fullness");
        String responseText;
        isFull ? responseText = "FULL" : isEmpty ? responseText = "EMPTY" : responseText = "AVERAGE";
        request->send(200, "text/plain", responseText); });
    webServer.on("/stats/waterlevel", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /stats/waterlevel");
        String responseText = String(waterLevelPercentage, 2);
        request->send(200, "text/plain", responseText); });
    Serial.println("Iniciando Servidor");
    webServer.begin();
}
