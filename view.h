#ifndef VIEW_H
#define VIEW_H

#include <QTime>
#include <QTimer>
#include "GL/glew.h"
#include <qgl.h>
#include <QGLWidget>
#include <CS123Common.h>
#include "lib/transforms.h"
#include "camera.h"
#include "skybox.h"

#include "shape.h"
#include "level.h"
#include "cone.h"
#include "cylinder.h"

#include "plant.h"
#include "ivy.h"
#include "ivyalt.h"

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

    GLuint m_skyboxShader;
    Skybox m_skybox;

    LabCamera m_camera;
    Transforms m_transform;

    Cylinder m_cylinder;
    Cone m_cone;
    Level m_level1;
    Level m_level2;
    Level m_level3;
    Level m_level4;

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

    Plant **m_plant;

    GLuint m_plantshader;
    GLuint m_bump_map_id;
    GLuint m_plant_tex_id;

    float m_theta, m_phi;
    bool m_use_cel_shading = true;
    bool m_blend_texture = true;
    bool m_use_bump_mapping = true;

    bool l1_mid;
    bool l2_mid;
    bool l3_mid;
    bool l4_mid;

    int m_ivy_index;

private slots:
    void tick();
};

#endif // VIEW_H

