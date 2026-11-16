#include "karel.h"

void setup (void) {
    loadWorld("BigBeeperPile");
}

void turnAround (void) {
    for (int i = 0; i < 2; i++) {
        turnLeft();
    }
}

/* Karel move one beeper
 * pre-cond: Karel stands on a beeper pile with N beepers facing east
 * post-cond: Karel stands on a beeper pile with N-1 beepers facing east
 */
void moveOneBeeper (void) {
    pickBeeper();
    turnAround();
    move();
    // Dies ist ein kommentar
    putBeeper();
    turnAround();
    move();
}

/* Karel moves beeperpile one to the left
 * pre-cond: Karel stands in front of a beeper pile with N beepers facing east
 * post-cond: Karel stands on (1,1) facing east
 */
void moveAllBeepers (void) {
    move();
    while (beepersPresent()) {
        moveOneBeeper();
    }
    turnAround();
    move();
    move();
    turnAround();
}

/* Karel doubles one beeper
 * pre-cond: Karel stands on a beeper pile with N beepers facing east
 * post-cond: Karel stands on a beeper pile with N-1 beepers facing east
 */
void doubleOneBeeper (void) {
    pickBeeper();
    move();
    putBeeper();
    putBeeper();
    turnAround();
    move();
    turnAround();
}

/* Karel doubles all beepers on the beeper pile
 * pre-cond: Karel stands on (1,1) facing east in front of a beeper pile with
 *           N beepers
 * post-cond: Karel stands on (1,1) facing east in front of a beeper pile with
 *            2N beepers
 */
void doubleAllBeepers (void) {
    move();
    while (beepersPresent()) {
        doubleOneBeeper();
    }
    moveAllBeepers();
}

void run (void) {
    doubleAllBeepers();
}

