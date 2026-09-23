#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

void swapEnds (int a[], int size) {
  int t = a[size-1];
  a[size-1] = a[0];
  a[0] = t;
}
