#ifndef IVY_H
#define IVY_H

#include "plant.h"

class Ivy : public Plant
{
public:
    Ivy();
    virtual void parseSystem(int level, GLuint vertexLocation, GLuint normalLocation);
};

#endif // IVY_H
