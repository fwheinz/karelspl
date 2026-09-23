#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gwindow.h"
#include "simpio.h"

#define WIDTH 800
#define HEIGHT 600

#define BRICK_WIDTH 30
#define BRICK_HEIGHT 12
#define BRICKS_IN_BASE 14

/* Draw a single row of the pyramid */
void drawRow(GWindow gw, int row) {
  // Calculating the y coordinate of the row is a bit tricky
  int y = HEIGHT - (BRICKS_IN_BASE-row+1)*BRICK_HEIGHT;
  int x = (WIDTH-row*BRICK_WIDTH)/2;

  for (int i = 0; i < row; i++) {
    setColor(gw, "orange");
    fillRect(gw, x + i*BRICK_WIDTH, y, BRICK_WIDTH, BRICK_HEIGHT);
    setColor(gw, "black");
    drawRect(gw, x + i*BRICK_WIDTH, y, BRICK_WIDTH, BRICK_HEIGHT);
  }
}

/* Draw the pyramid */
void drawPyramid(GWindow gw) {
  for (int row = 1; row <= BRICKS_IN_BASE; row++) {
    drawRow(gw, row);
  }
}

/* Setup the canvas and draw a pyramid */
int main(void) {
  GWindow gw = newGWindow(WIDTH, HEIGHT);
  pause(500);

  drawPyramid(gw);

  /* Wait for a mouse click, close the window and terminate program */
  waitForClick();
  closeGWindow(gw);

  return 0;
}
