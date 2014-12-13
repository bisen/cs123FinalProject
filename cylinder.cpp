#include "cylinder.h"

#define LEVEL 16

Cylinder::Cylinder(int one, int two, float three)
{
    m_p1 = one > 0 ? one : 1;
    m_p2 = two > 2 ? two : 3;
    m_p3 = three;
    m_is_tesselated = 0;
}

Cylinder::~Cylinder() {
    if(m_is_tesselated){
        delete[] m_buf;
        delete[] m_positions[0];
        delete[] m_positions[1];
        delete[] m_positions[2];
        delete[] m_positions;

        delete[] m_normals[0];
        delete[] m_normals[1];
        delete[] m_normals[2];
        delete[] m_normals;

        delete[] m_tangents[0];
        delete[] m_tangents[1];
        delete[] m_tangents[2];
        delete[] m_tangents;

        delete[] m_textures[0];
        delete[] m_textures[1];
        delete[] m_textures[2];
        delete[] m_textures;

        glDeleteBuffers(1, &m_buffer_id);
        glDeleteVertexArrays(1, &m_vaoid);
    }

}

glm::vec3 Cylinder::getVertex(float theta, float y) {
    return glm::vec3(R * glm::cos(theta), y, R * glm::sin(theta));
}

glm::vec3 **Cylinder::getPositions() {
    if(m_is_tesselated) {
        return m_positions;
    }
    return NULL;
}

glm::vec3 **Cylinder::getNormals() {
    if(m_is_tesselated) {
        return m_normals;
    }
    return NULL;
}

GLfloat *Cylinder::getBuf() {
    return m_buf;
}

void Cylinder::tesselate(int one, int two, float three) {
    m_p1 = one > 2 ? one : 3;
    m_p2 = two > 0 ? two : 1;
    m_p3 = three;

    if(m_is_tesselated){
        delete[] m_buf;
        delete[] m_positions[0];
        delete[] m_positions[1];
        delete[] m_positions[2];
        delete[] m_positions;

        delete[] m_normals[0];
        delete[] m_normals[1];
        delete[] m_normals[2];
        delete[] m_normals;

        delete[] m_tangents[0];
        delete[] m_tangents[1];
        delete[] m_tangents[2];
        delete[] m_tangents;

        delete[] m_textures[0];
        delete[] m_textures[1];
        delete[] m_textures[2];
        delete[] m_textures;
    }
    //size of buf is top face + middle + bottom face all times six for two vectors per vertex
    m_buf = new GLfloat[3 * (m_p1 * m_p2 * 66)];
    m_positions = new glm::vec3*[CYL_FACES];
    m_positions[0] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_positions[1] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_positions[2] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_normals = new glm::vec3*[CYL_FACES];
    m_normals[0] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_normals[1] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_normals[2] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_tangents = new glm::vec3*[CYL_FACES];
    m_tangents[0] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_tangents[1] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_tangents[2] = new glm::vec3[(m_p1+1) * (m_p2+1)];
    m_textures = new glm::vec2*[CYL_FACES];
    m_textures[0] = new glm::vec2[(m_p1+1) * (m_p2+1)];
    m_textures[1] = new glm::vec2[(m_p1+1) * (m_p2+1)];
    m_textures[2] = new glm::vec2[(m_p1+1) * (m_p2+1)];

    m_is_tesselated = 1;
    int i,j;
    //top face, flat circle
    for(i = m_p2; i >= 0; i--) {
        float r = (1.0f - ((float)i/(float)m_p2)) / 2.0f;
        for(j = m_p1; j >= 0; j--){
            float theta = ((float)j/m_p1) * (2 * M_PI);
            m_positions[0][(i * (m_p1+1)) + j] = glm::vec3(r * glm::cos(theta), R, r * glm::sin(theta));
            m_normals[0][(i * (m_p1+1)) + j] = glm::vec3(0.0f, 1.0f, 0.0f);
            m_tangents[0][(i * (m_p1+1)) + j] = glm::vec3(1.0f, 0.0f, 0.0f);
            m_textures[0][(i * (m_p1+1)) + j] = glm::vec2(r*cos(theta) + 0.5f, -r*sin(theta) + 0.5f);
        }
    }


    //middle cylindrical part
    for(i = m_p2; i >= 0; i--) {
        float y = ((float)i / (float)m_p2) - R;
        for(j = m_p1; j >= 0; j--) {
            float theta = ((float)j/(float)m_p1) * (2 * M_PI);
            if(j == m_p1) {
                m_positions[1][(i * (m_p1+1)) + j] = getVertex(0.0f, y);
            } else {
                m_positions[1][(i * (m_p1+1)) + j] = getVertex(theta, y);
            }
            m_normals[1][(i * (m_p1+1)) + j] = getVertex(theta, y + R);
            m_normals[1][(i * (m_p1+1)) + j].y = 0.0f;
            m_tangents[1][(i * (m_p1+1)) + j] = glm::vec3(-sin(theta),0.0f,cos(theta));
            m_textures[1][(i * (m_p1+1)) + j] = glm::vec2(1 - theta/(2*M_PI), y + R);
        }
    }


    //bottom face, flat circle
    for(i = m_p2; i >= 0; i--) {
        float r = ((float)i/(float)m_p2) / 2.0f;
        for(j = m_p1; j >= 0; j--){
            float theta = ((float)j/m_p1) * (2 * M_PI);
            m_positions[2][(i * (m_p1+1)) + j] = glm::vec3(r * glm::cos(theta), -R, r * glm::sin(theta));
            m_normals[2][(i * (m_p1+1)) + j] = glm::vec3(0.0f, -1.0f, 0.0f);
            m_tangents[2][(i * (m_p1+1)) + j] = glm::vec3(1.0f, 0.0f, 0.0f);
            m_textures[2][(i * (m_p1+1)) + j] = glm::vec2(r*cos(theta) + 0.5f, r*sin(theta) + 0.5f);
        }
    }
    makeBuf();
}

void Cylinder::makeBuf() {
    int count = 0;
    int i,j;

    for(int face = 0; face < CYL_FACES; face++){
        for(i = 0; i < m_p2; i++) {
            for(j = 0; j < m_p1; j++) {
                glm::vec3 bottom_left = m_positions[face][(i * (m_p1+1)) + j];
                glm::vec3 bottom_left_n = m_normals[face][(i * (m_p1+1)) + j];
                glm::vec3 bottom_left_tan = m_tangents[face][(i * (m_p1+1)) + j];
                glm::vec2 bottom_left_tex = m_textures[face][(i * (m_p1+1)) + j];

                glm::vec3 top_left = m_positions[face][((i+1) * (m_p1+1)) + j];
                glm::vec3 top_left_n = m_normals[face][((i+1) * (m_p1+1)) + j];
                glm::vec3 top_left_tan = m_tangents[face][((i+1) * (m_p1+1)) + j];
                glm::vec2 top_left_tex = m_textures[face][((i+1) * (m_p1+1)) + j];

                glm::vec3 top_right = m_positions[face][((i+1) * (m_p1+1)) + j+1];
                glm::vec3 top_right_n = m_normals[face][((i+1) * (m_p1+1)) + j+1];
                glm::vec3 top_right_tan = m_tangents[face][((i+1) * (m_p1+1)) + j+1];
                glm::vec2 top_right_tex = m_textures[face][((i+1) * (m_p1+1)) + j+1];

                glm::vec3 bottom_right = m_positions[face][(i * (m_p1+1)) + j+1];
                glm::vec3 bottom_right_n = m_normals[face][(i * (m_p1+1)) + j+1];
                glm::vec3 bottom_right_tan = m_tangents[face][(i * (m_p1+1)) + j+1];
                glm::vec2 bottom_right_tex = m_textures[face][(i * (m_p1+1)) + j+1];

                /* For GL_TRIANGLES, the following diagram:
             * 1 --- 3,4
             * |    / |
             * |  /   |
             * 2,5 ---6old_position_worldSpace
             */

                //First triangle
                this->placeVertex(bottom_left, bottom_left_n, bottom_left_tan, bottom_left_tex, count);
                count += 11;
                this->placeVertex(top_left, top_left_n, top_left_tan, top_left_tex, count);
                count += 11;
                this->placeVertex(top_right, top_right_n, top_right_tan, top_right_tex, count);
                count += 11;

                //Second triangle
                this->placeVertex(top_right, top_right_n, top_right_tan, top_right_tex, count);
                count += 11;
                this->placeVertex(bottom_right, bottom_right_n, bottom_right_tan, bottom_right_tex, count);
                count += 11;
                this->placeVertex(bottom_left, bottom_left_n, bottom_left_tan, bottom_left_tex, count);
                count += 11;

            }
        }
    }
    m_num_vert = count / 11;
}
