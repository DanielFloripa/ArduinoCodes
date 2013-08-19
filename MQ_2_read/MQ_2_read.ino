/* Sample Arduino for MQ2 Smoke Sensor Shield
   05/03/2011
**********************************************/
const int analogInPin = A1;  
int sensorValue = 0;
int outputValue = 0;
 
void setup(){
  Serial.begin(9600); 
}
 
void loop(){
  sensorValue = analogRead(analogInPin);            
  outputValue = map(sensorValue, 0, 1023, 0, 100);       
    Serial.print("\t Poluicao = ");      
    Serial.print(outputValue);
    Serial.println("%");
    if ((outputValue > 20) && (outputValue <= 50))
        Serial.println("\t ATENCAO: POLUICAO MEDIA%."); 
    if ((outputValue > 50) && (outputValue <=75 ))
      Serial.println("\t ATENCAO: POLUICAO ALTA > 50%, CUIDADO!!!");
    if (outputValue > 75)
      Serial.println("\t ATENCAO: POLUICAO ALTÍSSIMA > 75%, SAIA DESTE LOCAL!!!");
  delay(500);
}
