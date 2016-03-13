
#include <SPI.h>
#include <WiFi.h>


char ssid[] = "SLT WIFI";      //  your network SSID (name) 
char pass[] = "936051251v";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(8080);

char msg[255];
byte i;

void setup() {
  pinMode(8,OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  } 
  i=0;
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        msg[i] = client.read();
        //Serial.write(msg[i]);
        
        if (msg[i] == '\n' && currentLineIsBlank) {
          // send a standard http response header
          httpPage(client);
           break;
        }
        if (msg[i] == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        
        else if (msg[i] != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        i++;
      }
    }
    // give the web browser time to receive the data
    delay(1);
      // close the connection:
      client.stop();

      i=0;
      Serial.println("client disonnected");
      if(check(msg)==0)digitalWrite(8,HIGH);
      if(check(msg)==1)digitalWrite(8,LOW);
  }
}
void httpPage(WiFiClient client){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><body>");
          // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<h2>Pet Feeder</h2><br>"); 
          client.println("<h3>supply foods</h3><br>"); 
          client.println("<form action=\"http://192.168.1.2:8080/LED=0\">");
          client.println("<input type=\"submit\" value=\"ON\"/></form><br>");
          client.println("<h3>Check foods</h3><br>"); 
          client.println("<form action=\"http://192.168.1.2:8080/LED=1\">");
          client.println("<input type=\"submit\" value=\"OFF\"/></form>");
          client.println("</body></html>");
}

byte check(char msg[]){
  byte x;
  for(x=50;x<200;x++){
  if(msg[x]=='L' && msg[x+1]=='E'&& msg[x+2]=='D' && msg[x+3]=='=' && msg[x+4]=='1'){Serial.println("found on\n");return 0;}
  if(msg[x]=='L' && msg[x+1]=='E'&& msg[x+2]=='D' && msg[x+3]=='=' && msg[x+4]=='0'){Serial.println("found off\n");return 1;}
  //else{Serial.println("not found\n");}
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

