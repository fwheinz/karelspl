#include <sys/time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "spl.h"

const int false = 0, true = 1;

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

void render_polygon_sdl (SDL_Renderer *rend, GObject o) {
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

	if (!gw) return;
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
