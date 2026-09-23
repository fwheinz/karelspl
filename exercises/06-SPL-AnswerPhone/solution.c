#include <stdio.h>
#include <string.h>
#include "cslib.h"
#include "simpio.h"


/*
 Assumes all inputs are either 0 ( false ) or 1 ( true ) .
 Returns 1 if we answer the phone .
*/
int answerPhone (int isMorning, int isMom, int isSleeping) {
  if (!isSleeping && (!isMorning || isMom))
    return 1;
  else
    return 0;
}
