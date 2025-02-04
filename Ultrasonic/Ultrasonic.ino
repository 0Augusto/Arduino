//Author: Henrique Augusto

const int trigPin = 5;
const int echoPin = 4;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Envia um pulso de 10 µs no pino Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de retorno do pulso no pino Echo
  long duracao = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  float distancia = duracao * 0.034 / 2;

  // Exibe a distância no monitor serial
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(500); // Intervalo entre medições
}