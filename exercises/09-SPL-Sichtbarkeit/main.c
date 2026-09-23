#include <stdlib.h>
#include <stdio.h>
#include "cslib.h"

int x = 1234;
double y = 1.234567;

void functionOne() {
   printf("From functionOne:\n  x=%d, y=%f\n", x, y);
}

int main() {
	 printf("Bitte erst das erwartete Ergebnis aufschreiben, danach diese Zeile auskommentieren!\n"); exit(EXIT_FAILURE);

   int x = 4321;

   functionOne();
   printf("Within the main block:\n  x=%d, y=%f\n", x, y);
   /* a nested block */
   {
      double y = 7.654321;
      functionOne();
      printf("Within the nested block:\n  x=%d, y=%f\n", x, y);
   }
   return 0;
}
