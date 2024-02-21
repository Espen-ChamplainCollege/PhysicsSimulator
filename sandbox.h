#ifndef SANDBOX_H
#define SANDBOX_H

#include <chrono>
#include <ratio>
#include <QDebug>
#include <thread>
#include <unordered_map>
#include <QtMultimedia/QMediaPlayer>
#include "button.h"
#include "sphere.h"
#include "hexagon.h"
#include "triangle.h"

struct Sandbox {
  // std::vector<Shape> shapes <- We should have a parent class _Shape_
  // that all of our shapes inherit from, or something like that.
  std::unordered_map<Button, void(Sandbox::*)(void)> buttons;
  // this is temporary just to show how the buttons work
  std::vector<Sphere*> spheres;
  std::vector<Hexagon*> hexagons;
  std::vector<Triangle*> triangles;

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
      // hard coded right now but could implement this to be user controlled
      float size = 16;
      hexagons.push_back(
          new Hexagon(Point::randomPoint(size + 1, width - (size + 1), size + 1, height - (size + 1)),
              Point::randomPoint(-10, 10, -10, 10),
              Color::randomColor(),
              size
          ));
  }
  
  void addTriangle(){
      triangles.push_back(
          new Triangle(Point::randomPoint(21, width - 21, 21, height - 21),
              Point::randomPoint(-10, 10, -10, 10),
              Color::randomColor(),
              20)
        );

  }

  const void addButtons(){
    buttons[Button(Point((float)width / 2 - 125, height - 75), 250, 50, "Example Button (Sphere)")] 
      = &Sandbox::addSphere;
    buttons[Button(Point((float)width / 2 + 130, height - 75), 80, 50, "Hexagon")]
      = &Sandbox::addHexagon;
    buttons[Button(Point((float)width / 2 - 210, height - 75), 80, 50, "Triangle")]
      = &Sandbox::addTriangle;
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

  // Setting up sounds
  QMediaPlayer *sound = new QMediaPlayer;

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
      sound->setSource(QUrl::fromLocalFile("./PhysicsSimulator/boop.mp3"));
      sound->setVolume(5);
      sound->setLoopCount(1);
    // something like, for each shape in shapes, update pos.
    // painting is handled on a different thread so we dont have to worry about that here
    for(int i = 0; i < spheres.size(); i++){
      Point nextPos = spheres[i]->position + spheres[i]->velocity;
      if(nextPos.y + spheres[i]->radius > this->height || nextPos.y < 0){
        spheres[i]->velocity.y *= -1;
        sound->play();
      }
      if(nextPos.x + spheres[i]->radius > this->width || nextPos.x < 0){
        spheres[i]->velocity.x *= -1;
        sound->play();
      }
      spheres[i]->position += spheres[i]->velocity;
    }

    // update hexagons
    for (int i = 0; i < hexagons.size(); i++) {
        // if next pos will be out of bounds (top or bottom), go other direction
        if (hexagons[i]->points[4].y() + hexagons[i]->velocity.y > this->height || hexagons[i]->points[1].y() + hexagons[i]->velocity.y < 0) {
            hexagons[i]->velocity.y *= -1;
            sound->play();
        }
        // if next pos will be out of bounds (left or right), go other direction
        if (hexagons[i]->points[3].x() + hexagons[i]->velocity.x > this->width || hexagons[i]->points[0].x() + hexagons[i]->velocity.x < 0) {
            hexagons[i]->velocity.x *= -1;
            sound->play();
        }
        hexagons[i]->position += hexagons[i]->velocity;
        // update points of hexagon
        for (int j = 0; j < 6; j++)
        {
            hexagons[i]->points[j].setX(hexagons[i]->points[j].x() + hexagons[i]->velocity.x);
            hexagons[i]->points[j].setY(hexagons[i]->points[j].y() + hexagons[i]->velocity.y);
        }
    }

    // Update Triangles
    for(int i = 0; i < triangles.size(); i++){
        Point nextPos = triangles[i]->position + triangles[i]->velocity;
        if (nextPos.y + triangles[i]->radius > this->height || nextPos.y < 0){
            triangles[i]->velocity.y *= -1;
            sound->play();
        }
        if (nextPos.x + triangles[i]->radius > this->width || nextPos.x < 0){
            triangles[i]->velocity.x *= -1;
            sound->play();
        }
        triangles[i]->position += triangles[i]->velocity;
        for (int j = 0; j < 3; j++){
            triangles[i]->points[j].setX(triangles[i]->points[j].x() + triangles[i]->velocity.x);
            triangles[i]->points[j].setY(triangles[i]->points[j].y() + triangles[i]->velocity.y);
        }
    }
  }
};
#endif
