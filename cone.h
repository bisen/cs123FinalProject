#ifndef CONE_H
#define CONE_H
#include "shape.h"

#define CONE_FACES 2

class Cone : public Shape
{
public:
    Cone(int one, int two, float three);
    virtual ~Cone();
    void tesselate(int one, int two, float three);
    void makeBuf();
protected:
    glm::vec3 getVertex(float theta, float y);
    glm::vec3 getNormal(float theta, float y);
};

#endif // CONE_H
