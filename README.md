# Sistema de Monitoramento com Arduino

Este projeto inclui dois códigos separados para monitoramento de temperatura/umidade e medição de distância usando sensores com Arduino.

## Código 1: Monitor de Temperatura e Umidade com Média Móvel

### Descrição
Este código utiliza um sensor DHT11 para medir temperatura e umidade ambiente, calculando uma média móvel das últimas 5 leituras para suavizar os dados e fornecer valores mais estáveis.

### Características
- Medição de temperatura e umidade com sensor DHT11
- Cálculo de média móvel para suavizar flutuações
- Saída serial dos valores médios a cada 2 segundos
- Tratamento de erros na leitura do sensor

### Componentes Necessários
- Arduino (Qualquer modelo compatível)
- Sensor DHT11
- Cabos de conexão
- Resistor de 10kΩ (pull-up para o sensor DHT11)

### Esquema de Ligação
- Pino VCC do DHT11 → 5V do Arduino
- Pino DATA do DHT11 → Pino Digital 2 do Arduino
- Pino GND do DHT11 → GND do Arduino
- (Opcional) Resistor de 10kΩ entre VCC e DATA

### Instalação e Uso
1. Conecte o sensor DHT11 ao Arduino conforme o esquema
2. Instale a biblioteca DHT sensor library no IDE Arduino
3. Carregue o código no Arduino
4. Abra o Monitor Serial (9600 baud) para visualizar as leituras

### Estrutura do Código
- `numLeituras`: Define o tamanho da janela para a média móvel (5 leituras)
- Arrays `leiturasTemp` e `leiturasUmid`: Armazenam as leituras históricas
- Função `setup()`: Inicializa a comunicação serial e o sensor
- Função `loop()`: Realiza leituras, atualiza arrays e calcula médias

### Autor
Henrique Augusto

---

## Código 2: Medidor de Distância com Sensor Ultrassônico

### Descrição
Este código utiliza um sensor HC-SR04 para medir distâncias com tecnologia de ultrassom, exibindo os resultados em centímetros no monitor serial.

### Características
- Medição precisa de distâncias entre 2cm e 400cm
- Saída serial dos valores em centímetros
- Leituras a cada 500ms

### Componentes Necessários
- Arduino (Qualquer modelo compatível)
- Sensor ultrassônico HC-SR04
- Cabos de conexão

### Esquema de Ligação
- Pino VCC do HC-SR04 → 5V do Arduino
- Pino Trig do HC-SR04 → Pino Digital 5 do Arduino
- Pino Echo do HC-SR04 → Pino Digital 4 do Arduino
- Pino GND do HC-SR04 → GND do Arduino

### Instalação e Uso
1. Conecte o sensor HC-SR04 ao Arduino conforme o esquema
2. Carregue o código no Arduino
3. Abra o Monitor Serial (9600 baud) para visualizar as distâncias medidas

### Princípio de Funcionamento
1. Envia um pulso ultrassônico de 10μs através do pino Trig
2. Mede o tempo de retorno do eco no pino Echo
3. Calcula a distância usando a fórmula: distância = (duração × velocidade do som) / 2

### Estrutura do Código
- `trigPin` (Pino 5): Controla a emissão do pulso ultrassônico
- `echoPin` (Pino 4): Captura o retorno do eco
- Função `pulseIn()`: Mede a duração do pulso de retorno
- Cálculo de distância considerando a velocidade do som (0.034 cm/μs)

---

## Aplicações no Sistema de Irrigação Automatizado

Estes sensores podem ser integrados ao sistema de irrigação automatizado:

1. **Sensor DHT11**: Monitora condições ambientais para ajustar a irrigação com base na temperatura e umidade do ar
2. **Sensor Ultrassônico**: Pode ser usado para medir o nível de água em reservatórios, prevenindo operação a seco

## Possíveis Melhorias
- Integrar ambos os sensores em um único sketch
- Adicionar transmissão de dados via Wi-Fi (para Arduino Uno R4 WiFi)
- Implementar alertas quando os valores ultrapassarem limites pré-definidos
- Adicionar display LCD para visualização local dos dados

## Notas Importantes
- Para medições mais precisas com o DHT11, calibre o sensor comparando com um termômetro/higrômetro de referência
- O sensor ultrassônico pode ter leituras imprecisas em superfícies irregulares ou absorventes
- Ambos os sensores podem ser afetados por condições ambientais extremas

# Sistema de Monitoramento com Arduino

## 📋 Visão Geral
Este projeto evoluiu de dois códigos separados para um sistema integrado completo de monitoramento para irrigação automatizada, utilizando Arduino Uno R4 WiFi com sensores de temperatura/umidade e distância, display LCD e comunicação WiFi/MQTT.

## 🔄 Evolução do Projeto

### Versões Anteriores

#### Código 1: Monitor de Temperatura e Umidade com Média Móvel
**Descrição**: Utilizava apenas o sensor DHT11 para medir temperatura e umidade ambiente com cálculo de média móvel.

#### Código 2: Medidor de Distância com Sensor Ultrassônico
**Descrição**: Utilizava apenas o sensor HC-SR04 para medição de distâncias via ultrassom.

---

## 🚀 Código 3: Sistema Integrado Completo (Versão Atual)

### Descrição
Este código integra todos os componentes em um sistema completo de monitoramento para irrigação automatizada, com display LCD, comunicação WiFi/MQTT e sistema de alertas.

### Características Avançadas
- **Monitoramento integrado** de temperatura, umidade e distância/nível de água
- **Display LCD 16x2** para visualização local em tempo real
- **Comunicação WiFi** com Arduino Uno R4 WiFi
- **Protocolo MQTT** para transmissão de dados e alertas
- **Sistema de alertas** visual e remoto para condições críticas
- **Médias móveis** para leituras estáveis e precisas
- **Interface não-bloqueante** com temporizadores independentes

### Componentes Necessários
- Arduino Uno R4 WiFi
- Sensor DHT11 (temperatura e umidade)
- Sensor ultrassônico HC-SR04
- Display LCD 16x2 com controlador HD44780
- Potenciômetro 10K (para ajuste de contraste)
- Protoboard e cabos jumper
- Fonte de alimentação 9-12V

### Esquema de Ligação Completo

#### Display LCD (Modo 4 bits):
| Pino LCD | Pino Arduino |
|----------|-------------|
| VSS      | GND         |
| VDD      | 5V          |
| V0       | Potenciômetro (contraste) |
| RS       | Digital 12  |
| EN       | Digital 11  |
| D4       | Digital 10  |
| D5       | Digital 9   |
| D6       | Digital 8   |
| D7       | Digital 7   |
| A        | 5V (com resistor 220Ω) |
| K        | GND         |

#### Sensor DHT11:
| Pino DHT11 | Pino Arduino |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| DATA       | Digital 2   |

#### Sensor HC-SR04:
| Pino HC-SR04 | Pino Arduino |
|--------------|-------------|
| VCC          | 5V          |
| GND          | GND         |
| Trig         | Digital 5   |
| Echo         | Digital 4   |

### Bibliotecas Necessárias
- DHT Sensor Library
- LiquidCrystal (para display)
- ArduinoMqttClient
- WiFiS3 (já incluída no Arduino Uno R4 WiFi)

### Configuração e Instalação
1. Conecte todos os componentes conforme o esquema de ligação
2. Instale as bibliotecas necessárias via Arduino IDE
3. Configure as credenciais WiFi no código:
   ```cpp
   char ssid[] = "SUA_REDE_WIFI";
   char pass[] = "SUA_SENHA_WIFI";
   ```
4. Ajuste os limites de alerta conforme necessário:
   ```cpp
   const float TEMP_ALTA = 35.0;    // Temperatura máxima
   const float TEMP_BAIXA = 10.0;   // Temperatura mínima
   const float UMID_ALTA = 80.0;    // Umidade máxima
   const float UMID_BAIXA = 30.0;   // Umidade mínima
   const float DIST_ALERTA = 10.0;  // Nível mínimo (cm)
   ```
5. Carregue o código no Arduino Uno R4 WiFi

### Funcionamento Detalhado

#### Inicialização
1. O sistema inicia mostrando "Inicializando..." no LCD
2. Conecta-se automaticamente à rede WiFi configurada
3. Estabelece conexão com o broker MQTT
4. Inicializa os arrays para cálculo de médias móveis

#### Loop Principal
O sistema opera com três temporizadores independentes:
1. **Leitura de Sensores** (a cada 2 segundos)
2. **Atualização do Display** (a cada 1 segundo)
3. **Comunicação MQTT** (a cada 5 segundos)

#### Processamento de Dados
- Leituras dos sensores são armazenadas em arrays circulares
- Médias móveis são calculadas sobre as últimas 5 leituras
- Dados são exibidos no LCD no formato:
  ```
  T:25.5C U:45%   (Linha 1)
  N:25.4cm        (Linha 2)
  ```

#### Sistema de Alertas
O sistema monitora continuamente:
- Temperatura acima de 35°C ou abaixo de 10°C
- Umidade acima de 80% ou abaixo de 30%
- Nível de água abaixo de 10cm

Quando um alerta é detectado:
- Envia mensagem MQTT para o tópico de alertas
- Aciona alerta visual (LCD piscante)
- Registra o alerta no monitor serial

### Estrutura do Código

#### Variáveis Principais
- `leiturasTemp`, `leiturasUmid`, `leiturasDist`: Arrays para médias móveis
- `indice`: Índice circular para os arrays
- `ultimaLeitura`, `ultimoDisplay`, `ultimoMQTT`: Controladores de tempo

#### Funções Principais
1. `lerSensores()`: Realiza leituras do DHT11 e HC-SR04
2. `calcularMedia()`: Calcula média móvel dos valores
3. `atualizarDisplay()`: Atualiza as informações no LCD
4. `conectarWiFi()`: Gerencia conexão WiFi
5. `conectarMQTT()`: Gerencia conexão MQTT
6. `enviarDadosMQTT()`: Envia dados via MQTT
7. `verificarAlertas()`: Verifica condições de alerta
8. `enviarAlerta()`: Envia alertas via MQTT

### Aplicações no Sistema de Irrigação Automatizado

#### Monitoramento Ambiental
- **Sensor DHT11**: Monitora condições ambientais para ajustar a irrigação
- **Sensor HC-SR04**: Mede o nível de água em reservatórios

#### Controle Automatizado
- Acionamento de válvulas solenoides baseado na umidade do solo
- Controle de bombas de água baseado no nível do reservatório
- Adaptação do sistema de irrigação conforme condições climáticas

#### Monitoramento Remoto
- Visualização de dados em tempo real via MQTT
- Recebimento de alertas por email ou notificação push
- Integração com sistemas de automação residencial

### Personalização e Expansão

#### Adaptação para Outros Sensores
O código pode ser facilmente adaptado para:
- Sensores de umidade do solo
- Sensores de vazão de água
- Sensores de pH para hidroponia

#### Modificação de Intervalos
Ajuste os intervalos de operação modificando:
```cpp
const long intervaloLeitura = 2000;   // Leitura a cada 2 segundos
const long intervaloDisplay = 1000;   // Display a cada 1 segundo
const long intervaloMQTT = 5000;      // MQTT a cada 5 segundos
```

#### Integração com Plataformas IoT
- Arduino IoT Cloud
- Blynk
- Node-RED
- Home Assistant

### Notas Importantes

#### Calibração de Sensores
- Para medições precisas com o DHT11, compare com um termômetro/higrômetro de referência
- Calibre o sensor ultrassônico medindo distâncias conhecidas
- Ajuste os valores de "seco" e "molhado" conforme seu solo específico

#### Considerações de Precisão
- Sensores ultrassônicos podem ter leituras imprecisas em superfícies irregulares
- O DHT11 pode ser afetado por condições ambientais extremas
- Médias móveis ajudam a suavizar leituras erráticas

#### Otimização de Energia
- Para sistemas alimentados por bateria, considere implementar modos de sleep
- Reduza a frequência de leituras quando possível
- Desative o backlight do LCD quando não necessário

### Solução de Problemas

#### Conexão WiFi
- Verifique as credenciais WiFi no código
- Certifique-se de que a rede está disponível
- Verifique a força do sinal WiFi

#### Comunicação MQTT
- Verifique se o broker MQTT está acessível
- Confirme a porta e endereço do broker
- Verifique se o firewall não está bloqueando a conexão

#### Leituras Inconsistentes
- Verifique as conexões dos sensores
- Confirme a alimentação adequada (5V estáveis)
- Verifique interferências elétricas

### Autor
Henrique Augusto

### Licença
Este projeto está sob licença MIT. Veja o arquivo LICENSE para detalhes.

---

## 📞 Suporte
Para dúvidas e sugestões, entre em contato através do repositório do projeto ou via email.

