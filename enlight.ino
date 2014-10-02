/*
  enlight 1.0 - manage a relay with a capacitive touch
  by lesion
  Oct 2014
  This code is in the public domain.
*/

#include <CapacitiveSensor.h>
#define SENSOR_OUT 4
#define SENSOR_IN  2

#define RELAY      13

#define MIN_SWITCH_DELAY_MS 2000

// 10M resistor between pins SENSOR_OUT & SENSOR_IN , pin SENSOR_IN is sensor pin, add a wire and or foil if desired
CapacitiveSensor  touch = CapacitiveSensor(SENSOR_OUT,SENSOR_IN);


void setup()
{
   Serial.begin(9600);
   pinMode(RELAY,OUTPUT);
}


// relay status
boolean state=false;

// i'm touching ?
boolean touching=false;

// store last sensed capacitance
// to compare with the current one
// and calculates if there's variation
long lastCapacitance=0;

// each time we change status we control
// when we have done it last time
// to avoid fast toggle
long lastMillis = millis();

void loop()
{
    long capacitance =  sensor.capacitiveSensorRaw(10);

    Serial.print(capacitance*100/lastCapacitance);
    Serial.print("\t");

    // to toggle the status of relay
    // the difference in capacitance
    // has to be > of 150% from the previous value
    if(capacitance*100/lastCapacitance>150)
    {
      // check enought time is passed since last switch
      if(!touching && millis()-lastMillis>MIN_SWITCH_DELAY_MS)
      {
        lastMillis=millis();
        touching=true;
        state = !state;
        Serial.print("Changing state:");
        Serial.println(state);
        digitalWrite(RELAY,state);
      }
    }
    else
    {
      touching=false;
    }

   Serial.print(lastCapacitance);
   Serial.print("\t");
   Serial.print(capacitance);
   Serial.print("\t");
   Serial.println(state);

   lastCapacitance=capacitance;
   delay(50);
};