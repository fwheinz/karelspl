#include <stdio.h>
#include "cslib.h"

int noThirteenSum (int, int, int);

int main (void) {
  if (
    noThirteenSum(13, 10, 20) == 0   &&
    noThirteenSum(17, 13, 39) == 17  &&
    noThirteenSum(24, 18, 13) == 42  &&
    noThirteenSum(13, 13, 37) == 0   &&
    noThirteenSum(-10, 15, 23) == 28 &&
    noThirteenSum(13, 55, 13) == 0
  )
    printf("Tests passed!\n");
  else
    printf("Tests failed!\n");

  return 0;
}