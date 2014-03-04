#include <Servo.h> //Biblioteca utilizada
#define SERVO 6 //Porta utilizada para o motor

//Explicação sobre bibliotecas e include na legenda do código

Servo s; //Variável utilizada para o servo motor
int pos; //Posição para deixar o motor

void setup (){
  s.attach(SERVO);
  pos = 0;
  Serial.begin(9600);
  s.write(pos); //Começa o motor na posição 0
}
void loop(){
  delay(20000);
  for(pos = -120; pos < 10; pos++){
    s.write(pos);
    delay(1);
  }
  delay(5000);
  for(pos = 10; pos > -120; pos--){
    s.write(pos);
    delay(1);
  }
  delay(10);
}
