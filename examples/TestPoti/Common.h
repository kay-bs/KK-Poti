/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef COMMON_TESTPOTI
#define COMMON_TESTPOTI

#include "Classes.h"

#define POTI_MAX_VALUE 1023           // in this example the max value of analogRead()
#define ID_POTITEST 1
#define ID_STABLETEST 2
#define ID_MAPPEDTEST 3
#define ID_CENTEREDTEST 4
#define INPUT_PIN A7                  // must be analog pin A0 to A7
#define READ_CYCLE_MILLIS 100         // minimum difference between two actual read of analog raw value


// do the checking and print hint in case of and error
void check(int value, int ref, int id, int seq){
  if(value != ref){
    Serial.print("ID: ");
    Serial.print(id);
    Serial.print("/");
    Serial.print(seq);
    Serial.print(": '");
    Serial.print(value) ;
    Serial.print("' != reference '");
    Serial.print(ref);
    Serial.println("'");
  }
}

// for showing value information
void printValues(Poti* poti, bool newLine){
  Serial.print("curVal=");
  if(poti->getValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(poti->getValue());
  }

  Serial.print(", prevVal=");
  if(poti->getPrevValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(poti->getPrevValue());
  }

  if(newLine){
    Serial.print("\n");
  }
  else{
    Serial.print(", ");
  }
}

// for showing mapping information
void printMappings(MappedPoti* poti, bool newLine){
  Serial.print("curMap=");
  if(poti->getMappedValue() == POTI_MAPPING_UNDEFINED){
    Serial.print("POTI_MAPPING_UNDEFINED");
  }
  else{
    Serial.print(poti->getMappedValue());
  }

  Serial.print(", prevMap=");
  if(poti->getMappedPrevValue() == POTI_MAPPING_UNDEFINED){
    Serial.print("POTI_MAPPING_UNDEFINED");
  }
  else{
    Serial.print(poti->getMappedPrevValue());
  }

  if(newLine){
    Serial.print("\n");
  }
  else{
    Serial.print(", ");
  }
}

#endif