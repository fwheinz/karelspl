#include <stdio.h>
#include <string.h>
#include "simpio.h"


int main(void) {
  int steps = 0, n;
  
  printf("Bitte Zahl eingeben: ");
  n = getInteger();
  
  while (n != 1) {
    if (n % 2 == 1) {
      printf("%d ist ungerade, also nehme man 3n+1: ", n);
      n = 3*n+1;
    } else {
      printf("%d ist gerade, also halbiere man: ", n);
      n = n / 2;
    }
    printf("%d.\n", n);
    steps++;
  }
  printf("Ich habe %d Schritte benötigt,\n", steps);

  return 0;
}
