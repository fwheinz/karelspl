#ifndef _SPL_H
#define _SPL_H

typedef char * string;
typedef int bool;

extern const int false, true;

typedef struct ObjectCDT * GObject;
typedef struct ObjectCDT * GOval;
typedef struct ObjectCDT * GRect;
typedef struct ObjectCDT * GRoundRect;
typedef struct ObjectCDT * G3DRect;
typedef struct ObjectCDT * GRectangle;
typedef struct ObjectCDT * GLine;
typedef struct ObjectCDT * GWindow;
typedef struct ObjectCDT * GImage;
typedef struct ObjectCDT * GArc;
typedef struct ObjectCDT * GLabel;
typedef struct ObjectCDT * GPolygon;

typedef struct ObjectCDT * GPoint;
typedef struct ObjectCDT * GDimension;
typedef struct ObjectCDT * GRectangle;


/* gwindow.h */

GWindow newGWindow(double w, double h);
void closeGWindow (GWindow gw);
void requestFocus (GWindow gw);
void clear (GWindow gw);
void setVisible (GObject o, int visible);
bool isVisible (GObject o);

void drawLine (GWindow gw, double x1, double y1, double x2, double y2);
void drawPolarLine (GWindow gw, double x1, double y1, double r, double theta);
void drawOval (GWindow gw, double x, double y, double w, double h);
void fillOval (GWindow gw, double x, double y, double w, double h);
void drawRect (GWindow gw, double x, double y, double w, double h);
void fillRect (GWindow gw, double x, double y, double w, double h);

void setColor (GObject o, char *name);
void setColorGWindow (GObject o, char *name);
char * getColorGWindow(GWindow gw);
double getWidth (GObject o);
double getHeight (GObject o);
void repaint (GWindow gw);
void setWindowTitle(GWindow gw, string title);
const char * getWindowTitle(GWindow gw);

void draw (GWindow gw, GObject o);
void drawAt (GWindow gw, GObject o, double x, double y);
void add (GWindow gw, GObject o);
void addAt (GWindow gw, GObject o, double x, double y);
void addToRegion(GWindow gw, GObject gobj, string region); // not implemented!
void removeGWindow (GWindow gw, GObject o);
GObject getGObjectAt(GWindow gw, double x, double y);
void setRegionAlignment(GWindow gw, string region, string align);
void pause (double ms);


/* gobjects.h */

void freeGObject(GObject o);
double getX (GObject o);
double getY (GObject o);
GPoint getLocation (GObject o);
void setLocation (GObject o, double x, double y);
void move (GObject o, double x, double y);
double getWidth (GObject o);
double getHeight (GObject o);
GDimension getSize (GObject o);
GRectangle getBounds (GObject o);
void setColor (GObject o, char *name);
char * getColorGObject(GObject o);
void setVisible (GObject o, int visible);
bool isVisible (GObject o);
void sendForward(GObject o);
void sendToFront(GObject o);
void sendBackward(GObject o);
void sendToBack(GObject o);
bool contains(GObject o, double x, double y);
string getType (GObject o);
void setSize (GObject o, double w, double h);
void setBounds (GObject o, double x, double y, double w, double h);
void setFilled (GObject o, int filled);
bool isFilled (GObject o);
void setFillColor (GObject o, string color);
string getFillColor (GObject o);
GRect newGRect (double x, double y, double w, double h);
GRoundRect newGRoundRect (double x, double y, double w, double h, double corner); // Unimplemented; renders as regular rectangle
G3DRect newG3DRect (double x, double y, double w, double h, bool raised); // Unimplemented; renders as reguler rectangle
void setRaised(G3DRect rect, bool raised);
bool isRaised(G3DRect rect);
GOval newGOval (double x, double y, double w, double h);
GLine newGLine (double x1, double y1, double x2, double y2);
void setStartPoint(GLine line, double x, double y);
void setEndPoint(GLine line, double x, double y);
GPoint getStartPoint(GObject gobj);
GPoint getEndPoint(GObject gobj);
GArc newGArc(double x, double y, double width, double height, double start, double sweep);
void setStartAngle(GArc arc, double start);
double getStartAngle(GArc arc);
void setSweepAngle(GArc arc, double start);
double getSweepAngle(GArc arc);
void setFrameRectangle(GArc garc, double x, double y, double width, double height);
GRectangle getFrameRectangle(GArc arc);
GLabel newGLabel(string str);
void setFont(GLabel label, string font);
string getFont(GLabel label);
void setLabel(GLabel label, string str);
string getLabel(GLabel label);
double getFontAscent(GLabel label);
double getFontDescent(GLabel label);
GImage newGImage(char *filename);
void setRotation (GImage o, double a);
double getRotation (GImage o);
GPolygon newGPolygon(void); // Does not work filled yet
void addVertex(GPolygon poly, double x, double y);
void addEdge(GPolygon poly, double dx, double dy);
void addPolarEdge(GPolygon poly, double r, double theta);
//Vector getVertices(GPolygon poly);



typedef enum {
   ACTION_EVENT = 0x010,
   KEY_EVENT    = 0x020,
   TIMER_EVENT  = 0x040,
   WINDOW_EVENT = 0x080,
   MOUSE_EVENT  = 0x100,
   CLICK_EVENT  = 0x200,
   ANY_EVENT    = 0x3F0
} EventClassType;

typedef enum {
   WINDOW_CLOSED    = WINDOW_EVENT + 1,
   WINDOW_RESIZED   = WINDOW_EVENT + 2,
   ACTION_PERFORMED = ACTION_EVENT + 1,
   MOUSE_CLICKED    = MOUSE_EVENT + 1,
   MOUSE_PRESSED    = MOUSE_EVENT + 2,
   MOUSE_RELEASED   = MOUSE_EVENT + 3,
   MOUSE_MOVED      = MOUSE_EVENT + 4,
   MOUSE_DRAGGED    = MOUSE_EVENT + 5,
   KEY_PRESSED      = KEY_EVENT + 1,
   KEY_RELEASED     = KEY_EVENT + 2,
   KEY_TYPED        = KEY_EVENT + 3,
   TIMER_TICKED     = TIMER_EVENT + 1
} EventType;

typedef enum {
   SHIFT_DOWN     = 1 << 0,
   CTRL_DOWN      = 1 << 1,
   META_DOWN      = 1 << 2,
   ALT_DOWN       = 1 << 3,
   ALT_GRAPH_DOWN = 1 << 4,
   BUTTON1_DOWN   = 1 << 5,
   BUTTON2_DOWN   = 1 << 6,
   BUTTON3_DOWN   = 1 << 7
} ModifierCodes;

typedef enum {
   BACKSPACE_KEY = 8,
   TAB_KEY = 9,
   ENTER_KEY = 10,
   CLEAR_KEY = 12,
   ESCAPE_KEY = 27,
   PAGE_UP_KEY = 33,
   PAGE_DOWN_KEY = 34,
   END_KEY = 35,
   HOME_KEY = 36,
   LEFT_ARROW_KEY = 37,
   UP_ARROW_KEY = 38,
   RIGHT_ARROW_KEY = 39,
   DOWN_ARROW_KEY = 40,
   F1_KEY = 112,
   F2_KEY = 113,
   F3_KEY = 114,
   F4_KEY = 115,
   F5_KEY = 116,
   F6_KEY = 117,
   F7_KEY = 118,
   F8_KEY = 119,
   F9_KEY = 120,
   F10_KEY = 121,
   F11_KEY = 122,
   F12_KEY = 123,
   DELETE_KEY = 127,
   HELP_KEY = 156
} KeyCodes;

typedef struct ObjectCDT * GEvent;
typedef GEvent GWindowEvent;
typedef GEvent GActionEvent;
typedef GEvent GMouseEvent;
typedef GEvent GKeyEvent;
typedef GEvent GTimerEvent;

void freeEvent(GEvent e);
EventClassType getEventClass(GEvent e);
EventType getEventType(GEvent e);
GWindow getGWindow(GEvent e);
double getEventTime(GEvent e);
void setEventTime(GEvent e, double time);
int getModifiers(GEvent e);
void setModifiers(GEvent e, int modifiers);
void waitForClick();
GEvent waitForEvent(int mask);
GEvent getNextEvent(int mask);
GWindowEvent newGWindowEvent(EventType type, GWindow gw);
GActionEvent newGActionEvent(EventType type, GObject source, string actionCommand);
GObject getSource(GActionEvent e);
string getActionCommand(void *arg);
GMouseEvent newGMouseEvent(EventType type, GWindow gw, double x, double y);
double getX(GMouseEvent e);
double getY(GMouseEvent e);
GKeyEvent newGKeyEvent(EventType type, GWindow gw, int keyChar, int keyCode);
char getKeyChar(GKeyEvent e);
int getKeyCode(GKeyEvent e);
// GEvent newGTimerEvent(EventType type, GTimer timer);
// GTimer getGTimer(GTimerEvent e);


// Own functions

void setRepaint (GWindow gw, bool do_repaint);
bool getRepaint (GWindow gw);

#define main main_

#endif

