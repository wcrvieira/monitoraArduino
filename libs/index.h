#include <Servo.h>

// Definição dos pinos (constantes em letras maiúsculas)
const int PIN_VERMELHO = 2;
const int PIN_AMARELO   = 3;
const int PIN_VERDE     = 4;
const int PIN_SERVO     = 5;
const int PIN_ECHO      = 7;
const int PIN_TRIG      = 8;

// Instância do Servo
Servo servo1;

// Variáveis para controle de tempo do Servo (Substituindo o delay)
unsigned long tempoAnteriorServo = 0;
const long intervaloServo = 2000; // Tempo em milissegundos para mexer o servo
int posicaoServo = 0;

void setup() {
  // Inicialização do Servo
  servo1.attach(PIN_SERVO);
  servo1.write(posicaoServo);

  // Configuração dos pinos do Semáforo
  pinMode(PIN_VERMELHO, OUTPUT);
  pinMode(PIN_AMARELO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);

  // Configuração dos pinos do Sensor Ultrassônico
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Inicialização da comunicação Serial
  Serial.begin(9600);
  apagarSemaforo();
}

void loop() {
  // 1. Controle do Servo sem travar o código (Máquina de Estados/Millis)
  unsigned long tempoAtual = millis();
  
  if (tempoAtual - tempoAnteriorServo >= intervaloServo) {
    tempoAnteriorServo = tempoAtual;
    
    // Altera a posição do servo alternadamente
    if (posicaoServo == 0) {
      posicaoServo = 180;
    } else {
      posicaoServo = 0;
    }
    servo1.write(posicaoServo);
  }

  // 2. Leitura do Sensor Ultrassônico
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // pulseIn retorna unsigned long, alterado para evitar problemas de estouro
  unsigned long duracao = pulseIn(PIN_ECHO, HIGH);
  
  // Cálculo da distância
  float distancia = duracao * 0.0343 / 2;

  // Exibição no Monitor Serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // 3. Controle do Semáforo baseado na distância
  if (distancia > 0 && distancia <= 50) { // Garante que leituras zeradas/erros não acionem o vermelho
    digitalWrite(PIN_VERMELHO, HIGH);
    digitalWrite(PIN_AMARELO, LOW);
    digitalWrite(PIN_VERDE, LOW);
  } 
  else if (distancia > 50 && distancia <= 100) {
    digitalWrite(PIN_VERMELHO, LOW);
    digitalWrite(PIN_AMARELO, HIGH);
    digitalWrite(PIN_VERDE, LOW);
  } 
  else {
    digitalWrite(PIN_VERMELHO, LOW);
    digitalWrite(PIN_AMARELO, LOW);
    digitalWrite(PIN_VERDE, HIGH);
  }

  delay(60); // Pequeno intervalo recomendado entre leituras do HC-SR04
}

void apagarSemaforo() {
  digitalWrite(PIN_VERMELHO, LOW);
  digitalWrite(PIN_AMARELO, LOW);
  digitalWrite(PIN_VERDE, LOW);
}