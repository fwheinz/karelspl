#include <stdio.h>
#include "cslib.h"

int sumNoTeens (int, int, int);

int main (void) {
  if (
       sumNoTeens( 5, 30, 20) == 55  &&
       sumNoTeens(15, 16, 17) == 31  &&
       sumNoTeens(13, 14, 15) == 15  &&
       sumNoTeens(29, 19, 11) == 40  &&
       sumNoTeens(15, 11, 19) == 26  &&
       sumNoTeens(13, 16, 20) == 36
  )
    printf("Tests passed!\n");
  else
    printf("Tests failed!\n");

  return 0;
}