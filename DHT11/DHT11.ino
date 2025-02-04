#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int numLeituras = 5; // Número de leituras para a média
float leiturasTemp[numLeituras];
float leiturasUmid[numLeituras];
int indice = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  // Inicializa arrays com zeros
  for (int i = 0; i < numLeituras; i++) {
    leiturasTemp[i] = 0;
    leiturasUmid[i] = 0;
  }
}

void loop() {
  delay(2000);

  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro na leitura!");
    return;
  }

  // Atualiza arrays com as novas leituras
  leiturasTemp[indice] = temperatura;
  leiturasUmid[indice] = umidade;
  indice = (indice + 1) % numLeituras;

  // Calcula médias
  float mediaTemp = 0, mediaUmid = 0;
  for (int i = 0; i < numLeituras; i++) {
    mediaTemp += leiturasTemp[i];
    mediaUmid += leiturasUmid[i];
  }
  mediaTemp /= numLeituras;
  mediaUmid /= numLeituras;

  Serial.print("Temperatura Média: ");
  Serial.print(mediaTemp);
  Serial.print("°C | Umidade Média: ");
  Serial.print(mediaUmid);
  Serial.println("%");
}