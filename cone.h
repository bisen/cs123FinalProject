#ifndef CONE_H
#define CONE_H

#include "GL/glew.h"

class Cone
{
public:
    //Uninitialized Cone
    Cone();
    //Initialized Cone
    Cone(const GLuint vertexLocation, const GLuint normalLocation);
    // Initialize Cone: generate VAO, vertex data, and buffer it on GPU
    void init(const GLuint vertexLocation, const GLuint normalLocation);
    void draw();
private:
    bool m_isInitialized;
    GLuint m_vaoID;
};

#endif // CONE_H
