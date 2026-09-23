#include <stdio.h>
#include "cslib.h"


int lucky13 (int*, int);

void test (int *arr, int sz, int sol) {
  printf("lucky13({ ");
  for (int i = 0; i < sz; i++) {
    printf("%d%s", arr[i], i == sz-1 ? "" : ", ");
  }
  int res = lucky13(arr, sz);
  printf(" }, %d) -> %d (%s)\n", sz, res, res == sol ? "passed" : "failed");
}

int main (void) {
  int a1[] = {0, 2, 4};
  int a2[] = {1, 2, 3};
  int a3[] = {1, 2, 4};

  test(a1, 3, 1);
  test(a2, 3, 0);
  test(a3, 3, 0);

  return 0;
}