#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
#define DHT11PIN 2
dht11 DHT11;

void verificaSensor(void){
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk){
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }
}

double dewPoint(double celsius, double humidity){
        double RATIO = 373.15 / (273.15 + celsius);  // RATIO was originally named A0, possibly confusing in Arduino context
        double SUM = -7.90298 * (RATIO - 1);
        SUM += 5.02808 * log10(RATIO);
        SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
        SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM - 3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558 - T);
}

double dewPointFast(double celsius, double humidity){
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

byte mac[] = { 20, 89, 84, 02, 92, 06 };
IPAddress ip(192,168,0,123);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {;} //espera enquanto não comunica via serial
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
}


void loop() {
  verificaSensor();
  EthernetClient client = server.available(); //verifica clientes
  if (client) {
    Serial.println("Novo cliente");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          /* testar */ client.println("User-Agent: Arduino");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //verifica luminosidade
          int luz = analogRead(A0);
          int mapLuz = map(luz,0,1023,0,100); //retorna de 0% a 100%
          if (mapLuz >= 80)
              client.print("<b>Luz Acesa!!</b>");
          if (mapLuz < 80 && mapLuz >= 40)
              client.print("<b>Luz Fraca</b>");
          if(mapLuz < 40)
              client.print("<b>Luz Apagada!!</b>");
          client.print(" Com ");
          client.println(mapLuz);
          client.print("% de luminosidade");
          client.println("<br />");
          client.println("<br />");
          //verifica temperatura e humidade
          client.print("Humidade (%): ");
          client.println((float)DHT11.humidity, 2);
          client.println("<br />");
          client.print("Temperatura (oC): ");
          client.println((float)DHT11.temperature, 2); 
          client.println("<br />");       
          client.print("Ponto de Orvalho (oC): ");
          client.println(dewPoint(DHT11.temperature, DHT11.humidity));
                  
          client.println("<br />");       
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    delay(1000);
  }
}

