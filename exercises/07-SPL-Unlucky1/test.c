#include <stdio.h>

#include "cslib.h"

int unlucky1 (int*, int);

void test (int *arr, int sz, int sol) {
  printf("unlucky1({ ");
  for (int i = 0; i < sz; i++) {
    printf("%d%s", arr[i], i == sz-1 ? "" : ", ");
  }
  int res = unlucky1(arr, sz);
  printf(" }, %d) -> %d (%s)\n", sz, res, sol == res ? "passed" : "failed"); 
}

int main (void) {
  int a1[] = {1, 3, 4, 5};
  int a2[] = {2, 1, 3, 4, 5};
  int a3[] = {1, 1, 1};
  int a4[] = {1, 1, 1, 4, 5, 1, 3};

  test(a1, sizeof(a1)/sizeof(*a1), 1);
  test(a2, sizeof(a2)/sizeof(*a2), 1);
  test(a3, sizeof(a3)/sizeof(*a3), 0);
  test(a4, sizeof(a4)/sizeof(*a4), 1);

  return 0;
}
