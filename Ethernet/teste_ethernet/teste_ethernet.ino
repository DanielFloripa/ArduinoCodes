#include <SPI.h>
#include <String.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 0, 180 }; // ip in lan
byte gateway[] = { 192, 168, 1, 0 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
Server server(80); //server port
byte sampledata=50; //some sample data – outputs 2 (ascii = 50 DEC)

int ledPin = 4; // LED pin
char link[]="http://www.google.com.br/"; //link data
String readString = String(30); //string for fetching data from address
boolean LEDON = false; //LED status flag


void setup(){
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  //Set pin 4 to output
  pinMode(ledPin, OUTPUT);
  //enable serial datada print
  Serial.begin(9600); }

void loop(){
  // Create a client connection
  Client client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 30)
        {
          //store characters to string
          readString += (c);
        }
        //output chars to serial port
        Serial.print(c);
        //if HTTP request has ended
        if (c == '\n')
          {
          //lets check if LED should be lighted
          if(readString.indexOf("L=1")>=0)
          {
            //led has to be turned ON
            digitalWrite(ledPin, HIGH); // set the LED on
            LEDON = true;
          }
          else
          {
            //led has to be turned OFF
            digitalWrite(ledPin, LOW); // set the LED OFF
            LEDON = false;
          }

        // now output HTML data starting with standart header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();

        //set background to white
        client.print("<body style=background-color:white>");

        //send first heading
        client.println("<font face= 'Helvetica, Verdana, Sans Serif' color='black'>");
        client.println("<h1>Teste HTTP</h1>");
        client.println("<hr />");

        //output some sample data to browser
//        client.println("<font color='blue' size='5'>Sample data: ");
//        client.print(sampledata);//lets output some data
//        client.println("<br />");//some space between lines
//        client.println("<hr />");

        //printing some link
//        client.println("<font face='Helvetica, Verdana, Arial, Sans Serif' color='blue' size='5'>Link: ");
//        client.print("<a href=");
//        client.print(link);
//        client.println(">Visite o nicvix!</a>");
//        client.println("<br />");
//        client.println("<hr />");

        //controlling led via checkbox
        client.println("<h2>LED control</h2>");
        //address will look like http://192.168.1.110/?L=1 when submited
        client.println("<form method=get name=LED><input type=checkbox name=L value=1>LED<br><input type=submit value=submit></form>");
        client.println("<br />");
        //printing LED status
        client.print("<font size='5'>LED status: ");
        if (LEDON)
        client.println("<font color='green' size='5'>ON");
        else
        client.println("<font color='grey' size='5'>OFF");
        client.println("<hr />");
        client.println("</font>");
        client.println("</body></html>");
        //clearing string for next read
        readString="";
        //stopping client
        client.stop();
        }
      }
    }
  }
}

