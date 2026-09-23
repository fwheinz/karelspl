#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "gmath.h"
#include "simpio.h"

int lucky13 (int a[], int size) {
  for (int i = 0; i < size; i++) {
    if (a[i] == 1 || a[i] == 3)
      return 0;
  }
  return 1;
}
