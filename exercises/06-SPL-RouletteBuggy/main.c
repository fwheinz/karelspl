/*
 * File: Roulette.c
 * -------------------
 * This program simulates a small part of the casino game of
 * roulette.
 */

#include <stdio.h>
#include "random.h"

#define STARTING_MONEY 100
#define WAGER_AMOUNT 10

/**
* Welcomes the player to the game and gives instructions on the rules of
* roulette.
*/
void giveInstructions() {
	printf("Welcome to the roulette table!\n");
	printf("Roulette is played with a large wheel divided into\n");
	printf("compartments numbered from 0 to 36. Each player\n");
	printf("places bets on a playing field marked with the\n");
	printf("numbers and various categories. In this game,\n");
	printf("the only legal bets are the following categories:\n");
	printf("odd (o), even (e), low (l), or high (h). Note that 0 is not in any\n");
	printf("category. After the bet is placed, the wheel is\n");
	printf("spun, and a marble is dropped inside, which bounces\n");
	printf("around until it lands in a compartment. If the\n");
	printf("compartment matches the betting category you chose,\n");
	printf("you win back your wager plus an equal amount. If\n");
	printf("not, you lose your wager.\n");
}

/**
* Simulates the spinning of the roulette wheel. The method returns the
* number of the slot into which the ball fell.
*/
int spinRouletteWheel() {
	printf("The ball lands in %d.\n", randomInteger(0, 36));
	return randomInteger(0, 36);
}


/**
* Returns true if the outcome matches the category specified by bet. If the
* player chooses an illegal betting category, this function always returns
* false.
*/
int isWinningCategory(int outcome, char bet) {
	if (bet = 'o') {
		return (outcome % 2 == 1);
	} else if (bet = 'e') {
		return (outcome % 2 == 0);
	} else if (bet = 'l') {
		return (1 <= outcome && outcome <= 18);
	} else if (bet = 'h') {
		return (19 <= outcome && outcome <= 36);
	} else {
		return 0;
	}
}

/**
* Plays roulette until the user runs out of money.
*/
void playRoulette() {
	int money = STARTING_MONEY;
	char bet;
	while (money > 0) {
		printf("You now have $%d.\n", money);
		printf("Enter betting category - odd (o), even (e), low (l), or high (h):\n");
		scanf(" %c", &bet);
		int outcome = spinRouletteWheel();
		if (isWinningCategory(outcome, bet)) {
			printf("That number is %c, so you win.\n", bet);
			money = money + WAGER_AMOUNT;
		} else {
			printf("That number is not %c, so you lose.\n", bet);
			money = money - WAGER_AMOUNT;
		}
		printf("--------------------------------------\n");
	}
	printf("You ran out of money.");
}

int main() {
	giveInstructions();
	playRoulette();
	return 0;
}


