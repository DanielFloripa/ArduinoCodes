#include <String.h>
#include <XBee.h>

//********************************************************************************************************************************************************************************************************
#ifndef RFID
#define RFID
//========================================================================================================================================================================================================
#define DATA0 0x01 //Data pin 0 from RFID module                     //   CONFIGURAÇÃO 
#define DATA1 0x02 //Data pin 1 from RFID module                     //        DO
#define D_PORT PORTB                                                 //    MÓDULO RFID
#define D_DIR  DDRB
#define D_IN   PINB
//========================================================================================================================================================================================================
#endif
//********************************************************************************************************************************************************************************************************


//********************************************************************************************************************************************************************************************************
#define PRETO1 50                        //    VALORES LINHA SENSORES IF
#define PRETO2 30

//********************************************************************************************************************************************************************************************************

//********************************************************************************************************************************************************************************************************
#define SALA_A 400                //    POSIÇÃO DAS SALAS em cm
#define SALA_B 600
#define SALA_C 1200
#define SALA_D 1400
//********************************************************************************************************************************************************************************************************


//********************************************************************************************************************************************************************************************************
#define AVANCO 0                //  RESULTADO DAS LEITURAS, DETERMINA SE O ROBÔ AVAÇARÁ OU RETORNARÁ
#define RETORNO 1

//********************************************************************************************************************************************************************************************************


//********************************************************************************************************************************************************************************************************
unsigned char avanco[4] = {231,247,212,0};        // CADASTRO DOS CÓDIGOS DOS CARTÕES DE RFID
unsigned char retorno[4] = {231,90,71,0};
unsigned char card_num[4]={0,0,0,0};
//********************************************************************************************************************************************************************************************************



//********************************************************************************************************************************************************************************************************
int E1 = 5;    // Motor1 esquerda: velocidade
int E2 = 6;    // Motor2 direita: velocidade
int M1 = 4;    // Motor1 esquerda : direcao      // ==== MOTOR ==== //
int M2 = 7;    // Motor2 direita: direcao
int S1 = 10; // Sensor ultrasonico 1 em digital 8
boolean sentido = HIGH;
//********************************************************************************************************************************************************************************************************


//********************************************************************************************************************************************************************************************************
int pinE1 = 3; // Sensor IR da trazeira esquerda
int pinD1 = 5; // Sensor IR da trazeira direita
int pinE2 = 2; // Sensor IR do meio esquerda      // ==== PINOS SENSORES ==== //
int pinD2 = 4; // Sensor IR do meio direita
//********************************************************************************************************************************************************************************************************

//********************************************************************************************************************************************************************************************************
int IF_E1;
int IF_E2;      // ==== VALORES SENSORES ==== //
int IF_D1;
int IF_D2;
//********************************************************************************************************************************************************************************************************

//********************************************************************************************************************************************************************************************************
//   PARAMETROS DO ROBO

//  --- MISSÃO----
int missao[4] = {SALA_A,SALA_B,SALA_C,SALA_D}; // Salas que serão visitadas
int ind_missao = 0; // indice da sala atual que está sendo visitada
int passo = 0;
int tolerancia = 15; // cm

//    ---Parametros de funcionamento do robo
int pos_robo = 0;  // Posição atual do robo em cm
int velocidade = 100; // velocidade de locomoção do robo
const int pingPin = 13; // pino de leitura do sensor de distancia ultrasonico
int distancia_antenas = 1500; // distância entre as antenas fixas em cm
uint8_t niCmd[] = {'N','I'};
uint8_t dbCmd[] = {'D','B'};
uint8_t payload[] = {0,0,0,0,0,0,0};
unsigned char P0_ref = 0x31; //
int D0_ref = 100; //cm
float np = 4.0*-10;


// ********************************** FILTRO *************************************************
//                     VARIÁVEIS UTILIZADAS NO ALGORITMO DO FILTRO DE BUTTERWOLF
int FC = 1;
int FA = 20;

float w;
float a0,a1,a2; 
float b1,b2;
float k1,k2,k3;


float A_B_Pre_1 = 0;
float A_B_Pre_2 = 0;
float A_B_Pos_1 = 0;
float A_B_Pos_2 = 0;
float A_P1_Pre_1 = 0;
float A_P1_Pre_2 = 0;
float A_P1_Pos_1 = 0;
float A_P1_Pos_2 = 0;

float A_PP;
float A_Sp = 0.0;

float B_B_Pre_1 = 0;
float B_B_Pre_2 = 0;
float B_B_Pos_1 = 0;
float B_B_Pos_2 = 0;
float B_P1_Pre_1 = 0;
float B_P1_Pre_2 = 0;
float B_P1_Pos_1 = 0;
float B_P1_Pos_2 = 0;

float B_PP;
float B_Sp = 0.0;

int resetantenas = 0;

// ********************************** FILTRO *************************************************

XBee xbee = XBee();  // CRIA A INTERFACE RESPONSÁVEL PELA COMUNICAÇÃO COM O MÓDULO XBEE



XBeeAddress64 remoteAddress[2] = {XBeeAddress64(0x0013A200, 0x40300F7A),XBeeAddress64(0x0013A200, 0x40300F8F)}; // CRIA ARRAY COM O ENDEREÇO DE TODAS AS ANTENAS DA REDE
// Create a remote AT request with the DB command
RemoteAtCommandRequest remoteAtRequest = RemoteAtCommandRequest(remoteAddress[0],niCmd); // ENVIA O COMANDO PARA O MÓDULO
  
// Create a Remote AT response object
RemoteAtCommandResponse remoteAtResponse = RemoteAtCommandResponse(); // RECEBE A RESPOSTA DO COMANDO

// Create a remote AT request with the DB command
AtCommandRequest atRequest = AtCommandRequest(dbCmd); // ENVIA O COMANDO PARA O MÓDULO

// Create a AT response object
AtCommandResponse atResponse = AtCommandResponse(); // RECEBE A RESPOSTA DO COMANDO



//variáveis auxiliares para o calculo das distância
float t1;
float t2;
int Pd;
int P0;

void setup(void){
  Serial.begin(9600);
  xbee.begin(9600);  // CRIA A COMUNICAÇÃO COM O MÓDULO
  D_DIR&=~(DATA0+DATA1); // INICIALIZA OS PINOS DO ARDUINO PARA RECEBER OS DADOS DO RFID
  
  for (int i=6; i<8; i++) {
    pinMode(i, OUTPUT);  
  }
  for (int i=10; i<=12; i++) {
    pinMode(i, INPUT);
  }
  
}


//**************Função principal**************************************************************************************************************************************************************************
//  -Verifica se a missão já foi configurada
//  -Controla as salas a serem visitadas
//  -Verifica final da missão
//  

void loop(void){
  
  /*if(resetantenas == 0){
    delay(6000);
    inicia_termos();
    reset_antenas();
  }
  resetantenas = 1;
  procura_sala();*/
  
  captura();
  Serial.print("\nindice missao:");
  Serial.println(ind_missao);
  Serial.print("\n SALA:");
  Serial.println(missao[ind_missao]);

}
  
//**************Inicia termos do filtro**************************************************************************************************************************************************************************
  
void inicia_termos(){
  
  w = tan(PI*(FC/0.802)/FA);
  k1 = pow(2,0.5)*w;
  k2 = pow(w,2);
  a0 = k2/(1+k1+k2);
  k3 = (2*a0)/k2;
  a1 = 2*a0;
  a2 = a0;
  b1 = -2*a0+k3;
  b2 = 1-2*a0-k3;

}  
  
//**************Fim da Função Principal**************************************************************************************************************************************************************************


//**************Função Seg_linha**************************************************************************************************************************************************************************
//    -Responsável por guiar o robo no trilho 
//

void seg_linha(void){
  int count = 0;
  while(count < passo){
    count++;
        
  // === LEITURA DOS SENSORES === //
    IF_E1 = analogRead(pinE1); 
    IF_D1 = analogRead(pinD1); 
    IF_E2 = analogRead(pinE2); 
    IF_D2 = analogRead(pinD2); 
     
    //Serial.print("\nE1: ");
    //Serial.println(IF_E1);
  
    //Serial.print("\nD1: ");
    //Serial.println(IF_D1);
    
    if(sentido == LOW){  
  
        if(IF_E1 < PRETO1 || IF_D1 < PRETO1){
          
              if(IF_E1 < PRETO1){
          
                  advance(velocidade*0.7,velocidade);
              }
          
              if(IF_D1 < PRETO1){
          
                advance(velocidade,velocidade*0.7);
              }
          
        }else{
           advance(velocidade,velocidade);
            }
           
    }else{
        if(IF_E2 < PRETO2 || IF_D2 < PRETO2){
          
              if(IF_E2 < PRETO2){
          
                  advance(velocidade*0.7,velocidade);
              }
          
              if(IF_D2 < PRETO2){
          
                advance(velocidade,velocidade*0.7);
              }
          
        }else{
           advance(velocidade,velocidade);
            }
      } 
    }
  stopMotors();
}
//**************FIM da Função Seg_linha**************************************************************************************************************************************************************************



//**************Função Realiza Leitura**************************************************************************************************************************************************************************
//  -Resposável por posicionar o robo no local da leitura
//  -Realiza a leitura do RFID
//  -Retorna o robo para o trilho

void realiza_leitura(){

  //posiciona();
  //delay(2000);
  //avanca();
  captura();
//  envia_pc();
  //retorna_trilho();
  
}
//**************Fim da Função Realiza Leitura**************************************************************************************************************************************************************************



//********************************************************************************************************************************************


int procura_sala(){

  int dist_robo_sala;
  
  while(1){
    
    calcula_posicao();
    dist_robo_sala = abs(missao[ind_missao] - pos_robo);
    
    // ---- configura o passo do robo ----
    if(dist_robo_sala >200){
      passo = 15000;
    }else{
      if(dist_robo_sala >100){
        passo = 10000;
      }else{
        passo = 1000;
      }
    }
    
    if(pos_robo < (missao[ind_missao] + tolerancia) && pos_robo > (missao[ind_missao] - tolerancia)){
      realiza_leitura();
      return 1;
    }
    if(pos_robo < missao[ind_missao]){
      sentido = HIGH;
      seg_linha();
      }else{
        if(pos_robo > missao[ind_missao]){
          sentido = LOW;
          seg_linha();
        }
      }
   }
 }


int captura(){ // CAPTURA OS DADOS DO RFID
  
  unsigned char recieve_count=0;
  for(;;){
    unsigned char data0=0,data1=0;
    if(D_IN&DATA0){  //DATA0 incoming signal           // CAPTURA OS DADOS DO RFID
      data0=1;
    }
    if(D_IN&DATA1){  //DATA1 incoming signal
      data1=1;
    }
    if(data0!=data1){  // card detected
      recieve_count++;
      if(recieve_count==1){ //drop even bit
      }
      else if(recieve_count<10)// card data group 1
      {
        if(!data1)
        {
          card_num[0]|=(1<<(9-recieve_count));
        }
      }
      else if(recieve_count<18)// card data group 2
      {
        if(!data1)
        {
          card_num[1]|=(1<<(17-recieve_count));
        }
      }
      if(!data1) // card data group 3
      {		
        card_num[2]|=(1<<(25-recieve_count));
      }
      delayMicroseconds(80);  //Data impulse width delay 80us
    }
    else    // no card incoming or finish reading card
    {
      unsigned char i=0;
      if(recieve_count>= 25)  //output card number
      {
        recieve_count = 0; //reset flag
        verifica_leitura();
        for(i=0;i<4;i++)
        {			
          Serial.print(card_num[i],DEC);
          Serial.print(" ");
          card_num[i]=0; //reset card_number array
        }
        Serial.println();// output debug value
        return 1;
        
      }
      
      //----------------------------------------------------      
    }
  }
  
}

void verifica_leitura(){
  
  if(card_num[0] == avanco[0]){
    if(card_num[1] == avanco[1] ){
      Serial.println("AVANCA -->");
      if(ind_missao < 3 ){
        
        ind_missao++;
      }
    }else{
      if(card_num[1] == retorno[1]){
        Serial.println("RETORNO <--");
        if(ind_missao > 0 ){
          
          ind_missao--;
        }
      }else{
        Serial.println("Nao cadastrado");}
      }
  }else{
    Serial.println("Nao cadastrado");
  }
}

//********************************************************************************************************************************************
//  Controle dos Motores

void stopMotors(void) {
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
}

void advance(char a, char b) {
  analogWrite(E1, a); // controle de velocidade por PWM
  digitalWrite(M1, sentido);
  analogWrite(E2, b);
  digitalWrite(M2, sentido);
}


void calcula_posicao(){

 int pot_A;
 int pot_B;
 
 int dist_A;
 int dist_B;

 float med_dist_A = 0;
 float med_dist_B = 0;

 int pos_robo_AB;
 int pos_robo_BA;
 
 int dist_A_FILTRO = 0;
 int dist_B_FILTRO = 0;
 
 pos_robo = 0;
 
 for(int i=0; i < 5; i++){
 
      remoteAtRequest.setRemoteAddress64(remoteAddress[0]);
      sendRemoteAtCommand();
      pot_A = sendAtCommand();
      
      
      delay(100);
      
      remoteAtRequest.setRemoteAddress64(remoteAddress[1]);
      sendRemoteAtCommand();
      pot_B = sendAtCommand();
 
      t1 = (pot_A+P0_ref)/np;
      t2 = pow(10,t1);
      dist_A = t2*100; 
      
      t1 = (pot_B+P0_ref)/np;
      t2 = pow(10,t1);
      dist_B = t2*100;
      
      med_dist_A = med_dist_A + dist_A;
      med_dist_B = med_dist_B + dist_B; 
     
      
      
    }
    dist_A = ((med_dist_A/5)+30)*2;
    dist_B = (med_dist_B/5)*2;
    
    
//************************************ Aplicando Filtro ***************************************************************************************************************************
     
    A_PP = (a0*dist_A) + (a1*A_B_Pre_2) + (a2*A_B_Pre_1) + (b1*A_P1_Pre_2) + (b2*A_P1_Pre_1);
    
    A_B_Pos_1 = A_B_Pre_2;
    A_B_Pos_2 = dist_A;
    
    A_P1_Pos_1 = (a0*A_B_Pos_1) + (a1*dist_A) + (a2*A_B_Pre_2) + (b1*A_PP) + (b2*A_P1_Pre_2);
    A_P1_Pos_2 = (a0*A_B_Pos_2) + (a1*A_B_Pos_1) + (a2*dist_A) + (b1*A_P1_Pos_1) + (b2*A_PP);
    
    A_Sp = (a0*A_PP) + (a1*A_P1_Pos_1) + (a2*A_P1_Pos_2) + (b1*A_P1_Pos_1) + (b2*A_P1_Pos_2);
    
    A_B_Pre_1 = A_B_Pre_2;
    A_B_Pre_2 = dist_A;
        
    A_P1_Pre_1 = A_P1_Pre_2;
    A_P1_Pre_2 = A_PP;
    
    dist_A_FILTRO = A_Sp;
    Serial.print("\nDist A:");
    Serial.println(dist_A);
       
       
    B_PP = (a0*dist_B) + (a1*B_B_Pre_2) + (a2*B_B_Pre_1) + (b1*B_P1_Pre_2) + (b2*B_P1_Pre_1);
    
    B_B_Pos_1 = B_B_Pre_2;
    B_B_Pos_2 = dist_B;
    
    B_P1_Pos_1 = (a0*B_B_Pos_1) + (a1*dist_B) + (a2*B_B_Pre_2) + (b1*B_PP) + (b2*B_P1_Pre_2);
    B_P1_Pos_2 = (a0*B_B_Pos_2) + (a1*B_B_Pos_1) + (a2*dist_B) + (b1*B_P1_Pos_1) + (b2*B_PP);
    
    B_Sp = (a0*B_PP) + (a1*B_P1_Pos_1) + (a2*B_P1_Pos_2) + (b1*B_P1_Pos_1) + (b2*B_P1_Pos_2);
    
    B_B_Pre_1 = B_B_Pre_2;
    B_B_Pre_2 = dist_B;
    
    B_P1_Pre_1 = B_P1_Pre_2;
    B_P1_Pre_2 = B_PP;
    
    
    dist_B_FILTRO = B_Sp;
    Serial.print("\nDist B:");
    Serial.println(dist_B);
    
    
    pos_robo_AB = (pow(dist_A,2)-pow(dist_B,2) + pow(distancia_antenas,2))/(2*distancia_antenas);
    pos_robo_BA = (pow(dist_B,2)-pow(dist_A,2) + pow(distancia_antenas,2))/(2*distancia_antenas);
    
    if(pos_robo_AB < pos_robo_BA){
      pos_robo = dist_A_FILTRO;
      envia_pc(dist_A,dist_B,'A');
          
  
    }else{
      pos_robo = distancia_antenas - dist_B_FILTRO;
      envia_pc(dist_A,dist_B,'B');
    
    }
  }

void sendRemoteAtCommand() {  
  
  xbee.send(remoteAtRequest);
  
  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == REMOTE_AT_COMMAND_RESPONSE) {
      xbee.getResponse().getRemoteAtCommandResponse(remoteAtResponse);

      if (remoteAtResponse.isOk()) {
        //lcd.print("Command [");
        //lcd.print(remoteAtResponse.getCommand()[0]);
        //lcd.print(remoteAtResponse.getCommand()[1]);
        //lcd.println("] was successful!");

        if (remoteAtResponse.getValueLength() > 0) {
          //lcd.print("Command value length is ");
          //lcd.println(remoteAtResponse.getValueLength(), DEC);

          for (int i = 0; i < remoteAtResponse.getValueLength(); i++) {
            
            
          }
        }
      } else {
        
      }
    } else {
      //lcd.print("Expected Remote AT response but got ");
      //lcd.print(xbee.getResponse().getApiId(), HEX);
    }    
  } else {
    // remote at command failed
    if (xbee.getResponse().isError()) {
      //lcd.print("Error reading packet.  Error code: ");  
      //lcd.println(xbee.getResponse().getErrorCode());
    } else {
      //lcd.print("No response from radio");  
    }
  }
}



int sendAtCommand() {
  //nss.println("Sending command to the XBee");
  char resp_pot[1] = {};
  
  
  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {
        //nss.print("Command [");
        //nss.print(atResponse.getCommand()[0]);
        //nss.print(atResponse.getCommand()[1]);
        //nss.println("] was successful!");

        if (atResponse.getValueLength() > 0) {
          //nss.print("Command value length is ");
          //nss.println(atResponse.getValueLength(), DEC);
                    
          
            
            resp_pot[0] = (atResponse.getValue()[0]);
            //resp_pot[1] = (atResponse.getValue()[1]);
            
                      
            return resp_pot[0]*-1; 
           
        }
      } 
      else {
        //lcd.print("Command return error code: ");
        //lcd.println(atResponse.getStatus(), HEX);
      }
    } else {
      //lcd.print("Expected AT response but got ");
      //lcd.print(xbee.getResponse().getApiId(), HEX);
    }   
  } else {
    // at command failed
    if (xbee.getResponse().isError()) {
      //lcd.print("Error reading packet.  Error code: ");  
      //lcd.println(xbee.getResponse().getErrorCode());
    } 
    else {
      //lcd.print("No response from radio");  
    }
  }
}


void envia_pc(int dist_A_FILTRO,int dist_B_FILTRO,char antena)
{   
    
     
     
    payload[1] = dist_A_FILTRO%256;
    payload[0] = dist_A_FILTRO/256;    
    
    payload[3] = dist_B_FILTRO%256;
    payload[2] = dist_B_FILTRO/256;
    
    payload[4] = antena;
    
    payload[6] = pos_robo%256;
    payload[5] = pos_robo/256;
    
    //payload[7] = ;    
        
    XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x404c0b99);
    ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
    ZBTxStatusResponse txStatus = ZBTxStatusResponse();
    
    
    
    
    xbee.send(zbTx);
  
    // flash TX indicator

    
    // after sending a tx request, we expect a status response
    // wait up to half second for the status response
    if (xbee.readPacket(500)) {
        // got a response!
        Serial.println("\n\nResposta ");
        Serial.println(xbee.getResponse().getApiId(),HEX);
        // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getZBTxStatusResponse(txStatus);
    		
    	   // get the delivery status, the fifth byte
           if (txStatus.getDeliveryStatus() == SUCCESS) {
            	// success.  time to celebrate
                  Serial.println("\n\nSUCESSO ");        

           } else {
            	// the remote XBee did not receive our packet. is it powered on?

           }
        }      
    } else {
      // local XBee did not provide a timely TX Status Response -- should not happen

    }
    
    delay(1000);

}

void reset_antenas(){
  int cont =0;
  while(cont < 5){
    cont++;
    calcula_posicao();
    }
}


