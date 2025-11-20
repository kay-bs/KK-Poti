/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef STABLEPOTITESTS_TESTPOTI
#define STABLEPOTITESTS_TESTPOTI

#include "Common.h"

void doStablePotiTest(int id){  // ID_STABLETEST = 2
  TestStablePoti poti0Wait(INPUT_PIN, 0, 0, 0);
  TestStablePoti poti2Wait(INPUT_PIN, 2, 0, 0);
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

  // checking stabilization methods incl. waiting times

  // now with delayed measurements and hasChanged() information
  // first delay can be max 1000 micros shorter than defined
  // millis in instantiation
  seq = 40;
  poti2Wait.setReadCycleMillis(2);
  poti2Wait.setAddNumRawAvg(0);
  poti2Wait.setWeightPrev(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(1);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  check(poti2Wait.getValue(),1,id,seq+2);
  poti2Wait.setRawValue(0);
  while(!poti2Wait.hasChanged()){}; // due to readCycleMillis
  startmicro = micros();
  for(int i = 1 ; i < 10 ; i++){
    poti2Wait.setRawValue(i);
    check(poti2Wait.getValue(),i-1,id,seq+4);
    while(!poti2Wait.hasChanged()){};
    tmpMicros = micros() - startmicro; 
    startmicro = micros();
    check(tmpMicros > 1000, true,id,seq+5);
    check(poti2Wait.hasChanged(),false,id,seq+6);
    check(poti2Wait.getValue(),i,id,seq+7);
    check(poti2Wait.getPrevValue(),i-1,id,seq+8);
  }

  seq = 50;
  // now check average calculation
  poti2Wait.setReadCycleMillis(2);
  poti2Wait.setAddNumRawAvg(1);
  poti2Wait.setWeightPrev(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(10);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  delay(2); // due to readCycleMillis
  check(poti2Wait.hasChanged(),false,id,seq+2);
  poti2Wait.setRawValue(20);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+3);
  check(poti2Wait.getValue(),15,id,seq+4);

  poti2Wait.setReadCycleMillis(2);
  poti2Wait.setAddNumRawAvg(2);
  poti2Wait.setWeightPrev(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+5);
  delay(2); // due to readCycleMillis
  check(poti2Wait.hasChanged(),false,id,seq+6);
  poti2Wait.setRawValue(601);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),false,id,seq+7);
  poti2Wait.setRawValue(701);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+8);
  check(poti2Wait.getValue(),601,id,seq+9); // due to rounding

  seq = 60;
  poti2Wait.setReadCycleMillis(2);
  poti2Wait.setAddNumRawAvg(2);
  poti2Wait.setWeightPrev(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  poti2Wait.setRawValue(501);
  delay(2); // due to readCycleMillis
  check(poti2Wait.hasChanged(),false,id,seq+2);
  poti2Wait.setRawValue(601);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),false,id,seq+3);
  poti2Wait.setRawValue(705);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+4);
  check(poti2Wait.getValue(),602,id,seq+5);

  // now check weighting with previous value
  poti2Wait.setAddNumRawAvg(0);
  poti2Wait.setWeightPrev(1);
  poti2Wait.setReadCycleMillis(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+6);
  check(poti2Wait.getValue(),500,id,49);
  poti2Wait.setRawValue(700);
  check(poti2Wait.hasChanged(),true,id,seq+7);
  check(poti2Wait.getValue(),660,id,51);
  poti2Wait.setRawValue(800);
  check(poti2Wait.hasChanged(),true,id,seq+8);
  check(poti2Wait.getValue(),772,id,seq+9);

  seq = 70;
  poti2Wait.setWeightPrev(4);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  check(poti2Wait.getValue(),500,id,seq+2);
  poti2Wait.setRawValue(700);
  check(poti2Wait.hasChanged(),true,id,seq+3);
  check(poti2Wait.getValue(),600,id,seq+4);
  poti2Wait.setRawValue(800);
  check(poti2Wait.hasChanged(),true,id,seq+5);
  check(poti2Wait.getValue(),700,id,seq+6); // due to rounding

  seq = 80;
  poti2Wait.setWeightPrev(12);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  check(poti2Wait.getValue(),500,id,seq+2);
  poti2Wait.setRawValue(700);
  check(poti2Wait.hasChanged(),true,id,seq+3);
  check(poti2Wait.getValue(),550,id,seq+4);
  poti2Wait.setRawValue(800);
  check(poti2Wait.hasChanged(),true,id,seq+5);
  check(poti2Wait.getValue(),613,id,seq+6); // due to rounding

  // now check combined average and weighting
  seq = 90;
  poti2Wait.setAddNumRawAvg(1);
  poti2Wait.setWeightPrev(4);
  poti2Wait.setReadCycleMillis(0);
  poti2Wait.reset();
  poti2Wait.setRawValue(500);
  check(poti2Wait.hasChanged(),true,id,seq+1);
  check(poti2Wait.getValue(),500,id,seq+2);
  poti2Wait.setRawValue(700);
  check(poti2Wait.hasChanged(),false,id,seq+3);
  check(poti2Wait.getValue(),500,id,seq+4);
  poti2Wait.setRawValue(800);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+5);
  check(poti2Wait.getValue(),625,id,seq+6);
  poti2Wait.setRawValue(900);
  check(poti2Wait.hasChanged(),false,id,seq+7);
  check(poti2Wait.getValue(),625,id,seq+8);
  poti2Wait.setRawValue(1000);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+9);
  check(poti2Wait.getValue(),788,id,seq+10);
  poti2Wait.setRawValue(100);
  check(poti2Wait.hasChanged(),false,id,seq+11);
  check(poti2Wait.getValue(),788,id,seq+12);
  poti2Wait.setRawValue(50);
  delay(1); // due to addNumRawAvg
  check(poti2Wait.hasChanged(),true,id,seq+13);
  check(poti2Wait.getValue(),432,id,seq+14);

  // performance

  Serial.println("\nPerformance Stabilized:");

  Serial.print("1024 * hasChanged(), numAvg 0, prevWeight 0: ");
  poti0Wait.setAddNumRawAvg(0);
  poti0Wait.setWeightPrev(0);
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), numAvg 0, prevWeight 4: ");
  poti0Wait.setAddNumRawAvg(0);
  poti0Wait.setWeightPrev(4);
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), numAvg 1, prevWeight 4: ");
  poti0Wait.setAddNumRawAvg(1);
  poti0Wait.setWeightPrev(4);
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    delayMicroseconds(1000);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro - 1026000);
  Serial.println(" micros");
}

#endif
