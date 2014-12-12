#include "shape.h"

Shape::Shape(int one, int two, float three)
{
    m_p1 = one;
    m_p2 = two;
    m_p3 = three;

}

Shape::Shape() {

}

Shape::~Shape() {
}

void Shape::init(GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation) {
    glGenVertexArrays(1, &m_vaoid);
    glBindVertexArray(m_vaoid);

    glGenBuffers(1, &m_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, getNumVert() * 11 * sizeof(GLfloat), m_buf, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);
    glEnableVertexAttribArray(normalLocation);
    glEnableVertexAttribArray(tangentLocation);
    glEnableVertexAttribArray(textureLocation);

    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void*) 0);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_TRUE,  11*sizeof(GLfloat), (void*) (sizeof(GLfloat)*3));
    glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_TRUE,  11*sizeof(GLfloat), (void*) (sizeof(GLfloat)*6));
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE,  11*sizeof(GLfloat), (void*) (sizeof(GLfloat)*9));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Shape::draw() {
    glBindVertexArray(m_vaoid);
    glDrawArrays(GL_TRIANGLES, 0, m_num_vert);
    glBindVertexArray(0);
}

void Shape::tesselate(int one, int two, float three) {

}

void Shape::makeBuf() {

}


void Shape::placeVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec3 tangent, glm::vec2 texture, int index) {
    normal = glm::normalize(normal);
    tangent = glm::normalize(tangent);
    m_buf[index] = vertex.x;
    m_buf[index+1] = vertex.y;
    m_buf[index+2] = vertex.z;
    m_buf[index+3] = normal.x;
    m_buf[index+4] = normal.y;
    m_buf[index+5] = normal.z;
    m_buf[index+6] = tangent.x;
    m_buf[index+7] = tangent.y;
    m_buf[index+8] = tangent.z;
    m_buf[index+9] = texture.x;
    m_buf[index+10] = texture.y;
}

int Shape::getNumVert() {
    return this->m_num_vert;
}
