#include "karel.h"

/*
 * Loads Karels world with two pyramids on startup.
 */
void setup (void) {
    loadWorld("ClimbTwoPyramidsKarel");
}

/*
 * Karel turns right. Can be called anytime.
 */
void turnRight (void) {
    for (int i = 0; i < 3; i++) {
        turnLeft();
    }
}

/*
 * Karel climbs up the pyramid and collects all beepers.
 * pre-condition: Karel stands in front of a pyramid facing east.
 * post-condition: Karel stands on the top of a pyramid facing east.
 */
void climbUp() {
    while (frontIsBlocked()) {
        turnLeft();
        move();
        turnRight();
        move();
        if (beepersPresent()) {
            pickBeeper();
        }
    }
}

/*
 * Karel climbs down the pyramid and collects all beepers.
 * pre-condition: Karel stands on the top of a pyramid facing east.
 * post-condition: Karel stands one step behind the pyramid facing east.
 */
void climbDown() {
    move();
    turnRight();
    while (frontIsClear()) {
        move();
        if (beepersPresent()) {
            pickBeeper();
        }
        turnLeft();
        move();
        turnRight();
    }
    turnLeft();
}

/*
 * Karel climbs over a single pyramid and collects all beepers.
 * pre-condition: Karel stands on the ground in front of a pyramid, facing east.
 * post-condition: Karel stands one step behind the pyramid facing east.
 */
void climbPyramid() {
    climbUp();
    climbDown();
}

/*
 * Karel looks for the next pyramid in his world.
 * pre-condition: Karel stands on the ground, facing east.
 * post-condition: Karel stands in front of a wall, presumably the base of a pyramid.
 */
void findNextPyramid (void) {
    while (frontIsClear()) {
        move();
    }
}

/*
 * Karel climbs the two pyramids in his world.
 * pre-condition: Karel stands in his initial position (1,1) on the ground, facing east.
 * post-condition: Karel stands in his final position on the ground in front of the right wall.
 *                 All diamonds were collected from the two pyramids.
 */

void climbTwoPyramids (void) {
    for (int i = 0; i < 2; i++) {
        findNextPyramid();
        climbPyramid();
    }
    findNextPyramid();
}

/*
 * This function is called when "go" is clicked.
 */
void run (void) {
    climbTwoPyramids();
}
