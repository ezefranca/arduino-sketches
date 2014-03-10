#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,10,177);

char cmd[15];
char param1[15];
boolean systemArmed = true;

#define STRING_BUFFER_SIZE 128
char buffer[STRING_BUFFER_SIZE];

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void send404(EthernetClient client) {
     client.println("HTTP/1.1 404 OK");
     client.println("Content-Type: text/html");
     client.println("Connnection: close");
     client.println();
     client.println("<!DOCTYPE HTML>");
     client.println("<html><body>404</body></html>");
}

void sendStatus(EthernetClient client) {
 
   // Send a standard http response header
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: application/json");
   client.println("Connnection: close");
   client.println();
   client.println("{");

   // Output the value of each analog input pin
   for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
       int sensorReading = analogRead(analogChannel);
       client.print("\"analog_in_");
       client.print(analogChannel);
       client.print("\": ");
       client.print(sensorReading);
       
//       if (analogChannel != 5) {
           client.println(",");  
//       }
   }
   client.print("\"system_armed\": ");
   client.print( systemArmed );
   client.println("\n}");
}

/**
 * Parse the string and return an array which contains all path segments
 */
char** parse(char* str) {

	char ** messages;
	messages = (char**)malloc(sizeof(char *));
	char *p;
	p = strtok(str, " ");
	unsigned int i = 0;
	while (p != NULL) {
	  p = strtok(NULL, "/");
	  char *sp;
	  boolean last = false;
	  sp = strchr(p, ' ');
	  if (sp != NULL) {
            *sp++ = '\0';
	    last = true;
	  }
	  messages[i] = p;
	  i++;
	  if (last) {
	    break;
	  }
	  messages = (char**)realloc(messages, sizeof(char *) * i + 1);
	}

	messages[i] = '\0';
	return messages;
}

void armSystem() {
  systemArmed = true;
}

void disarmSystem() {
  systemArmed = false;
}

void handleCommand(EthernetClient client, char* cmd, char* param) {
    if (strcmp(cmd, "status") == 0) {
      Serial.println("status");
      sendStatus(client); 
    } else if (strcmp(cmd, "arm") == 0) {
      armSystem();
      sendStatus(client);
      Serial.println("arm");
    } else if (strcmp(cmd, "disarm") == 0) {
      disarmSystem();
      sendStatus(client);
      Serial.println("disarm");
    } else {
      send404(client);
    }  
}

int countSegments(char* str) {
  int p = 0;
  int count = 0;
  while (str[p] != '\0') {
    if (str[p] == '/') {
      count++;
    }
    p++;
  }
  // We don't want to count the / in 'HTTP/1.1'
  count--;
  return count;
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        
        char c;
        int bufindex = 0; // reset buffer
        buffer[0] = client.read();
        buffer[1] = client.read();
        bufindex = 2;
        // Read the first line to determin the request page
        while (buffer[bufindex-2] != '\r' && buffer[bufindex-1] != '\n') { 
            // read full row and save it in buffer
            c = client.read();
            if (bufindex<STRING_BUFFER_SIZE) {
              buffer[bufindex] = c;
            }
            bufindex++;
        }
        // Clean buffer for next row
        bufindex = 0;
 
        // Parse the query string
        int nSegments = countSegments(buffer);
        char **pathsegments = parse(buffer);
 
        int i = 0;
        for(i=0; i<nSegments; i++) {
          Serial.println(pathsegments[i]);
        }

        if (c == '\n' && currentLineIsBlank) {
          handleCommand(client, pathsegments[0], pathsegments[1]);
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    // Give the web browser time to receive the data
    delay(1);
    // Close the connection:
    client.stop();
    Serial.println("Client disonnected");
  }
}
