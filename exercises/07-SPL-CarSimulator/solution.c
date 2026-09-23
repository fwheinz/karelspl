#include <stdio.h>
#include <string.h>
// We use full paths here to enable replit code introspection
#include "cslib.h"
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gwindow.h"
#include "random.h"
#include "simpio.h"

/* Constants */
#define CAR_NUM 100
#define CAR_WIDTH 15
#define CAR_HEIGHT 5
#define WIDTH 600
#define HEIGHT 300
#define PAUSE_TIME 20
#define CAR_COLOR "CYAN"
#define MAX_SPEED 10

GRect cars [CAR_NUM];
int carSpeeds [CAR_NUM];

GWindow gw;

void setupCars (void) {
  for (int i = 0; i < CAR_NUM; i++) {
    int lane = randomInteger(0,HEIGHT/CAR_HEIGHT)*CAR_HEIGHT;
    cars[i] = newGRect(0, lane, CAR_WIDTH, CAR_HEIGHT);
    setColor(cars[i], CAR_COLOR);
    setFilled(cars[i], true);
    add(gw, cars[i]);
    carSpeeds[i] = randomInteger(1, MAX_SPEED);
  }
}

void moveCars (void) {
  for (int i = 0; i < CAR_NUM; i++) {
    move(cars[i], carSpeeds[i], 0);
    if (getX(cars[i]) > WIDTH) {
      setLocation(cars[i], 0, getY(cars[i]));
      carSpeeds[i] = randomInteger(1, MAX_SPEED);
    }
  }
}

void drive (void) {
  while (1) {
    moveCars();
    pause (PAUSE_TIME);
  }
}

void setupCanvas (void) {
  gw = newGWindow (WIDTH, HEIGHT) ;
  fillRect (gw, 0, 0, WIDTH, HEIGHT) ; // draws a black background
}

int main (void) {
  setupCanvas();
  setupCars();
  drive();
  
  return 0;
}
