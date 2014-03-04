#include 
 
dht11 sensor;
 
void setup() {
Serial.begin(9600);
}
 
void loop() {
Serial.print("Lendo sensor: ");
int chk = sensor.read(2);
switch(chk) {
case DHTLIB_OK:
Serial.println("OK");
break;
case DHTLIB_ERROR_CHECKSUM:
Serial.println("Erro no checksum");
break;
case DHTLIB_ERROR_TIMEOUT:
Serial.println("Tempo esgotado");
break;
default:
Serial.println("Erro desconhecido");
}
Serial.print("Umidade (%): ");
Serial.println((float)sensor.humidity, 2);
Serial.print("Temperatura (graus Celsius): ");
Serial.println((float)sensor.temperature, 2);
delay(2000);
}
