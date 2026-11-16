#include "karel.h"

void setup (void) {
    loadWorld("LivingRoom");
}

void turnRight (void) {
    for (int i = 0; i < 3; i++) {
        turnLeft();
    }
}

void goToWall (void) {
    while (frontIsClear()) {
        move();
    }
}

void run (void) {
    goToWall();
    turnLeft();
    while (rightIsBlocked()) {
        move();
        turnLeft();
    }
    turnRight();
    while (frontIsClear()) {
        if (beepersPresent()) {
            pickBeeper();
        } else {
            putBeeper();
        }
        move();
    }
    putBeeper();
}

