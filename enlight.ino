/*
  enlight 1.0 - manage a relay with a capacitive touch
  by lesion
  Oct 2014
  This code is in the public domain.
*/

#include <avr/io.h>
#include <util/delay.h>


// 10M resistor between pins SENSOR_OUT & SENSOR_IN
// attach your capacitor switch at pin SENSOR_PIN
#define SENSOR_IN  2
#define SENSOR_OUT 4

#define RELAY      13

#define MIN_SWITCH_DELAY_MS 2000


volatile uint8_t* port;
volatile uint8_t* ddr;
volatile uint8_t* pin;
uint8_t bitmask;


uint8_t readCapacitivePin(){

  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  _delay_ms(1);

  // Make the pin an input WITH the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up
  uint8_t cycles = 3;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}


void setup()
{
   Serial.begin(4800);
   pinMode(RELAY,OUTPUT);

  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.

  if ((SENSOR_IN >= 0) && (SENSOR_IN <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << SENSOR_IN;
    pin = &PIND;
  }
  if ((SENSOR_IN > 7) && (SENSOR_IN <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (SENSOR_IN - 8);
    pin = &PINB;
  }
  if ((SENSOR_IN > 13) && (SENSOR_IN <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (SENSOR_IN - 13);
    pin = &PINC;
  }
}



// relay status
boolean state=false;

// i'm touching ?
boolean touching=false;

// each time we change status we control
// when we have done it last time
// to avoid fast toggle
long lastMillis = millis();

void loop()
{
    long capacitance = readCapacitivePin();

    Serial.println(capacitance);

    // to toggle the status of relay
    // the difference in capacitance
    // has to be > of 150% from the previous value
    if(capacitance>1)
    {
      // check if enough time is passed since last switch
      if(!touching && millis()-lastMillis>MIN_SWITCH_DELAY_MS)
      {
        lastMillis=millis();
        touching=true;
        state = !state;
        digitalWrite(RELAY,state);
      }
    }
    else
    {
      touching=false;
    }

};
