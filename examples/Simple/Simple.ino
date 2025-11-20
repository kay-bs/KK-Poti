/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <Poti.h>

/*
  Simple example to show the current value of
  the connected potentiometer.

  The example will show the instability of
  measured values over the time, when the
  potentiometer is not touched. In the example
  are the available stabilisation methods not
  configured, means in use. 

  Prerequisite is an potentiometer connected
  with variable voltage pin to analog input
  pin. Output will be written to Serial.
*/

#define INPUT_PIN A7                  // must be analog pin A0 to A7
#define READ_CYCLE_MILLIS 200         // millis between two reads of analog raw value

Poti pot = Poti(INPUT_PIN, READ_CYCLE_MILLIS);

// the setup function is called once for initialization
void setup() {
  Serial.begin(9600);
}


// for showing relevant information
void printValues(){
  Serial.print("curVal=");
  if(pot.getValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot.getValue());
  }

  Serial.print(", prevVal=");
  if(pot.getPrevValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot.getPrevValue());
  }

  Serial.print("\n");
}


// the loop function runs over and over again forever
void loop() {
  // react on changing values by writing the relevant information
  if(pot.hasChanged()){
    printValues();
  }
}

