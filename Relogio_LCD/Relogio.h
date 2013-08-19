#ifndef RELOGIO_H
#define RELOGIO_H

#include "Arduino.h"

typedef struct{
  int dia;
  int semana;
  int mes;
  int ano;
  bool ok;
}
Data;

typedef struct{
  int hora;
  int minuto;
  int segundo;
  bool ok;
}Tempo;

void configuraRelogio(Data data, Tempo tempo);
int contaData(Data data, Tempo tempo);
int contaTempo(Tempo tempo);

void configuraRelogio(Data data, Tempo tempo){
  tempo.ok=0;
  while (tempo.ok == 0){
    Serial.print("Entre com hora: ");
    if(Serial.available())
    tempo.hora = Serial.read();
    Serial.println(" \n");
  
    Serial.print("Entre com minuto: ");
    tempo.minuto = Serial.read();
    Serial.println(" \n");
  
    Serial.print("Entre com segundo: ");
    tempo.segundo = Serial.read();
    Serial.println(" \n");
    
    Serial.println("Salvar? (0-nao / 1-sim): ");
    tempo.ok = Serial.read();
  }
  if(tempo.ok){
    Serial.print("Horario configurado: ");
    Serial.print(tempo.hora);
    Serial.print(":");
    Serial.print(tempo.minuto);
    Serial.print(":");
    Serial.print(tempo.segundo);
    Serial.println("\n");
  }
}

int contaData(Data data, Tempo tempo){
  int m=data.mes;
  int d=data.dia;
  int a=data.ano;
  if(contaTempo(tempo))
    d=d+contaTempo(tempo);
    delay(1000);

  if(m==12){	//incrementa ano
    if(d == 31){
      m=1;
      d=1;
      a=a+1;
    }
  }
  if((m==1)||(m==3)||(m==5)||(m==7)||(m==8)||(m==10)){
    if(d == 31){
      m=m+1;
      d=1;
    }
  }
  if((m==4)||(m==6)||(m==9)||(m==11)){
    if(d == 30){
      m=m+1;
      d=1;
    }
  }
  if(m==2){ 
    if(a % 4){	// ano bissexto
      if(d == 29){
        m=m+1;
        d=1;
      }
    }
    else{		//ano normal
      if(d == 28){
        m=m+1;
        d=1;
      }
    }
  }		
}

int contaTempo(Tempo tempo){
  int s=tempo.segundo;
  int m=tempo.minuto;
  int h=tempo.hora;
  //Data *d=data->dia;
  s=0;
  unsigned long tAnt, tAtu;
  while (s <= 60){
   // tAtu=millis();
   tAtu++;
    if (tAtu == 1000){
      s++;
      tAtu = 0;
    }
  }
  //if(s == 60){
  m=m+1;
  s=0;
  //}
  if(m==60){
    h=h+1;
    m=0;
  }
  if (h==24){
    h=0;
    return 1;
  }

}
#endif

