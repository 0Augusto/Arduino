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
