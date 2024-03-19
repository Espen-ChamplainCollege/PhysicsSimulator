#ifndef SANDBOX_H
#define SANDBOX_H

#include <chrono>
#include <ratio>
#include <QDebug>
#include <thread>
#include <unordered_map>
#include "gui/button.h"
#include "objects/shape.h"
#include "objects/sphere.h"
#include "objects/hexagon.h"
#include "objects/triangle.h"
#include "objects/userShape.h"
#include "objects/square.h"
#include "objects/staticbody.h"
#include "physics/collision.h"
#include "../bin/util.h"
#include "QCoreApplication"

#define DEBUG_MODE false

struct Sandbox {
    inline const static float DEFAULT_MASS = 200.0;

    inline const static bool DEBUG_VIEW_BOUNDING_BOXES = true;
    inline const static bool DEBUG_VIEW_TRIANGULATIONS = true;
    inline const static bool DEBUG_VIEW_CONTACTS = true;
    inline const static bool DEBUG_VIEW_VELOCITY = true;
    inline const static bool DEBUG_NO_FILL = true;
    inline const static bool DEBUG_PAUSE_ON_UPDATE = false;
    inline const static bool DEBUG_PAUSE_ON_CONTACT = true;
    inline const static bool DEBUG_AUTO_SPAWN_SQUARE = true;

    float width, height;
    Point position;
    // std::vector<Shape> shapes <- We should have a parent class _Shape_
    // that all of our shapes inherit from, or something like that.
    std::unordered_map<Button, void(Sandbox::*)(void)> buttons;
    std::unordered_map<CollisionKey, Collision> collisions;
    // this is temporary just to show how the buttons work
    std::vector<Sphere*> spheres;
    std::vector<Hexagon*> hexagons;
    std::vector<Shape*> shapes;
    std::vector<UserShape*> userShapes;

    Sandbox(int _width, int _height, std::chrono::duration<double, std::milli> _refreshRate, const Point &p = Point(0, 0))
    : refreshRate(_refreshRate), width(_width), height(_height), position(p){};
    ~Sandbox();


    std::thread start(){
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


    bool checkMouseClick = false;
    std::vector<Point> userShapePoints;
    int userShapeVerts = 5;

    void addUserShape() {
        checkMouseClick = true;
        // get mouse clicks
        while (userShapePoints.size() < userShapeVerts) {
            QCoreApplication::processEvents();
            continue;
        }
        float shapeWidthMin = 0;
        float shapeHeightMin = 0;
        float shapeWidthMax = 0;
        float shapeHeightMax = 0;
        for (int i = 0; i < userShapePoints.size(); i++)
        {
            if (userShapePoints[i].x < shapeWidthMin) {
                shapeWidthMin = userShapePoints[i].x;
            }
            if (userShapePoints[i].x > shapeWidthMax) {
                shapeWidthMax = userShapePoints[i].x;
            }
            if (userShapePoints[i].y < shapeHeightMin) {
                shapeHeightMin = userShapePoints[i].y;
            }
            if (userShapePoints[i].y > shapeHeightMax) {
                shapeHeightMax = userShapePoints[i].y;
            }
        }
        float shapeWidth = (shapeWidthMax - shapeWidthMin) / 2;
        float shapeHeight = (shapeHeightMax - shapeHeightMin) / 2;
        shapes.push_back(
            new UserShape(Point::averageOfVector(userShapePoints),
                shapeWidth,
                Color::randomColor(),
                DEFAULT_MASS,
                userShapePoints
            ));
        checkMouseClick = false;
        userShapePoints.clear();
    }
  
    // Example add hexagon function
    void addHexagon() {
        // hard coded right now but could implement this to be user controlled
    float hexWidth = 16;
    shapes.push_back(
        new Hexagon(Point::randomPoint(hexWidth + 1, width - (hexWidth + 1), hexWidth + 1, height - (hexWidth + 1)),
            hexWidth,
            Color::randomColor(),
            DEFAULT_MASS
        ));
    }
  
    std::vector<Triangle*> triangles;
    void addTriangle(){
        float triLen = 16;
        shapes.push_back(
            new Triangle(Point::randomPoint(triLen + 1, width - (triLen + 1), triLen + 1, height - (triLen + 1)),
                triLen,
                Color::randomColor(),
                DEFAULT_MASS
          ));

    }
    void addSquare(){
        float sqW = 16;
        shapes.push_back(
            new Square(Point(width / 2 - sqW / 2, height / 2 - sqW / 2), sqW, Color::randomColor(), DEFAULT_MASS)
        );
        shapes[shapes.size() - 1]->friction = 0.2;
    }
    
    void addStaticBody(const Point &center, float w, float h){
        shapes.push_back(
            new StaticBody(
                {
                    Point(center.x + w / 2, center.y + h / 2), Point(center.x - w / 2, center.y + h / 2),
                    Point(center.x - w / 2, center.y - h / 2), Point(center.x + w / 2, center.y - h / 2)
                },
                Color(255, 255, 255)
            )
        );
        shapes[shapes.size() - 1]->friction = 0.5;
        
    }

    void clearScreen() {
        shapes.clear();
    }

    void addMenu(){
        buttons.clear();
        buttons[Button(Point(width / 2 - 399, height - 75), 80, 50, "Open")]
            = &Sandbox::addButtons;
    }

    void addButtons(){
        buttons.clear();
        buttons[Button(Point(width / 2 + 145, height - 75), 80, 50, "Square")]
            = &Sandbox::addSquare;
        buttons[Button(Point(width / 2 + 230, height - 75), 80, 50, "Hexagon")]
            = &Sandbox::addHexagon;
        buttons[Button(Point(width / 2 + 60, height - 75), 80, 50, "Triangle")]
            = &Sandbox::addTriangle;
        buttons[Button(Point(width / 2 + 315, height - 75), 80, 50, "Clear")]
            = &Sandbox::clearScreen;
        buttons[Button(Point(width / 2 - 70, height - 75), 120, 50, "Custom (Click 5 times)")]
            = &Sandbox::addUserShape;
        buttons[Button(Point(width / 2 - 399, height - 75), 80, 50, "Close")]
            = &Sandbox::addMenu;
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
    typedef std::unordered_map<CollisionKey, Collision>::iterator MapIterator;
    bool gravityEnabled = true;
    bool stopped = false;
    const Point gravity = Point(0, 9.8);

    std::chrono::duration<double, std::milli> refreshRate;

    const void updateControl(){
        // While not stopped
        double timeStep = 1.0 / 60.0;


        if(DEBUG_AUTO_SPAWN_SQUARE) addSquare();
        

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
            while(paused){}
            
        }
    }
    const void applyForces(double time){
        for(int i = 0; i < shapes.size(); i++){
            if(shapes[i]->inverseMass == 0.0) continue;
            shapes[i]->velocity += (gravity + shapes[i]->force * shapes[i]->inverseMass) * time;
            shapes[i]->angularVelocity += shapes[i]->torque * shapes[i]->inverseI * time;
        }
    }
    const void processCollisions(double time){
        std::unordered_map<CollisionKey, Collision> possibilities;
        for(int i = 0; i < shapes.size(); i++){
            for(int k = 0; k < shapes.size(); k++){
                if(i == k || (shapes[i]->inverseMass == 0 && shapes[k]->inverseMass == 0)) continue;
                Collision collision(shapes[i], shapes[k]);
                CollisionKey key(shapes[i], shapes[k]);
                if(collision.possibleCollision){
                    if(shapes[i]->dry){
                        shapes[i]->dry = false;
                        shapes[i]->recalculateTriangulation();
                    }
                    if(shapes[k]->dry){
                        shapes[k]->dry = false;
                        shapes[k]->recalculateTriangulation();
                    }
                    // if(!collision.s1->isStatic) std::cout << collision.s1->position << std::endl;
                    // if(!collision.s2->isStatic) std::cout << collision.s2->position << std::endl;
                    if(possibilities.find(key) == possibilities.end()){
                        possibilities.insert({key, collision});
                    }
                } else {
                    collisions.erase(key);
                }
            }
        }
        for(MapIterator iter = collisions.begin(); iter != collisions.end(); ){
            if(iter->second.realIntersectionTest() > 0){
                ++iter;
            } else {
                iter = collisions.erase(iter);
            }
        }
        for(MapIterator iter = possibilities.begin(); iter != possibilities.end(); ){
            if(iter->second.realIntersectionTest() > 0){
                if(collisions.find(iter->first) == collisions.end()){
                    collisions.insert({iter->first, iter->second});
                } else {
                    collisions.at(iter->first).refresh(iter->second.contacts);
                }
                ++iter;
            }
        }

        for(MapIterator iter = collisions.begin(); iter != collisions.end(); ++iter) iter->second.cache(1.0 / time);
    }
    const void applyVelocities(double time){
        for(int i = 0; i < shapes.size(); i++){
            if(shapes[i]->inverseMass == 0.0) continue;
            Point prevPosition = shapes[i]->position;
            float prevRotation = shapes[i]->rotation;
            shapes[i]->position += shapes[i]->velocity;
            shapes[i]->rotation += shapes[i]->angularVelocity;
            shapes[i]->force = Point(0.0 ,0.0);
            shapes[i]->torque = 0.0;

            Util::realignPoints(shapes[i]->verts, shapes[i]->position - prevPosition);      
            Util::rotatePoints(shapes[i]->position, shapes[i]->rotation - prevRotation, shapes[i]->verts);
            shapes[i]->boundingRect->update(shapes[i]->verts);
            shapes[i]->dry = true;
        }
    }
    const int COLLISION_ITERATIONS = 10;
    const void solveCollisions(double time){
        for(int i = 0; i < COLLISION_ITERATIONS; i++){
            for(MapIterator iter = collisions.begin(); iter != collisions.end(); ++iter) iter->second.apply();
        }
    }
    const void update(double time){
        applyForces(time);
        processCollisions(time);
        solveCollisions(time);
        applyVelocities(time);
        if((DEBUG_PAUSE_ON_CONTACT && collisions.size() > 0) || DEBUG_PAUSE_ON_UPDATE){
            paused = true;
            while(paused){}
        }
    }
};

#endif
