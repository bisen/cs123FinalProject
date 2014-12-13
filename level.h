#ifndef LEVEL_H
#define LEVEL_H

#include "cylinder.h"
#include "cone.h"
#include "ivy.h"
#include "ivyalt.h"


//Represents one level of the tower
class Level
{
public:
    Level(Cylinder *cylinder, Cone *cone);
    virtual ~Level();
    void draw(GLuint shader, GLfloat d, GLfloat param_x, GLfloat param_y, GLfloat size, Transforms transform);
    void drawIvy(GLuint shader, Transforms t);
    void init(GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);

    void setSystem(Plant *p) {m_ivy = p;}

    void remakeIvy(GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);
private:
    Plant *m_ivy;
    Cone *m_cone;
    Cylinder *m_cylinder;
};

#endif // LEVEL_H
