#ifndef IVY_H
#define IVY_H

#include "plant.h"

class Ivy : public Plant
{
public:
    Ivy();
    virtual int parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);
};

#endif // IVY_H
