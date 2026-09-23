#include <stdio.h>

#include "cslib.h"

int swapEnds (int*, int);

void test (int *arr, int sz) {
  printf("swapEnds({ ");
  for (int i = 0; i < sz; i++) {
    printf("%d%s", arr[i], i == sz-1 ? "" : ", ");
  }
  printf(" }, %d) -> { ", sz);
  swapEnds(arr, sz);
  for (int i = 0; i < sz; i++) {
    printf("%d%s", arr[i], i == sz-1 ? "" : ", ");
  }
  printf(" }\n");
}

int main (void) {
  int a1[] = {1, 2, 3, 4};
  int a2[] = {1, 2, 3};
  int a3[] = {8, 6, 7, 9, 5};
  int a4[] = {1};

  test(a1, 4);
  test(a2, 3);
  test(a3, 5);
  test(a4, 1);

  return 0;
}