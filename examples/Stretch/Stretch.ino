/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <MappedPoti.h>

/*
  Example to show the mapped value of the
  connected potentiometer with and without
  the stretching method. The stretching can be
  compared in its consequences.

  In the example both Poti objects use the
  same INPUT_PIN. This should normally
  not be done, but is without problems possible.

  Prerequisite is an potentiometer connected
  with variable voltage pin to analog input
  pin. Output will be written to Serial.
*/

#define INPUT_PIN A7                  // must be analog pin A0 to A7
#define NUM_MAP_VALUES 10             // max 101, raw values will be mapped to this number of mapping values
#define READ_CYCLE_MILLIS 100         // minimum difference between two actual read of analog raw value
#define MAX_BLINK_CYCLE_MILLIS 1000   // min allowed is 1000
#define ADD_RAW_AVG 4                 // additional measurements for building an avarage
#define WEIGHT_PREV 8                 // weight of previous value in calculating new value
#define STRETCH 10                    // strength of stretching the map ranges

// without stretching
MappedPoti pot = MappedPoti(INPUT_PIN, READ_CYCLE_MILLIS, WEIGHT_PREV,
                            ADD_RAW_AVG, NUM_MAP_VALUES, 0);
// with stretching
MappedPoti potStretch = MappedPoti(INPUT_PIN, READ_CYCLE_MILLIS, WEIGHT_PREV,
                            ADD_RAW_AVG, NUM_MAP_VALUES, STRETCH);

// Status on or off independently of blinking
boolean lightOn = false;
// time, when the LED was toggeled
unsigned long lastLEDChange = 0;
// duration of LED on/off phases
unsigned long cycleDurationMillis = MAX_BLINK_CYCLE_MILLIS;


// the setup function is called once for initialization
void setup() {
  // for showing relevant information
  Serial.begin(9600);
}


// for showing relevant information
void printValues(MappedPoti* pot, char prefix){
  Serial.print(prefix);
  Serial.print(" curVal=");
  if(pot->getValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot->getValue());
  }

  Serial.print(", curMapVal=");
  if(pot->getMappedValue() == POTI_MAPPING_UNDEFINED){
    Serial.print("POTI_MAPPING_UNDEFINED");
  }
  else{
    Serial.print(pot->getMappedValue());
  }

  Serial.print("\n");
}


// the loop function runs over and over again forever
void loop() {
  // react on changing values by writing the relevant information
  if(pot.hasChanged()){
    printValues(&pot, ' ');
  }

  if(potStretch.hasChanged()){
    printValues(&potStretch, 'S');
  }
}

