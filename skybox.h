#ifndef SKYBOX_H
#define SKYBOX_H

#include <CS123Common.h>
#include "GL/glew.h"
#include <QGLWidget>
#include <QMenuBar>
#include <GL/glu.h>
#include <QFile>

class Skybox
{
public:
    Skybox();
    ~Skybox();
    void init(const GLuint vertexLocation, const QString &PosXFilename,
              const QString &NegXFilename,
              const QString &PosYFilename,
              const QString &NegYFilename,
              const QString &PosZFilename,
              const QString &NegZFilename);
    void draw();
private:
    bool m_isInitialized;
    GLuint m_vaoID;
    GLuint m_textureObj;
    QImage loadImage(const QString &filename);
};

#endif // SKYBOX_H
