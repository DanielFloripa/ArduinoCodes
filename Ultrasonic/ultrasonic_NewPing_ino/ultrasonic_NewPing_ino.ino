#include "NewPing.h"
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
void bar(int dist);
long tAnterior = 0;
//long melody = 1000;
int ledPin = 13, 
    ton = 9,
    potPin = 5,
    ledState = LOW,
    valPot = 0,
    LED[]={4,5,6,7,8},
    i;

void setup()
{
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(ledPin,OUTPUT);
  for(i=0;i<5;i++)
    pinMode(LED[i],OUTPUT);
}

void loop() 
{
  delay(5);  // Wait 5ms between pings (about 200 pings/sec). 29ms should be the shortest delay between pings.
  valPot = analogRead(potPin);
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  int dist=uS / US_ROUNDTRIP_CM;
  if(dist==0)              // se estiver fora de alcance,  
    dist=200;              // entao o maximo sera 200
  int esp=dist*sqrt(dist)+dist;
  long melodyfinal = 2*valPot/dist*sqrt(dist)+valPot;
  
  bar(dist); /*FUNCAO LEDS*/
  
  /*DEBUG SERIAL MONITOR*/
  Serial.print("Ping: ");
  Serial.print(dist); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("cm, e melody ");
  Serial.print(melodyfinal);
  Serial.print(", pot: ");
  Serial.println(valPot);

  /*Funcao DELAY*/
 unsigned long tAtual = millis();
  /* Serial.print(", e time ");
  Serial.println(tAtual - tAnterior);*/
 
  if(tAtual - tAnterior > esp){
    tAnterior = tAtual;  
    if (ledState == LOW){
      ledState = HIGH;
      if (dist>8)
        tone(ton, melodyfinal, dist);
       else
        tone(ton, melodyfinal, dist*sqrt(dist));
    }
    else
      ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
}

void bar(int dist)
{
  if(dist>150){
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], LOW);
    digitalWrite(LED[2], LOW);
    digitalWrite(LED[3], HIGH);
    digitalWrite(LED[4], HIGH);
  }
  if(dist<148 && dist>100){
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], LOW);
    digitalWrite(LED[2], LOW);
    digitalWrite(LED[3], HIGH);
    digitalWrite(LED[4], LOW);
  }
  if(dist<98 && dist>50){
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], LOW);
    digitalWrite(LED[2], HIGH);
    digitalWrite(LED[3], LOW);
    digitalWrite(LED[4], LOW);
  }
  if(dist<48 && dist>25){
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], HIGH);
    digitalWrite(LED[2], LOW);
    digitalWrite(LED[3], LOW);
    digitalWrite(LED[4], LOW);
  }
  if(dist<=25){
    digitalWrite(LED[0], HIGH);
    digitalWrite(LED[1], HIGH);
    digitalWrite(LED[2], LOW);
    digitalWrite(LED[3], LOW);
    digitalWrite(LED[4], LOW);
  }
}
  
