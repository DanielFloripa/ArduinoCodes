#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup(){
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  Serial.begin(9600);
  lcd.print("Ola, bem vindo!");
  delay(3500);
}

void loop(){
  int x=analogRead(A0);
  Serial.println(analogRead(A1));
  //lcd.print(x);
  lcd.setCursor(0, 0);
  if(x==722)
  {
lcd.print("Select PRESSIONADO");
  }
  if(x==480)
  {
lcd.print("LEFT PRESSIONADO");
  }
  if(x==132)
  {
lcd.print("UP PRESSIONADO");
  }
  if(x==308)
  {
lcd.print("DOWN PRESSIONADO");
  }
  if(x==0)
  {
lcd.print("RIGHT PRESSIONADO");
  }
  if(x>1000)
lcd.print("                ");
}

