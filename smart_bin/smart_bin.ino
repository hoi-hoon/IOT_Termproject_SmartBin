#include <Servo.h>
#include "WiFiEsp.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

Servo SV;

int outer_trig = 8;
int outer_echo = 9;

int inner_trig = 4;
int inner_echo = 5;

int full_cnt = 0;

String userKey = " ";     // Pushover user key
String apiToken = " ";    // Pushover API token
String Title = "Smart Bin";                
 
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
  SV.attach(10);
  pinMode(outer_echo, INPUT);
  pinMode(outer_trig, OUTPUT);
  pinMode(inner_echo, INPUT);
  pinMode(inner_trig, OUTPUT);

  digitalWrite(outer_trig, 0);
  digitalWrite(inner_trig, 0);
}

void loop() {
  unsigned long microseconds, distance_cm;
  
  digitalWrite(outer_trig, 0);
  delayMicroseconds(2);
  digitalWrite(outer_trig, 1);       
  delayMicroseconds(10);
  digitalWrite(outer_trig, 0);
  
  microseconds = pulseIn(outer_echo, 1, 24000);
  distance_cm = microseconds * 17/1000;

  if(distance_cm<=30){
    // Serial.print(distance_cm);
    Serial.println("Trash In");
    SV.write(100);
    delay(3000);
  }
  else{
    SV.write(10);
    }

  // 
  
  digitalWrite(inner_trig, 0); // Output pin_ULTRASONIC_T to LOW
  delayMicroseconds(2);
  digitalWrite(inner_trig, 1); // Output pin_ULTRASONIC_T to HIGH       
  delayMicroseconds(10);
  digitalWrite(inner_trig, 0); // Output pin_ULTRASONIC_T to LOW

  microseconds = pulseIn(inner_echo, 1, 24000);
  // time to dist
  distance_cm = microseconds * 17/1000;
  Serial.print("inner dist:  ");
  Serial.println(distance_cm);
  if(distance_cm<=20){
    
    Serial.println("so closed");
    full_cnt++;
    Serial.println(full_cnt);
  }
  else{
    full_cnt = 0;
  }

  if(full_cnt==10){
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue
      while (true);
    }
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(ssid, password);
    }
    Serial.println("You're connected to the network");
    sendToPushover("* Bin is full *");
  }
  
  delay(1000);
}

void sendToPushover(String Msg){
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
