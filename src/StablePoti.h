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

#ifndef STABLE_POTI
#define STABLE_POTI

#include "Poti.h"

/*
  Based on the Poti class and all its advantages the StablePoti class adds some
  functionallity to stabalize the measured raw values for a better and easier
  usage of the values.

  If the analog input is comming from an potentiometer, this means with an
  average potentiometer, that the measured analog values may change from one
  to the next measurement even if the potentiometer was not touched in the
  meantime. The variation of the raw values depends on thermical topics,
  mechanical topic (e.g. vibrations), varying power supply voltage (even a
  blinking LED may be a reason) and so on. Therefore two methods are implemented
  internally to stabilize the output value.

  The first method for stabilizing the internal value is the building of an
  average of several raw values before the processing by the next method is
  continued. This method is as option controlled by defining additional
  measurements with a time difference of 1 millisecond by parameter addNumRawAvg.
  Additional measurements based on addNumRawAvg are prioritized to the delay in
  reading raw values based on readCycleMillis.

  The second method for stabilizing the (by first method given) internal new
  value is done by taking the previously given and stabilized value into account.
  New and previous values are added with given weights into one new external
  usable value. This method is as option controlled by parameter weightPrev,
  that defines the weight of the previous value based on a fixed given weight of
  4 for the new value. Side effect of this average calculation is a reduced
  speed in the change of output values. The real analog value will be reached
  with delay. The higher the weight of the previous value is defined, the stronger
  is the delay.

  Advantages:
  - no active waits
  - high performance
  - memory usage per StablePoti instance (17 Byte)
  - handling current and previous value
  - value caching enables stable value analysis
  - easy handling in loops with little code
  - reduction of raw value reads (optional)
  - subclasses for own raw read logic possible (optional)
  - stabilization by calculating average of measurements (optional)
  - stabilization by weighting previous and current value (optional)
*/


class StablePoti : public Poti {

  protected:

    // number of measurements, for building an average raw value, by parameter _addNumRawAvg
    uint8_t _addNumRawAvg;
    // number of currently still necessary measurements required by parameter _addNumRawAvg (0 -> nothing left)
    uint8_t _openNumRawAvg;
    // internal sum of unmapped potentiometer values during internal processing of raw average logic
    int _internalRawAvg;

    // weight defined by parameter weightPrev
    uint8_t _weightPrev;
    // internal previous potentiometer value during processing of weighting logic
    // always set at the end of getStabilizedRawValue() with the resulting value
    int _prevValueInternal;


    /*
      Function for calculation of stabilized raw values.
      Includes the raw value measurment and the delayed read logic.

      @returns  POTI_VALUE_UNDEFINED if value shall be ignored (due to unfinished average
                calculation), otherwise the stabalized value after all necessary processings
    */
    int getStabilizedRawValue(){
      int rawValue, j;
      unsigned long current = millis();

      // processing with a minimum time difference defined by _readCycleMillis
      // but allowing additional measurements defined by _addNumRawAvg and
      // _openNumRawAvg for stabilisation
      if(_openNumRawAvg == 0){
        if(_readCycleMillis > 0 && _lastReadMillis > 0){
          if(current - _lastReadMillis < _readCycleMillis){
            return POTI_VALUE_UNDEFINED;
          }
        }
      }
      else {
        // additional measures with 1 ms difference
        if(current - _lastReadMillis < 1){
          return POTI_VALUE_UNDEFINED;
        }
      }

      _lastReadMillis = current;

      // measurement of current real raw value
      rawValue = getRawValue();

      // do a number of additional measurements and then take an average as raw value
      if(_addNumRawAvg > 0){
        if(_internalRawAvg == POTI_VALUE_UNDEFINED){
          // first measurement after instantiation or reset
          _internalRawAvg = rawValue;
        }
        else if(_openNumRawAvg == 0){
          // first measurement of addNumRawAvg done, additional measurements to be initialized
          _openNumRawAvg = _addNumRawAvg;
          _internalRawAvg = rawValue;
          return POTI_VALUE_UNDEFINED;
        }
        else{
          // next addtional measurement of addNumRawAvg
          _internalRawAvg += rawValue;
          if(--_openNumRawAvg > 0){
            return POTI_VALUE_UNDEFINED;
          }
          // last measurement of the sequence, now average calculation with rounding
          j = _addNumRawAvg + 1;
          rawValue = ((_internalRawAvg * 2) + j) / (j * 2);
        }
      }

      // summerize previous raw value and the new raw value by weighting.
      // different weights of both values. weight for new value is always 4
      // if previous value _prevValueInternal is undefined, directly use new value
      if(_weightPrev > 0 && _prevValueInternal != POTI_VALUE_UNDEFINED){
        j = _weightPrev + 4;
        rawValue = ((rawValue * 4) + (_prevValueInternal * _weightPrev) + (j / 2)) / j;
      }

      // new weighted value for future use in the calculation
      _prevValueInternal = rawValue;

      return rawValue;
    }


  public:

    /*
      Create a new StablePoti object to handle the input of an analog input pin.
      To stabilize the output values, two internal methods are implemented
      can be configured by the parameters.

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
    */
    StablePoti(uint8_t inputPin, uint8_t readCycleMillis, uint8_t weightPrev, uint8_t addNumRawAvg) :
      Poti(inputPin, readCycleMillis){
      _prevValueInternal = POTI_VALUE_UNDEFINED;
      _weightPrev = weightPrev;
      _addNumRawAvg = addNumRawAvg;
      _openNumRawAvg = 0;
      _internalRawAvg = POTI_VALUE_UNDEFINED;

      if(_weightPrev > 12){
        _weightPrev = 12;
      }
      
      if(_addNumRawAvg > 7){
        _addNumRawAvg = 7;
      }
    }


    /*
      Returns the information, if potentiometer value has changed between this
      and the previous call.

      The function must be called continously, at least once per loop run. It
      will measure raw values, calculate stabilization, identify changes and
      set current and previous values.

      @returns  true, if current value has changed or when called first time
    */
    bool hasChanged(){
      int rawValue = getStabilizedRawValue();

      if(rawValue == POTI_VALUE_UNDEFINED){
        return false;
      }

      if(rawValue != _curValue){
        _prevValue = _curValue;
        _curValue = rawValue;
        return true;
      }
      return false;
    }

    /*
      Reset all internal values, so that the behavior is like directly after the
      instantiation and before first call of hasChanged().
    */
    void reset(){
      Poti::reset();
      _prevValueInternal = POTI_VALUE_UNDEFINED;
      _internalRawAvg = POTI_VALUE_UNDEFINED;
      _openNumRawAvg = 0;
    }
};

#endif
