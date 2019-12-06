#include <Servo.h>

Servo SV;

int outer_trig = 8;
int outer_echo = 9;

int inner_trig = 4;
int inner_echo = 9;

void setup() {
  Serial.begin(9600);  
  // Out pin Input Setup
  pinMode(outer_echo, INPUT);
  // Trig pin Output Setup
  pinMode(outer_trig, OUTPUT);
  SV.attach(10);
  digitalWrite(outer_trig, 0);
}

void loop() {
  unsigned long microseconds, distance_cm;
  int full_cnt = 0;
  // Ultrasonic wave transmission
  digitalWrite(outer_trig, 0); // Output pin_ULTRASONIC_T to LOW
  delayMicroseconds(2);
  digitalWrite(outer_trig, 1); // Output pin_ULTRASONIC_T to HIGH       
  delayMicroseconds(10);
  digitalWrite(outer_trig, 0); // Output pin_ULTRASONIC_T to LOW
  
  microseconds = pulseIn(outer_echo, 1, 24000);
  distance_cm = microseconds * 17/1000;
  
  Serial.print("Distance :");
  Serial.print(distance_cm);
  Serial.println("[cm]");
  if(distance_cm<=10){
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
  Serial.print("Inner Distance :");
  Serial.print(distance_cm);
  Serial.println("[cm]");
  if(distance_cm<=10){
    Serial.println("so closed");
    full_cnt++;
  }
  else{
    full_cnt = 0;
  }

  if(full_cnt>10){
    Serial.println("-------------- Send Alert -------------");
  }
  delay(1000);
}
