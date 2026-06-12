# KK-Poti
## Introduction
Easy handling of potentiometers, attenuators and other kinds of analog input signals. Analog input signals can be returned directly or mapped to linear fixed values. Supports centered potentiometers for e.g. Bass, Treble, Balance.

A library for handling any kinds of analog inputs from potentiometers, attenuators and a lot of other devices. Several options are offered for an easy and performant handling.

The main target was to get an easy understandable code and the chance to concentrate on the real functionality of the project. All functionality for the analog inputs is encapsulated in five separate classes and only changes of the input need attention.

For an easier reading the five classes Poti, StablePoti, MappedPoti, HalfShiftMappedPoti and CenteredPoti are summerized as Poti classes when they are all meant.

Different analog inputs can be handled completely independent in parallel with individual Poti objects.

Each of the five existing classes has its specific advantages. Here are only some of them:

Advantages

    • no active waits 
    • high performance 
    • low memory usage 
    • handling current and previous values 
    • value caching enables stable value analysis 
    • easy handling in loops with little code 
    • subclasses for own raw read logic possible 
    • big number of examples
    • intensively tested by manual and automated tests
    • value mapping of values to different range (optional) 
    • reduction of raw value reads (optional) 
    • stabilization (against often small changes) of values (optional) 
    • support for potentiometers with a center position (optional) 
    • two different mapping algorithms (optional)
    • compensation of non linear value distribution of potentiometers (optional)

## Version History
<table>
  <tr>
	<th>Version</th>
	<th>Released</th>
	<th>Comment</th>
  </tr>
  <tr>
	<td>1.0.0</td>
	<td>20.11.2025</td>
	<td>First release with full functionality</td>
  </tr>
  <tr>
	<td>1.1.0</td>
	<td>31.05.2026</td>
	<td>Added new HalfShiftMappedPoti class. Correction of several comment and documentation issues.</td>
  </tr>
  <tr>
	<td>1.1.1</td>
	<td>12.06.2026</td>
	<td>Improved comments and documentation.</td>
  </tr>
</table>

## Installation
The library can be installed via Arduino Library Manager or by downloading the [archive](library/KK-Poti.zip) from directory „library“ and unpacking the archive in IDEs libraries directory.

## Getting Started
Only a few lines of code are necessary to understand the logic behind:
```
#include <Poti.h>

#define INPUT_PIN A7           // must be analog pin A0 to A7
#define READ_CYCLE_MILLIS 200  // millis between two reads of analog raw value

Poti pot = Poti(INPUT_PIN, READ_CYCLE_MILLIS);

void setup() {
  // for showing relevant information
  Serial.begin(9600);
}

void loop() {
  // react on changing values by writing the relevant information
  if(pot.hasChanged()){
    Serial.print("curVal=");
    Serial.print(pot.getValue());
    Serial.print(", prevVal=");

    if(pot.getPrevValue() == POTI_VALUE_UNDEFINED){
      Serial.print("POTI_VALUE_UNDEFINED");
    }
    else{
      Serial.print(pot.getPrevValue());
    }

    Serial.print("\n");
  }
}
```

See the complete list of examples in the examples folder and in the [documentation](doc/KK-Poti.pdf).

