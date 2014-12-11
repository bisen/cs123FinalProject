#ifndef SPHERE_H
#define SPHERE_H

#include "GL/glew.h"
#include "plant.h"
#include "ivy.h"

class Sphere
{
public:
    //Uninitialized Sphere
    Sphere();
    //Initialized Sphere
    Sphere(const GLuint vertexLocation, const GLuint normalLocation);
    // Initialize Sphere: generate VAO, vertex data, and buffer it on GPU
    void init(const GLuint vertexLocation, const GLuint normalLocation);
    void draw();
    void drawIvy(GLuint shader, Transforms t);
private:

    Ivy *m_ivy;

    bool m_isInitialized;
    GLuint m_vaoID;
};

#endif // SPHERE_H
