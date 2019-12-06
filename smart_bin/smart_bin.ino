#include <Servo.h>

Servo SV;

int outer_trig = 8;
int outer_echo = 9;

int inner_trig = 4;
int inner_echo = 5;

int full_cnt = 0;

void setup() {
  Serial.begin(9600);  
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
    Serial.println("-------------- Send Alert -------------");
  }
  
  delay(1000);
}
