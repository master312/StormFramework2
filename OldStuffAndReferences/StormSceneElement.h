#pragma once
#include <string>
#include "../core/utils/Plane.h"

/*
    * TODO:
        * Split components into 2 groups - renderable and non-renderable.
*/

class StormSceneElement {
public:
    StormSceneElement();
    ~StormSceneElement();

    void render();
    void tick(float deltaTime);
    
    /* Sets Z rendering order */
    int setZ(int zOrder);

    /* Returns Z rendering order */
    int getZ();

    /* Sets new position to this element */
    void setPosition(Vector2 position);

    /* Adds @position vector to current element position */
    void addPosition(Vector2 position);

    /* Returns plane representing this element */
    Plane& getPlane();
private:
    /* Unique identifier */
    int _Id;

    /* Element's name */
    std::string _Name;
    
    /* Plane that represents this element's position, size and rotation. */
    Plane _Plane;
    
    /* Element rendering order */
    int _ZOrder;

    // std::vector<StormSceneElementComponent> _Components;
};
