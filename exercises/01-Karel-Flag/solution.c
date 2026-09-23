/*
 * File: HelloKarel.c
 * --------------------------------
 * The HelloKarel program is a simple
 * program to demonstrate a minimal working
 * Karel example.
 * 
 */

#include "karel.h"

void setup (void) {
//    loadWorld("flagDistance0");
    loadWorld("flagDistance1");
//    loadWorld("flagDistance2");
}

void turnAround (void) {
	turnLeft();
	turnLeft();
}

void turnRight (void) {
	turnLeft();
	turnLeft();
	turnLeft();
}

/* Karel brings the rightmost beeper home.
	 pre-condition: Karel stands in (2,1) facing east
   post-condition: Karel brought the rightmost beeper home and stands in (2,1) facing east
*/
void bringBeeperHome (void) {
	while (beepersPresent()) {
		move();
	}
	turnAround();
	move();
	pickBeeper();
	while (frontIsClear()) {
		move();
	}
	putBeeper();
	turnAround();
	move();
}

/* Karel walks to his initial position.
	 pre-condition: Karel stands anywhere between his initial position and the flag facing east
	 post-condition: Karel stands in his initial position (1,1) facing east
*/
void goHome (void) {
	turnAround();
	while (frontIsClear()) {
		move();
	}
	turnAround();
}

/* Karel walks to the flag and drops beepers
	 pre-condition: Karel stands in his initial position (1,1) facing east
	 post-condition: Karel stands in front of the flag leaving a trail of beepers1 facing east
*/
void goToFlag (void) {
	while (frontIsClear()) {
		putBeeper();
		move();
	}
}

/* Karel measures the distance between his initial position and the flag
	 pre-condition: Karel stands in his initial position (1,1) facing east
	 post-condition: Karel stands at (1,2) facing north above a pile of beepers representing the distance
*/
void run (void) {
	goToFlag();
	goHome();
	if (frontIsClear()) {
		move();
		while (beepersPresent()) {
			bringBeeperHome();
		}
		turnAround();
		move();
	} else {
		turnAround();
	}
	turnRight();
	move();
}
