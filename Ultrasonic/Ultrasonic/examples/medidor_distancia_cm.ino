#include "Ultrasonic.h"
#define echoPin 13
#define trigPin 12

Ultrasonic ultrasonic(12,13);
 
void setup(){
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop(){
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigpin, LOW);
  
  int dist=(ultrasonic.Ranging(CM));
  
  Serial.print("Distancia em cm: ");
  Serial.println(dist);
  delay(1000);
}
  
