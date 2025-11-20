/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#ifndef CLASSES_TESTPOTI
#define CLASSES_TESTPOTI

#include "CenteredPoti.h"

/*
  Subclass of class Poti, that implements functionality for testing.
*/
class TestPoti : public Poti {
  private:
    int _internalValue;

  public:
    TestPoti(uint8_t inputPin, uint8_t readCycleMillis)
      : Poti(inputPin, readCycleMillis){};

    int getRawValue(){
      return _internalValue;
    }

    void setRawValue(int value){
      _internalValue = value;
    }

    void setReadCycleMillis(uint8_t readCycleMillis){
      _readCycleMillis = readCycleMillis;
    }
};

/*
  Subclass of class StablePoti, that implements functionality for testing.
*/
class TestStablePoti : public StablePoti {
  private:
    int _internalValue;

  public:
    TestStablePoti(uint8_t inputPin, uint8_t readCycleMillis,
                   uint8_t weightPrev, uint8_t addNumRawAvg)
      : StablePoti(inputPin, readCycleMillis, weightPrev, addNumRawAvg){};

    int getRawValue(){
      return _internalValue;
    }

    void setRawValue(int value){
      _internalValue = value;
    }

    void setReadCycleMillis(uint8_t readCycleMillis){
      _readCycleMillis = readCycleMillis;
    }

    void setAddNumRawAvg(uint8_t addNumRawAvg){
      _addNumRawAvg = addNumRawAvg;
      if(_addNumRawAvg > 7){
        _addNumRawAvg = 7;
      }
    }

    void setWeightPrev(uint8_t weightPrev){
      _weightPrev = weightPrev;
      if(_weightPrev > 12){
        _weightPrev = 12;
      }
    }
};


/*
  Subclass of class MappedPoti, that implements functionality for testing.
*/
class TestMappedPoti : public MappedPoti {
  private:
    int _internalValue;

  public:
    TestMappedPoti(uint8_t inputPin, uint8_t readCycleMillis,
                  uint8_t weightPrev, uint8_t addNumRawAvg,
                  uint8_t numMapping, uint8_t stretch)
      : MappedPoti(inputPin, readCycleMillis,
                  weightPrev, addNumRawAvg,
                  numMapping, stretch){};

    int getRawValue(){
      return _internalValue;
    }

    void setRawValue(int value){
      _internalValue = value;
    }

    void setReadCycleMillis(uint8_t readCycleMillis){
      _readCycleMillis = readCycleMillis;
    }

    void setAddNumRawAvg(uint8_t addNumRawAvg){
      _addNumRawAvg = addNumRawAvg;
      if(_addNumRawAvg > 7){
        _addNumRawAvg = 7;
      }
    }

    void setWeightPrev(uint8_t weightPrev){
      _weightPrev = weightPrev;
      if(_weightPrev > 12){
        _weightPrev = 12;
      }
    }

    void setNumMapping(uint8_t numMapping){
      _numMapping = numMapping;

      if(_numMapping > 100){
        _numMapping = 100;
      }

      if(_numMapping < 2){
        _numMapping = 2;
      }
    }

    void setStretch(uint8_t stretch){
      _stretch = stretch;
      if(_stretch > 20){
        _stretch = 20;
      }
    }
};

/*
  Subclass of class CenteredPoti, that implements functionality for testing.
*/
class TestCenteredPoti : public CenteredPoti {
  private:
    int _internalValue;

  public:
    TestCenteredPoti(uint8_t inputPin, uint8_t readCycleMillis,
                    uint8_t weightPrev, uint8_t addNumRawAvg,
                    uint8_t numMapping, uint8_t stretch,
                    uint8_t centerTol, int centerVal)
      : CenteredPoti(inputPin, readCycleMillis,
                  weightPrev, addNumRawAvg,
                  numMapping, stretch,
                  centerTol, centerVal){};

    int getRawValue(){
      return _internalValue;
    }

    void setRawValue(int value){
      _internalValue = value;
    }

    void setAddNumRawAvg(uint8_t addNumRawAvg){
      _addNumRawAvg = addNumRawAvg;
      if(_addNumRawAvg > 7){
        _addNumRawAvg = 7;
      }
    }

    void setWeightPrev(uint8_t weightPrev){
      _weightPrev = weightPrev;
      if(_weightPrev > 12){
        _weightPrev = 12;
      }
    }

    void setNumMapping(uint8_t numMapping){
      _numMapping = numMapping;
      if(_numMapping < 3){
        _numMapping = 3;
      }

      if(_numMapping > 101){
        _numMapping = 101;
      }

      // uneven number?
      if((_numMapping & 0x01) == 0){
        _numMapping++;
      }
    }

    void setStretch(uint8_t stretch){
      _stretch = stretch;
      if(_stretch > 20){
        _stretch = 20;
      }
    }

    int getCenterValLow(){
      return _centerValLow;
    }

    int getCenterValHigh(){
      return _centerValHigh;
    }

    int setCenterValLow(int centerValLow){
      _centerValLow = centerValLow;
      return _centerValLow;
    }

    int setCenterValHigh(int centerValHigh){
      _centerValHigh = centerValHigh;
      return _centerValHigh;
    }
};

#endif
