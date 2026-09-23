#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"


int squirrelParty (int numCigars, int isWeekend) {
  if (numCigars >= 40 && (isWeekend || numCigars <= 60))
    return 1;
  else
    return 0;
}
