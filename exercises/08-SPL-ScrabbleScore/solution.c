#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"

/*
 * Gets the score for a single letter.
 * Parameter: letter - the letter to get the score for
 * Returns: the score of the given letter
 */
int getScoreForLetter (char letter) {
        // The group score equals the index
        char *lettergroups[] = {
                "",           // Score 0
                "AEILNORSTU", // Score 1
                "DG",         // Score 2
                "BCMP",       // Score 3
                "FHVWY",      // Score 4
                "K",          // Score 5
                "",           // Score 6
                "",           // Score 7
                "JX",         // Score 8
                "",           // Score 9
                "QZ"          // Score 10
        };

        // Iterate over all groups
        for (int group = 0; group <= 10; group++) {
                char *letters = lettergroups[group];
                // Check, if the given letter is in the current group
                for (int j = 0; j < strlen(letters); j++) {
                        if (letters[j] == letter)
                                return group; // Return the index,
                                              // which equals the score
                }
        }

        return 0; // Letter not found
}

/*
 * Get the scrabble score for a whole word.
 * Parameter: word - A pointer to the word to get the score for
 * Returns: The score of the word
 */
int getScrabbleScore (char *word) {
        int score = 0;

        // Iterate, until we hit the end of the word
        // delimited by a zero byte
        while (*word) {
                // Add the score for the current letter
                score += getScoreForLetter(*word);
                // Advance to the next letter
                word++;
        }

        return score;
}

/*
 * Asks the user to enter a word
 * Returns: A pointer to the word the user entered
 */
char *getWord (void) {
        printf("Enter word: ");
        char *word = readLine(stdin);

        return word;
}

/*
 * Main entry point.
 * Read a word, calculate the score and print i.
 */
int main (void) {
        char *word = getWord();

        int score = getScrabbleScore(word);

        printf("Scrabble score for %s: %d\n", word, score);

	return 0;
}
