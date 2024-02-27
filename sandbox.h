#ifndef SANDBOX_H
#define SANDBOX_H

#include <chrono>
#include <ratio>
#include <QDebug>
#include <thread>
#include <unordered_map>
#include "button.h"
#include "shape.h"
#include "sphere.h"
#include "hexagon.h"
#include "triangle.h"
#include "util.h"

struct Sandbox {
  int width, height;
  Point position;
  // std::vector<Shape> shapes <- We should have a parent class _Shape_
  // that all of our shapes inherit from, or something like that.
  std::unordered_map<Button, void(Sandbox::*)(void)> buttons;
  // this is temporary just to show how the buttons work
  std::vector<Sphere*> spheres;
  std::vector<Hexagon*> hexagons;
  std::vector<Shape*> shapes;

  Sandbox(int _width, int _height, std::chrono::duration<double, std::milli> _refreshRate, const Point &p = Point(0, 0)) 
    : refreshRate(_refreshRate), width(_width), height(_height), position(p){};
  ~Sandbox();

  std::thread start(){return std::thread(&Sandbox::updateControl, this);};
  const inline void stop(){stopped = true;};
  const inline void pause(){paused = true;};
  const inline void unpause(){paused = false;}


  // Example add sphere function
  void addSphere(){
    spheres.push_back(
      // 21 so it doesnt get stuck in the wall (radius is 20)
      new Sphere(Point::randomPoint(21, width - 21, 21, height - 21), 
      Point::randomPoint(-10, 10, -10, 10),
      Color::randomColor(), 
      20
    ));
    spheres[spheres.size() - 1]->angularVelocity = Point::randomPoint(-10, 10, -10, 10).x;
    // ^ this is lazy, will fix later
  }
  
  // Example add hexagon function
  void addHexagon() {
      // hard coded right now but could implement this to be user controlled
    float hexWidth = 16;
    shapes.push_back(
      new Hexagon(Point::randomPoint(hexWidth + 1, width - (hexWidth + 1), hexWidth + 1, height - (hexWidth + 1)),
        hexWidth,
        Color::randomColor(),
        Point::randomPoint(-10, 10, -10, 10),
        0,
        Point(0, 0),
        Util::generalRandom(-5, 5) 
      ));
  }
  
  std::vector<Triangle*> triangles;
  void addTriangle(){
    float triLen = 16;
    shapes.push_back(
      new Triangle(Point::randomPoint(triLen + 1, width - (triLen + 1), triLen + 1, height - (triLen + 1)),
        triLen,
        Color::randomColor(),
        Point::randomPoint(-10, 10, -10, 10),
        0,
        Point(0, 0),
        Util::generalRandom(-5, 5) 
      ));

  }

  void clearScreen() {
    shapes.clear();
  }

  const void addButtons(){
    buttons[Button(Point((float)width / 2 - 125, height - 75), 250, 50, "Example Button (Sphere)")] 
      = &Sandbox::addSphere;
    buttons[Button(Point((float)width / 2 + 130, height - 75), 80, 50, "Hexagon")]
      = &Sandbox::addHexagon;
    buttons[Button(Point((float)width / 2 - 210, height - 75), 80, 50, "Triangle")]
      = &Sandbox::addTriangle;
    buttons[Button(Point((float)width / 2 + 315, height - 75), 80, 50, "Clear")]
      = &Sandbox::clearScreen;
  }
  const void tryClickButtons(const Point &pos){
    for(auto i = buttons.begin(); i != buttons.end(); i++){
      if(i->first.areaContains(pos)) {
        // "*this" dereference _this_ pointer. ".*" dereference the pointer to the function. "()" call the function.
        // basically just calling Sandbox::addSphere() but in the context of this sandbox
        (*this.*(i->second))();
        break;
        // I know this syntax is gonna raise eyebrows, I hate it too but it be what it be
      }
    }

  }

private:
  bool paused = false;
  bool stopped = false;

  std::chrono::duration<double, std::milli> refreshRate;

  const void updateControl(){
    // While not stopped
    while(!stopped){
      // Record the time we start to update
      auto updateStart = std::chrono::high_resolution_clock::now();
      update();
      // The duration is the end time - the start time.
      std::chrono::duration<double, std::milli> duration = 
        (std::chrono::high_resolution_clock::now() - updateStart);
      // if the duration was less than our refresh rate, sleep for a bit.
      // (if we dont do this, simulation will be literally faster on faster machines)
      if(duration < refreshRate) std::this_thread::sleep_for(refreshRate - duration);
      while(paused){}
    }
  }
  // Update function is moved here to give us more control.
  // Using the built in QT update timers could restrict us down the line
  const void update(){
    // something like, for each shape in shapes, update pos.
    // painting is handled on a different thread so we dont have to worry about that here


    // this is very slow like O(n^2) with n being the amount of vertices.
    for(int i = 0; i < shapes.size(); i++){
      bool ychange = false, xchange = false;
      for(int k = 0; k < shapes[i]->verts.size(); k++){
        if(ychange && xchange) break;
        if(!ychange && (shapes[i]->verts[k].y + shapes[i]->velocity.y > this->height ||
           shapes[i]->verts[k].y + shapes[i]->velocity.y < 0)){
          shapes[i]->velocity.y *= -1;
          ychange = true;
        }
        if(!xchange && (shapes[i]->verts[k].x + shapes[i]->velocity.x > this->width ||
           shapes[i]->verts[k].x + shapes[i]->velocity.x < 0)){
          shapes[i]->velocity.x *= -1;
          xchange = true;
        }
      }
      float prevRotation = shapes[i]->rotation;
      shapes[i]->rotation += shapes[i]->angularVelocity;
      Point oldPosition = shapes[i]->position;
      shapes[i]->position += shapes[i]->velocity;
      for(int k = 0; k < shapes[i]->verts.size(); k++){
        Point diff = shapes[i]->verts[k] - oldPosition;
        shapes[i]->verts[k] = shapes[i]->position + diff;
      }
      Util::rotatePoints(shapes[i]->position, shapes[i]->rotation - prevRotation, shapes[i]->verts);
    }
  }
};







#endif
