#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"

#define CYL_FACES 3

class Cylinder : public Shape
{
public:
    Cylinder(int one, int two, float three);
    virtual ~Cylinder();
    void tesselate(int one, int two, float three);
    void makeBuf();

    glm::vec3 **getPositions();
    glm::vec3 **getNormals();

    GLfloat *getBuf();
protected:
    glm::vec3 getVertex(float theta, float y);

};

#endif // CYLINDER_H
