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

#ifndef MAPPED_POTI
#define MAPPED_POTI

#include "StablePoti.h"

#define POTI_MAPPING_UNDEFINED  0xFF

/*
  Based on the Poti and StablePoti classes and all its advantages the
  MappedPoti class adds some functionallity for mapping the analog values
  to a configurable new and different range of values. Mapping reduces the
  precision of the analog values to a lower number of values and can also
  be seen as another stabilization method for easier handling of analog
  values based on potentiometers.

  When mapping is used, linear potentiometer shall be used. Logarithmical
  potentiometers are not suitable.

  Every returned value of getRawValue() is in relation to a position of
  a specific potentiometer. In this implementation it is always assumed,
  that the lowest raw value is 0 and the position for this value is when
  the potentiometer is turned completely to the left. When turned completely
  to the right side the maximum value (e.g. 1023 or 4095, depending on
  specific microcontroller) is read. The lowest mapping values of 0 always
  includes the analog value of 0 and the highest mapping value always
  includes the highest analog value.

  The range of mapping values is defined as 0 to numMapping - 1.
  "numMapping" is an instantiation parameter and allows values from 2 to
  100. If the parameter value is uneven, then the potentiometer is assumed
  to have the middle mapping value around the middle analog value (e.g.
  511 for maximum analog value 1023). The mapping is implemented in standard
  case (instantiation parameter stretch=0) by assuming a linear distribution
  of analog values and by calculation how many analog values need to be
  summerized in one mapping value. With ideal linear potentiometers each
  mapping value would mean the same distance/way that the potentiometer must
  be turned. During the calculation rounding errors may occour.

  Example for standard mapping by setting numMapping = 4 and stretch=0:
  0-255 -> mapping 0; 256-511 -> mapping 1; 512-767 -> mapping 2; 768-1023 -> mapping 3

  As already described in Poti class, linear potentiometers are in reality
  not linear. "linear" means in most cases not, that the measured values
  are equally distibuted. Normally are the values slowly changing at the
  beginning and the end of the movement. In the middle position normally
  values are changing fast with little movement.

  The MappedPoti class implements a method to compensate the not linear
  distribution of analog values. With instantiation parameter "stretch"
  the distribution of analog values to mapping values can be influenced.
  The higher the configured stretching is, the more values in the middle
  and the less values at the edges of the potentiometer are summerized in
  one mapping value. As a consequence the movement in the middle is bigger
  and to the edges is smaller for one mapping value than with linear mapping.

  In general it is still relevant ot have stable analog input values for
  MappedPoti because often changing analog values at the border of two
  mapping values may cause permanent changing of mapping values. Therefore
  MappedPoti is based on StablePoti.

  The external view of the MappedPoti is based on mapping values. Additional
  functions have been created for handling the mapping. The hasChanged()
  function reacts only on changed mapping values. When changes occured, the
  analog value of getValue() is the one, that was relevant for the change.
  Analog values can change in the background without reflection in the value
  functions. The mapping values are always based on the stored analog values.

  Very important for the mapping is the knowledge of the highest possible
  analog value (defined as maxAnalogVal). For getting and setting this
  information are functions available. Default is 1023, typically for many
  Arduino microcontroller. If the maximum number is different to the default,
  then the function setMaxAnalogValue() must be called before first use
  of function hasChanged() to set the real maximum number (e.g. 4095).
  
  Advantages:
  - no active waits
  - high performance
  - memory usage per MappedPoti instance (23 Byte)
  - handling current and previous value
  - value caching enables stable value analysis
  - easy handling in loops with little code
  - reduction of raw value reads (optional)
  - subclasses for own raw read logic possible (optional)
  - stabilization by calculating average of measurements (optional)
  - stabilization by weighting previous and current value (optional)
  - stabilization by mapping analog values
  - compensation for unequal distribution of analog values (optional)
*/


class MappedPoti : public StablePoti {

  protected:

    // mapped value for external requests/use based on _curValue
    uint8_t _curMapValue;
    // mapped value for external requests/use based on _prevValue
    uint8_t _prevMapValue;
    // number of requested mapping values, defined by parameter numMapping
    uint8_t _numMapping;
    // stretching of values to improve linear mapping, defined by parameter scale
    uint8_t _stretch;
    // maximum value that the analog read function can deliver, often and default is 1023
    int _maxAnalogVal;

    /*
      Internal calculation of the mapping value suitable for the given analog
      value (rawValue).
      If centered potentiometer is given, the center can be defined by lowest
      and highest analog value of the center mapping. If no values are externally
      given for a centered potientiometer but an uneven numMapping is defined,
      then internally a center is defined automatically.

      @param      rawValue        the analog input value that has to be mapped
      @param      centerValLow    lowest analog value of the center mapping or 0
      @param      centerValHigh   highest analog value of the center mapping or 0
      @returns                    mapped value suitable for the rawValue
    */
    uint8_t getMapping(int rawValue, int centerValLow, int centerValHigh){
      /*
        For calculating the mapping including the stretching, the analog
        values need to be separated into left and right side for processing.
        Left side are all values smaller than the lowest center value or an
        internal defined center (e.g. 511) = middle of the maximum analog
        value (e.g. 1023 / 2).

        ValHigh = highest analog value that's part of relevant value range (e.g. 510 or 1023)
        ValTot = total number of analog values in relevant value range (e.g. 511 or 512)
        MapTot = total number of mapping values for relevant value range
        Stretch = stretching strength, values 0 (no stretching, linear) to 20 (strong)
        Scale = 1 + Stretch/10
        ValCur = rawValue = current analog value
        MapCur = resulting mapping value suitable for rawValue
        StdDiv = number of analog values to be mapped to one mapping value, ValTot / MapTot
        [] = linear scaling function for the mapping divider in the range of 0 to 1 with slope and shifting
        {} = variable mapping divider depending on rawValue (standard mapping * scaling)

        Formular for left side:

        MapCur = trunc(ValCur / {StdDiv / Scale
                  * [(Scale - 1 / Scale) * ValCur / ValTot + 1 / Scale]})

        Formular for right side:

        MapCur = (2 * MapTot - 1)
                - trunc((ValHigh - ValCur) / {StdDiv / Scale
                * [(Scale - 1 / Scale) * (ValHigh - ValCur) / ValTot + 1 / Scale]})
      */

      float scale = 0.0;
      float stdDiv = 0.0;
      float valTot = 0.0;
      float mapTot = 0.0;
      float tmpFloat = 0.0;
      uint8_t mapValue;
      int i;
      bool centered = (centerValLow > 0);
      bool leftSide;

      // also centered, if uneven mapping number,
      // but no overwriting of externally given center values
      if(!centered && (_numMapping & 0x01) > 0){
        centered = true;
        i = ((_maxAnalogVal + 1) / _numMapping)>>1;
        centerValLow = (_maxAnalogVal>>1) - i;
        centerValHigh = (_maxAnalogVal>>1) + i;
      }

      // if center values available then special treatment of center position upfront
      if(centered && rawValue >= centerValLow && rawValue <= centerValHigh){
        return (_numMapping>>1); // uneven mapping number, middle value
      }

      leftSide = (centered && rawValue < centerValLow) || (!centered && rawValue < ((_maxAnalogVal + 1)>>1));

      if(leftSide){
        // left side
        valTot = (!centered ? (_maxAnalogVal + 1)>>1 : centerValLow);
        mapTot = (!centered ? _numMapping>>1 : (_numMapping - 1)>>1);
      }
      else{
        // right side
        valTot = (!centered ? (_maxAnalogVal + 1)>>1 : _maxAnalogVal - centerValHigh);
        mapTot = (!centered ? _numMapping>>1 : (_numMapping - 1)>>1);
      }
      stdDiv = valTot / mapTot;
      scale = 1.0 + _stretch / 10.0;

      if(leftSide){
        // left side
        tmpFloat = trunc(rawValue / (stdDiv / scale
                      * ((scale - 1.0 / scale) * rawValue / valTot + 1.0 / scale)));
      }
      else{
        // right side
        tmpFloat = (_numMapping - 1) - trunc((_maxAnalogVal - rawValue) / (stdDiv / scale
                      * ((scale - 1.0 / scale) * (_maxAnalogVal - rawValue) / valTot + 1.0 / scale)));
      }

      mapValue = uint8_t(tmpFloat);

      // potential correction of calculation errors
      if(mapValue > _numMapping){
        mapValue = 0;
      }
      else if(mapValue == _numMapping){
        mapValue = _numMapping - 1;
      }

      return mapValue;
    }


  public:

    /*
      Create a new MappedPoti object to handle the input of an analog input pin
      and map the analog values to a defined rang of mapping values.

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
      @param  numMapping        Number of mapping values. Range is from 2 to 100.
                                Default is 2, so mapping is always used.
      @param  stretch           Factor for stretching analog values during the
                                mapping calculation. Values from 0 (no use, linear)
                                to 20. Value 20 ist highest stretching.
    */
    MappedPoti(uint8_t inputPin, uint8_t readCycleMillis, uint8_t weightPrev, uint8_t addNumRawAvg,
      uint8_t numMapping, uint8_t stretch) :
      StablePoti(inputPin, readCycleMillis, weightPrev, addNumRawAvg){

      _curMapValue = POTI_MAPPING_UNDEFINED;
      _prevMapValue = POTI_MAPPING_UNDEFINED;
      _numMapping = numMapping;
      _stretch = stretch;
      _maxAnalogVal = 1023;

      if(_numMapping > 100){
        _numMapping = 100;
      }

      if(_numMapping < 2){
        _numMapping = 2;
      }

      // factor for mapping adjustment, values from 0 to 20
      if(_stretch > 20){
        _stretch = 20;
      }
    }

    /*
      Returns the number of defined mapping values.
      This can differ to the originally given instantiation
      parameter "numMapping" due to necessary corrections.

      @returns  internally set and possibly corrected number
                of mapping values
    */
    uint8_t getNumMappingValues(){
      return _numMapping;
    }


    /*
      Returns the maximum analog value with which the internal
      mapping calcuation is done. Can be the default value (1023)
      or the overwritten value from function setMaxAnalogValue().
      The value is always an uneven number.

      @returns  internally set maximum analog value
    */
    int getMaxAnalogValue(){
      return _maxAnalogVal;
    }


    /*
      Sets and returns the maximum analog value with which the
      internal mapping calculation shall be done. Will overwrite
      the default value (1023). The value must be always an uneven
      number and will, in case of an even one, decreased by 1.

      If the maximum number of analog values is not the default 1023,
      then this function must be called before first use of function
      hasChanged() to set the real maximum number (e.g. 4095).

      @param    maxAnalogVal  the maximum analog value for the internal
                              calculations. Must be an uneven number.
      @returns                internally set maximum analog value
    */
    int setMaxAnalogValue(int maxAnalogVal){
      if((maxAnalogVal & 0x0001) == 0){
        _maxAnalogVal = maxAnalogVal - 1;
      }
      else{
        _maxAnalogVal = maxAnalogVal;
      }
      return _maxAnalogVal;
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
      int rawValue = StablePoti::getStabilizedRawValue();
      uint8_t mapValue;

      if(rawValue == POTI_VALUE_UNDEFINED){
        return false;
      }

      // no change by current measurement?
      if(rawValue == internalPrevVal){
        return false;
      }

      mapValue = getMapping(rawValue, 0, 0);
      
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
      Returns current mapping value suitable to the analog value
      given by getValue(). The value was calculated and set by the
      last call of hasChanged() that returned true.

      @returns  current mapping value in range 0 to numMapping-1
                or POTI_MAPPING_UNDEFINED before first call of hasChanged()
    */
    uint8_t getMappedValue(){
      return _curMapValue;
    }


    /*
      Returns previous mapping value. The value was calculated and
      set by the last call of hasChanged() that returned true.
      This previous mapping value was the current mapping value before
      hasChanged() was called.

      @returns  previous mapping value from 0 to numMapping-1
                or POTI_MAPPING_UNDEFINED before hasChanged() has
                returned true two times
    */
    uint8_t getMappedPrevValue(){
      return _prevMapValue;
    }
    

    /*
      Reset all internal values, so that the behavior is like directly after the
      instantiation and before first call of hasChanged().
    */
    void reset(){
      StablePoti::reset();
      _curMapValue = POTI_MAPPING_UNDEFINED;
      _prevMapValue = POTI_MAPPING_UNDEFINED;
    }
};

#endif
