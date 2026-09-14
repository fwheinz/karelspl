#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "spl.h"
#include "karel.h"

#undef move

struct tile {
	char wall[5];

	int beepers;
	GObject beeper_img;
	GLabel beeper_label;
};

struct karel {
	int x, y;
	int dir;
	int bag;
	GImage img;
};

struct world {
	char *name;
	int w, h;
	struct tile *tiles;
	struct karel karel;
	int speed, running;
	GWindow gw;
	GRect go, reset, speedup, speeddown;
	GLabel l_go, l_reset, l_speedup, l_speeddown, l_speed, l_beepers;
};

struct world *ww;

#define INFINITE -1

enum {
	D_UNKNOWN,
	D_NORTH,
	D_WEST,
	D_SOUTH,
	D_EAST
};

void setSpeed (int sp);
void checkEvent (void);

static int str2dir (char *str) {
	if (strcasecmp(str, "north") == 0)
		return D_NORTH;
	else if (strcasecmp(str, "south") == 0)
		return D_SOUTH;
	else if (strcasecmp(str, "east") == 0)
		return D_EAST;
	else if (strcasecmp(str, "west") == 0)
		return D_WEST;
	return D_UNKNOWN;
}

static char *mystrsep(char **stringp, const char *delim)
{
    char *start = *stringp;
    char *p;

    if (start == NULL)
        return NULL;

    p = start + strcspn(start, delim);

    if (*p != '\0') {
        *p = '\0';
        *stringp = p + 1;
    } else {
        *stringp = NULL;
    }

    return start;
}

struct world *parseWorld (char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Opening file %s failed: %s\n", filename, strerror(errno));
		return NULL;
	}

	struct world *world = calloc(1, sizeof *world);

	char buf[1024], *line;
	while ((line = fgets(buf, sizeof(buf), f))) {
		line = mystrsep(&line, "\r\n");
		char *keyword = mystrsep(&line, ":");
		if (strcasecmp(keyword, "Dimension") == 0) {
			int w, h;
			int st = sscanf(line, " (%d, %d)", &w, &h);
			if (st == 2) {
				world->w = w;
				world->h = h;
				world->tiles = calloc(w*h, sizeof *world->tiles);
			} else {
				printf("Parsing dimension failed!\n");
			}
		} else if (strcasecmp(keyword, "Karel") == 0) {
			int x, y;
			char dir[10];
			int st = sscanf(line, " (%d, %d) %6s", &x, &y, dir);
			if (st == 3) {
				world->karel.x = x-1;
				world->karel.y = y-1;
				world->karel.dir = str2dir(dir);
				if (world->karel.dir == D_UNKNOWN) {
					printf("Unknown direction: %s, defaulting to east\n", dir);
					world->karel.dir = D_EAST;
				}
			} else {
				printf("Parsing karel failed!\n");
			}
		} else if (strcasecmp(keyword, "Wall") == 0) {
			int x, y;
			char dir[10];
			int st = sscanf(line, " (%d, %d) %6s", &x, &y, dir);
			if (st == 3) {
				x--; y--;
				int d = str2dir(dir);
				if (d == D_UNKNOWN) {
					printf("Wall: Unknown direction: %s\n", dir);
				} else {
					world->tiles[y*world->w+x].wall[d] = 1;
					int nd, dx, dy;
					switch (d) {
						case D_NORTH:
							dx = 0; dy = 1;
							nd = D_SOUTH;
							break;
						case D_SOUTH:
							dx = 0; dy = -1;
							nd = D_NORTH;
							break;
						case D_WEST:
							dx = -1; dy = 0;
							nd = D_EAST;
							break;
						case D_EAST:
							dx = 1; dy = 0;
							nd = D_WEST;
							break;
						default:
							nd = D_UNKNOWN;
							dx = 0; dy = 0;
							break;
					}
					int nx = x+dx;
					int ny = y+dy;
					if (nx >= 0 && nx < world->w && ny >= 0 && ny < world->h) {
						world->tiles[ny*world->w+nx].wall[nd] = 1;
					}

				}
			} else {
				printf("Parsing walls failed!\n");
			}
		} else if (strcasecmp(keyword, "Beeper") == 0) {
			int x, y, b;
			int st = sscanf(line, " (%d, %d) %d", &x, &y, &b);
			if (st == 3) {
				x--; y--;
				if (b == 0) {
					printf("Invalid beepers: %d\n", b);
				} else {
					world->tiles[y*world->w+x].beepers = b;
				}
			} else {
				printf("Parsing beepers failed!\n");
			}
		} else if (strcasecmp(keyword, "BeeperBag") == 0) {
			while (*line == ' ') line++;
			if (strncasecmp(line, "infinite", 8) == 0) {
				world->karel.bag = INFINITE;
			} else {
				world->karel.bag = atoi(line);
			}
		} else if (strcasecmp(keyword, "Speed") == 0) {
			double speed;
			while (*line == ' ') line++;
			speed = atof(line);
			if (speed < 1) speed = 1;
			world->speed = speed;
		} else {
			printf("Unhandled keyword: %s\n", keyword);
		}
	}

	for (int x = 0; x < world->w; x++) {
		world->tiles[x].wall[D_SOUTH] = 1;
		world->tiles[x+(world->h-1)*world->w].wall[D_NORTH] = 1;
	}

	for (int y = 0; y < world->h; y++) {
		world->tiles[y*world->w].wall[D_WEST] = 1;
		world->tiles[y*world->w+world->w-1].wall[D_EAST] = 1;
	}

	
	return world;
}

#define PADDING 10

static void setBeeper (struct world *world, int x, int _y, int beepers) {
	int y = world->h-_y-1;
	int sx = getWidth(world->gw)-PADDING*2;
	int sy = getHeight(world->gw)-PADDING*2;
	int w = world->w;
	int h = world->h;
	int s = sx / w;
	int s2 = sy / h;
	if (s > s2)
		s = s2;

	struct tile *t = &world->tiles[_y*w+x];
	t->beepers = beepers;
	if (beepers) {
		if (!t->beeper_label) {
			char nr[12] = " ";
			if (beepers > 1) 
				snprintf(nr, sizeof nr, "%d", beepers);
			t->beeper_label = newGLabel(nr);
			int fontsize = s/5;
			char fontsizestr[20];
			snprintf(fontsizestr, sizeof fontsizestr, "*-*-%d", fontsize);
			setFont(t->beeper_label, fontsizestr);
			move(t->beeper_label, PADDING+x*s+s/2-(fontsize*strlen(nr))/2, PADDING+y*s+s/2-fontsize/2);
			add(world->gw, t->beeper_label);
			sendToBack(t->beeper_label);
			}
		if (!t->beeper_img) {
			t->beeper_img = newGRect(PADDING+x*s+s/4, PADDING+y*s+s/4, s/2, s/2);
			setColor(t->beeper_img, "black");
			setFillColor(t->beeper_img, "light gray");
			setFilled(t->beeper_img, 1);
			add(world->gw, t->beeper_img);
			sendToBack(t->beeper_img);
		}
	} else {
		if (t->beeper_img) {
			removeGWindow(world->gw, t->beeper_img);
			freeGObject(t->beeper_img);
			t->beeper_img = NULL;
		}
		if (t->beeper_label) {
			removeGWindow(world->gw, t->beeper_label);
			freeGObject(t->beeper_label);
			t->beeper_label = NULL;
		}
	}
}

void setKarel (struct world *world, int x, int _y) {
	int sx = getWidth(world->gw)-PADDING*2;
	int sy = getHeight(world->gw)-PADDING*2;

	int w = world->w;
	int h = world->h;
	int s = sx / w;
	int s2 = sy / h;
	if (s > s2)
		s = s2;
	setSize(world->karel.img, s, s);
	int y = h-_y-1;
	setLocation(world->karel.img, PADDING+x*s, PADDING+y*s);
}

struct world * renderWorld (struct world *world) {
	if (!world->gw) {
		world->gw = newGWindow(1000, 800);
		world->karel.img = newGImage("data/karel.png");
		add(world->gw, world->karel.img);
	}

	int sx = getWidth(world->gw)-PADDING*2-100;
	int sy = getHeight(world->gw)-PADDING*2;

	int lx = getWidth(world->gw)-PADDING*2-100+10;
	world->go = newGRect(lx, 20, 60, 40);
	world->l_go = newGLabel("Go!");
	setFont(world->l_go, "*-*-25");
	add(world->gw, world->go);
	addAt(world->gw, world->l_go, lx+10, 25);

	world->reset = newGRect(lx, 80, 90, 40);
	world->l_reset = newGLabel("Reset");
	setFont(world->l_reset, "*-*-25");
	add(world->gw, world->reset);
	addAt(world->gw, world->l_reset, lx+10, 85);

	world->speedup = newGRect(lx, 180, 40, 40);
	add(world->gw, world->speedup);
	world->l_speedup = newGLabel("+");
	setFont(world->l_speedup, "*-*-30");
	addAt(world->gw, world->l_speedup, lx+10, 185);

	world->speeddown = newGRect(lx, 280, 40, 40);
	add(world->gw, world->speeddown);
	world->l_speeddown = newGLabel("-");
	setFont(world->l_speeddown, "*-*-30");
	addAt(world->gw, world->l_speeddown, lx+10, 285);

	world->l_speed = newGLabel("0");
	setFont(world->l_speed, "*-*-30");
	addAt(world->gw, world->l_speed, lx+10, 235);
	setSpeed(world->speed);

	world->l_beepers = newGLabel("oo");
	setFont(world->l_beepers, "*-*-30");
	addAt(world->gw, world->l_beepers, lx+10, 350);

	int w = world->w;
	int h = world->h;
	int s = sx / w;
	int s2 = sy / h;
	if (s > s2)
		s = s2;
	setColor(world->gw, "gray");
	setFillColor(world->gw, "gray");
//	drawRect(world->gw, PADDING, PADDING, w*s, h*s);
	for (int x = 0; x < w; x++) {
		for (int _y = 0; _y < h; _y++) {
			int y = h-_y-1;
			fillOval(world->gw, PADDING+x*s+s/2-1, PADDING+y*s+s/2-1, 3, 3);
			if (world->tiles[_y*w+x].wall[D_NORTH])
				drawLine(world->gw, PADDING+x*s, PADDING+y*s, PADDING+x*s+s, PADDING+y*s);
			if (world->tiles[_y*w+x].wall[D_SOUTH])
				drawLine(world->gw, PADDING+x*s, PADDING+(y+1)*s, PADDING+x*s+s, PADDING+(y+1)*s);
			if (world->tiles[_y*w+x].wall[D_WEST])
				drawLine(world->gw, PADDING+x*s, PADDING+y*s, PADDING+x*s, PADDING+(y+1)*s);
			if (world->tiles[_y*w+x].wall[D_EAST])
				drawLine(world->gw, PADDING+(x+1)*s, PADDING+y*s, PADDING+(x+1)*s, PADDING+(y+1)*s);
			int beepers = world->tiles[_y*w+x].beepers;
			setBeeper(world, x, _y, beepers);
		}
	}
	setKarel(world, world->karel.x, world->karel.y);

	return world;
}

void renderBeepers (void) {
	if (ww->karel.bag == INFINITE) {
		setLabel(ww->l_beepers, "oo");
	} else {
		char nr[20];
		snprintf(nr, sizeof nr, "%d", ww->karel.bag);
		setLabel(ww->l_beepers, nr);
	}
}

void crash (void) {
	int sx = getWidth(ww->gw);
	int sy = getHeight(ww->gw);
	setColor(ww->gw, "light gray");
	fillRect(ww->gw, sx/2-110, sy/2-50, 220, 100);
	setColor(ww->gw, "black");
	drawRect(ww->gw, sx/2-110, sy/2-50, 220, 100);
	GLabel msg = newGLabel("Crash!!");
	setFont(msg, "*-*-50");
	setColor(msg, "red");
	addAt(ww->gw, msg, sx/2-100, sy/2-25);
	waitForClick();
	exit(1);
}

static void karelPause (void) {
	checkEvent();
	pause(1000/(ww->speed+1));
}

int frontIsBlocked (void) {
	int x = ww->karel.x;
	int y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];

	return t->wall[ww->karel.dir];
}
int frontIsClear (void) { return !frontIsBlocked(); }

int rightIsBlocked (void) {
	int x = ww->karel.x;
	int y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];
	int dir = ww->karel.dir+1;
	if (dir > D_EAST)
		dir = D_NORTH;

	return t->wall[dir];
}
int rightIsClear (void) { return !rightIsBlocked(); }

int leftIsBlocked (void) {
	int x = ww->karel.x;
	int y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];
	int dir = ww->karel.dir-1;
	if (dir == 0)
		dir = D_EAST;

	return t->wall[dir];
}
int leftIsClear (void) { return !leftIsBlocked(); }

int beepersPresent (void) {
	int x = ww->karel.x;
	int y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];

	return t->beepers > 0;
}
int noBeepersPresent (void) { return !beepersPresent(); }

int beepersInBag (void) {
	return ww->karel.bag != 0;
}
int noBeepersInBag (void) { return !beepersInBag(); }

int facingWest (void) {
	return ww->karel.dir == D_WEST;
}
int notFacingWest (void) { return !facingWest(); }

int facingSouth (void) {
	return ww->karel.dir == D_SOUTH;
}
int notFacingSouth (void) { return !facingSouth(); }

int facingEast (void) {
	return ww->karel.dir == D_EAST;
}
int notFacingEast (void) { return !facingEast(); }

int facingNorth (void) {
	return ww->karel.dir == D_NORTH;
}
int notFacingNorth (void) { return !facingNorth(); }

void moveKarel (void) {
	if (frontIsBlocked()) {
		crash();
	}
	int dx, dy;
	switch (ww->karel.dir) {
		case D_NORTH:
			dx = 0; dy = 1;
			break;
		case D_SOUTH:
			dx = 0; dy = -1;
			break;
		case D_WEST:
			dx = -1; dy = 0;
			break;
		case D_EAST:
			dx = 1; dy = 0;
			break;
		default:
			fprintf(stderr, "Warning: Unknown direction: %d\n", ww->karel.dir);
			dx = dy = 0;
			break;
	}
	int nx = ww->karel.x+dx;
	int ny = ww->karel.y+dy;
	setKarel(ww, nx, ny);
	ww->karel.x = nx;
	ww->karel.y = ny;
	karelPause();
}

void turnLeft (void) {
	ww->karel.dir++;
	if (ww->karel.dir > D_EAST)
		ww->karel.dir = D_NORTH;

	switch(ww->karel.dir) {
		case D_EAST:
			setRotation(ww->karel.img, 0);
			break;
		case D_SOUTH:
			setRotation(ww->karel.img, 90);
			break;
		case D_WEST:
			setRotation(ww->karel.img, 180);
			break;
		case D_NORTH:
			setRotation(ww->karel.img, 270);
			break;
	}


	karelPause();
}

void pickBeeper (void) {
	int x = ww->karel.x, y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];
	if (!t->beepers)
		crash();
	setBeeper(ww, x, y, t->beepers-1);
	if (ww->karel.bag != INFINITE)
		ww->karel.bag++;
	renderBeepers();
	karelPause();
}

void putBeeper (void) {
	int x = ww->karel.x, y = ww->karel.y;
	struct tile *t = &ww->tiles[y*ww->w+x];
	if (ww->karel.bag != INFINITE && ww->karel.bag <= 0)
		crash();
	setBeeper(ww, x, y, t->beepers+1);
	if (ww->karel.bag != INFINITE)
		ww->karel.bag--;
	renderBeepers();
	karelPause();
}

void loadWorld (char *name) {
	char fname[strlen(name)+20];
	snprintf(fname, sizeof fname, "data/worlds/%s.w", name);
	ww = parseWorld(fname);
	renderWorld(ww);
}

void setSpeed (int sp) {
	if (sp > 10) sp = 10;
	if (sp < 1) sp = 1;
	ww->speed = sp;
	char nr[10];
	snprintf(nr, sizeof nr, "%d", ww->speed);
	setLabel(ww->l_speed, nr);
}

void processEvent (GEvent e) {
	if (!e)
		return;
	GObject o = getGObjectAt(ww->gw, getX(e), getY(e));
	if (o == ww->go && !ww->running) {
		ww->running = 1;
		run();
		ww->running = 0;
	} else if (o == ww->speedup) {
		setSpeed(ww->speed+1);
	} else if (o == ww->speeddown) {
		setSpeed(ww->speed-1);
	} else if (o == ww->reset) {
		setKarel(ww, 0, 0);
	}
}

void checkEvent (void) {
	GEvent e;
	while ((e = getNextEvent(CLICK_EVENT)))
		processEvent(e);
}

int main (void) {
	setup();

	while (1) {
		GEvent e = waitForEvent(CLICK_EVENT);
		processEvent(e);
	}
}

