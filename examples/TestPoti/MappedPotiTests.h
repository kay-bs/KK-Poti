/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef MAPPEDPOTITESTS_TESTPOTI
#define MAPPEDPOTITESTS_TESTPOTI

#include "Common.h"

void doMappedPotiTest(int id){  // ID_MAPPEDTEST = 3
  TestMappedPoti poti0Wait(INPUT_PIN, 0, 0, 0, 20, 0);
  unsigned long startmicro = 0;
  int seq, j;
  double x;

  seq = 0;
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+1);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+2);

  poti0Wait.setRawValue(10);
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+3);
  check(poti0Wait.hasChanged(),true,id,seq+4);
  check(poti0Wait.getValue(),10,id,seq+5);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+6);
  check(poti0Wait.hasChanged(),false,id,seq+7);

  seq = 10;
  poti0Wait.setRawValue(1023);
  check(poti0Wait.getValue(),10,id,seq+1);
  check(poti0Wait.hasChanged(),true,id,seq+2);
  check(poti0Wait.getValue(),1023,id,seq+3);
  check(poti0Wait.getPrevValue(),10,id,seq+4);
  check(poti0Wait.hasChanged(),false,id,seq+5);
  check(poti0Wait.getValue(),1023,id,seq+6);
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
  poti0Wait.setNumMapping(20);
  poti0Wait.reset();
  check(poti0Wait.getMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+1);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+2);
  poti0Wait.setRawValue(0);
  check(poti0Wait.getMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+3);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+4);
  check(poti0Wait.hasChanged(),true,id,seq+5);
  check(poti0Wait.getMappedValue(),0,id,seq+6);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+7);
  poti0Wait.setRawValue(poti0Wait.getMaxAnalogValue());
  check(poti0Wait.hasChanged(),true,id,seq+8);
  check(poti0Wait.getMappedValue(),19,id,seq+9);
  check(poti0Wait.getMappedPrevValue(),0,id,seq+10);

  seq = 50;
  poti0Wait.reset();
  check(poti0Wait.setMaxAnalogValue(4095),4095,id,seq+1);
  poti0Wait.setNumMapping(20);
  x = poti0Wait.getMaxAnalogValue() / poti0Wait.getNumMappingValues();
  check(x,204,id,seq+2);
  j = 0;
  for(int i = 0 ; i < 4096 ; i++){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+3);
      check(poti0Wait.getPrevValue(),(i==0 ? POTI_VALUE_UNDEFINED : j),id,seq+4);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x),id,seq+5);
      check(poti0Wait.getMappedPrevValue(),(i==0 ? POTI_MAPPING_UNDEFINED : int((i/x)-1)),id,seq+6);
    }
    check(poti0Wait.hasChanged(),false,id,seq+7);
  }

  seq = 60;
  poti0Wait.reset();
  check(poti0Wait.setMaxAnalogValue(1023),1023,id,seq+1);
  poti0Wait.setNumMapping(25);
  x = poti0Wait.getMaxAnalogValue() / poti0Wait.getNumMappingValues();
  check(x,40,id,seq+2);
  j = 0;
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+3);
      check(poti0Wait.getPrevValue(),(i==0 ? POTI_VALUE_UNDEFINED : j),id,seq+4);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x),id,seq+5);
      check(poti0Wait.getMappedPrevValue(),(i==0 ? POTI_MAPPING_UNDEFINED : int((i/x)-1)),id,seq+6);
    }
    check(poti0Wait.hasChanged(),false,id,seq+7);
  }

  // value = 1023 set / expected
  j = poti0Wait.getValue();
  for(int i = 1022 ; i >= 0 ; i--){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+8);
      check(poti0Wait.getPrevValue(),j,id,seq+9);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x)-1,id,seq+10);
      check(poti0Wait.getMappedPrevValue(),int((i/x)),id,seq+11);
    }
    check(poti0Wait.hasChanged(),false,id,seq+12);
  }

  // now check stretching

  seq = 80;
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(4);
  poti0Wait.setStretch(1); // stretching min
  poti0Wait.reset();
  poti0Wait.setRawValue(0);
  check(poti0Wait.hasChanged(),true,id,seq+1);
  check(poti0Wait.getValue(),0,id,seq+2);
  check(poti0Wait.getMappedValue(),0,id,seq+3);
  poti0Wait.setRawValue(231);
  check(poti0Wait.hasChanged(),false,id,seq+4);
  check(poti0Wait.getValue(),0,id,seq+5);
  check(poti0Wait.getMappedValue(),0,id,seq+6);
  poti0Wait.setRawValue(232);
  check(poti0Wait.hasChanged(),true,id,seq+7);
  check(poti0Wait.getValue(),232,id,seq+8);
  check(poti0Wait.getMappedValue(),1,id,seq+9);
  poti0Wait.setRawValue(511);
  check(poti0Wait.hasChanged(),false,id,seq+10);
  check(poti0Wait.getValue(),232,id,seq+11);
  check(poti0Wait.getMappedValue(),1,id,seq+12);
  poti0Wait.setRawValue(512);
  check(poti0Wait.hasChanged(),true,id,seq+13);
  check(poti0Wait.getValue(),512,id,seq+14);
  check(poti0Wait.getMappedValue(),2,id,seq+15);
  poti0Wait.setRawValue(791);
  check(poti0Wait.hasChanged(),false,id,seq+16);
  check(poti0Wait.getValue(),512,id,seq+17);
  check(poti0Wait.getMappedValue(),2,id,seq+18);
  poti0Wait.setRawValue(792);
  check(poti0Wait.hasChanged(),true,id,seq+19);
  check(poti0Wait.getValue(),792,id,seq+20);
  check(poti0Wait.getMappedValue(),3,id,seq+21);
  poti0Wait.setRawValue(1023);
  check(poti0Wait.hasChanged(),false,id,seq+22);
  check(poti0Wait.getValue(),792,id,seq+23);
  check(poti0Wait.getMappedValue(),3,id,seq+24);

  seq = 110;
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(4);
  poti0Wait.setStretch(20); // stretching max
  poti0Wait.reset();
  poti0Wait.setRawValue(0);
  check(poti0Wait.hasChanged(),true,id,seq+1);
  check(poti0Wait.getValue(),0,id,seq+2);
  check(poti0Wait.getMappedValue(),0,id,seq+3);
  poti0Wait.setRawValue(51);
  check(poti0Wait.hasChanged(),false,id,seq+4);
  check(poti0Wait.getValue(),0,id,seq+5);
  check(poti0Wait.getMappedValue(),0,id,seq+6);
  poti0Wait.setRawValue(52);
  check(poti0Wait.hasChanged(),true,id,seq+7);
  check(poti0Wait.getValue(),52,id,seq+8);
  check(poti0Wait.getMappedValue(),1,id,seq+9);
  poti0Wait.setRawValue(511);
  check(poti0Wait.hasChanged(),false,id,seq+10);
  check(poti0Wait.getValue(),52,id,seq+11);
  check(poti0Wait.getMappedValue(),1,id,seq+12);
  poti0Wait.setRawValue(512);
  check(poti0Wait.hasChanged(),true,id,seq+13);
  check(poti0Wait.getValue(),512,id,seq+14);
  check(poti0Wait.getMappedValue(),2,id,seq+15);
  poti0Wait.setRawValue(971);
  check(poti0Wait.hasChanged(),false,id,seq+16);
  check(poti0Wait.getValue(),512,id,seq+17);
  check(poti0Wait.getMappedValue(),2,id,seq+18);
  poti0Wait.setRawValue(972);
  check(poti0Wait.hasChanged(),true,id,seq+19);
  check(poti0Wait.getValue(),972,id,seq+20);
  check(poti0Wait.getMappedValue(),3,id,seq+21);
  poti0Wait.setRawValue(1023);
  check(poti0Wait.hasChanged(),false,id,seq+22);
  check(poti0Wait.getValue(),972,id,seq+23);
  check(poti0Wait.getMappedValue(),3,id,seq+24);

  // performance

  Serial.println("\nPerformance Mapping:");

  Serial.print("1024 * hasChanged(), stretch  0, mapping  4: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(4);
  poti0Wait.setStretch(0); // stretching max
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), stretch 20, mapping  4: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(4);
  poti0Wait.setStretch(20); // stretching max
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), stretch 20, mapping 25, change: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(25);
  poti0Wait.setStretch(20); // stretching max
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), stretch 20, mapping 25,  equal: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(25);
  poti0Wait.setStretch(20); // stretching max
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(10);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");
}

#endif
