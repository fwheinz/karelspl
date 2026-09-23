/*
 * File: Histogram.c
 * --------------
 * This program reads 5 integer numbers and prints
 * a corresponding histogram.
 */

#include <stdio.h>  /* for printf */
#include "simpio.h" /* for getInteger */

int n1, n2, n3, n4, n5;

/*
 * Prints a single histogram line.
 * pre-condition: none
 * post-condition: One histogram was printed
 * parameter: nrStars - number of stars to print in this histogram
 */
void printOneHistogram (int nrStars) {
    for (int i = 0; i < nrStars; i++) {
        printf("*");
    }
    printf("\n");
}

/*
 * Output the histogram defined from the numbers in the five global variables n1, n2, n3, n4, n5
 * pre-condition: n1-n5 were assigned from user input
 * post-condition: Histogram was printed
 */
void printHistogram (void) {
    printf("Printing histogram:\n");
    printOneHistogram(n1);
    printOneHistogram(n2);
    printOneHistogram(n3);
    printOneHistogram(n4);
    printOneHistogram(n5);
}

/*
 * Requests five numbers from the user and stores them in global variables.
 * pre-condition: none
 * post-condition: Global Variables n1, n2, n3, n4, n5 are filled with the numbers from the
 *                 user input.
 */
void enterNumbers (void) {
    printf("Please enter five numbers (press Enter for next number):\n");
    n1 = getInteger();
    n2 = getInteger();
    n3 = getInteger();
    n4 = getInteger();
    n5 = getInteger();
}

/*
 * Fetch five numbers from the user and print a corresponding histogram.
 */
int main (void) {
    enterNumbers();
    printHistogram();

    return 0;
}
