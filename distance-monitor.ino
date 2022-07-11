#include <DS1307RTC.h>
#include <HCSR04.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <SPI.h>
#include "time.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define CISTERNA 1
#define CAIXA 2

const String CAIXA_IP = "192.168.0.63";
const String CISTERNA_IP = "192.168.0.59";

const byte TIPO_DE_RECIPIENTE = CISTERNA;

// Configuração do WIFI
const char *WIFI_SSID = "BRENAN";
const char *WIFI_PASSWORD = "25602874";

const IPAddress SUBNET = IPAddress(255, 255, 255, 0);
const IPAddress GATEWAY = IPAddress(192, 168, 0, 1);
const IPAddress DNS(181, 213, 132, 2);
const IPAddress DNS1(8, 8, 8, 8);
const IPAddress DNS2(8, 8, 4, 4);

AsyncWebServer webServer(80);

const char *CAIXA_HOST_NAME = "CAIXA_ESP32";
const char *CISTERNA_HOST_NAME = "CISTERNA_ESP32";

const IPAddress CAIXA_STATIC_IP = IPAddress(192, 168, 1, 10);
const IPAddress CISTERNA_STATIC_IP = IPAddress(192, 168, 1, 11);

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configuração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;
UltraSonicDistanceSensor distanceSensor(TRIGGER_PIN, ECHO_PIN);

// Configuração da dos alertas de Vazio e Cheio
const float SENSOR_HEIGHT = 30.f;

const float ALTURA_QUANDO_CHEIO = 25.f;
const float ALTURA_QUANDO_VAZIO = 5.f;

const float PORCENTAGEM_QUANDO_CHEIO = (ALTURA_QUANDO_CHEIO / SENSOR_HEIGHT) * 100;
const float PORCENTAGEM_QUANDO_VAZIO = (ALTURA_QUANDO_VAZIO / SENSOR_HEIGHT) * 100;

// Variáveis de Controle / Estados
#define VAZIO 0
#define MEDIANO 1
#define CHEIO 2

byte estadoDoNivelDeAgua = VAZIO; // 0 VAZIO, 1 MEDIANO, 2 CHEIO
bool trava = false;
bool ligarBomba = false;
bool enchendo = false;

// Nível da Água
float porcentagemDoNivelDeAgua = 0.f;

// Configuração dos pinos de Vazio e Cheio (Vai ser retirado ao final)
const int PINO_CHEIO = 2;
const int PINO_VAZIO = 4;

const int PINO_BOMBEAR = 5; // Pino que vai controlar o relé e ligar a bomba

// Buffer para calcular o tempo estimado de esvaziar/encher
const int TAMANHO_HISTORICO_DE_NIVEL = 15;
float historicoNivelDeAgua[TAMANHO_HISTORICO_DE_NIVEL];

void setup()
{
    Serial.begin(115200);
    pinMode(PINO_CHEIO, OUTPUT);
    pinMode(PINO_VAZIO, OUTPUT);

    if (TIPO_DE_RECIPIENTE == CISTERNA)
        pinMode(PINO_BOMBEAR, OUTPUT);

    configuraFusoUTC();
    connectToWifiWithStaticIP();
    configuraWebServer();

    inicializaArrayDeHistoricoDeNiveis();

    Serial.printf("Porcentagem máxima: %f \n", PORCENTAGEM_QUANDO_CHEIO);
    Serial.printf("Porcentagem mínima: %f \n", PORCENTAGEM_QUANDO_VAZIO);
}

// ------------------------- LOOP PRINCIPAL -------------------------
void loop()
{

    // Serial.print("Printando a distancia: ");
    float distanceCm = distanceSensor.measureDistanceCm();
    float nivelDeAguaEmPorcentagem = (1 - (distanceCm / SENSOR_HEIGHT)) * 100;
    nivelDeAguaEmPorcentagem = limitLevelPercentage(nivelDeAguaEmPorcentagem);
    porcentagemDoNivelDeAgua = nivelDeAguaEmPorcentagem;

    // Serial.print(distance_cm);
    // printLevelInPercentage(nivelDeAguaEmPorcentagem);

    atualizaPinosDeNivel(nivelDeAguaEmPorcentagem);
    registraNivel(nivelDeAguaEmPorcentagem);

    if (TIPO_DE_RECIPIENTE == CISTERNA)
        ativaBomba();

    if (TIPO_DE_RECIPIENTE == CAIXA)
        gerenciaNivelDaCaixa();

    calculaTempoEstimado();

    // Serial.println();
    delay(1000);
}
// -------------------------------------------------------------------

// Faz o gerenciamento do nível da Caixa d'água
void gerenciaNivelDaCaixa()
{
    if (estadoDoNivelDeAgua == VAZIO)
        controlaABombaPorHTTPRequest(true && !trava);
    if (estadoDoNivelDeAgua == CHEIO)
        ;
}

// limita o valor do nível para nunca passar além de 100 ou se tornar negativo
float limitLevelPercentage(float nivelDeAguaEmPorcentagem)
{
    if (nivelDeAguaEmPorcentagem > 100.f)
        nivelDeAguaEmPorcentagem = 100.f;
    if (nivelDeAguaEmPorcentagem < 0.f)
        nivelDeAguaEmPorcentagem = 0.f;
    return nivelDeAguaEmPorcentagem;
}

// Por enquanto só printando
// TODO: registrar o nível no cartão SD
// Registra o nível no cartão SD
void registraNivel(float nivelDeAguaEmPorcentagem)
{

    if (filaCheia(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL))
        desenfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL);
    enfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL, nivelDeAguaEmPorcentagem);

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
    Serial.print(nivelDeAguaEmPorcentagem);
    Serial.print("%");
    String stateText;
    // Remover isso ao escrever no SD
    switch (estadoDoNivelDeAgua)
    {
    case VAZIO:
        stateText = "VAZIO";
        break;
    case MEDIANO:
        stateText = "MEDIANO";
        break;
    case CHEIO:
        stateText = "CHEIO";
        break;
    }
    Serial.printf(" MAX: %f MIN: %f %s", PORCENTAGEM_QUANDO_CHEIO, PORCENTAGEM_QUANDO_VAZIO, stateText);
    // -----------------------------
    Serial.println();
}

// TODO: Verificar utilizade dessa função
void printLevelInPercentage(float nivelDeAguaEmPorcentagem)
{
    Serial.print("Distance %: ");
    Serial.print(nivelDeAguaEmPorcentagem);
    Serial.println();
}

// Controla os pinos que dizem se o recipiente está cheio, vazio ou em algum nível intermediário
void atualizaPinosDeNivel(float nivelDeAguaEmPorcentagem)
{
    if (nivelDeAguaEmPorcentagem <= PORCENTAGEM_QUANDO_VAZIO)
    {
        digitalWrite(PINO_CHEIO, LOW);
        digitalWrite(PINO_VAZIO, HIGH);
        estadoDoNivelDeAgua = VAZIO;
    }
    else if (nivelDeAguaEmPorcentagem >= PORCENTAGEM_QUANDO_CHEIO)
    {
        digitalWrite(PINO_CHEIO, HIGH);
        digitalWrite(PINO_VAZIO, LOW);
        estadoDoNivelDeAgua = CHEIO;
    }
    else
    {
        digitalWrite(PINO_CHEIO, LOW);
        digitalWrite(PINO_VAZIO, LOW);
        estadoDoNivelDeAgua = MEDIANO;
    }
}

// TODO: Calcula o tempo estimado de enchimento ou esvaziamento
void calculaTempoEstimado()
{
    mostraArray(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL);
}

// Controla o pino que ligará a bomba baseado nas variáveis "ativaBomba" e "trava"
void ativaBomba()
{
    String estadoDaCaixa = checaEstadoDaCaixaViaHTTPRequest();
    if (estadoDaCaixa == "CHEIO" && estadoDaCaixa != "")
        ligarBomba = false;
    if (estadoDoNivelDeAgua == VAZIO || trava)
        ligarBomba = false;
    digitalWrite(PINO_BOMBEAR, (ligarBomba ? HIGH : LOW));
}

String checaEstadoDaCaixaViaHTTPRequest()
{
    if (WiFi.status() != WL_CONNECTED)
        return "";
    HTTPClient http;
    http.begin("http://" + CAIXA_IP + "/state");
    int httpResponseCode = http.GET();
    String response = "";
    if (httpResponseCode > 0)
    {
        response = http.getString(); // Get the response to the request

        Serial.println(httpResponseCode); // Print return code
        Serial.println(response);         // Print request answer
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
    return response;
}

// Conecta o ESP32 no WiFi
void conectaAoWifi()
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

void configuraFusoUTC()
{
    conectaAoWifi();
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

void controlaABombaPorHTTPRequest(bool deveLigar)
{
    if (WiFi.status() != WL_CONNECTED)
        return;
    HTTPClient http;
    http.begin("http://" + CISTERNA_IP + "/pump?mode=" + (deveLigar ? "on" : "off"));
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("");
    if (httpResponseCode > 0)
    {
        String response = http.getString(); // Get the response to the request

        Serial.println(httpResponseCode); // Print return code
        Serial.println(response);         // Print request answer
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
}

// Não está funcionando
void connectToWifiWithStaticIP()
{
    // Serial.println("Connecting to WiFi with static IP");
    switch (TIPO_DE_RECIPIENTE)
    {
    case CISTERNA:
        WiFi.setHostname(CISTERNA_HOST_NAME);
        // WiFi.config(CISTERNA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
        break;
    case CAIXA:
        WiFi.setHostname(CAIXA_HOST_NAME);
        // WiFi.config(CAIXA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
        break;
    }
    Serial.println(WiFi.macAddress());
    conectaAoWifi();
}

//  Configura os EndPoints para a API
void configuraWebServer()
{
    // Também não funciona com o IP estático.
    Serial.println("Configurando o WebServer...");
    Serial.println("Configurando rotas");
    webServer.on("/state", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /state");
        String responseText;
        switch (estadoDoNivelDeAgua) {
            case VAZIO:
                responseText = "VAZIO";
                break;
            case CHEIO:
                responseText = "CHEIO";
                break;
            case MEDIANO:
                responseText = "MEDIANO";
                break;
        }
        request->send(200, "text/plain", responseText); });

    webServer.on("/level", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /level");
        String responseText = String(porcentagemDoNivelDeAgua, 2);
        request->send(200, "text/plain", responseText); });

    webServer.on("/lock", HTTP_POST, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /lock");
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
        trava = modo == "on" ? true : modo == "off" ? false : trava;
        String responseText = trava ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });

    webServer.on("/isLocked", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /isLocked");
        String responseText = trava ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });

    webServer.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /ping");
        String responseText = "pong";
        request->send(200, "text/plain", responseText); });

    // -------- FUNÇÕES DA CISTERNA --------
    if (TIPO_DE_RECIPIENTE == CISTERNA)
    {
        webServer.on("/pump", HTTP_POST, [](AsyncWebServerRequest *request)
                     {
        Serial.println("Acesso a rota: /pump");
        if (!request->hasParam("mode")) {
            request->send(400, "text/plain", "parâmetro \"mode\" necessário");
            return;
        }
        String modo = request->getParam("mode")->value();
        if (modo != "on" && modo != "off") {
            request->send(400, "text/plain", "valores aceitos pelo parâmetro \"mode\" são: \"on\" - para ligar e \"off\" para desligar");
            return;
        }
        if (modo == "on" && (estadoDoNivelDeAgua == VAZIO)) {
            request->send(400, "text/plain", "Cisterna vazia. Não será possível ligar a Bomba");
            return;
        }
        if (modo == "on" && trava)
        {
            request->send(400, "text/plain", "O Sistema está bloqueado. Não será possível ligar a Bomba. Desbloqueie o sistema e tente novamente");
            return;
        }
        ligarBomba = modo == "on" ? true : modo == "off" ? false : ligarBomba; // Caso não seja "on" ou "off", mantém a bomba no estado em que ela estiver.
        String responseText = ligarBomba ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });

        webServer.on("/isPumping", HTTP_GET, [](AsyncWebServerRequest *request)
                     {
        Serial.println("Acesso a rota: /isPumping");
        String responseText = ligarBomba ? "LIGADO" : "DESLIGADO";
        request->send(200, "text/plain", responseText); });
    }
    // -------- --------- -- ---------- --------

    Serial.println("Iniciando Servidor");

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    webServer.onNotFound([](AsyncWebServerRequest *request)
                         {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404);
  } });
    webServer.begin();
}

// --------------- Funções de Fila -------------------
void inicializaArrayDeHistoricoDeNiveis()
{
    for (int index = 0; index < TAMANHO_HISTORICO_DE_NIVEL; index++)
    {
        historicoNivelDeAgua[index] = -1.f;
    }
}

bool filaCheia(float *fila, int tamanhoDaFila)
{
    for (int index = 0; index < TAMANHO_HISTORICO_DE_NIVEL; index++)
    {
        if (historicoNivelDeAgua[index] == -1.f)
            return false;
    }
    return true;
}

void enfileira(float *fila, int tamanhoDaFila, float elemento)
{
    if (filaCheia(fila, tamanhoDaFila))
        return;
    for (int index = 0; index < tamanhoDaFila; index++)
    {
        if (fila[index] <= -1.f)
            fila[index] = elemento;
    }
}

float desenfileira(float *fila, int tamanhoDaFila)
{
    if (fila[0] == -1.f)
        return -1.f;
    float poppedElement = fila[0];
    moveElementosParaEsquerda(fila, tamanhoDaFila);
    return poppedElement;
}

void moveElementosParaEsquerda(float *vetor, int tamanhoDoVetor)
{
    for (int index = 0; index < tamanhoDoVetor; index++)
    {
        if (index == tamanhoDoVetor)
            vetor[index] = -1;
        vetor[index] = vetor[index + 1];
    }
}

void mostraArray(float *vetor, int tamanhoDoVetor)
{
    for (int index = 0; index < tamanhoDoVetor; index++)
    {
        Serial.printf("%f ", vetor[index]);
    }
    Serial.println();
}
// ----------------------------------------------------
