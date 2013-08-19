//codigo para strobe com potenciometro//

int analog=2;
int razao=6;
int led=13;

void setup(){
 pinMode(led,OUTPUT); 
 Serial.begin(9600);
}

void loop(){

int val=analogRead(analog)/razao;
if(val>=50){
 Serial.println(val);
 digitalWrite(led,HIGH);
 delay(val);
 Serial.println(val);
 digitalWrite(led,LOW);
 delay(val);
 }
 else{
 float reducao=2*sqrt(val)+35;
 Serial.println(reducao);
 digitalWrite(led,HIGH);
 delay(reducao);
 Serial.println(reducao);
 digitalWrite(led,LOW);
 delay(reducao);
 }
}
