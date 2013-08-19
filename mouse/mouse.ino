int sensorPin0 = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue0 = 0;  // variable to store the value coming from the sensor
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;
int sensorValue4 = 0;
int sensorValue5 = 0;

void setup() {
 Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  delay(1000);
sensorValue0 = analogRead(A0);
Serial.print("A0: ");
Serial.println(sensorValue0);

sensorValue1 = analogRead(A1);
Serial.print("A1: ");
Serial.println(sensorValue1);

sensorValue2 = analogRead(A2);
Serial.print("A2: ");
Serial.println(sensorValue2);

sensorValue3 = analogRead(A3);
Serial.print("A3: ");
Serial.println(sensorValue3);

sensorValue4 = analogRead(A4);
Serial.print("A4: ");
Serial.println(sensorValue4);

sensorValue5 = analogRead(A5);
Serial.print("A5: ");
Serial.println(sensorValue5);
Serial.println("\n");

}
