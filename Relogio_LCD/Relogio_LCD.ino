#include <LiquidCrystal.h>
#include "Relogio.h"
// este eh padrao para minha shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

Data data;
Tempo tempo;

int dados;

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("Ola, bem vindo!");
  delay(3500);
  configuraRelogio(data,tempo);

}

void loop(){
  delay(1000);
      if (Serial.available()) {
        dados = Serial.read();
        Serial.print("Recebido: ");
        Serial.println(dados);
    }
  int botao=analogRead(A0);
  //Serial.println(botao);
  //lcd.print(x);
  lcd.setCursor(0, 0);
  if(botao==722)
  {
lcd.print("Select PRESSIONADO");
  }
  if(botao==480)
  {
lcd.print("LEFT PRESSIONADO");
  }
  if(botao==132)
  {
lcd.print("UP PRESSIONADO");
  }
  if(botao==308)
  {
lcd.print("DOWN PRESSIONADO");
  }
  if(botao==0)
  {
lcd.print("RIGHT PRESSIONADO");
  }
  if(botao>1000)
lcd.print("                ");
}

//int confData(Data)
