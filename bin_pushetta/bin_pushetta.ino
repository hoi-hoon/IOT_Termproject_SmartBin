#include "WiFiEsp.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif


String userKey = "";     // your Pushover user key
String apiToken = "";    // your Pushover API token
String Title = "ESP8266 Pushover Test";                // your title of the message
 
char ssid[] = "Hoi";                        
char password[] = "123123123";
int status = WL_IDLE_STATUS; 

int length;

WiFiEspClient client;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
}
 
void loop() {
   if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, password);
  }

  Serial.println("You're connected to the network");
  
  sendToPushover("* Door OPEN !!! * 문이 열렸습니다 *");  
  while(true);
}
 
void sendToPushover(String Msg) {
  Msg = Msg+"&title="+Title;
  length = Msg.length()+81;
  if (client.connect("api.pushover.net", 80)) {
    client.println("POST /1/messages.json HTTP/1.1");
    client.println("Host: api.pushover.net");
    client.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(length);
    client.println("\r\n");
    client.print("token="+apiToken+"&user="+userKey+"&message="+Msg);
    client.stop();
  }
} 
