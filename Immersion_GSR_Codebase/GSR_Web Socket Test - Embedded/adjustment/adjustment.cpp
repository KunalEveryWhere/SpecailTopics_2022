#include <Arduino.h>

#define GSRPin 32

int sensorValue=0;
int gsr_average=0;

void setup(){
  Serial.begin(115200);
  Serial.println("This is Adjustment Protocol. \n Kindly turn the adjustment dail until value settles to ~2600; \nid Est: right before it falls to zero");
}

 

void loop(){

  long sum=0;

  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch

      {

      sensorValue=analogRead(GSRPin);

      sum += sensorValue;

      delay(5);

      }

   gsr_average = sum/10;

   Serial.println(gsr_average);

}