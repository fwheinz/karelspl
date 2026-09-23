#include <stdio.h>
#include "cslib.h"

int squirrelParty (int, int);

int main (void) {
  if (
    squirrelParty(10, 0) == 0 &&
    squirrelParty(39, 0) == 0 &&
    squirrelParty(10, 0) == 0 &&
    squirrelParty(39, 0) == 0 &&
    squirrelParty(40, 0) == 1 &&
    squirrelParty(51, 0) == 1 &&
    squirrelParty(60, 0) == 1 &&
    squirrelParty(61, 0) == 0 &&
    squirrelParty(97, 0) == 0 &&
    squirrelParty(10, 1) == 0 &&
    squirrelParty(39, 1) == 0 &&
    squirrelParty(10, 1) == 0 &&
    squirrelParty(39, 1) == 0 &&
    squirrelParty(40, 1) == 1 &&
    squirrelParty(51, 1) == 1 &&
    squirrelParty(60, 1) == 1 &&
    squirrelParty(61, 1) == 1 &&
    squirrelParty(97, 1) == 1
  )
    printf("Tests passed!\n");
  else
    printf("Tests failed!\n");

  return 0;
}