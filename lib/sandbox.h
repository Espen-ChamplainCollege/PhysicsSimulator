#ifndef SANDBOX_H
#define SANDBOX_H

#include <chrono>
#include <ratio>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <thread>
#include <unordered_map>
#include "gui/button.h"
#include "objects/shape.h"
#include "objects/sphere.h"
#include "objects/hexagon.h"
#include "objects/triangle.h"
#include "../bin/util.h"
#include <iostream>

#define DEBUG_MODE false

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

    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    std::thread start(){
      mediaPlayer = new QMediaPlayer;
      audioOutput = new QAudioOutput;
      mediaPlayer->setAudioOutput(audioOutput);
      mediaPlayer->setSource(QUrl::fromLocalFile("../boop.wav"));
      audioOutput->setVolume(50);
      return std::thread(&Sandbox::updateControl, this);
    };
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
            1
        ));
    }
  
    std::vector<Triangle*> triangles;
    void addTriangle(){
        float triLen = 16;
        shapes.push_back(
            new Triangle(Point::randomPoint(triLen + 1, width - (triLen + 1), triLen + 1, height - (triLen + 1)),
                triLen,
                Color::randomColor(),
                1
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


    bool paused = false;
private:
    bool stopped = false;
    const Point gravity = Point(0, 9.8);

    std::chrono::duration<double, std::milli> refreshRate;

    const void updateControl(){
        // While not stopped
        double timeStep = 1.0 / 60.0;
        while(!stopped){
            // Record the time we start to update
            auto updateStart = std::chrono::high_resolution_clock::now();
            update(timeStep);
            // The duration is the end time - the start time.
            std::chrono::duration<double, std::milli> duration =
                (std::chrono::high_resolution_clock::now() - updateStart);
            // if the duration was less than our refresh rate, sleep for a bit.
            // (if we dont do this, simulation will be literally faster on faster machines)
            if(duration < refreshRate) std::this_thread::sleep_for(refreshRate - duration);
            #if DEBUG_MODE
              paused = true;
            #endif // DEBUG_MODE
            while(paused){}
            
        }
    }
    // Update function is moved here to give us more control.
    // Using the built in QT update timers could restrict us down the line
    const void update(double time){
        // something like, for each shape in shapes, update pos.
        // painting is handled on a different thread so we dont have to worry about that here

        for(int i = 0; i < shapes.size(); i++){
            Point prevPosition = shapes[i]->position;
            float prevRotation = shapes[i]->rotation;
            shapes[i]->velocity += (gravity + shapes[i]->force * (1 / shapes[i]->mass)) * time;
            shapes[i]->angularVelocity += (shapes[i]->torque * (1 / shapes[i]->inertiaTensor)) * time;
            shapes[i]->position += shapes[i]->velocity;
            shapes[i]->rotation += shapes[i]->angularVelocity;
            shapes[i]->force = Point(0, 0);
            shapes[i]->torque = 0;

            Util::realignPoints(shapes[i]->verts, prevPosition, shapes[i]->position);      
            Util::rotatePoints(shapes[i]->position, shapes[i]->rotation - prevRotation, shapes[i]->verts);
        }

        
        std::vector<Shape*> toRemove;
        for(int i = 0; i < shapes.size(); i++){
          if(shapes[i]->position.y > this->height) toRemove.push_back(shapes[i]);
        }
        for(int i = 0; i < toRemove.size(); i++){
          for(int k = 0; k < shapes.size(); k++){
            if(shapes[k] == toRemove[i]) shapes.erase(shapes.begin() + k);
            break;
          }
        }
        toRemove.clear();
    }
};

#endif
