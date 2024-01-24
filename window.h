#ifndef ___SM_LIB_WINDOW
#define ___SM_LIB_WINDOW
#include "GL/freeglut_std.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "point.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "image.h"
#include "color.h"

#define SM_WINDOW_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define SM_WINDOW_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)
#define SM_SCREEN_WIDTH glutGet(GLUT_SCREEN_WIDTH)
#define SM_SCREEN_HEIGHT glutGet(GLUT_SCREEN_HEIGHT)

struct Window;
enum SM_CallbackType {
  DISPLAY,
  MOTION,
  MOUSE,
  KEYBOARD,
  PASSIVE_MOTION,
  SPECIAL,
  RESIZE,
  IDLE,
};
enum SM_ArgID {
  MOUSE_X = 0,
  MOUSE_Y = 1,
  MOUSE_SCROLL = 2,
  MOUSE_BUTTON = 3,
  KEY = 4,
  SPECIAL_KEY = 5,
  RESIZEWIDTH = 6,
  RESIZEHEIGHT = 7,
};
struct EventArgs {
  int args[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  EventArgs(const std::vector<std::pair<SM_ArgID, int>> &v){
    for(int i = 0; i < v.size(); i++){
      args[v[i].first] = v[i].second;
    }
  }
  const int operator[](int i) const { return args[i]; }
};
struct Event {
  Window* origin;
  SM_CallbackType type;
  EventArgs args;
  Event(Window* origin_, SM_CallbackType type_, const EventArgs &args_) : origin(origin_), type(type_), args(args_){};
};
struct Callback {
  SM_CallbackType type;
  void (*func)(const Event &e);
  Callback(SM_CallbackType type_, void(*func_)(const Event &e)) : type(type_), func(func_){};
};
struct Window {
  int ID;
  int width, height;
  Point position, mousePosition;
  std::string title;
  Color bg;
  inline static Window* mainWindow;
  static void initializeContext(int argc = 0, char *argv[] = {}){
    glutInit(&argc, argv);
  }
  Window(int parent, int w, int h, const Point &p){
    width = w;
    height = h;
    position = p;
    unalignPoint(position);
    ID = glutCreateSubWindow(parent, position.x, position.y, w, h);
    instances.push_back(this);
    setupFunctions();
  }
  const Window& operator=(const Window &w){
    return w;
  }
  Window(){}
  Window(int w, int h, const std::string t = "", const Point &p = Point()){
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    width = w;
    height = h;
    position = p;
    title = t;
    glutInitWindowSize(width, height);
    glutInitWindowPosition(position.x, position.y);
    ID = glutCreateWindow(title.c_str());
    glClearColor(0, 0, 0, 1);
    glLoadIdentity();
    glOrtho((float)-width / 2, (float)width / 2, (float)-height / 2, (float)height / 2, -1, 1);
    glDisable(GL_LIGHTING);
    instances.push_back(this);
    setupFunctions();
  }
  bool batch = false;
  void beginDraw(bool enableAlphaBlend = true) { 
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT);
    if(enableAlphaBlend){
      glEnable(GL_BLEND); 
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    }
    batch = false;
  }
  void endDraw(){
    glEnd();
    glFlush();
    batch = false;
  }
  void setColor(const Color &c){ glColor4f(c.r, c.g, c.b, c.a); }
  void beginBatch(GLenum mode){ glBegin(mode); batch = true; }
  void endBatch(){ glEnd(); batch = false; }
  void drawRect(const Point &p1, const Point &p2, bool fill){
    if(!batch){
      fill ? glBegin(GL_QUADS) : glBegin(GL_LINES);
    }
    vertex(p1);
    doubleVertex(Point(p2.x, p1.y));
    doubleVertex(p2);
    doubleVertex(Point(p1.x, p2.y));
    vertex(p1);
  }
  void drawRect(const Point &center, int w, int h, bool fill){ drawRect(Point(center.x - w / 2, center.y + h / 2), Point(center.x + w / 2, center.y - h / 2), fill); }
  void drawLine(const Point &start, const Point &end){ if(!batch) glBegin(GL_LINES); vertex(start); vertex(end); }
  void drawImage(const Point &pos, Image* img, const Point &scale = Point(1, 1)){
    glRasterPos2i(pos.x, pos.y - img->h);
    glPixelZoom(scale.x, scale.y);
    glDrawPixels(img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->raw);
  }
  void drawText(const Point &pos, const std::string &text){
    glRasterPos2i(pos.x, pos.y);
    for(int i = 0; i < text.length(); i++){
      glutBitmapCharacter(&glutBitmapTimesRoman24, text[i]);
    }
  }
  inline const void changeBackground(const Color &c){ glClearColor(c.r, c.g, c.b, c.a); bg = c; } 
  inline const void requestRedraw(){ glutPostWindowRedisplay(ID); }
  inline const void kill(){ glutDestroyWindow(ID); }
  inline const void registerCallback(const Callback &cbk){instances[ID]->callbacks.push_back(cbk);}
  inline const void registerCallback(SM_CallbackType t, void(*func)(const Event &e)){instances[ID]->callbacks.push_back(Callback(t, func));}
  inline void show(){ showWindow(this); }
  inline void hide(){ hideWindow(this); }
  const static Window createSubwindow(int width, int height, int parent = glutGetWindow(), const Point &p = Point()){ return Window(parent, width, height, p); }
  const inline static void showWindow(Window* win){ glutSetWindow(win->ID); glutShowWindow(); }
  const inline static void hideWindow(Window* win){ glutSetWindow(win->ID); glutHideWindow(); }
  const inline static std::thread begin(){ return std::thread(glutMainLoop); }
private:
  inline void vertex(const Point &p){ glVertex2i(p.x, p.y); }
  inline void doubleVertex(const Point &p) { glVertex2i(p.x, p.y); glVertex2i(p.x, p.y); }
  std::vector<Callback> callbacks;
  inline static std::vector<Window*> instances = {nullptr};
  static void staticCallback(SM_CallbackType type, const Event &e){
    int win = glutGetWindow();
    if(instances[win] == nullptr) return;
    for(int i = 0; i < instances[win]->callbacks.size(); i++){
      if(instances[win]->callbacks[i].type == type){
        instances[win]->callbacks[i].func(e);
      }
    }
  }
  const inline static void unalignPoint(Point &p){ p = Point(p.x + mainWindow->width / 2, mainWindow->height - (p.y + mainWindow->height / 2)); }
  const inline static void alignPoint(Point &p){ p = Point(p.x - mainWindow->width / 2, p.y - mainWindow->height / 2); }
  static void updateMousePos(Window* ins, int x, int y){
    ins->mousePosition = Point(x, ins->height - y);
    alignPoint(ins->mousePosition);
  }
  static void _sDisplay(void){ 
    staticCallback(DISPLAY, Event(instances[glutGetWindow()], DISPLAY, EventArgs({}))); 
  }
  static void _sMotion(int x, int y){ 
    Window* ins = instances[glutGetWindow()];
    updateMousePos(ins, x, y);
    staticCallback(MOTION, Event(ins, MOTION, EventArgs({{MOUSE_X, ins->mousePosition.x}, {MOUSE_Y, ins->mousePosition.y}})));
  }
  static void _sMouse(int scroll, int press, int x, int y){ 
    Window* ins = instances[glutGetWindow()];
    updateMousePos(ins, x, y);
    staticCallback(MOUSE, Event(ins, MOUSE, EventArgs({{MOUSE_SCROLL, scroll}, {MOUSE_BUTTON, press}, {MOUSE_X, ins->mousePosition.x}, {MOUSE_Y, ins->mousePosition.y}})));
  }
  static void _sKeyboard(unsigned char c, int x, int y) { 
    Window* ins = instances[glutGetWindow()];
    updateMousePos(ins, x, y);
    staticCallback(KEYBOARD, Event(ins, KEYBOARD, EventArgs({{KEY, (int)c}, {MOUSE_X, ins->mousePosition.x}, {MOUSE_Y, ins->mousePosition.y}}))); 
  }
  static void _sPassive(int x, int y) {
    Window* ins = instances[glutGetWindow()];
    updateMousePos(ins, x, y);
    staticCallback(PASSIVE_MOTION, Event(ins, PASSIVE_MOTION, EventArgs({{MOUSE_X, ins->mousePosition.x}, {MOUSE_Y, ins->mousePosition.y}}))); 
  } 
  static void _sSpecial(int c, int x, int y){
    Window* ins = instances[glutGetWindow()];
    updateMousePos(ins, x, y);
    staticCallback(SPECIAL, Event(ins, SPECIAL, EventArgs({{SPECIAL_KEY, c}, {MOUSE_X, ins->mousePosition.x}, {MOUSE_Y, ins->mousePosition.y}})));
  }
  static void _sResize(int w, int h){
    Window* ins = instances[glutGetWindow()];
    ins->width = w;
    ins->height = h;
    staticCallback(RESIZE, Event(ins, RESIZE, EventArgs({{RESIZEWIDTH, w}, {RESIZEHEIGHT, h}})));
  }
  static void _sIdle(void){
    staticCallback(IDLE, Event(instances[glutGetWindow()], IDLE, EventArgs({}))); 
  }
  void setupFunctions(){
    glutDisplayFunc(_sDisplay);
    glutMotionFunc(_sMotion);
    glutMouseFunc(_sMouse);
    glutKeyboardFunc(_sKeyboard);
    glutPassiveMotionFunc(_sPassive);
    glutSpecialFunc(_sSpecial);
    glutReshapeFunc(_sResize);
    glutIdleFunc(_sIdle);
  }

};
#endif
