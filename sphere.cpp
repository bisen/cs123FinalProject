#include "sphere.h"
#include <iostream>
#include "math.h"

#define M_PI 3.14159265359
#define PARAM2 50
#define PARAM1 50

Sphere::Sphere()
{
    m_isInitialized = false;
}

Sphere::Sphere(const GLuint vertexLocation, const GLuint normalLocation){
    init(vertexLocation, normalLocation);
}

void Sphere::init(const GLuint vertexLocation, const GLuint normalLocation){
    m_isInitialized = true;

    GLfloat *vertexData = new GLfloat[(PARAM2+2)*6*2+(3*PARAM1-2)*PARAM2*36];

    GLfloat a1[] = {0, 0, 0.5, 0, 0, 1};
    GLfloat a2[] = {0, 0, -0.5, 0, 0, -1};
    std::copy(a1, a1 + 6, vertexData);
    std::copy(a2, a2 + 6, vertexData + (PARAM2+2)*6);

    for (int i = 0; i<PARAM2+1; i++){


        GLfloat a1[] = {0.5*cos(i*2*M_PI/(PARAM2))/PARAM1, 0.5*sin(i*2*M_PI/(PARAM2))/PARAM1, 0.5, 0, 0, 1};
        GLfloat a2[] = {0.5*cos(-i*2*M_PI/(PARAM2))/PARAM1, 0.5*sin(-i*2*M_PI/(PARAM2))/PARAM1, -0.5, 0, 0, -1};
        std::copy(a1, a1 + 6, vertexData + 6+i*6);
        std::copy(a2, a2 + 6, vertexData + (PARAM2+2+i)*6+6);
    }

    for (int j = 1; j<PARAM1; j++) {
        for (int i = 0; i<PARAM2+1; i++){

            GLfloat iSin = sin(i*2*M_PI/(PARAM2));
            GLfloat iCos = cos(i*2*M_PI/(PARAM2));
            GLfloat nextSin = sin((i+1)*2*M_PI/(PARAM2));
            GLfloat nextCos = cos((i+1)*2*M_PI/(PARAM2));

            GLfloat a1[] = {0.5*nextCos*(j*1.0/PARAM1), 0.5*nextSin*(j*1.0/PARAM1), 0.5, 0, 0, 1};
            GLfloat a3[] = {0.5*iCos*((j+1.0)/PARAM1), 0.5*iSin*((j+1.0)/PARAM1), 0.5, 0, 0, 1};
            GLfloat a2[] = {0.5*iCos*(j*1.0/PARAM1), 0.5*iSin*(j*1.0/PARAM1), 0.5, 0, 0, 1};
            GLfloat a4[] = {0.5*iCos*((j+1.0)/PARAM1), 0.5*iSin*((j+1.0)/PARAM1), 0.5, 0, 0, 1};
            GLfloat a5[] = {0.5*nextCos*((j+1.0)/PARAM1), 0.5*nextSin*((j+1.0)/PARAM1), 0.5, 0, 0, 1};
            GLfloat a6[] = {0.5*nextCos*(j*1.0/PARAM1), 0.5*nextSin*(j*1.0/PARAM1), 0.5, 0, 0, 1};
            std::copy(a1, a1 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36);
            std::copy(a2, a2 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+6);
            std::copy(a3, a3 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+12);
            std::copy(a4, a4 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+18);
            std::copy(a5, a5 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+24);
            std::copy(a6, a6 + 6, vertexData + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+30);
        }
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
            GLfloat a4[] = {0.5*iCos*((j+1.0)/PARAM1), 0.5*iSin*((j+1.0)/PARAM1), -0.5, 0, 0, -1};
            GLfloat a6[] = {0.5*nextCos*((j+1.0)/PARAM1), 0.5*nextSin*((j+1.0)/PARAM1), -0.5, 0, 0, -1};
            GLfloat a5[] = {0.5*nextCos*(j*1.0/PARAM1), 0.5*nextSin*(j*1.0/PARAM1), -0.5, 0, 0, -1};
            std::copy(a1, a1 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36);
            std::copy(a2, a2 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+6);
            std::copy(a3, a3 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+12);
            std::copy(a4, a4 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+18);
            std::copy(a5, a5 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+24);
            std::copy(a6, a6 + 6, vertexData + PARAM2*36*(PARAM1-1) + PARAM2*36*(j-1)+(PARAM2+2)*12+i*36+30);
        }
    }

    for (int j = 0; j<PARAM1; j++) {
        for (int i = 0; i<PARAM2; i++){
            GLfloat iSin = sin(i*2*M_PI/(PARAM2));
            GLfloat iCos = cos(i*2*M_PI/(PARAM2));
            GLfloat nextSin = sin((i+1)*2*M_PI/(PARAM2));
            GLfloat nextCos = cos((i+1)*2*M_PI/(PARAM2));

            GLfloat a1[] = {0.5*nextCos, 0.5*nextSin, 0.5-(j*1.0/PARAM1), nextCos, nextSin, 0};
            GLfloat a2[] = {0.5*iCos, 0.5*iSin, 0.5-(j*1.0/PARAM1), iCos, iSin, 0};
            GLfloat a3[] = {0.5*iCos, 0.5*iSin, 0.5-((j+1.0)/PARAM1), iCos, iSin, 0};
            GLfloat a4[] = {0.5*iCos, 0.5*iSin, 0.5-((j+1.0)/PARAM1), iCos, iSin, 0};
            GLfloat a5[] = {0.5*nextCos, 0.5*nextSin, 0.5-((j+1.0)/PARAM1), nextCos, nextSin, 0};
            GLfloat a6[] = {0.5*nextCos, 0.5*nextSin, 0.5-(j*1.0/PARAM1), nextCos, nextSin, 0};
            std::copy(a1, a1 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36);
            std::copy(a2, a2 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36+6);
            std::copy(a3, a3 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36+12);
            std::copy(a4, a4 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36+18);
            std::copy(a5, a5 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36+24);
            std::copy(a6, a6 + 6, vertexData + PARAM2*72*(PARAM1-1) + PARAM2*36*(j) + (PARAM2+2)*12+i*36+30);
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

void Sphere::draw(){
    if (!m_isInitialized){
        std::cout << "You must call init() before you can draw!" << std::endl;
    } else{
        // Rebind your vertex array and draw the triangles
        glBindVertexArray(m_vaoID);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (PARAM2+2) /* Number of vertices to draw */);
        glDrawArrays(GL_TRIANGLE_FAN, (PARAM2+2), (PARAM2+2) /* Number of vertices to draw */);
        glDrawArrays(GL_TRIANGLES, 2*(PARAM2+2), 6*(3*PARAM1-2)*PARAM2 /* Number of vertices to draw */);
        glBindVertexArray(0);
    }
}
