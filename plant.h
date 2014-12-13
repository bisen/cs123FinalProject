#ifndef PLANT_H
#define PLANT_H

#include "cylinder.h"
#include <QStack>
#include "shape.h"
#include <glm/gtc/matrix_transform.hpp>
#include "lsys.h"
#include <CS123Common.h>
#include "transforms.h"


typedef struct node {
    glm::mat4 model;
    glm::vec3 color;
} Node;

class Plant
{
public:
    Plant();
    virtual ~Plant();

    void render(GLuint shader, Transforms t);
    virtual int parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);
    void init(GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);

    static void printVertex(glm::vec4 v) { printf("%f, %f, %f\n", v.x, v.y, v.z); }

    float m_factor;

protected:

    void copyAndMult(GLfloat *buf, int index, int index2, glm::mat4 matrix, glm::mat4 inverseMat);

    LSys *m_system;
    char *m_initial;

    int m_initialized;
    QList<Node> *m_scenegraph;

    GLfloat *m_buf;
    int m_num_vert;

    GLuint m_vaoid, m_vboid;

    Cylinder *m_cyl;
};

#endif // PLANT_H
