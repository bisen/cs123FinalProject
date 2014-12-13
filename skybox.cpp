#include "skybox.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &m_textureObj);
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_bufid);
}

void Skybox::init(const GLuint vertexLocation,
                  const QString &PosXFilename,
                  const QString &NegXFilename,
                  const QString &PosYFilename,
                  const QString &NegYFilename,
                  const QString &PosZFilename,
                  const QString &NegZFilename)
{
    QImage PosXImage = loadImage(PosXFilename);
    QImage NegXImage = loadImage(NegXFilename);
    QImage PosYImage = loadImage(PosYFilename);
    QImage NegYImage = loadImage(NegYFilename);
    QImage PosZImage = loadImage(PosZFilename);
    QImage NegZImage = loadImage(NegZFilename);

    // Generate a new OpenGL texture ID to put our image into
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_textureObj);
    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, m_textureObj);

    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, PosXImage.width(), PosXImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, PosXImage.bits());
    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, NegXImage.width(), NegXImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NegXImage.bits());
    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, PosYImage.width(), PosYImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, PosYImage.bits());
    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, NegYImage.width(), NegYImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NegYImage.bits());
    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, PosZImage.width(), PosZImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, PosZImage.bits());
    glTexImage2D (
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, NegZImage.width(), NegZImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NegZImage.bits());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    m_isInitialized = true;

    GLfloat vertexData[] = {
        -10.0f,  10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        -10.0f,  10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f,  10.0f
    };

    // VAO init
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Vertex buffer init
    glGenBuffers(1, &m_bufid);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufid);

    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*36, &vertexData, GL_STATIC_DRAW);

    // Expose vertices to shader
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(
       vertexLocation,
       3,                  // num vertices per element (3 for triangle)
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       NULL            // array buffer offset
    );

    //Clean up -- unbind things
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

QImage Skybox::loadImage(const QString &filename) {
    QImage image;
    QFile file(filename);
    assert(file.exists());
    image.load(file.fileName());
    image = image.mirrored(false, true);
    return QGLWidget::convertToGLFormat(image);
}

void Skybox::draw()
{
    glDepthMask (GL_FALSE);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, m_textureObj);
    glBindVertexArray (m_vaoID);
    glDrawArrays (GL_TRIANGLES, 0, 36);
    glDepthMask (GL_TRUE);
}
