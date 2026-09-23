#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cslib.h"

#define NRSIZE 20

void addSeparatorToNumericString (char number[], char formattedNumber[],
                                  char separator) {
  int len = strlen(number);

  // Determine the length of the first segment. It can have 1, 2 or 3 elements
  int c = len%3;
  if (c == 0)
    c = 3;
  
  int j = 0, k = 0; // Indexes into the string arrays
  for (int i = 0; i < c; i++) {
    formattedNumber[j++] = number[k++];
  }
  // The first segment is copied, subtract the number of characters 
  // from the total length
  len -= c;
  // len is now guaranteed to be a multiple of 3 (can also be 0)
  while (len > 0) {
    formattedNumber[j++] = separator;
    for (int i = 0; i < 3; i++) {
      formattedNumber[j++] = number[k++];
    }
    len -= 3;
  }
  // Don't forget the zero byte
  formattedNumber[j] = '\0';
}

int main (void) {
  char number[NRSIZE+1];
  char formattedNumber[NRSIZE+NRSIZE/3+3];

  printf("Zahl eingeben: ");
  fgets(number, sizeof(number), stdin);
  number[strlen(number)-1] = '\0'; // Remove newline

  addSeparatorToNumericString (number, formattedNumber, '.');

  printf("Formatierte Zahl: %s\n", formattedNumber);

  return 0;
}
