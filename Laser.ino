#include <Servo.h>

const int trigPin = 4;
const int echoPin = 5;
const int servoPin = 9;
Servo meuServo;

// Configurações do servo
const int ANGULO_MINIMO = 0;     // Ângulo mínimo de 0 graus
const int ANGULO_MAXIMO = 180;   // Ângulo máximo de 180 graus
const int INCREMENTO_ANGULO = 1; // Incremento de 1 grau por passo
const int ATRASO_MOVIMENTO = 15; // Delay para movimento suave (ms)

// Variáveis de controle
int anguloAtual = ANGULO_MINIMO;
bool incrementando = true; // Direção do movimento

// Configurações do sensor
const int INTERVALO_LEITURA = 100;
unsigned long ultimaLeitura = 0;
const int DISTANCIA_ALERTA = 30; // Distância para alerta (cm)

void setup() { // Complexidade: 1 (sem decisões)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  meuServo.attach(servoPin);
  
  Serial.begin(9600);
  delay(1000);
  Serial.println("Sistema de varredura 180° contínua iniciado");
}

void loop() { // Complexidade: 3 (2 condições if)
  // Movimenta o servo de forma contínua entre 0 e 180 graus
  moverServo();
  
  unsigned long tempoAtual = millis();
  // Faz leituras periódicas do sensor
  if (tempoAtual - ultimaLeitura >= INTERVALO_LEITURA) { // +1 condição
    int distancia = lerDistancia();
    
    // Verifica se é um objeto próximo
    if (distancia <= DISTANCIA_ALERTA && distancia >= 2) { // +1 condição (&&)
      exibirDetecao(anguloAtual, distancia);
    }
    
    ultimaLeitura = tempoAtual;
  }
  
  processarComandosSerial();
}

void moverServo() { // Complexidade: 2 (1 condição if)
  // Move o servo para a próxima posição
  meuServo.write(anguloAtual);
  delay(ATRASO_MOVIMENTO);
  
  // Atualiza ângulo e direção
  if (incrementando) { // +1 condição
    anguloAtual += INCREMENTO_ANGULO;
    if (anguloAtual >= ANGULO_MAXIMO) { // +1 condição aninhada
      incrementando = false;
    }
  } else {
    anguloAtual -= INCREMENTO_ANGULO;
    if (anguloAtual <= ANGULO_MINIMO) { // +1 condição aninhada
      incrementando = true;
    }
  }
}

int lerDistancia() { // Complexidade: 2 (1 condição if)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 20000);
  
  if (duration == 0) return 100; // +1 condição
  
  int distance = (duration * 0.034) / 2;
  return (distance < 2 || distance > 100) ? 100 : distance; // +1 condição (||)
}

void exibirDetecao(int angulo, int distancia) { // Complexidade: 1
  Serial.print(">>> OBJETO PRÓXIMO! Ângulo: ");
  Serial.print(angulo);
  Serial.print("° | Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

void processarComandosSerial() { // Complexidade: 9 (switch com 7 casos + 1 condição if)
  if (Serial.available()) { // +1 condição
    char comando = Serial.read();
    
    switch(comando) { // +1 para o switch
      case 's': // +1 para cada case
      case 'S':
        pararVarredura();
        break;
        
      case 'g': 
      case 'G':
        retomarVarredura();
        break;
        
      case 'i': 
      case 'I':
        exibirStatus();
        break;
        
      case 'h':
      case 'H':
      case '?':
        exibirAjuda();
        break;
        
      default: // +1 para default
        break;
    }
  }
}

void pararVarredura() { // Complexidade: 1
  // Para a varredura mantendo a posição atual
  Serial.println(">>> VARREURA PARADA - Servo mantém posição");
}

void retomarVarredura() { // Complexidade: 1
  // Retoma a varredura
  Serial.println(">>> VARREURA RETOMADA");
}

void exibirStatus() { // Complexidade: 1
  Serial.println("\n=== STATUS DO SISTEMA ===");
  Serial.print("Ângulo atual: ");
  Serial.println(anguloAtual);
  Serial.print("Direção: ");
  Serial.println(incrementando ? "INCREMENTANDO" : "DECREMENTANDO");
  Serial.print("Tempo de operação: ");
  Serial.print(millis() / 1000);
  Serial.println(" segundos");
  Serial.println("==========================\n");
}

void exibirAjuda() { // Complexidade: 1
  Serial.println("\n=== COMANDOS DISPONÍVEIS ===");
  Serial.println("s - Parar varredura");
  Serial.println("g - Retomar varredura"); 
  Serial.println("i - Exibir status do sistema");
  Serial.println("h - Exibir esta ajuda");
  Serial.println("============================\n");
}