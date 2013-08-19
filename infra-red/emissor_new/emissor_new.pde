int sensor=7;
int BAUD=300;
int LED=13;


void setup(){
  Serial.begin(BAUD);
  pinMode(sensor, OUTPUT);
}
 
void loop(){
  // if incoming serial
  if (Serial.available()) {
    readSerial();
    digitalWrite(sensor, HIGH);
  } else {
    digitalWrite(sensor, LOW);
  }
  delay(10);
 
}
 
void readSerial(){
  char val = Serial.read();
  Serial.print(val);
  //val = 1;
  if ((val == 'ÿ')||(val == 'ü')){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }
    
}
