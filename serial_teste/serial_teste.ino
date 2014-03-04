
//Teste serial
//Hackathon Raspberry Pi - @soujava cpbr7 - 2013
//Ezequiel França

int led = 13;

void setup() {
  pinMode(led, OUTPUT);  
  Serial.begin(115200);
  Serial.println("Teste de Serial");
  Serial.println("---------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    char caracter;
    caracter = Serial.read();
    
    if(caracter=='a'){
      digitalWrite(led, HIGH);
    } 
    
    if(caracter=='b') {
      digitalWrite(led, LOW);   
    }
  }
}
