#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gwindow.h"
#include "simpio.h"

int main(void) {
  GWindow gw = newGWindow(600, 400);
  pause(500);
  setColor(gw, "orange");
  fillOval(gw, 200, 100, 200, 200);

  /* Wait for a mouse click, close the window and terminate program */
  waitForClick();
  closeGWindow(gw);

  return 0;
}
