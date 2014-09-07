/*
 *
 *
 * Ezequiel Franca dos Santos
 * Pisca Led com FSK
 * 18/04/2014
 *
 */
 
 
// Biblioteca do Softmodem 
#include <SoftModem.h>

int mq3_analogPin = A3; // connected to the output pin of MQ3 
// Objeto sofmodem
SoftModem modem;

// Pino para o Led
int led = 13;
int sensorPin=2; //Variável para guardar o valor lido pelo sensor
int tempo; //Variável para o tempo de leitura do sensor
int lidomaximo; //Variável para armazenar o maior valor lido durante a leitura
int i; //Variável de contagem
int j; //Variável de contagem


void setup()
{
  // Seta o led como saida
  pinMode(led, OUTPUT);  
  Serial.begin(9600);
 
  delay(1000);
  // Inicializa o Modem
  modem.begin();
}

void loop() {

  while(modem.available()){
    int c = modem.read();
    //Serial.println('recebeu');
    Serial.println(c);
    if(c == 33) {
      bafometro();
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}

int bafometro()
{
  // give ample warmup time for readings to stabilize

  int mq3_value = analogRead(mq3_analogPin);
  //Serial.print('valor bafometro:');
  for(int i = 0; i <= 10; i++)
  modem.write(mq3_value);
  Serial.println(mq3_value);
  delay(10); //Just here to slow down the output.
  return mq3_value;
}



