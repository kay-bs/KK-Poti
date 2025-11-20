/*
  Copyright (c) 2025 Kay Kasper
  under the MIT License (MIT)
*/

#include <CenteredPoti.h>
#include "Common.h"
#include "PotiTests.h"
#include "StablePotiTests.h"
#include "MappedPotiTests.h"
#include "CenteredPotiTests.h"

/*
  Example that tests the functionality
  of Poti, StablePoti, MappedPoti and
  CenteredPoti classes. Several checks and
  performance measurements are done
  continously in the loop.

  Prerequisite is the Serial class for
  writing the output.
*/

// the setup function is called once for initialization
void setup() {
  // for showing relevant information
  Serial.begin(9600);
  Serial.println("Test started");
}


// the loop function runs over and over again forever
void loop() {
  doPotiTest(ID_POTITEST);
  doStablePotiTest(ID_STABLETEST);
  doMappedPotiTest(ID_MAPPEDTEST);
  doCenteredPotiTest(ID_CENTEREDTEST);
  delay(3000);
}
