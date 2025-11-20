/*
  The MIT License (MIT)

  Copyright (c) 2025 Kay Kasper

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the “Software”),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  The Software is provided “as is”, without warranty of any kind, express
  or implied, including but not limited to the warranties of merchantability,
  fitness for a particular purpose and noninfringement. In no event shall
  the authors or copyright holders be liable for any claim, damages or other
  liability, whether in an action of contract, tort or otherwise, arising
  from, out of or in connection with the software or the use or other dealings
  in the Software.
*/

#ifndef POTI
#define POTI

#include <Arduino.h>

#define POTI_VALUE_UNDEFINED    0x7FFF

/*
  The Poti class is used for easy handling of potentiometers, attenuators and
  other kinds of analog input signals.

  Analog input signals are directly returned without stabilization by reading
  a raw value from an anlog input pin.

  Normally it is interresting to check the measured value only, when it changed.
  Then typically actions have to be started. Therefore the logic in the Poti
  class is to ask in the processing loop only with function hasChanged(), if
  something has changed between now and the previous call of hasChanged().

  The evaluation of value changes is done only in the function hasChanged()
  and it is the only function, that calls getRawState(), to read the raw input
  value. Therefore the function has to be called in the programm loop at least
  once, to identify changes as fast as necessary.

  Only when hasChanged() deliveres true, a different value than before must be
  analyzed. Between calls of hasChanged() the values are stable and will not
  change. The functions getValue() and getPrevValue() will always return the
  value, that was identified and internally set by the last call of hasChanged().

  When function hasChanged() is called the first time, it always returns true.
  This is because the internally used previous value is defined as
  POTI_VALUE_UNDEFINED. This makes it possible to react immediately when the
  programm starts in the loop and no other initialization logic is necessary.

  Raw input value measurements (by function getRawValue()) is based on a slow
  analog to digital conversion (about 100 nanos in Arduinos). Therefore the
  number of calls of getRawState() calls can optionally be reduced to a minimum
  with an included logic that implements a minimum time delay between the calls
  of getRawValue().

  The function getRawValue() can be overwritten by a subclass, so that a
  possibility is given to use whatever input and translate it into different
  "analog" values.

  Normally it is not necessary to configure the used analog input pin for analog
  read. But if it is necessary, this configuration must be done in the setup()
  before the first time hasChanged() is called.

  If the analog input is comming from an potentiometer, this means with an
  average potentiometer, that the measured analog values may change from one
  to the next measurement even if the potentiometer was not touched in the
  meantime. The variation of the raw values depends on thermical topics,
  mechanical topic (e.g. vibrations), varying power supply voltage (even a
  blinking LED may be a reason) and so on.

  Linear and logarithmical potentiometers are supported by Poti class. But "linear"
  means in most cases not, that the measured values are equally distibuted.
  Normally are the values slowly changing at the beginning and the end of the
  movement. In the middle position normally values are changing fast with little
  movement. This has to be taken into account.

  Advantages:
  - no active waits
  - high performance
  - low memory usage per Poti instance (10 Byte)
  - handling current and previous state
  - value caching enables stable value analysis
  - easy handling in loops with little code
  - reduction of raw value reads (optional)
  - subclasses for own raw read logic possible (optional)
*/


class Poti {

  protected:

    // to be used analog input pin, defined by parameter inputPin
    uint8_t _inputPin;
    // milliseconds defined by parameter readCycleMillis
    uint8_t _readCycleMillis;
    // timestamp of last measurement of the potentiometer value, for implementation of _readCycleMillis
    unsigned long _lastReadMillis;

    // current unmapped potentiometer value for external requests/use
    int _curValue;
    // previous unmapped potentiometer value for external requests/use
    int _prevValue;


    /*
      Returns the raw analog value. The function can be overwritten for
      implementing an own logic.
      
      In this default implementation the value is read by an A/D converter
      of the used microcontroller. Depending on the microcontoller, values
      are in the range of 0 to MAX with MAX = 1023 for most Arduino controllers.
      The default implementation uses analogRead(inputPin) and about 0,1ms
      on Arduino controllers to make the measurement.

      @returns  raw value from 0 to MAX (typically 1023) of the specific microcontroller
    */
    virtual int getRawValue(){
      return analogRead(_inputPin);
    }


  public:

    /*
      Create a new Poti object to handle the input of an analog input pin.

      @param  inputPin          Analog pin for reading the analog raw value.
                                Possible pin configuration must be done before
                                hasChanged() calls. Values for Arduino e.g. A0 to A7.
      @param  readCycleMillis   Minimum time in milliseconds that must have been
                                waited between succeeding calls of getRawValue().
                                Values from 0 to 255. Value 0 means no waits and
                                getRawvalue() is called by each hasChanged() call.
    */
    Poti(uint8_t inputPin, uint8_t readCycleMillis){
      _curValue = POTI_VALUE_UNDEFINED;
      _prevValue = POTI_VALUE_UNDEFINED;
      _inputPin = inputPin;
      _readCycleMillis = readCycleMillis;
      _lastReadMillis = 0;
    }


    /*
      Returns current value. The value was calculated and set by the last call
      of hasChanged() that returned true.

      @returns  current value from 0 to MAX (typically 1023) of the specific microcontroller
                or POTI_VALUE_UNDEFINED before first call of hasChanged().
    */
    int getValue(){
      return _curValue;
    }


    /*
      Returns the information, if potentiometer value has changed between this
      and the previous call.

      The function must be called continously, at least once per loop run. It
      will measure raw values, identify changes and set current and previous
      values.

      @returns  true, if current value has changed or when called first time
    */
    bool hasChanged(){
      int rawValue;
      unsigned long current = millis();

      if(_readCycleMillis > 0 && _lastReadMillis > 0){
        if(current - _lastReadMillis < _readCycleMillis){
          return false;
        }
      }

      _lastReadMillis = current;

      // measurement of current real raw value
      rawValue = getRawValue();

      if(rawValue != _curValue){
        _prevValue = _curValue;
        _curValue = rawValue;
        return true;
      }
      return false;
    }


    /*
      Returns previous value. The value was calculated and set by the last call
      of hasChanged() that returned true. This previous value was the current
      value before hasChanged() was called.

      @returns  previous value from 0 to MAX (typically 1023) of the specific microcontroller
                or POTI_VALUE_UNDEFINED before hasChanged() has returned true two times
    */
    int getPrevValue(){
      return _prevValue;
    }


    /*
      Reset all internal values, so that the behavior is like directly after the
      instantiation and before first call of hasChanged().
    */
    void reset(){
      _curValue = POTI_VALUE_UNDEFINED;
      _prevValue = POTI_VALUE_UNDEFINED;
      _lastReadMillis = 0;
    }
};

#endif
