#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

// Configurações do DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configurações do sensor ultrassônico
#define TRIG_PIN 5
#define ECHO_PIN 4

// Configurações do LCD (usando biblioteca LiquidCrystal da Adafruit)
// Definição dos pinos para o LCD (modo 4 bits)
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Configurações WiFi e MQTT
char ssid[] = "SUA_REDE_WIFI";
char pass[] = "SUA_SENHA_WIFI";
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "broker.mqtt-dashboard.com";
int port = 1883;
const char topic[] = "arduino/irrigacao";
const char topicAlertas[] = "arduino/irrigacao/alertas"; // Tópico separado para alertas

// Variáveis para médias móveis
const int numLeituras = 5;
float leiturasTemp[numLeituras];
float leiturasUmid[numLeituras];
float leiturasDist[numLeituras];
int indice = 0;

// Limites para alertas
const float TEMP_ALTA = 35.0;
const float TEMP_BAIXA = 10.0;
const float UMID_ALTA = 80.0;
const float UMID_BAIXA = 30.0;
const float DIST_ALERTA = 10.0; // 10cm - reservatório baixo

// Timers para controle não bloqueante
unsigned long ultimaLeitura = 0;
unsigned long ultimoDisplay = 0;
unsigned long ultimoMQTT = 0;
const long intervaloLeitura = 2000;
const long intervaloDisplay = 1000;
const long intervaloMQTT = 5000;

void setup() {
  Serial.begin(9600);
  
  // Inicializar sensores
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Inicializar LCD
  lcd.begin(16, 2);
  lcd.print("Inicializando...");
  
  // Inicializar arrays
  for (int i = 0; i < numLeituras; i++) {
    leiturasTemp[i] = 0;
    leiturasUmid[i] = 0;
    leiturasDist[i] = 0;
  }
  
  // Conectar WiFi
  conectarWiFi();
  
  // Conectar MQTT
  conectarMQTT();
  
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // Ler sensores a cada intervalo
  if (tempoAtual - ultimaLeitura >= intervaloLeitura) {
    lerSensores();
    ultimaLeitura = tempoAtual;
  }
  
  // Atualizar display a cada intervalo
  if (tempoAtual - ultimoDisplay >= intervaloDisplay) {
    atualizarDisplay();
    ultimoDisplay = tempoAtual;
  }
  
  // Enviar dados via MQTT a cada intervalo
  if (tempoAtual - ultimoMQTT >= intervaloMQTT) {
    enviarDadosMQTT();
    verificarAlertas();
    ultimoMQTT = tempoAtual;
  }
  
  // Manter conexão MQTT
  if (!mqttClient.connected()) {
    conectarMQTT();
  }
  mqttClient.poll();
}

void lerSensores() {
  // Ler DHT11
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro na leitura do DHT!");
    lcd.clear();
    lcd.print("Erro DHT11");
    return;
  }
  
  // Ler sensor ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia = duracao * 0.034 / 2;
  
  // Atualizar arrays com médias móveis
  leiturasTemp[indice] = temperatura;
  leiturasUmid[indice] = umidade;
  leiturasDist[indice] = distancia;
  indice = (indice + 1) % numLeituras;
}

float calcularMedia(float valores[]) {
  float soma = 0;
  for (int i = 0; i < numLeituras; i++) {
    soma += valores[i];
  }
  return soma / numLeituras;
}

void atualizarDisplay() {
  float mediaTemp = calcularMedia(leiturasTemp);
  float mediaUmid = calcularMedia(leiturasUmid);
  float mediaDist = calcularMedia(leiturasDist);
  
  // Linha 1: Temperatura e Umidade
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(mediaTemp, 1);
  lcd.print("C U:");
  lcd.print(mediaUmid, 0);
  lcd.print("%");
  
  // Limpar o restante da linha
  lcd.print("   ");
  
  // Linha 2: Distância e Status
  lcd.setCursor(0, 1);
  lcd.print("N:");
  lcd.print(mediaDist, 1);
  lcd.print("cm");
  
  // Limpar o restante da linha
  lcd.print("     ");
  
  // Adicionar indicador de alerta se necessário
  if (mediaDist <= DIST_ALERTA) {
    lcd.setCursor(14, 1);
    lcd.print("!");
  } else {
    lcd.setCursor(14, 1);
    lcd.print(" ");
  }
}

void conectarWiFi() {
  lcd.clear();
  lcd.print("Conectando WiFi");
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    WiFi.begin(ssid, pass);
    delay(2000);
    tentativas++;
    
    lcd.setCursor(0, 1);
    lcd.print("Tentativa ");
    lcd.print(tentativas);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.print("WiFi Conectado!");
    lcd.setCursor(0, 1);
    lcd.print("IP: ");
    // Mostrar apenas os últimos 8 caracteres do IP (por limitação de espaço)
    String ip = WiFi.localIP().toString();
    if (ip.length() > 8) {
      lcd.print(ip.substring(ip.length() - 8));
    } else {
      lcd.print(ip);
    }
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Erro WiFi!");
    while (true); // Travar em caso de falha
  }
}

void conectarMQTT() {
  lcd.clear();
  lcd.print("Conectando MQTT");
  
  if (!mqttClient.connect(broker, port)) {
    lcd.setCursor(0, 1);
    lcd.print("Falha MQTT");
    delay(2000);
    return;
  }
  
  lcd.clear();
  lcd.print("MQTT Conectado!");
  delay(1000);
}

void enviarDadosMQTT() {
  float mediaTemp = calcularMedia(leiturasTemp);
  float mediaUmid = calcularMedia(leiturasUmid);
  float mediaDist = calcularMedia(leiturasDist);
  
  String mensagem = "T:" + String(mediaTemp, 1) + 
                   "C U:" + String(mediaUmid, 0) + 
                   "% N:" + String(mediaDist, 1) + "cm";
  
  mqttClient.beginMessage(topic);
  mqttClient.print(mensagem);
  mqttClient.endMessage();
  
  Serial.println("Dados enviados: " + mensagem);
}

void verificarAlertas() {
  float mediaTemp = calcularMedia(leiturasTemp);
  float mediaUmid = calcularMedia(leiturasUmid);
  float mediaDist = calcularMedia(leiturasDist);
  
  // Verificar e enviar alertas
  if (mediaTemp >= TEMP_ALTA) {
    enviarAlerta("Temperatura Alta: " + String(mediaTemp, 1) + "C");
  } else if (mediaTemp <= TEMP_BAIXA) {
    enviarAlerta("Temperatura Baixa: " + String(mediaTemp, 1) + "C");
  }
  
  if (mediaUmid >= UMID_ALTA) {
    enviarAlerta("Umidade Alta: " + String(mediaUmid, 0) + "%");
  } else if (mediaUmid <= UMID_BAIXA) {
    enviarAlerta("Umidade Baixa: " + String(mediaUmid, 0) + "%");
  }
  
  if (mediaDist <= DIST_ALERTA) {
    enviarAlerta("Nivel Baixo: " + String(mediaDist, 1) + "cm");
  }
}

void enviarAlerta(String mensagem) {
  mqttClient.beginMessage(topicAlertas);
  mqttClient.print(mensagem);
  mqttClient.endMessage();
  
  Serial.println("Alerta: " + mensagem);
  
  // Piscar LCD para alerta
  for (int i = 0; i < 3; i++) {
    lcd.noDisplay();
    delay(200);
    lcd.display();
    delay(200);
  }
}