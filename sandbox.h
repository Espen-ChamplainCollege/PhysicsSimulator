#ifndef SANDBOX_H
#define SANDBOX_H

#include <chrono>
#include <ratio>
#include <QDebug>
#include <thread>
#include <unordered_map>
#include "button.h"
#include "sphere.h"
#include "hexagon.h"

struct Sandbox {
  // std::vector<Shape> shapes <- We should have a parent class _Shape_
  // that all of our shapes inherit from, or something like that.
  std::unordered_map<Button, void(Sandbox::*)(void)> buttons;
  // this is temporary just to show how the buttons work
  std::vector<Sphere*> spheres;
  std::vector<Hexagon*> hexagons;

  Sandbox(int _width, int _height, std::chrono::duration<double, std::milli> _refreshRate) 
    : refreshRate(_refreshRate), width(_width), height(_height){};
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
  }
  // Example add hexagon function
  void addHexagon() {
      hexagons.push_back(
          // 21 so it doesnt get stuck in the wall (radius is 20)
          new Hexagon(Point::randomPoint(21, width - 21, 21, height - 21),
              Point::randomPoint(-10, 10, -10, 10),
              Color::randomColor(),
              20
          ));
  }
  const void addButtons(){
    buttons[Button(Point((float)width / 2 - 125, 50), 250, 50, "Example Button (Sphere)")] 
      = &Sandbox::addSphere;
    buttons[Button(Point((float)width / 2 - 125, 1), 250, 50, "Example Button (Hexagon)")]
      = &Sandbox::addHexagon;
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
  int width, height;

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
    for(int i = 0; i < spheres.size(); i++){
      Point nextPos = spheres[i]->position + spheres[i]->velocity;
      if(nextPos.y + spheres[i]->radius > this->height || nextPos.y < 0){
        spheres[i]->velocity.y *= -1;
      }
      if(nextPos.x + spheres[i]->radius > this->width || nextPos.x < 0){
        spheres[i]->velocity.x *= -1;
      }
      spheres[i]->position += spheres[i]->velocity;
    }
    for (int i = 0; i < hexagons.size(); i++) {
        Point nextPos = hexagons[i]->position + hexagons[i]->velocity;
        if (nextPos.y + hexagons[i]->radius > this->height || nextPos.y < 0) {
            hexagons[i]->velocity.y *= -1;
        }
        if (nextPos.x + hexagons[i]->radius > this->width || nextPos.x < 0) {
            hexagons[i]->velocity.x *= -1;
        }
        hexagons[i]->position += hexagons[i]->velocity;
        // update points of hexagon
        for (int j = 0; j < 6; j++)
        {
            hexagons[i]->points[j].setX(hexagons[i]->points[j].x() + hexagons[i]->velocity.x);
            hexagons[i]->points[j].setY(hexagons[i]->points[j].y() + hexagons[i]->velocity.y);
        }
    }
  }
};







#endif
