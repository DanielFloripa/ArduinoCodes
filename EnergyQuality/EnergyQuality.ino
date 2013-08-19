// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

void setup()
{  
  Serial.begin(9600);
  emon1.current(1, 10);             // Current: input pin, calibration sensor limit.
}

void loop()
{
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only. Parametro eh num de voltas
  double Vcc = emon1.readVcc();
  Serial.print("IRMS*230: ");
  Serial.println(Irms*220.0);	       // Apparent power
  Serial.print("IRMS:     ");
  Serial.println(Irms);		       // Irms
  delay(1000);
}
