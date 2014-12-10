#include "cone.h"
#include <iostream>
#include "math.h"

#define M_PI 3.14159265359
#define PARAM2 50
#define PARAM1 50

Cone::Cone()
{
    m_isInitialized = false;
}

Cone::Cone(const GLuint vertexLocation, const GLuint normalLocation){
    init(vertexLocation, normalLocation);
}

void Cone::init(const GLuint vertexLocation, const GLuint normalLocation){
    m_isInitialized = true;

    GLfloat *vertexData = new GLfloat[(PARAM2+2)*6+(2*PARAM1-2)*PARAM2*36+PARAM2*18];

    GLfloat a1[] = {0, 0, -0.5, 0, 0, -1};
    std::copy(a1, a1 + 6, vertexData);

    for (int i = 0; i<PARAM2+1; i++){

        GLfloat a1[] = {0.5*cos(-i*2*M_PI/(PARAM2))/PARAM1, 0.5*sin(-i*2*M_PI/(PARAM2))/PARAM1, -0.5, 0, 0, -1};
        std::copy(a1, a1 + 6, vertexData + i*6+6);
    }

    for (int j = 1; j<PARAM1; j++) {
        for (int i = 0; i<PARAM2+1; i++){

            GLfloat iSin = sin(i*2*M_PI/(PARAM2));
            GLfloat iCos = cos(i*2*M_PI/(PARAM2));
            GLfloat nextSin = sin((i+1)*2*M_PI/(PARAM2));
            GLfloat nextCos = cos((i+1)*2*M_PI/(PARAM2));

            GLfloat a1[] = {0.5*nextCos*(j*1.0/PARAM1), 0.5*nextSin*(j*1.0/PARAM1), -0.5, 0, 0, -1};
            GLfloat a2[] = {0.5*iCos*((j+1.0)/PARAM1), 0.5*iSin*((j+1.0)/PARAM1), -0.5, 0, 0, -1};
            GLfloat a3[] = {0.5*iCos*(j*1.0/PARAM1), 0.5*iSin*(j*1.0/PARAM1), -0.5, 0, 0, -1};
            GLfloat a5[] = {0.5*iCos*((j+1.0)/PARAM1), 0.5*iSin*((j+1.0)/PARAM1), -0.5, 0, 0, -1};
            GLfloat a4[] = {0.5*nextCos*((j+1.0)/PARAM1), 0.5*nextSin*((j+1.0)/PARAM1), -0.5, 0, 0, -1};
            GLfloat a6[] = {0.5*nextCos*(j*1.0/PARAM1), 0.5*nextSin*(j*1.0/PARAM1), -0.5, 0, 0, -1};
            std::copy(a1, a1 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36);
            std::copy(a2, a2 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36+6);
            std::copy(a3, a3 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36+12);
            std::copy(a4, a4 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36+18);
            std::copy(a5, a5 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36+24);
            std::copy(a6, a6 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*6+i*36+30);
        }
    }

    for (int i = 0; i<PARAM2; i++){
        GLfloat iSin = sin(i*2*M_PI/(PARAM2));
        GLfloat iCos = cos(i*2*M_PI/(PARAM2));
        GLfloat nextSin = sin((i+1)*2*M_PI/(PARAM2));
        GLfloat nextCos = cos((i+1)*2*M_PI/(PARAM2));

        GLfloat a1[] = {0, 0, 0.5, cos((i+0.5)*2*M_PI/(PARAM2))*(sqrt(3)/2.0), sin((i+0.5)*2*M_PI/(PARAM2))*(sqrt(3)/2.0), (1.0/2)};
        GLfloat a2[] = {0.5*iCos/PARAM1, 0.5*iSin/PARAM1, 0.5-(1.0/PARAM1), iCos*(sqrt(3)/2.0), iSin*(sqrt(3)/2.0), (1.0/2)};
        GLfloat a3[] = {0.5*nextCos/PARAM1, 0.5*nextSin/PARAM1, 0.5-(1.0/PARAM1), nextCos*(sqrt(3)/2), nextSin*(sqrt(3)/2), (1.0/2)};
        std::copy(a1, a1 + 6, vertexData + PARAM2*36*(PARAM1-1) + (PARAM2+2)*6+i*18);
        std::copy(a2, a2 + 6, vertexData + PARAM2*36*(PARAM1-1) + (PARAM2+2)*6+i*18+6);
        std::copy(a3, a3 + 6, vertexData + PARAM2*36*(PARAM1-1) + (PARAM2+2)*6+i*18+12);
    }

    for (int j = 0; j<PARAM1-1; j++) {
        for (int i = 0; i<PARAM2; i++){
            GLfloat normalX = cos((i)*2*M_PI/(PARAM2))*(sqrt(3)/2.0);
            GLfloat normalY = sin((i)*2*M_PI/(PARAM2))*(sqrt(3)/2.0);
            GLfloat normalZ = 1/2.0;
            GLfloat nextNormalX = cos((i+1.0)*2*M_PI/(PARAM2))*(sqrt(3)/2.0);
            GLfloat nextNormalY = sin((i+1.0)*2*M_PI/(PARAM2))*(sqrt(3)/2.0);

            GLfloat iSin = sin(i*2*M_PI/(PARAM2));
            GLfloat iCos = cos(i*2*M_PI/(PARAM2));
            GLfloat nextSin = sin((i+1)*2*M_PI/(PARAM2));
            GLfloat nextCos = cos((i+1)*2*M_PI/(PARAM2));

            GLfloat a1[] = {(0.5-(j*1.0/(2*PARAM1)))*nextCos, (0.5-(j*1.0/(2*PARAM1)))*nextSin, 0.5-(1-(j*1.0)/PARAM1), nextNormalX, nextNormalY, normalZ};
            GLfloat a3[] = {(0.5-(j*1.0/(2*PARAM1)))*iCos, (0.5-(j*1.0/(2*PARAM1)))*iSin, 0.5-(1-(j*1.0)/PARAM1), normalX, normalY, normalZ};
            GLfloat a2[] = {(0.5-((j+1.0)/(2*PARAM1)))*iCos, (0.5-((j+1.0)/(2*PARAM1)))*iSin, 0.5-(1-(j+1.0)/PARAM1), normalX, normalY, normalZ};
            GLfloat a5[] = {(0.5-((j+1.0)/(2*PARAM1)))*iCos, (0.5-((j+1.0)/(2*PARAM1)))*iSin, 0.5-(1-(j+1.0)/PARAM1), normalX, normalY, normalZ};
            GLfloat a4[] = {(0.5-((j+1.0)/(2*PARAM1)))*nextCos, (0.5-((j+1.0)/(2*PARAM1)))*nextSin, 0.5-(1-(j+1.0)/PARAM1), nextNormalX, nextNormalY, normalZ};
            GLfloat a6[] = {(0.5-(j*1.0/(2*PARAM1)))*nextCos, (0.5-(j*1.0/(2*PARAM1)))*nextSin, 0.5-(1-(j*1.0)/PARAM1), nextNormalX, nextNormalY, normalZ};
            std::copy(a1, a1 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36);
            std::copy(a2, a2 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36+6);
            std::copy(a3, a3 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36+12);
            std::copy(a4, a4 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36+18);
            std::copy(a5, a5 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36+24);
            std::copy(a6, a6 + 6, vertexData + PARAM2*18 + PARAM2*36*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*6+i*36+30);
        }
    }

    // VAO init
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Vertex buffer init
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint)*6*((PARAM2+2)*2+(3*PARAM1-2)*PARAM2*6), vertexData, GL_STATIC_DRAW);

    // Expose vertices to shader
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(
       vertexLocation,
       3,                  // num vertices per element (3 for triangle)
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       sizeof(GLfloat) * 6,                  // stride
       (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(normalLocation);
    glVertexAttribPointer(
       normalLocation,
       3,                  // num vertices per element (3 for triangle)
       GL_FLOAT,           // type
       GL_TRUE,           // normalized?
       sizeof(GLfloat) * 6,                  // stride
       (void*)(sizeof(GLfloat) * 3)            // array buffer offset
    );

    //Clean up -- unbind things
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Cone::draw(){
    if (!m_isInitialized){
        std::cout << "You must call init() before you can draw!" << std::endl;
    } else{
        // Rebind your vertex array and draw the triangles
        glBindVertexArray(m_vaoID);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (PARAM2+2) /* Number of vertices to draw */);
        glDrawArrays(GL_TRIANGLES, (PARAM2+2), (2*PARAM1-2)*PARAM2*6+PARAM2*3 /* Number of vertices to draw */);
        glBindVertexArray(0);
    }
}
