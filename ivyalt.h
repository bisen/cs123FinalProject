#ifndef IVYALT_H
#define IVYALT_H

#include "plant.h"
#include "ivy.h"

class IvyAlt : public Ivy
{
public:
    IvyAlt();
    virtual int parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);
};

#endif // IVYALT_H
