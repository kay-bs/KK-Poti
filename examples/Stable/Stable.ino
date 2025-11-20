/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <StablePoti.h>

/*
  Example to show the current value of
  the connected potentiometer with and without
  stabilization methods. Methods can be
  compared in their consequences.

  In the example both Poti objects use the
  same INPUT_PIN. This should normally
  not be done, but is without problems possible.

  Prerequisite is an potentiometer connected
  with variable voltage pin to analog input
  pin. Output will be written to Serial.
*/

#define INPUT_PIN A7            // must be analog pin A0 to A7
#define READ_CYCLE_MILLIS 100   // min difference between two actual read of analog raw value
#define ADD_RAW_AVG 4           // additional measurements for building an avarage
#define WEIGHT_PREV 8           // weight of previous value in calculating new value

// Poti with stabilization methods switched off
Poti pot = Poti(INPUT_PIN, READ_CYCLE_MILLIS);
// Poti with both stabilization methods switched on
StablePoti potStable = StablePoti(INPUT_PIN, READ_CYCLE_MILLIS, WEIGHT_PREV, ADD_RAW_AVG);

// the setup function is called once for initialization
void setup() {
  // for showing relevant information
  Serial.begin(9600);
}


// for showing relevant information
void printValues(Poti* pot, char prefix){
  Serial.print(prefix);
  Serial.print(" curVal=");
  if(pot->getValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot->getValue());
  }

  Serial.print(", prevVal=");
  if(pot->getPrevValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot->getPrevValue());
  }

  Serial.print("\n");
}


// the loop function runs over and over again forever
void loop() {
  // react on changing values by writing the relevant information

  if(pot.hasChanged()){
    printValues(&pot, ' ');
  }

  if(potStable.hasChanged()){
    printValues(&potStable, 'S');
  }
}


