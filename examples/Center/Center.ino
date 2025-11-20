/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <CenteredPoti.h>

/*
  Example to show the usage of a potentiometer
  with defined mapping and center position.

  The variable value of the analog pin will change the speed
  of blinking of the build in LED. 

  The example reduces the effect of instability
  of measured values over the time, when the
  potentiometer is not touched by using a
  mapping. In the example are the other available
  stabilisation methods not configured, means in use. 

  Prerequisite is an potentiometer connected
  with variable voltage pin to analog input
  pin. Output will be written to Serial.
  LED is assumed to be always available at
  pin LED_BUILTIN.
*/

#define INPUT_PIN A7                  // must be analog pin A0 to A7
#define NUM_MAP_VALUES 11             // max 101, raw values will be mapped to this uneven number of mapping values
#define READ_CYCLE_MILLIS 100         // minimum difference between two actual read of analog raw value
#define MAX_BLINK_CYCLE_MILLIS 1000   // min allowed is 1000
#define CENTER_VAL 512                // assumed middle position / center of the potentiometer
#define CENTER_TOL 50                 // tolerance for middle position / center 

CenteredPoti pot = CenteredPoti(INPUT_PIN, READ_CYCLE_MILLIS, 0, 0, NUM_MAP_VALUES, 0, CENTER_TOL, CENTER_VAL);

// Status on or off independently of blinking
boolean lightOn = false;
// time, when the LED was toggeled
unsigned long lastLEDChange = 0;
// duration of LED on/off phases
unsigned long cycleDurationMillis = MAX_BLINK_CYCLE_MILLIS;


// the setup function is called once for initialization
void setup() {
  // for showing relevant information
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
}


// for showing relevant information
void printValues(){
  Serial.print("curCenterVal=");
  if(pot.getCenteredValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot.getCenteredValue());
  }

  Serial.print(", curCenterMapVal=");
  if(pot.getCenteredMappedValue() == POTI_MAPPING_UNDEFINED){
    Serial.print("POTI_MAPPING_UNDEFINED");
  }
  else{
    Serial.print(pot.getCenteredMappedValue());
  }

  Serial.print(", prevCenterVal=");
  if(pot.getCenteredPrevValue() == POTI_VALUE_UNDEFINED){
    Serial.print("POTI_VALUE_UNDEFINED");
  }
  else{
    Serial.print(pot.getCenteredPrevValue());
  }

  Serial.print(", prevCenterMapVal=");
  if(pot.getCenteredMappedPrevValue() == POTI_MAPPING_UNDEFINED){
    Serial.print("POTI_MAPPING_UNDEFINED");
  }
  else{
    Serial.print(pot.getCenteredMappedPrevValue());
  }

  Serial.print("\n");
}


// switch the LED on (1) or off (0)
void setLED(byte ledOn) {
  digitalWrite(LED_BUILTIN, ledOn);
}


// the loop function runs over and over again forever
void loop() {
  // toggle the LED for blinking
  if(millis() - lastLEDChange > cycleDurationMillis){
    if(lightOn){
      setLED(0);
    }
    else{
      setLED(1);
    }
    lightOn = !lightOn;
    lastLEDChange = millis();
  }

  // react on changing states by calculating the cycle duration new
  if(pot.hasChanged()){
    cycleDurationMillis = MAX_BLINK_CYCLE_MILLIS / (NUM_MAP_VALUES / 2 + 1 + pot.getCenteredMappedValue());
    printValues();
  }
}

