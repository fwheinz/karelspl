#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

int unlucky1 (int a[], int size) {
  if ((size >= 2 && a[0] == 1 && a[1] == 3) ||
      (size >= 3 && a[1] == 1 && a[2] == 3) ||
      (size >= 2 && a[size-2] == 1 && a[size-1] == 3))
    return 1;
  else
    return 0;
}
