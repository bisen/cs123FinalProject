#ifndef PLANT_H
#define PLANT_H

#include <QStack>
#include "shape.h"
#include "cylinder.h"
#include <glm/gtc/matrix_transform.hpp>
#include "lsys.h"
#include <CS123Common.h>


typedef struct node {
    glm::mat4 model;
    glm::vec4 up;

    glm::mat3 rotation;
    glm::vec3 translation;
} Node;

class Plant
{
public:
    Plant();
    virtual ~Plant();

    void render(GLuint shader);

//    void gen(int n);
    virtual void parseSystem(int level, GLuint vertexLocation, GLuint normalLocation);

    static void printVertex(glm::vec4 v) { printf("%f, %f, %f\n", v.x, v.y, v.z); }

    float m_factor;

    void draw();

private:

    LSys *m_system;
    char *m_initial;

    int m_initialized;
    QList<Node> *m_scenegraph;

    Cylinder *m_cyl;

    GLfloat *m_buf;

    GLuint m_vbo;
    GLuint m_vao;

};

#endif // PLANT_H
