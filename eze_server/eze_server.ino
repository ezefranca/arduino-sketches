//simple button GET server code to control servo and arduino pin 5
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html
//for use with W5100 based ethernet shields
//Powering a servo from the arduino usually DOES NOT WORK.
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605
 
#include <SPI.h>
#include <Ethernet.h>
  
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 37 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port
 
String readString;
 
//////////////////////
 
void setup(){
 
  pinMode(6, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  //Ethernet.begin(mac, ip);
  server.begin();
  //the pin for the servo co
  //enable serial data print
  Serial.begin(9600);
  Serial.println("server LED test 1.0"); // so I can keep track of what is loaded
}
 
void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        //read char by char HTTP request
        if (readString.length() < 100) {
 
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
 
        //if HTTP request has ended
        if (c == '\n') {
 
          ///////////////
          Serial.println(readString); //print to serial monitor for debuging
 
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
 
          client.println("<html>");
          client.println("<head>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
        //  client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />");
          client.println("<title>Home Automation</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<H1>Home Automation</H1>");
          client.println("<hr />");
          client.println("<br />");
         
          client.println("<a href=\"/?lighton\"\">Turn On Light</a>");
          client.println("<a href=\"/?lightoff\"\">Turn Off Light</a><br />");        
 
          client.println("</body>");
          client.println("</html>");
 
          delay(1);
          //stopping client
          client.stop();
 
          ///////////////////// control arduino pin
          if(readString.indexOf("?lighton") >0)//checks for on
          {
            digitalWrite(6, HIGH);    // set pin 4 high
            Serial.println("Led On");
          }
          else{
          if(readString.indexOf("?lightoff") >0)//checks for off
          {
            digitalWrite(6, LOW);    // set pin 4 low
            Serial.println("Led Off");
          }
          }
          //clearing string for next read
          readString="";
 
        }
      }
    }
  }
}
