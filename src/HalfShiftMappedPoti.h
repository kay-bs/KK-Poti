/*
  MIT License

  Copyright (c) 2026 Kay Kasper

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef HALF_SHIFT_MAPPED_POTI
#define HALF_SHIFT_MAPPED_POTI

#include "MappedPoti.h"

/*
  Based on the Poti, StablePoti and MappedPoti classes and all its advantages
  the HalfShiftMappedPoti class implements a different kind of mapping.

  Compared to the normal mapping of an ideal linear potentiometer without
  stretching, where the mapping logic trys to map a fixed number of analog
  values to one mapping value (e.g. 10 mapping values and 1024 analog values
  result in about 102 analog values per mapping value -> analog 0 to 101 maps to
  mapping 0, ..., analog 922 to 1023 maps to mapping 9),
  the half shift mapping logic shifts the lower half of the analog values
  to a lower mappig value. This results for the given example in:

  analog 0 to 56 maps to 0     (half of the number of mapped analog values)
  analog 57 to 170 maps to 1   (full number of mapped analog values)
  analog 171 to 283 maps to 2
  ...
  analog 853 to 967 maps to 8  (full number of mapped analog values)
  analog 968 to 1023 maps to 9 (half of the number of mapped analog values)

  The mapping logic is ideal for situations, where analog values with expected
  fixed analog difference shall be mapped with enough tolerance
  (e.g. 0.5V difference with 11 mapping values: 0V, 0.5V, 1.0V, ... , 4.5V, 5V).

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
  - memory usage per HalfShiftMappedPoti instance (23 Byte)
  - handling current and previous value
  - value caching enables stable value analysis
  - easy handling in loops with little code
  - reduction of raw value reads (optional)
  - subclasses for own raw read logic possible (optional)
  - stabilization by calculating average of measurements (optional)
  - stabilization by weighting previous and current value (optional)
  - stabilization by mapping analog values
  - compensation for unequal distribution of analog values (optional)
  - transformation of linear analog and mapping values with very high tolerance
*/


class HalfShiftMappedPoti : public MappedPoti {

  public:

    /*
      Create a new HalfShiftMappedPoti object to handle the input of an
      analog input pin and map the analog values to a defined rang of mapping
      values with half width of the mapping range at the beginning and the end
      of the analog value range.

      Parameter weightPrev defines weight of previous value. Current value has
      fixed weight of 4. The higher the value, the more stable and slower
      will the output value change.
                                
      Parameter addNumRawAvg value x>0 means x+1 measurements are done and
      calculation is x milliseconds delayed (no active waiting). Each
      additional measurement adds 1 millisecond delay before final calculation.

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
      @param  numMapping        Number of mapping values.
                                Range is from 2 to 100. Default is 2.
      @param  stretch           Factor for stretching analog values during the
                                mapping calculation. Values from 0 (no use, linear)
                                to 20. Value 20 ist highest stretching.
    */
    HalfShiftMappedPoti(uint8_t inputPin, uint8_t readCycleMillis, uint8_t weightPrev, uint8_t addNumRawAvg,
      uint8_t numMapping, uint8_t stretch) :
      MappedPoti(inputPin, readCycleMillis, weightPrev, addNumRawAvg, numMapping, stretch){

      _numMapping = numMapping;
      if(_numMapping < 2){
        _numMapping = 2;
      }

      if(_numMapping > 100){
        _numMapping = 100;
      }
      // switch to different doubled mapping
      _numMapping = (_numMapping - 1) * 2; 
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
      int rawValue = getStabilizedRawValue();
      uint8_t mapValue;

      if(rawValue == POTI_VALUE_UNDEFINED){
        return false;
      }

      // no change by current measurement?
      if(rawValue == internalPrevVal){
        return false;
      }

      mapValue = getMapping(rawValue, 0, 0);
      mapValue = (mapValue + 1) / 2;
      
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
      Returns the number of defined mapping values.

      @returns  number of mapping values
    */
    uint8_t getNumMappingValues(){
      return (_numMapping + 2) / 2;
    }
};

#endif
