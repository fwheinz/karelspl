#include <stdio.h>
#include <string.h>

#include "cslib.h"
#include "gmath.h"
#include "gobjects.h"
#include "gwindow.h"
#include "gevents.h"
#include "simpio.h"

int hello (void) {
  printf("Hello World");

  return 100;
}

void printhashes (int nrHashes) {
  for (int i = 0; i < nrHashes; i++) {
    printf("#");
  }
  printf("\n");
}

#define WIDTH 800
#define HEIGHT 600

void fillCircle(GWindow gw, double x, double y, double r) {
  fillOval(gw, x-r, y-r, 2*r, 2*r);
}

enum weekday {
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY,
  UNKNOWN
};

enum weekday getWeekday (int input) {
  if (input == 1) {
    return MONDAY;
  } else if (input == 2) {
    return TUESDAY;
  } else if (input == 3) {
    return WEDNESDAY;
  } else if (input == 4) {
    return THURSDAY;
  } else if (input == 5) {
    return FRIDAY;
  } else if (input == 6) {
    return SATURDAY;
  } else if (input == 7) {
    return SUNDAY;
  } else {
    return UNKNOWN;
  }
}

void printWeekDay (enum weekday day) {
  if (day == MONDAY) {
    printf("Monday");
  } else if (day == TUESDAY) {
    printf("Tuesday");
  } else if (day == WEDNESDAY) {
    printf("Wednesday");
  } else if (day == THURSDAY) {
    printf("Thursday");
  } else if (day == FRIDAY) {
    printf("Friday");
  } else if (day == SATURDAY) {
    printf("Saturday");
  } else if (day == SUNDAY) {
    printf("Sunday");
  } else {
    printf("Invalid!");
  }
}

void printWeekDay2 (enum weekday day) {
  switch (day) {
    case MONDAY:
      printf("Monday");
      break;
    case TUESDAY:
      printf("Tuesday");
      break;
    case WEDNESDAY:
      printf("Wednesday");
      break;
    case THURSDAY:
      printf("Thursday");
      break;
    case FRIDAY:
      printf("Friday");
      break;
    case SATURDAY:
    case SUNDAY:
      printf("Weekend");
      break;
    default:
      printf("Invalid!");
      break;
  }

  printf("\n");

}

int determineSleepLate (enum weekday day, int holiday) {
  if ( (day == SATURDAY) || (day == SUNDAY) || (holiday == 1) ) {
    return 1;
  }

  return 0;
}

void km2meters (int *distance) {
  *distance = *distance * 1000;
}

void goSouthEast (int *lat, int *lon) {
  *lat = *lat + 1;
  *lon = *lon + 1;
}

void main(void) {
  int latitude, longitude;

  latitude = 32;
  longitude = -64;

  goSouthEast(&latitude, &longitude);

  printf("Koordinaten: %d / %d\n", latitude, longitude);
}





