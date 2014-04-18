//Programa : Ethernet Shield W5100 e HC-SR04
//Alteracoes e adaptacoes : Ezequiel Franca
//
//Baseado no programa exemplo de
//by David A. Mellis e Tom Igoe
 
#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>

#include <aJSON.h>
#include <avr/pgmspace.h>

 
//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6 //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7 //Porta ligada ao pino Echo do sensor
//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
 
//Definicoes de IP, mascara de rede e gateway
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,200);          //Define o endereco IP
IPAddress gateway(192,168,1,1);      //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede

//Coisas para o Json
char* string;
 
//Inicializa o servidor web na porta 80
EthernetServer server(80);
 
void setup()
{
  //Inicializa Serial
    Serial.begin(9600);
    Serial.println("OLA SERIAL");
  
  //Inicializa a interface de rede
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void criarJson(float medidaSensor){
//    char* json;
//    aJsonObject *root,*fmt;
//    root=aJson.createObject();  
//    aJson.addItemToObject(root, "distancia", aJson.createItem(medidaSensor));
//    aJson.addItemToObject(root, "temperatura", fmt = aJson.createObject());
//    aJson.addStringToObject(fmt,"Umidade",     "rect");
//    aJson.addNumberToObject(fmt,"Ruido",        1920);
//    aJson.addNumberToObject(fmt,"Contato",       1080);
//    aJson.addFalseToObject (fmt,"interlace");
//    aJson.addNumberToObject(fmt,"frame rate",   24);
//    json = aJson.print(root);
//  return json;

Serial.print(" \{ \"Sensores\" : [ { \"temperatura\": \"25\", \"umidade\": \"50%\", \"distancia\":");
Serial.print((float)(medidaSensor),1);
Serial.print("] } "); 
}
 
void loop() {
  float cmMsec;
  long microsec = ultrasonic.timing();
  //Le e armazena as informacoes do sensor ultrasonico
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  criarJson(cmMsec);
    
//  if (string != NULL) {
    Serial.println(string);
  //} 
  //else {
   // Serial.println("ERRO NO JSON");
  //}
 
 
  //Aguarda conexao do browser
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
      client.println("Refresh: 2");  //Recarrega a pagina a cada 2 segundos
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#FF0000><b><u>");
          client.print("Envio de informacoes pela rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          //Mostra o estado da porta digital 3
          int porta_digital = digitalRead(3);
          client.print("Porta Digital 3 : ");
          client.print("<b>");
          client.print(porta_digital);
          client.println("</b>");
          client.print("  (0 = Desligada, 1 = Ligada)");
          client.println("<br />");
          //Mostra as informacoes lidas pelo sensor ultrasonico
          client.print("Sensor Ultrasonico : ");
          client.print("<b>");
          client.print(cmMsec);
          client.print(" cm");
          client.println("</b></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    }
}
