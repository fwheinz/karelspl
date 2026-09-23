#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "spl.h"

#ifdef _WIN32
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#endif

enum {
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW
};

enum {
	T_UNKNOWN,
	T_EVENT,
	T_WINDOW,
	T_POINT,
	T_DIMENSION,
	T_OVAL,
	T_RECT,
	T_ROUNDRECT,
	T_3DRECT,
	T_LINE,
	T_IMAGE,
	T_ARC,
	T_LABEL,
	T_POLYGON
};

struct color {
	int r,g,b;
};

typedef struct ObjectCDT * GObject;
struct ObjectCDT {
	int type;
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Texture *bg;
	SDL_Surface *image;
	struct color color, fillcolor;
	double x, y;
	double w, h;
	double corner;
	double start, sweep;
	bool raised;
	bool visible, filled;
	bool norepaint;
	struct ObjectCDT *vector;
	int n;
	GWindow gw;
	GObject next;

	int eclass;
	int etype;
	double ts;
	int modifiers;
	GObject source;
	char *actioncommand;
	TTF_Font *ttf;
	int fontsize;
	char *text;
	int size;
	char keychar, keycode;
};

typedef struct ObjectCDT Point;

struct color name2color (char *name) {
	if (!strcasecmp(name, "red")) {
		return (struct color) { 255, 0, 0 };
	}	else if (!strcasecmp(name, "green")) {
		return (struct color) { 0, 255, 0 };
	}	else if (!strcasecmp(name, "blue")) {
		return (struct color) { 0, 0, 255 };
	}	else if (!strcasecmp(name, "yellow")) {
		return (struct color) { 255, 255, 0 };
	}	else if (!strcasecmp(name, "cyan")) {
		return (struct color) { 0, 255, 255 };
	}	else if (!strcasecmp(name, "purple") || !strcasecmp(name, "magenta")) {
		return (struct color) { 255, 0, 255 };
	}	else if (!strcasecmp(name, "pink")) {
		return (struct color) { 255, 192, 203 };
	}	else if (!strcasecmp(name, "orange")) {
		return (struct color) { 255, 165, 0 };
	}	else if (!strcasecmp(name, "white")) {
		return (struct color) { 255, 255, 255 };
	}	else if (!strcasecmp(name, "black")) {
		return (struct color) { 0, 0, 0 };
	}	else if (!strcasecmp(name, "grey") || !strcasecmp(name, "gray")) {
		return (struct color) { 128, 128, 128 };
	}	else if ((!strcasecmp(name, "light_gray") || !strcasecmp(name, "light gray") || 
			        !strcasecmp(name, "light_grey") || !strcasecmp(name, "light grey"))) {
		return (struct color) { 192, 192, 192 };
	}	else if ((!strcasecmp(name, "dark_gray") || !strcasecmp(name, "dark gray") || 
			        !strcasecmp(name, "dark_grey") || !strcasecmp(name, "dark grey"))) {
		return (struct color) { 64, 64, 64 };
	}	else if (!strcasecmp(name, "brown")) {
		return (struct color) { 150, 75, 0 };
	} else if (*name == '#') {
		struct color ret;
		int st = sscanf(name, "#%02x%02x%02x", &ret.r, &ret.g, &ret.b);
		if (st == 3)
			return ret;
	}
	fprintf(stderr, "Unknown color: %s\n", name);
	return (struct color) { 0, 0, 0 };
}

string getType (GObject o) {
	switch (o->type) {
		case T_EVENT:
			return "GEvent";
		case T_WINDOW:
			return "GWindow";
		case T_POINT:
			return "GPoint";
		case T_DIMENSION:
			return "GDimension";
		case T_OVAL:
			return "GOval";
		case T_RECT:
			return "GRect";
		case T_ROUNDRECT:
			return "GRoundRect";
		case T_3DRECT:
			return "G3DRect";
		case T_LINE:
			return "GLine";
		case T_ARC:
			return "GArc";
		case T_LABEL:
			return "GLabel";
		case T_IMAGE:
			return "GImage";
		case T_POLYGON:
			return "GPolygon";
		default:
			return "Unknown";
	}
}

int sdl_handle_events (int block);

void render_oval_sdl (SDL_Renderer *rend, GObject o) {
	double cx = o->x + o->w / 2;
	double cy = o->y + o->h / 2;

	double rx = o->w / 2.0;
	double ry = o->h / 2.0;

	// Draw filled area
	if (o->filled) {
		if (o->fillcolor.r >= 0) 
			SDL_SetRenderDrawColor(rend, o->fillcolor.r, o->fillcolor.g, o->fillcolor.b, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
		for (double py = o->y; py < o->y + o->h; ++py) {
			for (double px = o->x; px < o->x + o->w; ++px) {
				double dx = (px - cx) / rx;
				double dy = (py - cy) / ry;

				double distance = dx * dx + dy * dy;

				if (distance <= 1.0) {
					SDL_RenderDrawPoint(rend, px, py);
				}
			}
		}
	}

	// Draw outer shape
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
	double h = ((rx - ry)*(rx - ry)) / ((rx + ry)*(rx + ry));
	int segments = M_PI * (rx + ry) *
		(1.0 + (3.0 * h) / (10.0 + sqrt(4.0 - 3.0 * h)));

	if (segments < 12)
		segments = 12;

	double step = 2.0 * M_PI / segments;

	int prevX = cx + rx + 0.5;
	int prevY = cy + 0.5;

	for (int i = 1; i <= segments; ++i) {
		double angle = i * step;

		int px = cx + rx * cos(angle) + 0.5;
		int py = cy + ry * sin(angle) + 0.5;

		SDL_RenderDrawLine(rend, prevX, prevY, px, py);

		prevX = px;
		prevY = py;
	}
}

void render_oval (GWindow gw, GObject o) {
	render_oval_sdl(gw->rend, o);
}

void render_arc_sdl (SDL_Renderer *rend, GObject o) {
	int cx = o->x + o->w / 2;
	int cy = o->y + o->h / 2;

	double rx = o->w / 2.0;
	double ry = o->h / 2.0;

	// Draw filled area
	if (o->filled) {
		if (o->fillcolor.r >= 0) 
			SDL_SetRenderDrawColor(rend, o->fillcolor.r, o->fillcolor.g, o->fillcolor.b, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
		for (int py = o->y; py < o->y + o->h; ++py) {
			for (int px = o->x; px < o->x + o->w; ++px) {
				double dx = (px - cx) / rx;
				double dy = (py - cy) / ry;

				double distance = dx * dx + dy * dy;

				if (distance <= 1.0) {
					SDL_RenderDrawPoint(rend, px, py);
				}
			}
		}
	}

	// Draw outer shape
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
	double h = ((rx - ry)*(rx - ry)) / ((rx + ry)*(rx + ry));
	int segments = M_PI * (rx + ry) *
		(1.0 + (3.0 * h) / (10.0 + sqrt(4.0 - 3.0 * h)));

	if (segments < 12)
		segments = 12;

	double step = 2.0 * M_PI / segments;

	int prevX = cx + rx + 0.5;
	int prevY = cy + 0.5;

	for (int i = segments; i > 0; i--) {
		double angle = i * step;
		double degrees = angle*180/M_PI;

		int px = cx + rx * cos(angle) + 0.5;
		int py = cy + ry * sin(angle) + 0.5;

		if (degrees > o->start && degrees < o->start+o->sweep)
			SDL_RenderDrawLine(rend, prevX, prevY, px, py);

		prevX = px;
		prevY = py;
	}
	if (o->filled) {
		double a1 = o->start/180.0*M_PI;
		double a2 = (o->start+o->sweep)/180.0*M_PI;

		int px = cx + rx * cos(a1) + 0.5;
		int py = cy + ry * sin(a1) + 0.5;
		SDL_RenderDrawLine(rend, cx, cy, px, py);
		px = cx + rx * cos(a2) + 0.5;
		py = cy + ry * sin(a2) + 0.5;
		SDL_RenderDrawLine(rend, cx, cy, px, py);
	}
}

void render_arc (GWindow gw, GObject o) {
	render_arc_sdl(gw->rend, o);
}


void repaint (GWindow gw);

double getX (GObject o) { return o->x; }
double getY (GObject o) { return o->y; }
double getWidth (GObject o) { return o->w; }
double getHeight (GObject o) { return o->h; }

void setColor (GObject o, char *name) {
	o->color = name2color(name);
	repaint(o->gw);
}

void setColorGWindow (GObject o, char *name) {
	setColor(o, name);
}

char *getColorGObject (GObject o) {
	char *c = malloc(8);
	snprintf(c, 8, "#%02hhx%02hhx%02hhx", o->color.r, o->color.g, o->color.b);

	return c;
}

void setFillColor (GObject o, char *name) {
	o->fillcolor = name2color(name);
	repaint(o->gw);
}

string getFillColor (GObject o) {
	char *c = malloc(8);
	snprintf(c, 8, "#%02hhx%02hhx%02hhx", o->fillcolor.r, o->fillcolor.g, o->fillcolor.b);

	return c;
}

char *getColorGWindow (GWindow gw) {
	return getColorGObject(gw);
}

void setFilled (GObject o, int filled) {
	o->filled = !!filled;

	repaint(o->gw);
}

bool isFilled (GObject o) {
	return o->filled;
}

void setVisible (GObject o, int visible) {
	o->visible = !!visible;
	if (o->type == T_WINDOW) {
		if (o->visible) {
			SDL_ShowWindow(o->win);
		} else {
			SDL_HideWindow(o->win);
		}
	} else {
		repaint(o->gw);
	}
}

bool isVisible (GObject o) {
	return o->visible;
}

void render_rect_sdl (SDL_Renderer *rend, GObject o) {
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);

	SDL_Rect r;

	r.x = o->x;
	r.y = o->y;
	r.w = o->w;
	r.h = o->h;

	if (o->filled) {
		if (o->fillcolor.r >= 0) 
			SDL_SetRenderDrawColor(rend, o->fillcolor.r, o->fillcolor.g, o->fillcolor.b, SDL_ALPHA_OPAQUE);
		else
			SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(rend, &r);
	}
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(rend, &r);
}

void render_rect (GWindow gw, GObject o) {
	render_rect_sdl(gw->rend, o);
}

void render_line_sdl (SDL_Renderer *rend, GObject o) {
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(rend, o->x, o->y, o->w+o->x, o->h+o->y);
}

void render_line (GWindow gw, GObject o) {
	render_line_sdl(gw->rend, o);
}

void render_image_sdl (SDL_Renderer *rend, GObject o) {
	SDL_Rect rect = { 0 };
	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, o->image);
	rect.x = o->x;
	rect.y = o->y;
	rect.w = o->w;
	rect.h = o->h;

	double a = o->start;

	SDL_RenderCopyEx(rend, tex, NULL, &rect, a, NULL, SDL_FLIP_NONE);
}

void render_image (GWindow gw, GObject o) {
	render_image_sdl(gw->rend, o);
}

int drawFilledPolygon(SDL_Renderer *renderer, const Point *points, int count, SDL_Color color);

void render_polygon_sdl (SDL_Renderer *rend, GObject o) {
	if (o->filled) {
		SDL_Color color;
		if (o->fillcolor.r >= 0) 
			color = (SDL_Color) { o->fillcolor.r, o->fillcolor.g, o->fillcolor.b, SDL_ALPHA_OPAQUE };
		else
			color = (SDL_Color) { o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE };
		drawFilledPolygon(rend, o->vector, o->n, color);
	}
	SDL_SetRenderDrawColor(rend, o->color.r, o->color.g, o->color.b, SDL_ALPHA_OPAQUE);
	for (int i = 0; i < o->n; i++) {
		SDL_RenderDrawLine(rend, o->vector[i].x, o->vector[i].y, o->vector[(i+1)%o->n].x, o->vector[(i+1)%o->n].y);
	}
}

void render_polygon (GWindow gw, GObject o) {
	render_polygon_sdl(gw->rend, o);
}

void render_label_sdl (SDL_Renderer *rend, GObject o) {
	if (!o->ttf)
		return;
	struct color *c = &o->color;
	SDL_Color color = { c->r, c->g, c->b, SDL_ALPHA_OPAQUE };
	SDL_Surface *text = TTF_RenderUTF8_Blended(o->ttf, o->text, color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, text);
	SDL_Rect rect = { 0 };
	rect.x = o->x;
	rect.y = o->y;
	rect.w = text->w;
	rect.h = text->h;

	SDL_RenderCopy(rend, tex, NULL, &rect);
}

void render_label (GWindow gw, GObject o) {
	render_label_sdl(gw->rend, o);
}

void render_object (GWindow gw, GObject o) {
	if (!o->visible)
		return;
	switch (o->type) {
		case T_OVAL:
			render_oval(gw, o);
			break;
		case T_RECT:
		case T_ROUNDRECT:
		case T_3DRECT:
			render_rect(gw, o);
			break;
		case T_LINE:
			render_line(gw, o);
			break;
		case T_IMAGE:
			render_image(gw, o);
			break;
		case T_ARC:
			render_arc(gw, o);
			break;
		case T_LABEL:
			render_label(gw, o);
			break;
		case T_POLYGON:
			render_polygon(gw, o);
			break;
		default:
			fprintf(stderr, "Unsupported object type: %d\n", o->type);
			break;
	}
}


void repaint (GWindow gw) {
  sdl_handle_events(0);

	if (!gw || gw->norepaint) return;
	SDL_SetRenderDrawColor(gw->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(gw->rend);
	SDL_RenderCopy(gw->rend, gw->bg, NULL, NULL);

	GObject o = gw->next;
	while (o) {
		render_object(gw, o);
		o = o->next;
	}

	SDL_RenderPresent(gw->rend);
}

void setRepaint (GWindow gw, int do_repaint) {
	gw->norepaint = !do_repaint;
	if (!gw->norepaint)
		repaint(gw);
}

bool getRepaint (GWindow gw) {
	return !gw->norepaint;
}


GObject newGObject (void) {
	GObject o = calloc(1, sizeof *o);
	o->visible = 1;
	o->filled = 0;
	o->color = name2color("black");
	o->fillcolor.r = o->fillcolor.g = o->fillcolor.b = -1;

	return o;
}

void freeGObject (GObject o) {
	free(o->text);
	TTF_CloseFont(o->ttf);
	free(o->vector);

	free(o);
}

GPoint newGPoint (double x, double y) {
	GPoint o = newGObject();
	o->type = T_POINT;
	o->x = x;
	o->y = y;
	o->w = 1;
	o->h = 1;

	return o;
}

GDimension newGDimension (double w, double h) {
	GPoint o = newGObject();
	o->type = T_DIMENSION;
	o->x = 0;
	o->y = 0;
	o->w = w;
	o->h = h;

	return o;
}

GOval newGOval (double x, double y, double w, double h) {
	GOval o = newGObject();
	o->type = T_OVAL;
	o->x = x;
	o->y = y;
	o->w = w;
	o->h = h;

	return o;
}

bool oval_contains (GObject o, double x, double y) {
	int cx = o->x + o->w / 2;
	int cy = o->y + o->h / 2;

	double rx = o->w / 2.;
	double ry = o->h / 2.;

	double dx = (x - cx) / rx;
	double dy = (y - cy) / ry;

	double distance = dx * dx + dy * dy;

	return distance <= 1.;
}

void drawOval (GWindow gw, double x, double y, double w, double h) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	GOval o = newGOval(x, y, w, h);
	o->color = gw->color;
	SDL_SetRenderDrawColor(gw->rend, gw->color.r, gw->color.g, gw->color.b, SDL_ALPHA_OPAQUE);
	render_oval_sdl(gw->rend, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	freeGObject(o);
	repaint(gw);
}

void fillOval (GWindow gw, double x, double y, double w, double h) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	GOval o = newGOval(x, y, w, h);
	o->color = gw->color;
	setFilled(o, 1);
	render_oval_sdl(gw->rend, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	freeGObject(o);
	repaint(gw);
}

GRect newGRect (double x, double y, double w, double h) {
	GRect o = newGObject();
	o->type = T_RECT;
	o->x = x;
	o->y = y;
	o->w = w;
	o->h = h;

	return o;
}

GRoundRect newGRoundRect (double x, double y, double w, double h, double corner) {
	GRect o = newGRect(x, y, w, h);
	o->type = T_ROUNDRECT;
	o->corner = corner;

	return o;
}

G3DRect newG3DRect (double x, double y, double w, double h, bool raised) {
	GRect o = newGRect(x, y, w, h);
	o->type = T_3DRECT;
	o->raised = raised;

	return o;
}
void setRaised (G3DRect o, bool raised) {
	o->raised = raised;

	repaint(o->gw);
}

bool isRaised(G3DRect o) {
	return o->raised;
}

bool rect_contains (GObject o, double x, double y) {
	return (x >= o->x && x <= o->x+o->w &&
			    y >= o->y && y <= o->y+o->h);
}

void drawRect (GWindow gw, double x, double y, double w, double h) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	GOval o = newGOval(x, y, w, h);
	o->color = gw->color;
	SDL_SetRenderDrawColor(gw->rend, gw->color.r, gw->color.g, gw->color.b, SDL_ALPHA_OPAQUE);
	render_rect_sdl(gw->rend, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	freeGObject(o);
	repaint(gw);
}

void fillRect (GWindow gw, double x, double y, double w, double h) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	GOval o = newGOval(x, y, w, h);
	o->color = gw->color;
	setFilled(o, 1);
	render_rect_sdl(gw->rend, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	freeGObject(o);
	repaint(gw);
}


GLine newGLine (double x1, double y1, double x2, double y2) {
	GLine o = newGObject();
	o->type = T_LINE;
	o->x = x1;
	o->y = y1;
	o->w = x2-x1;
	o->h = y2-y1;

	return o;
}

GImage newGImage (char *filename) {
	GImage o = newGObject();
	o->type = T_IMAGE;
	o->image = IMG_Load(filename);
	if (!o->image) {
		printf("Loading image %s failed!\n", filename);
	}
	o->w = o->image->w;
	o->h = o->image->h;

	return o;
}

void setRotation (GImage o, double a) {
	o->start = a;
	repaint(o->gw);
}

double getRotation (GImage o) {
	return o->start;
}

GPolygon newGPolygon () {
	GPolygon o = newGObject();
	o->type = T_POLYGON;

	return o;
}

void addVertex (GPolygon o, double x, double y) {
	o->vector = realloc(o->vector, (o->n+1) * sizeof (*(o->vector)));
	memset(&o->vector[o->n], 0, sizeof (*(o->vector)));
	o->vector[o->n].type = T_POINT;
	o->vector[o->n].x = x;
	o->vector[o->n].y = y;
	o->n++;

	repaint(o->gw);
}

void addEdge (GPolygon o, double dx, double dy) {
	if (o->n) {
		double x = o->vector[o->n-1].x+dx;
		double y = o->vector[o->n-1].y+dy;
		addVertex(o, x, y);
	} else {
		addVertex(o, dx, dy);
	}

	repaint(o->gw);
}

void addPolarEdge (GPolygon o, double r, double theta) {
	double x = 0;
	double y = 0;
	if (o->n) {
		x = o->vector[o->n-1].x;
		y = o->vector[o->n-1].y;
	}
	x = x + r * cos(theta * M_PI / 180.0);
	y = y + r * sin(theta * M_PI / 180.0);
	addVertex(o, x, y);

	repaint(o->gw);
}

GArc newGArc (double x, double y, double w, double h, double start, double sweep) {
	GArc o = newGObject();
	o->type = T_ARC;
	o->x = x;
	o->y = y;
	o->w = w;
	o->h = h;
	o->start = start;
	o->sweep = sweep;

	return o;
}

void setStartAngle(GArc o, double start) {
	o->start = start;
	repaint(o->gw);
}

double getStartAngle(GArc o) {
	return o->start;
}

void setSweepAngle(GArc o, double sweep) {
	o->sweep = sweep;
	repaint(o->gw);
}

double getSweepAngle(GArc o) {
	return o->sweep;
}

void setFrameRectangle (GArc o, double x, double y, double w, double h) {
	o->x = x;
	o->y = y;
	o->w = w;
	o->h = h;

	repaint(o->gw);
}

GRectangle getFrameRectangle (GArc o) {
	return newGRect(o->x, o->y, o->w, o->h);
}


GLabel newGLabel(string str) {
	GLabel o = newGObject();
	o->type = T_LABEL;
	o->text = strdup(str);
	o->fontsize = 12;

	o->ttf = TTF_OpenFont("data/default.ttf", o->fontsize);
	if (!o->ttf) {
		fprintf(stderr, "WARNING: Default font missing!\n");
	}

	return o;
}

void setLabel (GLabel o, string str) {
	free(o->text);
	o->text = strdup(str);

	repaint(o->gw);
}

char *getLabel (GLabel o) {
	return strdup(o->text);
}

void setFont (GLabel o, string str) {
	char *font = strdup(str);

	char *family = strtok(font, "-");
	char *style = strtok(NULL, "-");
	char *_size = strtok(NULL, "-");
	int size = 0;
	if (o->ttf) {
		size = o->fontsize;
	}
	if (_size && _size[0] != '*') {
		int tmp = atoi(_size);
		if (tmp)
			size = tmp;
	}
	if (size == 0)
		size = 12;

	o->fontsize = size;
	TTF_CloseFont(o->ttf);
	char fname[100];
	if (!family || family[0] == '*') {
		snprintf(fname, sizeof fname, "data/default.ttf");
	} else if (family && family[0] != '*' && (!style || style[0] == '*')) {
		snprintf(fname, sizeof fname, "%s.ttf", family);
	} else {
		snprintf(fname, sizeof fname, "%s-%s.ttf", family, style);
	}

	o->ttf = TTF_OpenFont(fname, size);
	if (!o->ttf) {
		o->ttf = TTF_OpenFont("data/default.ttf", size);
	}
	free(font);
	repaint(o->gw);
}


void drawLine (GWindow gw, double x1, double y1, double x2, double y2) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	GLine o = newGLine(x1, y1, x2, y2);
	o->color = gw->color;
	SDL_SetRenderDrawColor(gw->rend, gw->color.r, gw->color.g, gw->color.b, SDL_ALPHA_OPAQUE);
	render_line_sdl(gw->rend, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	freeGObject(o);
	repaint(gw);
}

void drawPolarLine (GWindow gw, double x1, double y1, double r, double theta) {
	double x2 = x1 + r * cos(theta * M_PI / 180.0);
	double y2 = y1 - r * sin(theta * M_PI / 180.0);
	drawLine(gw, x1, y1, x2, y2);
}

void setStartPoint(GLine o, double x, double y) {
	o->x = x;
	o->y = y;

	repaint(o->gw);
}

void setEndPoint(GLine o, double x, double y) {
	o->w = x-o->x;
	o->h = y-o->y;

	repaint(o->gw);
}

GPoint getStartPoint(GObject o) {
	return newGPoint(o->x, o->y);
}

GPoint getEndPoint(GObject o) {
	return newGPoint(o->x+o->w, o->y+o->h);
}

void draw (GWindow gw, GObject o) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	render_object(gw, o);
	SDL_SetRenderTarget(gw->rend, NULL);
	repaint(gw);
}

void drawAt(GWindow gw, GObject o, double x, double y) {
	o->x = x;
	o->y = y;
	draw(gw, o);
}

void add (GWindow gw, GObject o) {
	if (o->gw) {
		removeGWindow(o->gw, o);
	}

	if (!gw->next) {
		gw->next = o;
	} else {
		GObject ob = gw->next;
		while (ob->next)
			ob = ob->next;
		ob->next = o;
	}
	o->gw = gw;
	repaint(gw);
}

void addAt (GWindow gw, GObject o, double x, double y) {
	o->x = x;
	o->y = y;
	add(gw, o);
}

void move (GObject o, double x, double y) {
	o->x += x;
	o->y += y;
	repaint(o->gw);
}

void setLocation (GObject o, double x, double y) {
	o->x = x;
	o->y = y;
	repaint(o->gw);
}

GPoint getLocation (GObject o) {
	return newGPoint(o->x, o->y);
}

GDimension getSize (GObject o) {
	return newGDimension(o->w, o->h);
}

void setSize (GObject o, double w, double h) {
	o->w = w;
	o->h = h;

	repaint(o->gw);
}

GRectangle getBounds (GObject o) {
	return newGRect(o->x, o->y, o->w, o->h);
}

void setBounds (GObject o, double x, double y, double w, double h) {
	o->x = x;
	o->y = y;
	o->w = w;
	o->h = h;

	repaint(o->gw);
}

void sendBackward (GObject o) {
	GWindow gw = o->gw;
	if (!gw || gw->next == o || !gw->next)
		return;
	GObject tmp = gw;
	while (tmp->next && tmp->next->next != o) {
		tmp = tmp->next;
	}
	if (!tmp->next)
		return;

	GObject x = tmp->next;
	GObject y = o->next;

	// current Situation:  tmp -> x -> o -> y
	// desired Situation:  tmp -> o -> x -> y

	tmp->next = o;
	o->next = x;
	x->next = y;

	repaint(gw);
}

void sendToBack (GObject o) {
	GWindow gw = o->gw;
	if (!gw || gw->next == o) // No GWindow set or already at front
		return;
	GObject tmp = gw;
	while (tmp && tmp->next != o) {
		tmp = tmp->next;
	}
	if (!tmp)
		return; // Object not found, should never happen

	tmp->next = tmp->next->next; // Remove o from list
	o->next = gw->next; // put o to ...
	gw->next = o;       // ... start of list

	repaint(gw);
}


void sendForward (GObject o) {
	GWindow gw = o->gw;
	if (!gw || !o->next) // No GWindow set or already at back
		return;
	GObject tmp = gw;
	while (tmp && tmp->next != o) {
		tmp = tmp->next;
	}
	if (!tmp) // Object not found, should never happen
		return;
	
	GObject x = o->next;
	GObject y = x->next;

	// current Situation:  tmp -> o -> x -> y
	// desired Situation:  tmp -> x -> o -> y

	tmp->next = x;
	x->next = o;
	o->next = y;

	repaint(gw);
}

void sendToFront (GObject o) {
	GWindow gw = o->gw;
	if (!gw || !o->next) // No GWindow set or already at back
		return;
	GObject tmp = gw;
	while (tmp && tmp->next != o) {
		tmp = tmp->next;
	}
	if (!tmp) // Object not found, should never happen
		return;

	tmp->next = tmp->next->next; // Remove o from list
	while (tmp->next)  // Find end of list
		tmp = tmp->next;
	tmp->next = o; // Append object at end of list
	o->next = NULL; // and mark it as last object in list

	repaint(gw);
}

bool contains (GObject o, double x, double y) {
	switch (o->type) {
		case T_WINDOW:
		case T_RECT:
			return rect_contains(o, x, y);
			break;
		case T_OVAL:
			return oval_contains(o, x, y);
			break;
		default:
			return 0;
	}
}

GWindow wins;

GWindow newGWindow(double w, double h) {
	GWindow gw = newGObject();

	gw->gw = wins;
	wins = gw;

	gw->type = T_WINDOW;
	gw->w = w;
	gw->h = h;
	gw->win = SDL_CreateWindow("SPL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
	gw->rend = SDL_CreateRenderer(gw->win, -1, SDL_RENDERER_ACCELERATED);
	gw->bg = SDL_CreateTexture(gw->rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	clear(gw);

	repaint(gw);

	return gw;
}

void unlinkGWindow (GWindow gw) {
	if (wins == gw)
		wins = wins->gw;
	else {
		GWindow tmp = wins;
		while (tmp && tmp->gw != gw) {
			tmp = tmp->gw;
		}
		if (tmp) {
			tmp->gw = tmp->gw->gw;
		}
	}
}

void setWindowTitle (GWindow gw, string title) {
	SDL_SetWindowTitle(gw->win, title);
}

const char * getWindowTitle (GWindow gw) {
	return SDL_GetWindowTitle(gw->win);
}

void requestFocus (GWindow gw) {
	SDL_SetWindowInputFocus(gw->win);
}

void closeGWindow (GWindow gw) {
	unlinkGWindow(gw);
	SDL_DestroyWindow(gw->win);
}

void clear (GWindow gw) {
	SDL_SetRenderTarget(gw->rend, gw->bg);
	SDL_SetRenderDrawColor(gw->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(gw->rend);
	SDL_SetRenderTarget(gw->rend, NULL);
}

void removeGWindow (GWindow gw, GObject o) {
	if (gw->next == o) {
		gw->next = gw->next->next;
	} else {
		GObject ob = gw->next;
		while (ob && ob->next != o) {
			ob = ob->next;
		}
		if (ob) {
			ob->next = ob->next->next;
		}
	}
	o->next = NULL;
	o->gw = NULL;

	repaint(gw);
}

GObject getGObjectAt (GWindow gw, double x, double y) {
	GObject ob = gw->next;
	GObject found = NULL;
	while (ob) {
		if (contains(ob, x, y))
			found = ob;
		ob = ob->next;
	}

	return found;
}

void pause (double ms) {
	int st;
	
	do {
		st = sdl_handle_events(0);
	} while (st);
	SDL_Delay(ms);
}


/* Event handling */


GEvent event_queue;

void freeEvent (GEvent e) {
	free(e->actioncommand);
	free(e);
}

static GEvent getEvent (int mask, int block) {
	sdl_handle_events(block);
	while (event_queue) { 
		GEvent ret = event_queue;
		event_queue = event_queue->next;
		if (ret->eclass & mask) {
			return ret;
		}
	}

	return NULL;
}


GEvent getNextEvent (int mask) {
	return getEvent(mask, 0);
}

GEvent waitForEvent (int mask) {
	GEvent e;
	int block = 0;
	do {
		e = getEvent (mask, block);
		if (!e)
			block = 1;
	} while (!e);

	return e;
}

void waitForClick(void) {
	waitForEvent(CLICK_EVENT);
}


EventClassType getEventClass (GEvent e) {
	return e->eclass;
}

EventType getEventType (GEvent e) {
	return e->etype;
}

GWindow getGWindow (GEvent e) {
	return e->gw;
}

double getEventTime (GEvent e) {
	return e->ts;
}

void setEventTime (GEvent e, double time) {
	e->ts = time;
}

int getModifiers(GEvent e) {
	return e->modifiers;
}

void setModifiers(GEvent e, int modifiers) {
	e->modifiers = modifiers;
}

GEvent newEvent (EventClassType c) {
	GEvent e = newGObject();
	e->type = T_EVENT;
	e->eclass = c;
	struct timeval t;
	gettimeofday(&t, NULL);
	setEventTime(e, t.tv_sec + t.tv_usec/1000);

	if (!event_queue) {
		event_queue = e;
	} else {
		GEvent tmp = event_queue;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = e;
	}

	return e;
}

GWindowEvent newGWindowEvent (EventType type, GWindow gw) {
	GEvent e = newEvent(WINDOW_EVENT);
	e->etype = type;
	e->gw = gw;

	return e;
}

GActionEvent newGActionEvent (EventType type, GObject source, string actionCommand) {
	GEvent e = newEvent(ACTION_EVENT);
	e->etype = type;
	e->source = source;
	e->actioncommand = actionCommand;

	return e;
}

GObject getSource (GEvent e) {
	return e->source;
}

string getActionCommand(void *arg) {
	GEvent e = arg;
	return e->actioncommand;
}

GActionEvent newGMouseEvent (EventType type, GWindow gw, double x, double y) {
	GEvent e = newEvent(MOUSE_EVENT);
	e->etype = type;
	e->gw = gw;
	e->x = x;
	e->y = y;

	return e;
}

GActionEvent newGKeyEvent (EventType type, GWindow gw, int keyChar, int keyCode) {
	GEvent e = newEvent(KEY_EVENT);
	e->etype = type;
	e->gw = gw;
	e->keychar = keyChar;
	e->keycode = keyCode;

	return e;
}

char getKeyChar(GKeyEvent e) {
	return e->keychar;
}

int getKeyCode(GKeyEvent e) {
	return e->keycode;
}


/* SDL Event handling */

GWindow sdl_get_window_by_id (Uint32 windowID) {
	GWindow gw = wins;

	while (gw && SDL_GetWindowID(gw->win) != windowID) {
		gw = gw->gw;
	}

	return gw;
}

void sdl_handle_window_event (SDL_WindowEvent *e) {
	GWindow gw = sdl_get_window_by_id(e->windowID);

	if (!gw)
		return;

	switch (e->event) {
		case SDL_WINDOWEVENT_CLOSE:
			newGWindowEvent(WINDOW_CLOSED, gw);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			newGWindowEvent(WINDOW_RESIZED, gw);
			int _w, _h;
			SDL_GetWindowSize(gw->win, &_w, &_h);
			gw->w = _w;
			gw->h = _h;
			break;
	}
}

void sdl_handle_quit_event (SDL_QuitEvent *e) {
	GWindow gw = wins;

	exit(0);

	if (!gw)
		return;

	newGWindowEvent(WINDOW_CLOSED, gw);
}

void sdl_handle_mousebutton_event(SDL_MouseButtonEvent *e) {
	GWindow gw = sdl_get_window_by_id(e->windowID);
	switch (e->type) {
		case SDL_MOUSEBUTTONDOWN:
			newGMouseEvent(MOUSE_PRESSED, gw, e->x, e->y);
			break;
		case SDL_MOUSEBUTTONUP:
			newGMouseEvent(MOUSE_RELEASED, gw, e->x, e->y);
			GEvent event = newGMouseEvent(MOUSE_CLICKED, gw, e->x, e->y);
			event->eclass |= CLICK_EVENT;
			break;
	}
}

void sdl_handle_mousemotion_event(SDL_MouseMotionEvent *e) {
	GWindow gw = sdl_get_window_by_id(e->windowID);
	if (e->state == SDL_PRESSED) {
			newGMouseEvent(MOUSE_DRAGGED, gw, e->x, e->y);
	} else {
			newGMouseEvent(MOUSE_MOVED, gw, e->x, e->y);
	}
}

void sdl_handle_key_event(SDL_KeyboardEvent *e) {
	GWindow gw = sdl_get_window_by_id(e->windowID);
	int ch = e->keysym.sym;
	switch (e->state) {
		case SDL_PRESSED:
			if (!e->repeat)
				newGKeyEvent(KEY_PRESSED, gw, ch, ch);
			newGKeyEvent(KEY_TYPED, gw, ch, ch);
			break;
		case SDL_RELEASED:
			newGKeyEvent(KEY_RELEASED, gw, ch, ch);
			break;
	}
}

int  sdl_handle_events (int block) {
	SDL_Event event;
	int st = block ? SDL_WaitEvent(&event) : SDL_PollEvent(&event);
	if (st == 0) return 0;
	switch (event.type) {
		case SDL_WINDOWEVENT:
			sdl_handle_window_event(&event.window);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			sdl_handle_mousebutton_event(&event.button);
			break;
		case SDL_MOUSEMOTION:
			sdl_handle_mousemotion_event(&event.motion);
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			sdl_handle_key_event(&event.key);
			break;
		case SDL_QUIT:
			sdl_handle_quit_event(&event.quit);
			break;
	}
	return 1;
}

#undef main

int main_(int argc, char **argv);

int main (int argc, char **argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF|IMG_INIT_WEBP);
	TTF_Init();

	main_(argc, argv);

	return 0;
}




// Filled Polygons

#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


static float cross(Point a, Point b, Point c)
{
    return (b.x - a.x) * (c.y - a.y)
         - (b.y - a.y) * (c.x - a.x);
}

static bool pointInTriangle(Point p, Point a, Point b, Point c)
{
    float c1 = cross(a, b, p);
    float c2 = cross(b, c, p);
    float c3 = cross(c, a, p);

    bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(hasNeg && hasPos);
}

static float polygonArea(const Point *p, int n)
{
    float area = 0.0f;

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += p[i].x * p[j].y;
        area -= p[j].x * p[i].y;
    }

    return area * 0.5f;
}

int drawFilledPolygon(
    SDL_Renderer *renderer,
    const Point *points,
    int count,
    SDL_Color color)
{
    if (count < 3)
        return false;

    /*
     * Ear clipping expects a consistent winding order.
     * Make our working index list counter-clockwise.
     */
    int *indices = malloc(sizeof(int) * count);
    if (!indices)
        return false;

    if (polygonArea(points, count) > 0) {
        for (int i = 0; i < count; ++i)
            indices[i] = i;
    } else {
        for (int i = 0; i < count; ++i)
            indices[i] = count - 1 - i;
    }

    /*
     * At most count - 2 triangles.
     */
    SDL_Vertex *vertices =
        malloc(sizeof(SDL_Vertex) * (count - 2) * 3);

    if (!vertices) {
        free(indices);
        return false;
    }

    int remaining = count;
    int triangleCount = 0;

    while (remaining > 3) {
        bool foundEar = false;

        for (int i = 0; i < remaining; ++i) {
            int prev = indices[(i + remaining - 1) % remaining];
            int curr = indices[i];
            int next = indices[(i + 1) % remaining];

            Point a = points[prev];
            Point b = points[curr];
            Point c = points[next];

            /*
             * Must be convex.
             */
            if (cross(a, b, c) <= 0)
                continue;

            /*
             * Make sure no other polygon vertex lies
             * inside this candidate triangle.
             */
            bool containsPoint = false;

            for (int j = 0; j < remaining; ++j) {
                int v = indices[j];

                if (v == prev || v == curr || v == next)
                    continue;

                if (pointInTriangle(points[v], a, b, c)) {
                    containsPoint = true;
                    break;
                }
            }

            if (containsPoint)
                continue;

            /*
             * We found an ear.
             */
            SDL_Vertex *v = &vertices[triangleCount * 3];

            v[0].position.x = a.x;
            v[0].position.y = a.y;

            v[1].position.x = b.x;
            v[1].position.y = b.y;

            v[2].position.x = c.x;
            v[2].position.y = c.y;

            for (int k = 0; k < 3; ++k) {
                v[k].color = color;
                v[k].tex_coord.x = 0;
                v[k].tex_coord.y = 0;
            }

            triangleCount++;

            /*
             * Remove the ear from the polygon.
             */
            for (int j = i; j < remaining - 1; ++j)
                indices[j] = indices[j + 1];

            remaining--;
            foundEar = true;
            break;
        }

        /*
         * Usually means the polygon is malformed,
         * self-intersecting, or has problematic duplicate
         * / collinear vertices.
         */
        if (!foundEar) {
            free(vertices);
            free(indices);
            return false;
        }
    }

    /*
     * Final triangle.
     */
    if (remaining == 3) {
        Point a = points[indices[0]];
        Point b = points[indices[1]];
        Point c = points[indices[2]];

        SDL_Vertex *v = &vertices[triangleCount * 3];

        v[0].position.x = a.x;
        v[0].position.y = a.y;

        v[1].position.x = b.x;
        v[1].position.y = b.y;

        v[2].position.x = c.x;
        v[2].position.y = c.y;

        for (int k = 0; k < 3; ++k) {
            v[k].color = color;
            v[k].tex_coord.x = 0;
            v[k].tex_coord.y = 0;
        }

        triangleCount++;
    }

    int result = SDL_RenderGeometry(
        renderer,
        NULL,
        vertices,
        triangleCount * 3,
        NULL,
        0
    );

    free(vertices);
    free(indices);

    return result == 0;
}




#define MAX_MESSAGE 1024

void error(string msg, ...) {
   va_list args;
   char errbuf[MAX_MESSAGE + 1];

   va_start(args, msg);
   vsnprintf(errbuf, MAX_MESSAGE, msg, args);
   va_end(args);
   fprintf(stderr, "error: %s\n", msg);
}

void unhandledError(string msg) {
   fprintf(stderr, "error: %s\n", msg);
   exit(EXIT_FAILURE);
}

#define newArray(size, type) malloc(size)
#define getBlock(size) malloc(size)
#define freeBlock(buf) free(buf)

/*
 * File: simpio.c
 * --------------
 * This file implements the simpio.h interface.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (C) 2013 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "cslib.h"
#include "simpio.h"
#include "strlib.h"

/*
 * Constants:
 * ----------
 * INITIAL_BUFFER_SIZE -- Initial buffer size
 */

#define INITIAL_BUFFER_SIZE 128

/* Exported entries */

/*
 * Functions: getInteger, getLong, getReal
 * ---------------------------------------
 * These functions first read a line and then call sscanf to
 * translate the number.  Reading an entire line is essential to
 * good error recovery, because the characters after the point of
 * error would otherwise remain in the input buffer and confuse
 * subsequent input operations.  The sscanf line allows white space
 * before and after the number but no other extraneous characters.
 */

int getInteger(void) {
   string line;
   int value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getInteger: unexpected end of file");
      switch (sscanf(line, " %d %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter an integer\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

long getLong(void) {
   string line;
   long value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getLong: unexpected end of file");
      switch (sscanf(line, " %ld %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter an integer\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

double getReal(void) {
   string line;
   double value;
   char termch;

   while (true) {
      line = getLine();
      if (line == NULL) error("getReal: unexpected end of file");
      switch (sscanf(line, " %lf %c", &value, &termch)) {
        case 1:
          freeBlock(line);
          return value;
        case 2:
          printf("Unexpected character: '%c'\n", termch);
          break;
        default:
          printf("Please enter a real number\n");
          break;
      }
      freeBlock(line);
      printf("Retry: ");
   }
}

/*
 * Function: getLine
 * -----------------
 * This function is a simple wrapper; all the work is done by
 * readLine.
 */

string getLine(void) {
   return readLine(stdin);
}

/*
 * Function: readLine
 * ------------------
 * This function operates by reading characters from the file
 * into a dynamically allocated buffer.  If the buffer becomes
 * full before the end of the line is reached, a new buffer
 * twice the size of the previous one is allocated.  This
 * function works correctly with any of the standard newline
 * sequences: "\n", "\r", or "\r\n".
 */

string readLine(FILE *infile) {
   string line, nline;
   int n, ch, size;

   n = 0;
   size = INITIAL_BUFFER_SIZE;
   line = (string) getBlock(size + 1);
   while (true) {
      ch = getc(infile);
      if (ch == '\n' || ch == EOF) break;
      if (ch == '\r') {
         ch = getc(infile);
         if (ch != '\n') ungetc(ch, infile);
         break;
      }
      if (n == size) {
         size *= 2;
         nline = (string) getBlock(size + 1);
         strncpy(nline, line, n);
         freeBlock(line);
         line = nline;
      }
      line[n++] = ch;
   }
   if (n == 0 && ch == EOF) {
      freeBlock(line);
      return NULL;
   }
   line[n] = '\0';
   nline = (string) getBlock(n + 1);
   strcpy(nline, line);
   freeBlock(line);
   return nline;
}

/*
 * Function: readLinesFromStream
 * -----------------------------
 * This function operates by reading lines from the file into a
 * dynamically allocated buffer that doubles in size whenever the
 * existing space is exhausted.
 */


string *readLinesFromStream(FILE *infile) {
   string *buffer, *nbuffer, line;
   int i, n, size;

   n = 0;
   size = INITIAL_BUFFER_SIZE;
   buffer = newArray(size, string);
   while (true) {
      line = readLine(infile);
      if (line == NULL) break;
      if (n == size) {
         size *= 2;
         nbuffer = newArray(size, string);
         for (i = 0; i < n; i++) {
            nbuffer[i] = buffer[i];
         }
         freeBlock(buffer);
         buffer = nbuffer;
      }
      buffer[n++] = line;
   }
   nbuffer = newArray(n + 1, string);
   for (i = 0; i < n; i++) {
      nbuffer[i] = buffer[i];
   }
   nbuffer[n] = NULL;
   freeBlock(buffer);
   return nbuffer;
}

/*
 * Function: readLinesFromFile
 * ---------------------------
 * This function is a wrapper for <code>readLinesFromStream</code>,
 * which does all the real work.
 */

string *readLinesFromFile(string filename) {
   FILE *infile;
   string *lines;

   if (stringEqual(filename, "-")) {
      infile = stdin;
   } else {
      infile = fopen(filename, "r");
      if (infile == NULL) error("Can't open %s", filename);
   }
   lines = readLinesFromStream(infile);
   if (infile != stdin) fclose(infile);
   return lines;
}
/*
 * File: strlib.c
 * --------------
 * This file implements the strlib.h interface.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (C) 2013 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

/*
 * General implementation notes:
 * -----------------------------
 * This module implements the strlib library by mapping all
 * functions into the appropriate calls to the ANSI <string.h>
 * interface.  The implementations of the individual functions
 * are all quite simple and do not require individual comments.
 * For descriptions of the behavior of each function, see the
 * interface.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cslib.h"
#include "strlib.h"

/* Constants */

#define MAX_NUMBER_DIGITS 30

/* Private function prototypes */

static string createString(int len);

/* Section 1 -- Basic string operations */

string concat(string s1, string s2) {
   string s;
   int len1, len2;

   if (s1 == NULL || s2 == NULL) {
      error("concat: NULL string passed as an argument");
   }
   len1 = strlen(s1);
   len2 = strlen(s2);
   s = createString(len1 + len2);
   strcpy(s, s1);
   strcpy(s + len1, s2);
   return s;
}

char charAt(string s, int i) {
   int len;

   if (s == NULL) error("charAt: String value is NULL");
   len = strlen(s);
   if (i < 0 || i > len) {
      error("charAt: Index is out of range");
   }
   return s[i];
}

string substring(string s, int p1, int p2) {
   int len;
   string result;

   if (s == NULL) error("substring: String value is NULL");
   len = strlen(s);
   if (p1 < 0) p1 = 0;
   if (p2 >= len) p2 = len - 1;
   len = p2 - p1 + 1;
   if (len < 0) len = 0;
   result = createString(len);
   strncpy(result, s + p1, len);
   result[len] = '\0';
   return result;
}

string charToString(char ch) {
   string result;

   result = createString(1);
   result[0] = ch;
   result[1] = '\0';
   return result;
}

int stringLength(string s) {
   if (s == NULL) error("stringLength: String value is NULL");
   return strlen(s);
}

string copyString(string s) {
   string newstr;

   if (s == NULL) error("copyString: String value is NULL");
   newstr = createString(strlen(s));
   strcpy(newstr, s);
   return newstr;
}

/* Section 2 -- String comparison functions */

bool stringEqual(string s1, string s2) {
   if (s1 == NULL || s2 == NULL) {
      error("stringEqual: String value is NULL");
   }
   return strcmp(s1, s2) == 0;
}

bool stringEqualIgnoreCase(string s1, string s2) {
   int i;

   if (s1 == NULL || s2 == NULL) {
      error("stringEqualIgnoreCase: String value is NULL");
   }
   for (i = 0; s1[i] != '\0'; i++) {
      if (tolower(s1[i]) != tolower(s2[i])) return false;
   }
   return s2[i] == '\0';
}

int stringCompare(string s1, string s2) {
   int cmp;

   if (s1 == NULL || s2 == NULL) {
      error("stringCompare: String value is NULL");
   }
   cmp = strcmp(s1, s2);
   if (cmp == 0) return 0;
   return (cmp < 0) ? -1 : +1;
}

bool startsWith(string s1, string s2) {
   return strncmp(s1, s2, strlen(s2)) == 0;
}

bool endsWith(string s1, string s2) {
   int n1, n2;

   n1 = strlen(s1);
   n2 = strlen(s2);
   if (n2 > n1) return false;
   return strcmp(s1 + n1 - n2, s2) == 0;
}

/* Section 3 -- Search functions */

int findChar(char ch, string text, int start) {
   char *cptr;

   if (text == NULL) error("findChar: String value is NULL");
   if (start < 0) start = 0;
   if (start > strlen(text)) return -1;
   cptr = strchr(text + start, ch);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findString(string str, string text, int start) {
   char *cptr;

   if (str == NULL) error("findString: String value is NULL");
   if (text == NULL) error("findString: String value is NULL");
   if (start < 0) start = 0;
   if (start > strlen(text)) return -1;
   cptr = strstr(text + start, str);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findLastChar(char ch, string text) {
   char *cptr;

   if (text == NULL) error("findLastChar: String value is NULL");
   cptr = strrchr(text, ch);
   if (cptr == NULL) return -1;
   return (int) (cptr - text);
}

int findLastString(string str, string text) {
   int i, nc;

   if (str == NULL) error("findLastString: String value is NULL");
   if (text == NULL) error("findLastString: String value is NULL");
   nc = strlen(str);
   for (i = strlen(text) - nc; i >= 0; i--) {
      if (strncmp(str, text + i, nc) == 0) return i;
   }
   return -1;
}

/* Section 4 -- Conversion functions */

string toLowerCase(string s) {
   string result;
   int i;

   if (s == NULL) {
      error("toLowerCase: String value is NULL");
   }
   result = createString(strlen(s));
   for (i = 0; s[i] != '\0'; i++) {
      result[i] = tolower(s[i]);
   }
   result[i] = '\0';
   return result;
}

string toUpperCase(string s) {
   string result;
   int i;

   if (s == NULL) {
      error("toUpperCase: String value is NULL");
   }
   result = createString(strlen(s));
   for (i = 0; s[i] != '\0'; i++) {
      result[i] = toupper(s[i]);
   }
   result[i] = '\0';
   return result;
}

string integerToString(int n) {
   char buffer[MAX_NUMBER_DIGITS];

   sprintf(buffer, "%d", n);
   return copyString(buffer);
}

int stringToInteger(string s) {
   int result;
   char dummy;

   if (s == NULL) {
      error("stringToInteger: String value is NULL");
   }
   if (sscanf(s, " %d %c", &result, &dummy) != 1) {
      error("stringToInteger: Illegal number %s", s);
   }
   return result;
}

/*
 * Implementation notes: realToString
 * ----------------------------------
 * Some implementations of printf pad the exponent field to three
 * digits, but all seem to pad it to at least two.  The string
 * manipulation code in the function definition standardizes on
 * the two-digit form.
 */

string realToString(double d) {
   char buffer[MAX_NUMBER_DIGITS];
   int len;

   sprintf(buffer, "%G", d);
   len = strlen(buffer);
   if (len > 5 && buffer[len - 5] == 'E' && buffer[len - 3] == '0') {
      buffer[len - 3] = buffer[len - 2];
      buffer[len - 2] = buffer[len - 1];
      buffer[len - 1] = '\0';
   }
   return copyString(buffer);
}

double stringToReal(string s) {
   double result;
   char dummy;

   if (s == NULL) error("stringToReal: String value is NULL");
   if (sscanf(s, "%lf %c", &result, &dummy) != 1) {
      error("stringToReal: Illegal number %s", s);
   }
   return result;
}

string trim(string str) {
   int start, finish;

   start = 0;
   while (str[start] != '\0' && isspace(str[start])) {
      start++;
   }
   finish = strlen(str) - 1;
   while (finish >= start && isspace(str[finish])) {
      finish--;
   }
   return substring(str, start, finish);
}

string quoteString(string str) {
   int i, n;
   string result;
   char ch, *cp;

   n = 2;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '\a': case '\b': case '\f': case '\n': case '\r':
       case '\t': case '\v': case '"': case '\\':
         n += 2;
         break;
       default:
         n += (ch < 32 || ch >= 127) ? 4 : 1;
         break;
      }
   }
   result = newArray(n + 1, char);
   cp = result;
   *cp++ = '"';
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '\a': *cp++ = '\\'; *cp++ = 'a'; break;
       case '\b': *cp++ = '\\'; *cp++ = 'b'; break;
       case '\f': *cp++ = '\\'; *cp++ = 'f'; break;
       case '\n': *cp++ = '\\'; *cp++ = 'n'; break;
       case '\r': *cp++ = '\\'; *cp++ = 'r'; break;
       case '\t': *cp++ = '\\'; *cp++ = 't'; break;
       case '\v': *cp++ = '\\'; *cp++ = 'v'; break;
       case '"': *cp++ = '\\'; *cp++ = '"'; break;
       case '\\': *cp++ = '\\'; *cp++ = '\\'; break;
       default:
         if (ch < 32 || ch >= 127) {
            sprintf(cp, "\\%03o", ch);
            cp += 4;
         } else {
            *cp++ = ch;
         }
         break;
      }
   }
   *cp++ = '"';
   *cp = '\0';
   return result;
}

string quoteHTML(string str) {
   int i, n;
   string result;
   char ch, *cp;

   n = 0;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '&': n += 5; break;
       case '<': n += 4; break;
       case '>': n += 4; break;
       default: n++; break;
      }
   }
   result = newArray(n + 1, char);
   cp = result;
   for (i = 0; (ch = str[i]) != '\0'; i++) {
      switch (ch) {
       case '&': strcpy(cp, "&amp;"); cp += 5; break;
       case '<': strcpy(cp, "&lt;"); cp += 4; break;
       case '>': strcpy(cp, "&gt;"); cp += 4; break;
       default: *cp++ = ch; break;
      }
   }
   *cp = '\0';
   return result;
}

int stringArrayLength(string array[]) {
   int i;

   if (array == NULL) return 0;
   for (i = 0; array[i] != NULL; i++) {
      /* Empty */
   }
   return i;
}

int searchStringArray(string str, string array[]) {
   int i;

   if (array == NULL) return -1;
   for (i = 0; array[i] != NULL; i++) {
      if (stringEqual(str, array[i])) return i;
   }
   return -1;
}

/*
 * Private function: createString
 * Usage: s = createString(len);
 * -----------------------------
 * This function dynamically allocates space for a string of
 * len characters, leaving room for the null character at the
 * end.
 */

static string createString(int len) {
   return (string) getBlock(len + 1);
}

/*
 * File: random.c
 * --------------
 * This file implements the random.h interface.
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (C) 2013 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cslib.h"
#include "random.h"

/* Private function prototype */

static void initRandomSeed(void);

/*
 * Implementation notes: randomInteger
 * -----------------------------------
 * The code for randomInteger produces the number in four steps:
 *
 * 1. Generate a random real number d in the range [0 .. 1).
 * 2. Scale the number to the range [0 .. N) where N is the number of values.
 * 3. Translate the number so that the range starts at the appropriate value.
 * 4. Convert the result to the next lower integer.
 *
 * The implementation is complicated by the fact that both the expression
 *
 *     RAND_MAX + 1
 *
 * and the expression for the number of values
 *
 *     high - low + 1
 *
 * can overflow the integer range.  These calculations must therefore be
 * performed using doubles instead of ints.
 */

int randomInteger(int low, int high) {
   double d, s;

   initRandomSeed();
   d = rand() / ((double) RAND_MAX + 1);
   s = d * ((double) high - low + 1);
   return (int) (floor(low + s));
}

/*
 * Implementation notes: randomReal
 * --------------------------------
 * The code for randomReal is similar to that for randomInteger,
 * without the final conversion step.
 */

double randomReal(double low, double high) {
   double d, s;

   initRandomSeed();
   d = rand() / ((double) RAND_MAX + 1);
   s = d * (high - low);
   return low + s;
}

/*
 * Implementation notes: randomChance
 * ----------------------------------
 * The code for randomChance calls randomReal(0, 1) and then checks
 * whether the result is less than the requested probability.
 */

bool randomChance(double p) {
   initRandomSeed();
   return randomReal(0, 1) < p;
}

/*
 * Implementation notes: setRandomSeed
 * -----------------------------------
 * The setRandomSeed function simply forwards its argument to srand.
 * The call to initRandomSeed is required to set the initialized flag.
 */

void setRandomSeed(int seed) {
   initRandomSeed();
   srand(seed);
}

/*
 * Implementation notes: initRandomSeed
 * ------------------------------------
 * The initRandomSeed function declares a static variable that keeps track
 * of whether the seed has been initialized.  The first time initRandomSeed
 * is called, initialized is false, so the seed is set to the current time.
 */

static void initRandomSeed(void) {
   static bool initialized = false;
   if (!initialized) {
      srand((int) time(NULL));
      initialized = true;
   }
}

