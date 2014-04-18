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

// Objeto sofmodem
SoftModem modem;

// Pino para o Led
int led = 13;

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
    Serial.println(c);
    if(c == 255) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}
