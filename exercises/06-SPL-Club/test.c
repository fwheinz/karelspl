#include <stdio.h>
#include "cslib.h"

int checkAdmission (int, int);

int main (void) {
  if (
       checkAdmission(9, 2) == 0  &&
       checkAdmission(5, 2) == 0  &&
       checkAdmission(2, 9) == 0  &&
       checkAdmission(2, 0) == 0  &&
       checkAdmission(0, 5) == 0  &&
       checkAdmission(3, 7) == 1  &&
       checkAdmission(7, 3) == 1  &&
       checkAdmission(5, 5) == 1  &&
       checkAdmission(3, 9) == 2  &&
       checkAdmission(9, 3) == 2  &&
       checkAdmission(9, 8) == 2 
  )
    printf("Tests passed!\n");
  else
    printf("Tests failed!\n");

  return 0;
}