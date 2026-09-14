#ifndef _KAREL_H
#define _KAREL_H

/* Interface */
void setup (void);
void loadWorld (char *world);
void run (void);

/* Actions */
void moveKarel(void);
void turnLeft(void);
void pickBeeper(void);
void putBeeper(void);

/* Sensors */
int frontIsClear();
int rightIsClear();
int leftIsClear();
int frontIsBlocked();
int rightIsBlocked();
int leftIsBlocked();
int beepersPresent();
int noBeepersPresent();
int beepersInBag();
int noBeepersInBag();
int facingNorth();
int facingEast();
int facingSouth();
int facingWest();
int notFacingNorth();
int notFacingEast();
int notFacingSouth();
int notFacingWest();

#define move() moveKarel()


#endif /* _KAREL_H */
