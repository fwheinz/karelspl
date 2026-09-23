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

#define RINGSIZE 15

int main(void) {
  GWindow gw = newGWindow(WIDTH, HEIGHT);
  pause(500);

  // Draw 16 circles, the outermost is white
  // so the innermost is red.
  for (int i = 16; i >0; i--) {
    int x = (WIDTH-i*RINGSIZE)/2;
    int y = (HEIGHT-i*RINGSIZE)/2;
    if (i%2 == 0) { // i is even
      setColor(gw, "white");
    } else { // i is odd
      setColor(gw, "red");
    }
    fillOval(gw, x, y, i*15, i*15);
  }

  /* Wait for a mouse click, close the window and terminate program */
  waitForClick();
  closeGWindow(gw);

  return 0;
}
