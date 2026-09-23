/*
 * File: DiamondMiningKarel.c
 * -----------------------------------------
 *
 * Karel wants to mine diamonds. He searches all mines in his worlds and collects
 * the diamonds he finds there.
 *
 */

#include "karel.h"

/*
 * This beams Karel into a DiamondMining-World
 */
void setup (void) {
    loadWorld("DiamondMining1");
//    loadWorld("DiamondMining2");
}

/*
 * Karel turns 90 degrees to the right (clockwise)
 */
void turnRight (void) {
    turnLeft();
    turnLeft();
    turnLeft();
}

/*
 * Karel turns around (180 degrees)
 */
void turnAround (void) {
    turnLeft();
    turnLeft();
}

/*
 * Karel climbs up the mine shaft.
 * pre-condition: Karel stands directly below the mine exit facing west
 * post-condition: Karel stands on top of the mine shaft facing east
 */
void ascendMine (void) {
    turnRight();
    move();
    while (rightIsBlocked()) {
        move();
    }
    turnRight();
}

/*
 * Karel seeks the mine exit.
 * pre-condition: Karel stands on the right end of the mine facing east
 * post-condition: Karel stands directly below the mine exit facing west
 */
void findExit (void) {
    turnAround();
    while (rightIsBlocked()) {
        move();
    }
}

/*
 * Karel tries to collect all diamonds at the current position.
 * pre-condition: none
 * post-condition: No diamonds (beepers) are left at the current position.
 */
void tryToCollectDiamonds (void) {
    while (beepersPresent()) {
        pickBeeper();
    }
}

/*
 * Karel searches and collects all diamonds in the mine.
 * pre-condition: Karel is at the bottom of the mine, facing south
 * post-condition: Karel is at the right end of the mine, facing east
 */
void searchDiamonds (void) {
    /* Move to the left end of the mine */
    turnRight();
    while (frontIsClear()) {
        move();
    }
    turnAround();

    /* Walk to the right end collecting all diamonds */
    tryToCollectDiamonds();
    while (frontIsClear()) {
        move();
        tryToCollectDiamonds();
    }
}

/*
 * Karel dives into the mine.
 * pre-condition: Karel stands on top of the mine facing east.
 * post-condition: Karel stands at the bottom of the mine, facing south
 */
void descendMine (void) {
    turnRight();
    while (frontIsClear()) {
        move();
    }
}

/*
 * Karel mines the diamonds in a single mine.
 * pre-condition: Karel stands on top of a mine, facing east.
 * post-condition: Karel stands on top of a mine, facing east. All diamonds were mined.
 */
void mineDiamonds (void) {
    descendMine();
    searchDiamonds();
    findExit();
    ascendMine();
}

/*
 * Karel walks through his world, mining all diamonds he can find.
 * pre-condition: Karel stands in his initial position on the ground, facing east.
 * post-condition: Karel stands in his final position on the ground in front of the right wall.
 *                 All diamonds were mined.
 */
void mineAllDiamonds (void) {
    while (frontIsClear()) {
        /* The right wall is not yet reached */
        if (rightIsClear()) {
            /* We have found a mine entry, start mining diamonds */
            mineDiamonds();
        }
        move();
    }
}

/*
 * This function is called when "go" is clicked.
 */
void run (void) {
    mineAllDiamonds();
}
