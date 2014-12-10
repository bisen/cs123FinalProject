#ifndef VIEW_H
#define VIEW_H

#include <QTime>
#include <QTimer>
#include "GL/glew.h"
#include <qgl.h>
#include <QGLWidget>
#include <CS123Common.h>

#include "sphere.h"
#include "cone.h"
#include "lib/transforms.h"
#include "camera.h"

#include "plant.h"
#include "shape.h"
#include "cylinder.h"
#include "ivy.h"

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void updateCamera();

    LabCamera m_camera;
    Transforms m_transform;
    Sphere m_sphere;
    Cone m_cone;
    float m_fps;
    float m_increment;
    GLuint m_shader;
    GLfloat m_param_x_1;
    GLfloat m_param_y_1;
    GLfloat m_size_1 = 1.0;
    GLfloat m_param_x_2;
    GLfloat m_param_y_2;
    GLfloat m_size_2 = 1.0;
    GLfloat m_param_x_3;
    GLfloat m_param_y_3;
    GLfloat m_size_3 = 1.0;
    GLfloat m_param_x_4;
    GLfloat m_param_y_4;
    GLfloat m_size_4 = 1.0;
    GLfloat m_dir_theta = -1.0;
    GLfloat m_dir_y = 1.0;
    GLfloat m_pos_y;

    Cylinder *c;

    Plant *m_plant;

    GLuint m_plantshader;

    float m_theta, m_phi;

private slots:
    void tick();
};

#endif // VIEW_H

