#include <stdio.h>
#include <string.h>

#include "cslib.h"

int cube (int n) {
  return n * n * n;
}

int main(void) {
  int number = 5;
  
  printf("The number: %d\n", number);
  printf("The result: %d\n", cube(number));

  return 0;
}
