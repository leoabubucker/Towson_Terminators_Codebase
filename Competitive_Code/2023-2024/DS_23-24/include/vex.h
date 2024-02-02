#ifndef VEX_H //Header File Guard
#define VEX_H //Header File Guard

//START - VEX Initializations - DO NOT EDIT
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
//END - VEX Initializations - DO NOT EDIT

#include "robot-config.h" //USER INCLUDED - MUST BE AFTER VEX INITIALIZATIONS: Global Definitions: motors, motor-groups, Motor Collection

#endif //Header File Guard