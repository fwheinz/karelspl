/*
 * File: LivingRoomKarel.c
 * -----------------------------------------
 *
 * In this program, Karel simply climbs a cupboard and picks
 * a beeper on its way.
 *
 */

#include "karel.h"

void setup (void) {
    loadWorld("LivingRoom");
}

void turnRight (void) {
    turnLeft();
    turnLeft();
    turnLeft();
}

/*
 * pre-cond: Karel stands at top of the table facing east
 * post-cond: Karel stands at beeper location and has picked up the beeper
 */
void goToBeeperAndPick (void) {
    while (noBeepersPresent()) {
        move();
    }
    pickBeeper();
}

/* Karel moves until he stands in front of a wall
 * pre-condition: none
 * post-condition: Karel stands in front of a wall
 */
void moveWhileFrontIsClear (void) {
    while (frontIsClear()) {
        move();
    }
}

/* Karel moves to the wall and puts a beeper
 * pre-condition: Karel stands on the table facing east
 * post-condition: Karel stands before the wall on a beeper
 */
void moveToWallAndPutBeeper(void) {
    moveWhileFrontIsClear();
    putBeeper();
}

/* Karel climbs the table
 * pre-condition: Karel stands in front of the table facing north
 * post-condition: Karel stands at top of the table facing east
 */
void climbTable (void) {
    while (rightIsBlocked()) {
        move();
    }
    turnRight();
}

/* Moves to the table
 * pre-cond: Karel in initial position (1,1) facing east
 * post-cond: Karel stands in front of the table facing east
 */
void moveToTable (void) {
    moveWhileFrontIsClear();
}

void run (void) {
    moveToTable();
    turnLeft();
    climbTable();
    goToBeeperAndPick();
    moveToWallAndPutBeeper();
}
