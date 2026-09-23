#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

/*
 * Prints the given number of stars
 */
void printStars(int len) {
	while (len--) {
		putchar('*');
	}
}

/*
 * Prints the histogram results with bars of stars.
 * Parameter: histogram - the histogram array
 */
void printHistogram (int histogram[]) {
	printf("Histogram for your sentence:\n");
	for (int i = 0; i < 26; i++) {
		if (histogram[i] > 0) {
			printf("%c:", 'a' + i);
			printStars(histogram[i]);
			printf("\n");
		}
	}
}

/*
 * Prints the raw results with numbers.
 * Parameter: histogram - the histogram array
 */
void printResults (int histogram[]) {
	printf("Raw results for your sentence:\n");
	for (int i = 0; i < 26; i++) {
		if (histogram[i] > 0) {
			printf("%c:%d\n", 'a' + i, histogram[i]);
		}
	}
}

/*
 * Reads in a sentence and counts the lowercase letters in it
 * Parameter: histogram - the histogram array to fill
 */
void readAndCount (int histogram[]) {
	char buf[1000] = "";
	printf("Please enter sentence: ");
	fgets(buf, sizeof(buf), stdin);

	for (int i = 0; i < strlen(buf); i++) {
		char ch = buf[i];
		if (ch >= 'a' && ch <= 'z') {
			int idx = ch - 'a';
			histogram[idx]++;
		}
	}
}

/*
 * Main entry point.
 * Defines the histogram array, reads and counts the letters,
 * prints the results as well as the histogram.
 */
int main (void) {
	int histogram[26] = { 0 };

	readAndCount(histogram);
	printResults(histogram);
	printHistogram(histogram);

	return 0;
}
