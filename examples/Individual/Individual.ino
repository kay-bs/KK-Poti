/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <Poti.h>

/*
  Example to show the possibility to create
  subclasses with own logic for mapping or
  stabilization of the values measured from
  the connected potentiometer. The function
  getRawValue() is therefore overwittten.

  Prerequisite is an potentiometer connected
  with variable voltage pin to analog input
  pin. Output will be written to Serial.
*/

#define INPUT_PIN A7                  // must be analog pin A0 to A7
#define READ_CYCLE_MILLIS 100         // minimum difference between two actual read of analog raw value
#define POTI_MAX_VALUE 1023           // in this example the max value of analogRead()
#define POTI_STEPS 10                 // number of steps for an individual mapping logic

/*
  Subclass of class Poti, that implements an own function
  getRawValue() to stabilize the values by using an own
  special mapping logic.
*/
class OwnPoti : public Poti {
  public:
    uint8_t _numSteps;
    uint8_t _stepWidth;

    /*
      Additional parameter for number of steps of the individual
      mapping logic. Range from 4 to 60.
    */
    OwnPoti(uint8_t inputPin, uint8_t readCycleMillis, uint8_t numSteps)
      : Poti(inputPin, readCycleMillis){

      _numSteps = 60;
      if(numSteps >= 4 && numSteps <= 60){
        _numSteps = numSteps;
      }
      _stepWidth = byte(POTI_MAX_VALUE / (int)((_numSteps - 1) <<2)); // 4 * steps-1
    }

    /*
      Own function for manipulating the raw value before it is
      used by the normal Poti class. Returned values are in the
      original rawValue() range 0 to POTI_MAX_VALUE, but
      highest return values will be always less than POTI_MAX_VALUE.
      Other option would be to return mapped values directly.
    */
    int getRawValue(){
      int rawValue = Poti::getRawValue();
      int tmpValue = rawValue / _stepWidth;

      if((tmpValue & 0x0003) == 0){
        tmpValue = tmpValue * _stepWidth;
      }
      else if((tmpValue & 0x0003) == 3){
        tmpValue = (tmpValue + 1) * _stepWidth;
      }
      else if(_curValue == POTI_VALUE_UNDEFINED){
        tmpValue = (tmpValue & 0xFFF8) * _stepWidth;
      }
      else{
        tmpValue = _curValue;
      }

      if(tmpValue > POTI_MAX_VALUE){
        tmpValue = tmpValue - 4 * _stepWidth;
      }

      return tmpValue;
    }
};


OwnPoti pot = OwnPoti(INPUT_PIN, READ_CYCLE_MILLIS, POTI_STEPS);

// the setup function is called once for initialization
void setup() {
  // for showing relevant information
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

