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
    loadWorld("DamagedPillar1");
//    loadWorld("DamagedPillar2");
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

/* Karel climbs up a pillar.
	 pre-condition: Karel stands at the base of a pillar facing east
	 post-condition: Karel stands at the top of a fixed pillar facing north
*/
void climbPillar (void) {
	turnLeft();
	while (frontIsClear()) {
		if (noBeepersPresent()) {
			putBeeper();
		}
		move();
	}
	if (noBeepersPresent()) {
		putBeeper();
	}
}

/* Karel climbs down a pillar.
	 pre-condition: Karel stands at the top of a pillar facing south
	 post-condition: Karel stands at the base of a fixed pillar facing east
*/
void climbDown (void) {
	while (frontIsClear()) {
		move();
	}
	turnLeft();
}


/* Karel fixes the pillar he stands by.
	 pre-condition: Karel stands at the base of a pillar facing east
	 post-condition: Karel stands at the bottom of a fixed pillar facing east
*/
void fixOnePillar (void) {
	climbPillar();
	turnAround();
	climbDown();
}

/* Karel fixes all pillars in his world.
	 pre-condition: Karel stands in his initial position (1,1) facing east
   post-condition: Karel stands in the lower right corner facing east, all pillars are repaired
*/
void fixAllPillars (void) {
	fixOnePillar();
	while (frontIsClear()) {
		move();
		move();
		move();
		move();
		fixOnePillar();
	}
}


void run (void) {
	fixAllPillars();
}
