/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef POTITESTS_TESTPOTI
#define POTITESTS_TESTPOTI

#include "Common.h"

void doPotiTest(int id){  // ID_POTITEST = 1
  TestPoti poti0Wait(INPUT_PIN, 0);
  TestPoti poti2Wait(INPUT_PIN, 2);
  unsigned long tmpMicros, startmicro = 0;
  int seq = 0;

  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+1);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+2);

  poti0Wait.setRawValue(10);
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+3);
  check(poti0Wait.hasChanged(),true,id,seq+4);
  check(poti0Wait.getValue(),10,id,seq+5);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+6);
  check(poti0Wait.hasChanged(),false,id,seq+7);

  seq = 10;
  poti0Wait.setRawValue(0);
  check(poti0Wait.getValue(),10,id,seq+1);
  check(poti0Wait.hasChanged(),true,id,seq+2);
  check(poti0Wait.getValue(),0,id,seq+3);
  check(poti0Wait.getPrevValue(),10,id,seq+4);
  check(poti0Wait.hasChanged(),false,id,seq+5);
  check(poti0Wait.getValue(),0,id,seq+6);
  check(poti0Wait.getPrevValue(),10,id,seq+7);

  seq = 20;
  poti0Wait.reset();
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+1);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+2);
  poti0Wait.setRawValue(100);
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+3);
  check(poti0Wait.hasChanged(),true,id,seq+4);
  check(poti0Wait.getValue(),100,id,seq+5);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+6);

  seq = 30;
  poti0Wait.reset();
  poti0Wait.setRawValue(0);
  for(int i = 0 ; i < 4096 ; i++){
    poti0Wait.setRawValue(i);
    check(poti0Wait.getValue(),(i==0 ? POTI_VALUE_UNDEFINED : i-1),id,seq+1);
    check(poti0Wait.hasChanged(),true,id,seq+2);
    check(poti0Wait.getValue(),i,id,seq+3);
    check(poti0Wait.getPrevValue(),(i==0 ? POTI_VALUE_UNDEFINED : i-1),id,seq+4);
  }

  // value = 4095 set / expected
  for(int i = 4094 ; i >= 0 ; i--){
    poti0Wait.setRawValue(i);
    check(poti0Wait.getValue(),i+1,id,seq+5);
    check(poti0Wait.hasChanged(),true,id,seq+6);
    check(poti0Wait.getValue(),i,id,seq+7);
    check(poti0Wait.getPrevValue(),i+1,id,seq+8);
  }

  // now with delayed measurements and hasChanged() information
  // first delay can be max 1000 micros shorter than defined
  // millis in instantiation, 
  seq = 40;
  poti2Wait.setRawValue(0);
  startmicro = micros();
  check(poti2Wait.hasChanged(),true,id,seq+1);
  for(int i = 1 ; i < 10 ; i++){
    poti2Wait.setRawValue(i);
    check(poti2Wait.getValue(),i-1,id,seq+2);
    while(!poti2Wait.hasChanged()){};
    tmpMicros = micros() - startmicro; 
    startmicro = micros();
    check(tmpMicros >= 1000, true,id,seq+3);
    check(poti2Wait.hasChanged(),false,id,seq+4);
    check(poti2Wait.getValue(),i,id,seq+5);
    check(poti2Wait.getPrevValue(),i-1,id,seq+6);
  }

  // performance

  Serial.println("\nPerformance Standard:");

  Serial.print("1024 * hasChanged(): ");
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");
}

#endif
