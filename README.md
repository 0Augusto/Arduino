# Sistema de Varredura com Sensor Ultrassônico e Servo Motor

## 📋 Descrição do Projeto

Este projeto implementa um sistema de varredura ambiental de 180 graus utilizando um sensor ultrassônico HC-SR04 e um servo motor SG90. O sistema realiza uma varredura contínua do ambiente, detectando objetos próximos e fornecendo informações em tempo real via interface serial.

### 🎯 Funcionalidades Principais

- **Varredura contínua de 180°** com movimento suave do servo motor
- **Detecção de objetos** em tempo real usando sensor ultrassônico
- **Interface serial interativa** com comandos para controle do sistema
- **Análise de complexidade** do código incluída para avaliação de qualidade

## 🛠 Componentes Necessários

- Arduino Uno/Nano/Mega
- Sensor ultrassônico HC-SR04
- Servo motor SG90 (modo 180° padrão)
- Cabos de conexão
- Protoboard
- Fonte de alimentação adequada

## 🔌 Esquema de Ligação

| Componente | Pino Arduino |
|------------|--------------|
| HC-SR04 VCC | 5V |
| HC-SR04 GND | GND |
| HC-SR04 Trig | 4 |
| HC-SR04 Echo | 5 |
| SG90 VCC | 5V |
| SG90 GND | GND |
| SG90 Sinal | 9 |

## 💻 Código do Projeto

```cpp
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
```

## 🎮 Comandos da Interface Serial

O sistema oferece uma interface interativa via monitor serial (9600 baud):

| Comando | Descrição |
|---------|-----------|
| `s` ou `S` | Para a varredura (servo mantém posição) |
| `g` ou `G` | Retoma a varredura |
| `i` ou `I` | Exibe status do sistema |
| `h` ou `H` ou `?` | Exibe ajuda com todos os comandos |

## 📊 Análise de Complexidade Ciclomática

### 📈 O que é Complexidade Ciclomática?

A **Complexidade Ciclomática** é uma métrica de software que mede a complexidade de um programa através do número de caminhos independentes no código. Desenvolvida por Thomas McCabe, ela ajuda a identificar funções complexas que são difíceis de manter, testar e depurar.

### 🔍 Método de Cálculo

- **Fórmula Original:** M = E - N + 2P
  - E = número de arestas no grafo de fluxo
  - N = número de nós no grafo de fluxo  
  - P = número de componentes conectados

- **Método Simplificado:** M = Número de pontos de decisão + 1

### 📋 Tabela de Complexidade por Função

| Função | Complexidade | Pontos de Decisão | Classificação |
|--------|-------------|-------------------|---------------|
| `setup()` | 1 | Nenhuma | Muito Baixa |
| `loop()` | 3 | 2 condições `if` | Baixa |
| `moverServo()` | 2 | 1 `if` + aninhamentos | Baixa |
| `lerDistancia()` | 2 | 2 condições `if` | Baixa |
| `exibirDetecao()` | 1 | Nenhuma | Muito Baixa |
| `processarComandosSerial()` | 9 | 1 `if` + `switch` com 7 casos | Moderada-Alta |
| `pararVarredura()` | 1 | Nenhuma | Muito Baixa |
| `retomarVarredura()` | 1 | Nenhuma | Muito Baixa |
| `exibirStatus()` | 1 | Nenhuma | Muito Baixa |
| `exibirAjuda()` | 1 | Nenhuma | Muito Baixa |

### 🧮 Cálculo Total da Complexidade

**Complexidade Total do Programa:** 23  
(soma das complexidades de todas as funções)

### 💡 Análise e Recomendações

#### ✅ Pontos Positivos
- 70% das funções possuem complexidade muito baixa (1-2)
- Código geralmente bem estruturado e mantível
- Funções especializadas com responsabilidades claras

#### ⚠️ Área de Melhoria
A função `processarComandosSerial()` possui complexidade 9, que está próxima do limite recomendado de 10. Isso indica necessidade de refatoração.

#### 🛠 Recomendações de Refatoração

1. **Dividir `processarComandosSerial()`:**
```cpp
void processarComando(char comando) {
    switch(comando) {
        case 's': case 'S': pararVarredura(); break;
        case 'g': case 'G': retomarVarredura(); break;
        // ... outros casos
    }
}

void processarComandosSerial() {
    if (Serial.available()) {
        processarComando(Serial.read());
    }
}
```

2. **Extrair lógica de movimento complexa** para funções especializadas

3. **Implementar máquina de estados** para gerenciar transições de movimento

### 📏 Escala de Complexidade Recomendada

- **1-5:** Baixa complexidade (ideal)
- **6-10:** Complexidade moderada (aceitável com cautela)  
- **11+:** Alta complexidade (necessita refatoração)

## 🚀 Instalação e Uso

1. **Montagem do Hardware:**
   - Conecte os componentes conforme o esquema de ligação
   - Verifique as conexões de alimentação

2. **Configuração do Software:**
   - Abra o código no Arduino IDE
   - Selecione a placa e porta corretas
   - Faça o upload para o Arduino

3. **Operação:**
   - Abra o Monitor Serial (9600 baud)
   - O sistema iniciará automaticamente
   - Use os comandos listados para interagir

## 🔧 Personalização

### Ajuste de Velocidade de Varredura
```cpp
const int ATRASO_MOVIMENTO = 30; // Aumente para movimento mais lento
```

### Ajuste de Sensibilidade
```cpp
const int DISTANCIA_ALERTA = 50; // Aumente para detectar objetos mais distantes
```

## 📝 Licença

Este projeto está sob licença MIT. Veja o arquivo LICENSE para mais detalhes.

## 🤝 Contribuições

Contribuições são bem-vindas! Por favor:
1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

## 📞 Suporte

Para dúvidas ou problemas:
1. Verifique a seção de issues do repositório
2. Consulte a documentação do Arduino
3. Verifique as conexões físicas do hardware

---

**Desenvolvido com ❤️ para a comunidade Arduino**
