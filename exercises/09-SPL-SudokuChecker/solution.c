/*
*
* main / SudokuStarter.c
*
* This program checks whether a sudoko square contains
* numbers in the right range (i.e. 1 to 9) and whether
* each number occurs only once in a square.
*
*/

#include <stdio.h>

#include "cslib.h"

int checkSudokuSquare(int sudokuSquare[][3]) {
	int numbers[9] = { 0 };

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (sudokuSquare[i][j] <= 0 || sudokuSquare[i][j] >= 10)
				return 0;
			if (++numbers[sudokuSquare[i][j]] > 1)
				return 0;
		}
	}

	return 1;
}

int main() {
  // Test square - Run your solution against all three two dimensional arrays
  int sudokuSquareOne[3][3] = { {1, 2, 3},
            {7, 8, 9},
            {4,5,6}};
  int sudokuSquareTwo[3][3] = { {1, 1, 3},
            {8, 8, 9},
            {4,5,6}};
  int sudokuSquareThree[3][3] = { {10, 2, 3},
            {7, -1, 9},
            {4,5,6}};

  int result;
  result = checkSudokuSquare(sudokuSquareOne);
  printf("Der 1. Sudokulösungsversuch ist (0 = falsch, 1 = richtig): %d (korrekt: 1)\n", result);
  result = checkSudokuSquare(sudokuSquareTwo);
  printf("Der 2. Sudokulösungsversuch ist (0 = falsch, 1 = richtig): %d (korrekt: 0)\n", result);
  result = checkSudokuSquare(sudokuSquareThree);
  printf("Der 3. Sudokulösungsversuch ist (0 = falsch, 1 = richtig): %d (korrekt: 0)\n", result);
  return 0;
}
