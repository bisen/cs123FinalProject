#ifndef SHAPE_H
#define SHAPE_H

#include <CS123Common.h>
#define R 0.5f

class Shape
{
public:
    Shape();
    Shape(int one, int two, float three);
    virtual ~Shape();
    virtual void draw();
    virtual void tesselate(int one, int two, float three);
    void init(GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation);
    virtual void makeBuf();
    GLfloat *m_buf;
    int getNumVert();
protected:
    int m_p1;
    int m_p2;
    float m_p3;
    int m_num_vert;
    GLuint m_vaoid;
    GLuint m_buffer_id;
    glm::vec3 **m_positions;
    glm::vec3 **m_normals;
    glm::vec3 **m_tangents;
    glm::vec2 **m_textures;
    void placeVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec3 tangent, glm::vec2 texture, int index);
    int m_is_tesselated;

};

#endif // SHAPE_H
