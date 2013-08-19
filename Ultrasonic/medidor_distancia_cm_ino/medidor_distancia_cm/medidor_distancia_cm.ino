#include "Ultrasonic.h"

Ultrasonic ultrasonic(12,13);
 
void setup(){
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
}

void loop(){
  digitalWrite(12, LOW);
  delay(2);
  digitalWrite(12, HIGH);
  delay(10);
  digitalWrite(12, LOW);
  
  int dist=(ultrasonic.Ranging(CM));
  
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.println(" CM");
  delay(500);
}
  
