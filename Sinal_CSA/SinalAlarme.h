                /*______________________________________________
                 *      SISTEMA AUTOMATICO DE SINAL             |
                 *      Colégio   Santo    Antônio              |
                 *                                              |
                 * DANIEL CAMARGO                               |
                 * daniel@colmeia.udesc.br                      |
                 * Inicio : 02/07/2013                          |
                 *                                              |
                 * RTC de Felipe Couto - 10/03/2012             |
                 *                                              |
                 * NTP de OORC de Brandon Bearden 21/03/12      |
                 *_____________________________________________*/

#ifndef SINALALARME_H
#define	SINALALARME_H

#include <Arduino.h>

/*CONFIGURACAO LCD
 * Se utilizar uma shield LCD, descomente as
 * duas linhas abaixo e as espelhadas pelo código.
 */
//#include <LiquidCrystal.h>            //Biblioteca do LCD.
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  //pinagem padrao da shield LCD+keypad

/*CONFIGURACAO DE REDE PARA CHIP 28J60
        * GND - -> GND
        * SCK -> D13
        * SO  -> D12
        * SI  -> D11
        * CS  -> D10
        * VCC 5V ->5V OU IOREF
        * INT -> D2
        * RESET -> RESET
        *________________*/

#include <etherShield.h>
#include <ETHER_28J60.h>
ETHER_28J60 ether; //APENAS INSTANCIANDO

/*CONFIGURACAO NTP TIME SERVER
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>
IPAddress timeServer(200,160,7,186);    //lista disponivel em: http://www.ntp.br/NTP/MenuNTPLinuxBSD
const long timeZoneOffset = -10800L;    // Zona -3h em segundos = 60*60*(-3) 
uint8_t ntpSyncTime = 15;
unsigned int localPort = 8888;
const int NTP_PACKET_SIZE= 48;
byte packetBuffer[NTP_PACKET_SIZE];
EthernetUDP Udp;
unsigned long ntpLastUpdate = 0;
time_t prevDisplay = 0; */

/*CONFIGURACAO RTC*/
#include <Wire.h>               //Comunicação analogica
#include <DS1307new.h>          //Biblioteca do RTC
char dateTime[20];
char date[10];
char time[10];
char proximo[10];
int RTCValues[7];

/*protótipos das funções*/
bool configuraRTC();
void getDateTime();
void mostraDataHora();
bool isFeriado();
bool isFimDeSemana();
bool isHorario();
void alarme();
bool getStatus();

#endif	/* SINALALARME_H */

