#ifndef LEVEL_H
#define LEVEL_H

#include "cylinder.h"
#include "cone.h"
#include "ivy.h"


class Level
{
public:
    Level(Cylinder *cylinder, Cone *cone);
    virtual ~Level();
    void draw(GLuint shader, GLfloat d, GLfloat param_x, GLfloat param_y, GLfloat size, Transforms transform);
    void drawIvy(GLuint shader, Transforms t);
private:
    Ivy *m_ivy;
    Cone *m_cone;
    Cylinder *m_cylinder;
};

#endif // LEVEL_H
