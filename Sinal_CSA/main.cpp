#include "SinalAlarme.h"
#include "Horarios.h"

//FORNECA UM MAC E UM IP DISPONIVEL NA REDE E COLOQUE AQUI
static uint8_t mac[6] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x24 };
static uint8_t ip[4] = {192, 168, 0, 133}; //USE ESSE ENDEREÇO PARA ACESSAR OS DADOS PELO BROWSER
static uint16_t port = 80;

/*Função para ajustar o tempo via Serial*/
bool configuraRTC(){
        uint8_t _dia, _mes, _ano, _semana, _hora, _minuto, cont=1;
        if( cont == 1){ // O contador garante que nenhum parâmetro seja pulado
                Serial.print("Dia de 1 a 31: ");
                delay(1000);
                _dia = Serial.parseInt(); 
                if (_dia < 1 || _dia > 31) return 0; // se a entrada estiver errada, sai da função
                Serial.println(_dia);
                cont++;
        }
        if( cont == 2){
                Serial.print("Mes de 1 a 12: ");
                delay(1000);
                _mes = Serial.parseInt();
                if (_mes < 1 || _mes > 12) return 0;
                Serial.println(_mes);
                cont++;
        }
        if( cont == 3){
                Serial.print("Ano de 0 a 99: ");
                delay(1000);
                _ano = Serial.parseInt();
                if (_ano > 99) return 0;
                Serial.println(_ano);
                cont++;
        }
        if( cont == 4){
                Serial.print("Semana de 1 a 7: ");
                delay(1000);
                _semana = Serial.parseInt();
                if (_semana > 7) return 0;
                Serial.println(_semana);
                cont++;
        }
        if( cont == 5){
                Serial.print("Hora de 0 a 23: ");
                delay(1000);
                _hora = Serial.parseInt();
                if (_hora > 23) return 0;
                Serial.println(_hora);
                cont++;
        }
        if( cont == 6){
                Serial.print("Minuto de 0 a 59: ");
                delay(1000);
                _minuto = Serial.parseInt();
                if (_minuto > 59) return 0;
                Serial.println(_minuto);
                cont++;
        }
        DS1307.setDate(_ano, _mes, _dia, _semana, _hora, _minuto, 00);
        return 1; //sucesso
}

void getDateTime() {
        DS1307.getDate(RTCValues);
        if (RTCValues[6] > 59) { //PRIMEIRO AJUSTE -> Se o segundo for maior que 59, o RTC não esta ajustado.
                DS1307.setDate(13, 8, 1, 05, 21, 00, 00); //mudar para getWeb
        } else {
                //RTCValues[x] -> [0]=ANO, [1]=MES, [2]=DIA-MES, [3]=DIA-SEMANA, [4]=HORA, [5]=MINUTO, [6]=SEGUNDO  
                sprintf(dateTime, "%02d/%02d/%02d %02d:%02d", RTCValues[2], RTCValues[1], RTCValues[0], RTCValues[4], RTCValues[5]);
                sprintf(date, "%02d/%02d/%02d", RTCValues[2], RTCValues[1], RTCValues[0]);
                sprintf(time, "%02d:%02d:%02d", RTCValues[4], RTCValues[5], RTCValues[6]);
        }
}

void mostraDataHora() {
        long tAtual = millis();
        Serial.println(tAtual);
/*        if(tAtual%1000 == 0 ){
                tAnterior = tAtual;*/
                 Serial.print(date);
                 Serial.print(" -> ");
                 Serial.println(time);
                 delay(998); //tempo verificado pela diferença ao imprimir
}

bool isFeriado() { //Percorre o vetor feriado[]
        for (int i = 0; i < QTD_FER; i += 2) {
                if (RTCValues[2] == feriado[i] && RTCValues[1] == feriado[i + 1]) { //se dia atual é Feriado && mês atual é Feriado
                        Serial.println("\nFeriado!\n");
                        return 1;
                }
        }
        return 0;
}

bool isFimDeSemana() {
        if ((RTCValues[3] == 1) || (RTCValues[3] == 7)) { //Se domingo ou sábado
                Serial.println("\nFim de Semana!\n");
                return 1;
        }
        return 0;
}

bool isHorario() { //Percorre o vetor horario[]
        for (int i = 0; i < QTD_HOR; i += 2) {
                if (RTCValues[4] == horario[i] && RTCValues[5] == horario[i + 1] && RTCValues[6] == 00) {
                        return 1;
                }
        }
        return 0;
}

/*Função que avalia as três condições necessárias para tocar o alarme*/
void alarme() {
        int tAtual, tAnterior, i = 0;
        if ((!isFimDeSemana() && !isFeriado()) && isHorario()) { //Se não é fim de semana & não é feriado & está no horário[]
                tAnterior = millis(); //usado para evitar o delay()
                do {
                        tAtual = millis();
                        i = tAtual - tAnterior;
                        digitalWrite(9,HIGH);
                        if (i%1000 < 20) // toca o tone a cada 1 segundo com diferença de 20 millis
                                tone(6,i/5,100);
                        Serial.println("ALARME!!!");
                } while(i < 10000); //toca o alarme por 10 segundos 
                digitalWrite(9,LOW);
        }
}

/*Retorna o próximo horário de Alarme, Se está no fim de semana ou feriado*/
bool getStatus(){
        int n=0;
        ether.print("<H1>Horários Agendados:</H1>");
        for(int i = 0; i < QTD_HOR; i += 2){
                n++;
                ether.print(n);
                ether.print("o ALARME: ");
                ether.print(horario[i]);
                ether.print(":");
                ether.print(horario[i + 1]);
                ether.print("</br>");
        }
        if(isFimDeSemana()) 
                ether.print("<br><H2>Fim de Semana!</H2></br>");
        if(isFeriado())
                ether.print("<br><H2>Feriado!</H2></br>");
        if(!isFeriado() && !isFimDeSemana()){
                for (int i = 0; i < QTD_HOR; i += 2) {
                        if (RTCValues[4] <= horario[i] && RTCValues[5] < horario[i + 1] ){
                                ether.print("<br><h3>Proximo Alarme: ");
                                ether.print(horario[i]);
                                ether.print("hs ");
                                ether.print(horario[i + 1]);
                                ether.print("min</h3></br>");
                                Serial.print("Próximo alarme: ");
                                Serial.print(horario[i]);
                                Serial.print(":");
                                Serial.println(horario[i+1]);
                                return 1;
                        }
                }
        }
        return 0;
}

void setup() {
        for(int i=0;i<14;i++){
                pinMode(i,OUTPUT);
                digitalWrite(i,LOW);
        }
        ether.setup(mac, ip, port);
        DS1307.begin();
        Serial.begin(9600);
        pinMode(9, OUTPUT);
        pinMode(5, INPUT);//terra do Tone
}

void loop(){
        getDateTime();
        mostraDataHora();
        alarme();
        if(Serial.read()=='x'){
                Serial.println("Modo de Edição: ");
                if(!configuraRTC())
                        Serial.println("ERRO!!! Entrada fora do padrão!!");
        }
        if (ether.serviceRequest()) {
                Serial.println("Servico online!!!!");
                ether.print("<br>Data:");
                ether.print(RTCValues[2]);
                ether.print("/");
                ether.print(RTCValues[1]);
                ether.print("/");
                ether.print(RTCValues[0]);
                ether.print("</br><br> Hora: ");
                ether.print(RTCValues[4]);
                ether.print("hs:");
                ether.print(RTCValues[5]);
                ether.print("min</br>");
                //getStatus();
                ether.respond();
        }
        else{
                //Serial.println("Servico fora");
                //getStatus();
        }
}