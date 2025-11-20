/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef CENTEREDPOTITESTS_TESTPOTI
#define CENTEREDPOTITESTS_TESTPOTI

#include "Common.h"

void doCenteredPotiTest(int id){  // ID_CENTEREDTEST = 4
  TestCenteredPoti poti0Wait(INPUT_PIN, 0, 0, 0, 25, 0, 81, 512);
  unsigned long startmicro = 0;
  int seq, j;
  double x;

  seq = 0;
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+1);
  check(poti0Wait.getCenteredValue(),POTI_VALUE_UNDEFINED,id,seq+2);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+3);
  check(poti0Wait.getCenteredPrevValue(),POTI_VALUE_UNDEFINED,id,seq+4);

  poti0Wait.setRawValue(10);
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+4);
  check(poti0Wait.hasChanged(),true,id,seq+5);
  check(poti0Wait.getValue(),10,id,seq+6);
  check(poti0Wait.getCenteredValue(),-421,id,seq+7);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+8);
  check(poti0Wait.getCenteredPrevValue(),POTI_VALUE_UNDEFINED,id,seq+9);
  check(poti0Wait.hasChanged(),false,id,seq+10);

  seq = 10;
  poti0Wait.setRawValue(1023);
  check(poti0Wait.getValue(),10,id,seq+1);
  check(poti0Wait.getCenteredValue(),-421,id,seq+2);
  check(poti0Wait.hasChanged(),true,id,seq+3);
  check(poti0Wait.getValue(),1023,id,seq+4);
  check(poti0Wait.getCenteredValue(),430,id,seq+5);
  check(poti0Wait.getPrevValue(),10,id,seq+6);
  check(poti0Wait.getCenteredPrevValue(),-421,id,seq+7);
  check(poti0Wait.hasChanged(),false,id,seq+8);
  check(poti0Wait.getValue(),1023,id,seq+9);
  check(poti0Wait.getPrevValue(),10,id,seq+10);

  seq = 20;
  poti0Wait.reset();
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+1);
  check(poti0Wait.getCenteredValue(),POTI_VALUE_UNDEFINED,id,seq+2);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+3);
  check(poti0Wait.getCenteredPrevValue(),POTI_VALUE_UNDEFINED,id,seq+4);
  poti0Wait.setRawValue(100);
  check(poti0Wait.getValue(),POTI_VALUE_UNDEFINED,id,seq+5);
  check(poti0Wait.getCenteredValue(),POTI_VALUE_UNDEFINED,id,seq+6);
  check(poti0Wait.hasChanged(),true,id,seq+7);
  check(poti0Wait.getValue(),100,id,seq+8);
  check(poti0Wait.getCenteredValue(),-331,id,seq+9);
  check(poti0Wait.getPrevValue(),POTI_VALUE_UNDEFINED,id,seq+10);
  check(poti0Wait.getCenteredPrevValue(),POTI_VALUE_UNDEFINED,id,seq+11);

  poti0Wait.setNumMapping(20);
  check(poti0Wait.getNumMappingValues(),21,id,seq+12);
  poti0Wait.setNumMapping(1);
  check(poti0Wait.getNumMappingValues(),3,id,seq+13);
  poti0Wait.setNumMapping(102);
  check(poti0Wait.getNumMappingValues(),101,id,seq+14);

  // check mapping

  seq = 40;
  poti0Wait.setNumMapping(25);
  check(poti0Wait.getNumMappingValues(),25,id,seq+0);
  poti0Wait.reset();
  check(poti0Wait.getMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+1);
  check(poti0Wait.getCenteredMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+3);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+2);
  check(poti0Wait.getCenteredMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+4);
  poti0Wait.setRawValue(0);
  check(poti0Wait.getMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+5);
  check(poti0Wait.getCenteredMappedValue(),POTI_MAPPING_UNDEFINED,id,seq+6);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+7);
  check(poti0Wait.getCenteredMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+8);
  check(poti0Wait.hasChanged(),true,id,seq+9);
  check(poti0Wait.getMappedValue(),0,id,seq+10);
  check(poti0Wait.getCenteredMappedValue(),-12,id,seq+11);
  check(poti0Wait.getMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+12);
  check(poti0Wait.getCenteredMappedPrevValue(),POTI_MAPPING_UNDEFINED,id,seq+13);
  poti0Wait.setRawValue(poti0Wait.getMaxAnalogValue());
  check(poti0Wait.hasChanged(),true,id,seq+14);
  check(poti0Wait.getMappedValue(),24,id,seq+15);
  check(poti0Wait.getCenteredMappedValue(),12,id,seq+16);
  check(poti0Wait.getMappedPrevValue(),0,id,seq+17);
  check(poti0Wait.getCenteredMappedPrevValue(),-12,id,seq+18);
  poti0Wait.setRawValue(poti0Wait.getMaxAnalogValue()/2);
  check(poti0Wait.hasChanged(),true,id,seq+19);
  check(poti0Wait.getMappedValue(),12,id,seq+20);
  check(poti0Wait.getCenteredMappedValue(),0,id,seq+21);
  check(poti0Wait.getMappedPrevValue(),24,id,seq+22);
  check(poti0Wait.getCenteredMappedPrevValue(),12,id,seq+23);

  seq = 70;
  poti0Wait.reset();
  check(poti0Wait.setMaxAnalogValue(4095),4095,id,seq+1);
  poti0Wait.setNumMapping(25);
  x = poti0Wait.getMaxAnalogValue() / poti0Wait.getNumMappingValues();
  check(x,163,id,seq+2);
  poti0Wait.setCenterValLow((4096 / 2) - (x / 2));
  poti0Wait.setCenterValHigh((4096 / 2) + (x / 2));
  j = 0;
  for(int i = 0 ; i < 4096 ; i++){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+3);
      check(poti0Wait.getPrevValue(),(i==0 ? POTI_VALUE_UNDEFINED : j),id,seq+4);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x),id,seq+5);
      check(poti0Wait.getCenteredMappedValue(),int(i/x) - 12,id,seq+6);
      check(poti0Wait.getMappedPrevValue(),(i==0 ? POTI_MAPPING_UNDEFINED : int((i/x)-1)),id,seq+7);
    }
    check(poti0Wait.hasChanged(),false,id,seq+8);
  }

  seq = 80;
  poti0Wait.reset();
  check(poti0Wait.setMaxAnalogValue(1023),1023,id,seq+1);
  poti0Wait.setNumMapping(25);
  x = poti0Wait.getMaxAnalogValue() / poti0Wait.getNumMappingValues();
  check(x,40,id,seq+2);
  poti0Wait.setCenterValLow((1024 / 2) - (x / 2));
  poti0Wait.setCenterValHigh((1024 / 2) + (x / 2));
  j = 0;
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+3);
      check(poti0Wait.getPrevValue(),(i==0 ? POTI_VALUE_UNDEFINED : j),id,seq+4);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x),id,seq+5);
      check(poti0Wait.getCenteredMappedValue(),int(i/x) - 12,id,seq+6);
      check(poti0Wait.getMappedPrevValue(),(i==0 ? POTI_MAPPING_UNDEFINED : int((i/x)-1)),id,seq+7);
    }
    check(poti0Wait.hasChanged(),false,id,seq+8);
  }

  // value = 1023 set / expected

  seq = 90;
  j = poti0Wait.getValue();
  for(int i = 1022 ; i >= 0 ; i--){
    poti0Wait.setRawValue(i);
    if(poti0Wait.hasChanged()){
      check(poti0Wait.getValue(),i,id,seq+1);
      check(poti0Wait.getPrevValue(),j,id,seq+2);
      j = poti0Wait.getValue();
      check(poti0Wait.getMappedValue(),int(i/x)-1,id,seq+3);
      check(poti0Wait.getCenteredMappedValue(),int(i/x)-1 - 12,id,seq+4);
      check(poti0Wait.getMappedPrevValue(),int((i/x)),id,seq+5);
      check(poti0Wait.getCenteredMappedPrevValue(),int((i/x)) - 12,id,seq+6);
    }
    check(poti0Wait.hasChanged(),false,id,seq+7);
  }

  // now check mapping and range translation, center 512

  seq = 100;
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(5);
  x = poti0Wait.getMaxAnalogValue() / poti0Wait.getNumMappingValues();
  check(x,204,id,seq+0);
  poti0Wait.setCenterValLow((1024 / 2) - (x / 2));
  poti0Wait.setCenterValHigh((1024 / 2) + (x / 2));
  poti0Wait.reset();
  poti0Wait.setRawValue(0);
  check(poti0Wait.hasChanged(),true,id,seq+1);
  check(poti0Wait.getValue(),0,id,seq+2);
  check(poti0Wait.getMappedValue(),0,id,seq+3);
  check(poti0Wait.getCenteredValue(),-410,id,seq+4);
  check(poti0Wait.getCenteredMappedValue(),-2,id,seq+5);
  poti0Wait.setRawValue(204);
  check(poti0Wait.hasChanged(),false,id,seq+6);
  check(poti0Wait.getValue(),0,id,seq+5);
  check(poti0Wait.getMappedValue(),0,id,seq+7);
  check(poti0Wait.getCenteredValue(),-410,id,seq+8);
  check(poti0Wait.getCenteredMappedValue(),-2,id,seq+9);
  poti0Wait.setRawValue(205);
  check(poti0Wait.hasChanged(),true,id,seq+10);
  check(poti0Wait.getValue(),205,id,seq+11);
  check(poti0Wait.getMappedValue(),1,id,seq+12);
  check(poti0Wait.getCenteredValue(),-205,id,seq+13);
  check(poti0Wait.getCenteredMappedValue(),-1,id,seq+14);
  poti0Wait.setRawValue(409);
  check(poti0Wait.hasChanged(),false,id,seq+15);
  check(poti0Wait.getValue(),205,id,seq+16);
  check(poti0Wait.getMappedValue(),1,id,seq+17);
  check(poti0Wait.getCenteredValue(),-205,id,seq+18);
  check(poti0Wait.getCenteredMappedValue(),-1,id,seq+19);
  poti0Wait.setRawValue(410);
  check(poti0Wait.hasChanged(),true,id,seq+20);
  check(poti0Wait.getValue(),410,id,seq+21);
  check(poti0Wait.getMappedValue(),2,id,seq+22);
  check(poti0Wait.getCenteredValue(),0,id,seq+23);
  check(poti0Wait.getCenteredMappedValue(),0,id,seq+24);
  poti0Wait.setRawValue(614);
  check(poti0Wait.hasChanged(),false,id,seq+25);
  check(poti0Wait.getValue(),410,id,seq+26);
  check(poti0Wait.getMappedValue(),2,id,seq+27);
  check(poti0Wait.getCenteredValue(),0,id,seq+28);
  check(poti0Wait.getCenteredMappedValue(),0,id,seq+29);
  poti0Wait.setRawValue(615);
  check(poti0Wait.hasChanged(),true,id,seq+30);
  check(poti0Wait.getValue(),615,id,seq+31);
  check(poti0Wait.getMappedValue(),3,id,seq+32);
  check(poti0Wait.getCenteredValue(),1,id,seq+33);
  check(poti0Wait.getCenteredMappedValue(),1,id,seq+34);
  poti0Wait.setRawValue(818);
  check(poti0Wait.hasChanged(),false,id,seq+35);
  check(poti0Wait.getValue(),615,id,seq+36);
  check(poti0Wait.getMappedValue(),3,id,seq+37);
  check(poti0Wait.getCenteredValue(),1,id,seq+38);
  check(poti0Wait.getCenteredMappedValue(),1,id,seq+39);
  poti0Wait.setRawValue(819);
  check(poti0Wait.hasChanged(),true,id,seq+40);
  check(poti0Wait.getValue(),819,id,seq+41);
  check(poti0Wait.getMappedValue(),4,id,seq+42);
  check(poti0Wait.getCenteredValue(),205,id,seq+43);
  check(poti0Wait.getCenteredMappedValue(),2,id,seq+44);
  poti0Wait.setRawValue(1023);
  check(poti0Wait.hasChanged(),false,id,seq+45);
  check(poti0Wait.getValue(),819,id,seq+46);
  check(poti0Wait.getMappedValue(),4,id,seq+47);
  check(poti0Wait.getCenteredValue(),205,id,seq+48);
  check(poti0Wait.getCenteredMappedValue(),2,id,seq+49);

  // performance

  Serial.println("\nPerformance Centered:");

  Serial.print("1024 * hasChanged(), mapping 15, change: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(15);
  poti0Wait.setCenterValLow(444);
  poti0Wait.setCenterValHigh(580);
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(i);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");

  Serial.print("1024 * hasChanged(), mapping 15,  equal: ");
  poti0Wait.setMaxAnalogValue(1023);
  poti0Wait.setNumMapping(15);
  poti0Wait.setCenterValLow(444);
  poti0Wait.setCenterValHigh(580);
  poti0Wait.reset();
  startmicro = micros();
  for(int i = 0 ; i < 1024 ; i++){
    poti0Wait.setRawValue(0);
    poti0Wait.hasChanged();
  }
  Serial.print(micros() - startmicro);
  Serial.println(" micros");
}

#endif
