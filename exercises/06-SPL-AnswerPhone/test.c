#include <stdio.h>
#include "cslib.h"

int answerPhone (int, int, int);

int main (void) {
  if (
    answerPhone(0, 0, 0) == 1 &&
    answerPhone(0, 0, 1) == 0 &&
    answerPhone(0, 1, 0) == 1 &&
    answerPhone(0, 1, 1) == 0 &&
    answerPhone(1, 0, 0) == 0 &&
    answerPhone(1, 0, 1) == 0 &&
    answerPhone(1, 1, 0) == 1 &&
    answerPhone(1, 1, 1) == 0
  )
    printf("Tests passed!\n");
  else
    printf("Tests failed!\n");

  return 0;
}