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

#ifndef CENTERED_POTI
#define CENTERED_POTI

#include "MappedPoti.h"

/*
  Based on the Poti, StablePoti and MappedPoti classes and all its advantages
  the CenteredPoti class adds some functionality specially for potentiometers
  that are used in cases where a center position is necessary (e.g. for
  treble, bass, balance in amplifiers). The main functionality in this
  class is to transform all given values and standard mappings (starting with
  0 to a max value) to a different range from -x ... 0 ... +x where
  x = (numMapping-1)/2.

  Mapping is used and linear potentiometer shall be used. Logarithmical
  potentiometers are not suitable. For understanding the mapping, the
  description of the MappedPoti class is relevant. Centered potentiometer
  must have always an uneven number of mapping values, so that a symmetric
  distribution of mapping value on the left and right side is possible.
  The central mapping value will always be defined as value 0 compared to
  the linear mapping values from 0 to maxMapping-1 in MappedPoti class.
  The centered mapping values on the left side have negative signs and the
  more the potentiometer is turned left, the more negative becomes the value.
  The centered mapping values on the right side have positive signs and the
  more the potentiometer is turned right, the more positive becomes the value.

  Very important for the mapping is the knowledge of the highest possible
  analog value (defined as maxAnalogVal). For getting and setting this
  information are functions available. Default is 1023, typically for many
  Arduino microcontroller. If the maximum number is different to the
  default, then the function setMaxAnalogValue() must be called before
  first use of function hasChanged() to set the real maximum number
  (e.g. 4095).
  
  Advantages:
  - no active waits
  - high performance
  - memory usage per CenteredPoti instance (27 Byte)
  - handling current and previous value
  - value caching enables stable value analysis
  - easy handling in loops with little code
  - reduction of raw value reads (optional)
  - subclasses for own raw read logic possible (optional)
  - stabilization by calculating average of measurements (optional)
  - stabilization by weighting previous and current value (optional)
  - stabilization by mapping analog values
  - compensation for unequal distribution of analog values (optional)
  - transformation of linear analog and mapping values to center based ranges
*/


class CenteredPoti : public MappedPoti {

  protected:

    // low border for centered potentiometers based on parameters centerVal - centerTol
    int _centerValLow;
    // high border for centered potentiometers based on parameters centerVal + centerTol
    int _centerValHigh;

  public:

    /*
      Create a new CenteredPoti object to handle the input of an analog input pin
      and map the analog values to a defined rang of mapping values from
      -x ... 0 ... +x where x = (numMapping-1)/2.

      Parameter weightPrev defines weight of previous value. Current value has
      fixed weight of 4. The higher the value, the more stable and slower
      will the output value change.
                                
      Parameter addNumRawAvg value x>0 means x+1 measurements are done and
      calculation is x milliseconds delayed (no active waiting). Each
      additional measurement adds 1 millisecond delay before final calculation.

      Parameters centerVal and centerTol must always be set to define the center
      of the potentiometer. Most importand is the analog value centerVal, where
      the physical center of the potentiometer is (in ideal case it would be
      e.g. 511 or 512). Based on the centerVal the width of the center has to be
      defined by centerTol, which is the tolerance on the left and right side of
      centerVal. Center is defined to be analog values (included) from
      (centerVal - centerTol) to (centerVal + centerTol).

      @param  inputPin          Analog pin for reading the analog raw value.
                                Possible pin configuration must be done before
                                hasChanged() calls. Values for Arduino e.g. A0 to A7.
      @param  readCycleMillis   Minimum time in milliseconds that must have been
                                waited between succeeding calls of getRawValue().
                                Values from 0 to 255. Value 0 means no waits and
                                getRawvalue() is called by each hasChanged() call.
      @param  weightPrev        Weight of the previous value, when the new output
                                value is calculated as combined value.
                                Values 0 to 12. Value 0 means no weighting logic.
      @param  addNumRawAvg      Additional nummer of raw value measurements for
                                building an average with first measurement.
                                Values 0 to 7. Value 0 means no average calculation.
      @param  numMapping        Number of mapping values. Must be a uneven number.
                                Even numbers will be increased by one.
                                Range is from 3 to 101. Default is 3.
      @param  stretch           Factor for stretching analog values during the
                                mapping calculation. Values from 0 (no use, linear)
                                to 20. Value 20 ist highest stretching.
      @param  centerTol         Tolerance on left and right side of centerVal.
                                Center is defined as 2*centerTol + 1 values.
                                Values from 10 to 255. Default is 10.
      @param  centerVal         Analog value of the physical center position of the
                                turning knob. If 0 is given, a standard value is
                                calculated internally as maxAnalogVal/2.
    */
    CenteredPoti(uint8_t inputPin, uint8_t readCycleMillis, uint8_t weightPrev, uint8_t addNumRawAvg,
      uint8_t numMapping, uint8_t stretch, uint8_t centerTol, int centerVal) :
      MappedPoti(inputPin, readCycleMillis, weightPrev, addNumRawAvg, numMapping, stretch){

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

      if(centerTol < 10){
        centerTol = 10;
      }

      if(centerVal == 0){
        _centerValLow = (_maxAnalogVal>>1) - centerTol;
        _centerValHigh = (_maxAnalogVal>>1) + centerTol;
      }
      else{
        _centerValLow = centerVal - centerTol;
        _centerValHigh = centerVal + centerTol;
      }
    }


    /*
      Returns the information, if mapping value has changed between this
      and the previous call.

      The function must be called continously, at least once per loop run. It
      will measure raw values, calculate stabilization and mapping, identify
      changes and set current and previous values and mappings.

      @returns  true, if current mapping value has changed or when called
                first time
    */
    bool hasChanged(){
      int internalPrevVal = _prevValueInternal;
      int rawValue = MappedPoti::getStabilizedRawValue();
      uint8_t mapValue;

      if(rawValue == POTI_VALUE_UNDEFINED){
        return false;
      }

      // no change by current measurement?
      if(rawValue == internalPrevVal){
        return false;
      }

      mapValue = MappedPoti::getMapping(rawValue, _centerValLow, _centerValHigh);
      
      // Mapping-Wechsel?
      if(mapValue != _curMapValue){
        _prevValue = _curValue;
        _curValue = rawValue;
        _prevMapValue = _curMapValue;
        _curMapValue = mapValue;
        return true;
      }
      return false;
    }


    /*
      Returns current value in a centered range -y ... 0 ... +z
      with y = minAnalogCenterVal - currentAnalogVal
      and z = maxAnalogVal - maxAnalogCenterVal

      The value was calculated and set by the last call of
      hasChanged() that returned true.

      @returns  current value in the centered range
                or POTI_VALUE_UNDEFINED before first call of hasChanged().
    */
    int getCenteredValue(){
      if(_curValue == POTI_VALUE_UNDEFINED){
        return POTI_VALUE_UNDEFINED;
      }
      if(_curValue < _centerValLow){
        return _curValue - _centerValLow; 
      }
      if(_curValue > _centerValHigh){
        return _curValue - _centerValHigh; 
      }
      return 0;
    }


    /*
      Returns current mapping value in a centered range
      -x ... 0 ... +x where x = (numMapping-1)/2.

      The value was calculated and set by the
      last call of hasChanged() that returned true.

      @returns  current mapping value in the centered range
                or POTI_MAPPING_UNDEFINED before first call of hasChanged()
    */
    int getCenteredMappedValue(){
      if(_curMapValue == POTI_MAPPING_UNDEFINED){
        return POTI_MAPPING_UNDEFINED;
      }
      return ((int)_curMapValue) - (_numMapping>>1);
    }


    /*
      Returns previous value in a centered range -y ... 0 ... +z
      with y = minAnalogCenterVal - currentAnalogVal
      and z = maxAnalogVal - maxAnalogCenterVal

      The value was calculated and set by the last call of
      hasChanged() that returned true. This previous
      value was the current value before hasChanged() was called.

      @returns  previous value in the centered range
                or POTI_VALUE_UNDEFINED before hasChanged() has returned true two times
    */
    int getCenteredPrevValue(){
      if(_prevValue == POTI_VALUE_UNDEFINED){
        return POTI_VALUE_UNDEFINED;
      }
      if(_prevValue < _centerValLow){
        return _prevValue - _centerValLow; 
      }
      if(_prevValue > _centerValHigh){
        return _prevValue - _centerValHigh; 
      }
      return 0;
    }


    /*
      Returns previous mapping value in a centered range
       -x ... 0 ... +x where x = (numMapping-1)/2.

      The value was calculated and set by the last call of
      hasChanged() that returned true. This previous
      value was the current value before hasChanged() was called.

      @returns  previous mapping value in the centered range
                or POTI_MAPPING_UNDEFINED before hasChanged() has returned true two times
    */
    int getCenteredMappedPrevValue(){
      if(_prevMapValue == POTI_MAPPING_UNDEFINED){
        return POTI_MAPPING_UNDEFINED;
      }
      return ((int)_prevMapValue) - (_numMapping>>1);

    }
};

#endif
