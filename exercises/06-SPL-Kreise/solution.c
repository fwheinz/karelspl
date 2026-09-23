#include <stdio.h>
#include <string.h>
// We use full paths here to enable replit code introspection
#include "cslib.h"
#include "random.h"
#include "gevents.h"
#include "gmath.h"
#include "gobjects.h"
#include "gwindow.h"
#include "simpio.h"

#define WIDTH 700
#define HEIGHT 500

void randomCircle (GWindow gw) {
  int diameter = randomInteger(5, 100);
  int x = randomInteger(0, WIDTH-diameter);
  int y = randomInteger(0, HEIGHT-diameter);
  setColor(gw, "orange");
  fillOval(gw, x, y, diameter, diameter);
  setColor(gw, "black");
  drawOval(gw, x, y, diameter, diameter);
}

int main(void) {
  GWindow gw = newGWindow(WIDTH, HEIGHT);
  pause(500);

  for (int i = 0; i < 250; i++) {
    randomCircle(gw);
  }

  while (1) {
    GEvent e = waitForEvent(KEY_EVENT);
    if (e != NULL && getEventType(e) == KEY_TYPED && getKeyChar(e) == 'n')
      randomCircle(gw);
  }

  waitForClick();
  closeGWindow(gw);

  return 0;
}
