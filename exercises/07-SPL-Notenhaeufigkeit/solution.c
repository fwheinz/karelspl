#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

/* Read the file examscores.txt and fill the array arr.
 * Read at most 'size' elements and return the number of
 * elements read.
 * Parameters:  arr - The array to fill
 *             size - The array size
 * Returns: Number of elements read
*/
int readScores (int arr[], int size) {
  FILE *infile = fopen("data/examscores.txt", "r");
  if (!infile) {
    printf("Error reading scores!\n");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (i < size) {
    char *line = readLine (infile) ;
    if (line == NULL) {
      break;
    }
    int num = atoi (line);
    arr[i] = num;
    i++;
  }
  fclose(infile);

  return i;
}

/* Fills the histogram array with the values from the value array
 * Parameters:  arr - the values array
 *             size - the number of elements in the values array
 *        histogram - the histogram array to fill
 */
void fillHistogram (int arr[], int size, int histogram[]) {
  for (int i = 0; i < size; i++) {
    int index = arr[i]/10;
    histogram[index]++;
  }
}

/* Print the histogram array with stars
 * Parameter:   histogram - the histogram array
 */
void printHistogram (int histogram[]) {
  for (int i = 0; i < 11; i++) {
    if (i < 10) {
      printf("%02d-%02d: ", i*10, (i+1)*10-1);
    } else {
      printf("  100: ");
    }
    for (int j = 0; j < histogram[i]; j++)
      printf("*");
    printf("\n");
  }
}

int main(void) {
  int scores[200], histogram[11] = {0};

  int nrscores = readScores(scores, 200);
  fillHistogram(scores, nrscores, histogram);
  printHistogram(histogram);

  return 0;
}
