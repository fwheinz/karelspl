#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "simpio.h"
#include "random.h"

/*
 * Checks if the password:
 * - has at least 8 characters
 * - contains at least 2 letters
 * - contains at least 2 digits
 * - contains at least 1 uppercase letter
 * - contains at least 1 lowercase letter
 * - contains at least 1 special char (everything else)
 * Returns: 1 if condition is fulfilled, 0 else
 */
int isStrongPassword (char word[]) {
	int len = strlen(word);
	int letters = 0, digits = 0, ucletters = 0, lcletters = 0, special = 0;

	while (*word) {
		char ch = *word;
		if (ch >= 'a' && ch <= 'z') {
			letters++;
			lcletters++;
		} else if (ch >= 'A' && ch <= 'Z') {
			letters++;
			ucletters++;
		} else if (ch >= '0' && ch <= '9') {
			digits++;
		} else {
			special++;
		}
		word++;
	}

	int safe =
		(len >= 8) &&
		(letters >= 2) &&
		(digits >= 2) &&
		(ucletters >= 1) &&
		(lcletters >= 1) &&
		(special >= 1);

	return safe;
}

/*
 * Reads a password from the user to check.
 */
char *readPassword (void) {
	printf("Bitte Passwort eingeben: ");
	char *pw = readLine(stdin);

	return pw;
}

/*
 * Main program entry.
 * Reads a password from the user, checks it and prints
 * the verdict.
 */
int main (void) {
	printf("Passwortchecker.\n");
	char *password = readPassword();
	int safe = isStrongPassword(password);
	if (safe) {
		printf("OK - Passwort sicher.\n");
	} else {
		printf("Achtung - Passwort unsicher.\n");
	}

	return 0;
}
