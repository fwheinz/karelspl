#include "karel.h"

void setup (void) {
    loadWorld("SteepleChase");
}

/* Karel climbs up a single hurdle
 * pre-cond: Karel stands in front of a hurdle facing east
 * post-cond: Karel stands at top of a hurdle facing north
 */
void climbUp (void) {
    turnLeft();
    while (rightIsBlocked()) {
        move();
    }
}

/* Karel climbs up a single hurdle
 * pre-cond: Karel stands at top of a hurdle facing north
 * post-cond: Karel stands at top behind a hurdle facing east
 */
void climbOver (void) {
    turnLeft();
    turnLeft();
    turnLeft();
    move();
}

/* Karel climbs up a single hurdle
 * pre-cond: Karel stands at top behind a hurdle facing east
 * post-cond: Karel stands at the ground behind a hurdle facing east
 */
void climbDown (void) {
    turnLeft();
    turnLeft();
    turnLeft();
    while (frontIsClear()) {
        move();
    }
    turnLeft();
}

/* Karel seeks the next hurdle
 * pre-cond: Karel stands on the ground facing east
 * post-cond: Karel stand on the ground in front of a hurdle facing east
 */
void goToHurdle (void) {
    while (frontIsClear()) {
        move();
    }
}

/* Karel climbs over a single hurdle
 * pre-cond: Karel stands in front of a hurdle facing east
 * post-cond: Karel stands behind a hurdle facing east
 */
void climbOneHurdle(void) {
    climbUp();
    climbOver();
    climbDown();
}

/* Karel climbs over all hurdles
 * pre-cond: Karel stands in (1,1) facings east
 * post-cond: Karel stands in (15,1) facing east
 */
void climbAllHurdles(void) {

}

void run (void) {
    for (int i = 0; i < 14; i++) {
        if (frontIsBlocked()) {
            climbOneHurdle();
        } else {
            move();
        }

    }
//    climbAllHurdles();
}
