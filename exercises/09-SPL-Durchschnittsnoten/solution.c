#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

/* Calculate the average grade from an array.
 * Parameters:
 *   grades   - Array of grades
 *   nrgrades - Number of elements in the array.
 * Returns: Average grade
 */
double calculateAverage(int *grades, int nrgrades) {
  double sum = 0;

  for (int i = 0; i < nrgrades; i++) {
    sum += grades[i];
  }

  return sum/nrgrades;
}

int main(void) {
  int nrgrades = 0;
  int *grades = NULL;

  int nr;
  do {
    printf("Please enter number of grades: ");
    scanf("%d", &nr);
    if (nr > 0) {
      // Enlarge the memory for the additional grades
      int *tmp = realloc(grades, (nrgrades+nr)*sizeof(int));
      if (tmp == NULL) {
        printf("Out of memory!\n");
        break;
      }
      // Reallocation was successful
      grades = tmp;
      // Enter the additional grades
      for (int i = 0; i < nr; i++) {
        printf("Enter grade: ");
        scanf("%d", &grades[nrgrades+i]);
      }
      // Remember the additional space
      nrgrades += nr;
      double average = calculateAverage (grades, nrgrades);
      printf("Average grade: %f\n", average);
    }
  } while (nr != 0);
  printf("Bye!\n");
  
  return 0;
}
