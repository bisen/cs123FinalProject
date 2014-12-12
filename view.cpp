#include "lib/ResourceLoader.h"
#include "view.h"
#include <QApplication>
#include <QKeyEvent>

View::View(QWidget *parent) : QGLWidget(parent), m_cylinder(Cylinder(50,50,0)),m_cone(Cone(50,50,0)), m_level(Level(&m_cylinder, &m_cone))
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    setCursor(Qt::BlankCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // Set up the camera
    m_camera.eye.x = 0.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = 0.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 0.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 1.0f, m_camera.up.z = 0.0f;
    m_camera.fovy = 70.0f, m_camera.near = 0.1f, m_camera.far = 1000.0f;

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));

    m_theta = 0.0f;
    m_phi = M_PI / 2.0f;

//    m_plant = new Ivy();

    m_param_x_1 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_param_y_1 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_size_1 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));

    m_param_x_2 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_param_y_2 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_size_2 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));

    m_param_x_3 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_param_y_3 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_size_3 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));

    m_param_x_4 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_param_y_4 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    m_size_4 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
}

View::~View()
{
//    delete m_plant;
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / 60);

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    m_skyboxShader = ResourceLoader::loadShaders(":/shaders/skybox.vert", ":/shaders/skybox.frag");
    m_plantshader = ResourceLoader::loadShaders(":/shaders/plant.vert", ":/shaders/plant.frag");
    m_shader = ResourceLoader::loadShaders(":/shaders/shader.vert", ":/shaders/shader.frag");

    m_skybox.init(glGetAttribLocation(m_shader, "position"), "assets/PosX.png","assets/NegX.png","assets/PosZ.png","assets/NegZ.png","assets/PosY.png","assets/NegY.png");
    m_cylinder.tesselate(50,50,0);
    m_cylinder.init(glGetAttribLocation(m_shader, "position"),glGetAttribLocation(m_shader, "normal"),glGetAttribLocation(m_shader, "tangent"),glGetAttribLocation(m_shader, "texCoord"));
    m_cone.tesselate(50,50,0);
    m_cone.init(glGetAttribLocation(m_shader, "position"),glGetAttribLocation(m_shader, "normal"),glGetAttribLocation(m_shader, "tangent"),glGetAttribLocation(m_shader, "texCoord"));


    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Load the initial settings
    updateCamera();

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.2D
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    //m_plant->parseSystem(15, glGetAttribLocation(m_shader, "position"), glGetAttribLocation(m_shader, "normal"), glGetAttribLocation(m_shader, "tangent"), glGetAttribLocation(m_shader, "texCoord"));

    QImage bumpMap;
    bumpMap.load(QString::fromStdString("assets/heightmapalt.png"));
    bumpMap = QGLWidget::convertToGLFormat(bumpMap);

    // Generate a new OpenGL texture ID to put our image into
    glActiveTexture(GL_TEXTURE1);
    m_bump_map_id = 0;
    glGenTextures(1, &m_bump_map_id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, m_bump_map_id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bumpMap.width(), bumpMap.height(), GL_RGBA, GL_UNSIGNED_BYTE, bumpMap.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void View::paintGL()
{
    // Get the time in seconds
    m_pos_y += (m_dir_y / (float) 60);

    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw skybox
    glUseProgram(m_skyboxShader);
    glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader, "v"), 1, GL_FALSE, &m_transform.getTransform()[0][0]);
    m_skybox.draw();
    glUseProgram(0);

    float d1 = (m_pos_y<0?-1:1)*2-fmod(m_pos_y,4);
    float d2 = (m_pos_y+1<0?-1:1)*2-fmod(m_pos_y+1,4);
    float d3 = (m_pos_y+2<0?-1:1)*2-fmod(m_pos_y+2,4);
    float d4 = (m_pos_y+3<0?-1:1)*2-fmod(m_pos_y+3,4);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "useLighting"), GL_TRUE);
    glUniform3f(glGetUniformLocation(m_shader, "ambient_color"), 0.2, 0.2, 0.2);
    glUniform3f(glGetUniformLocation(m_shader, "lightPosition_worldSpace"), 3.0, 1.0, 3.0);
    glUniform1i(glGetUniformLocation(m_shader, "smoothShading"), GL_TRUE);
    glUniform1i(glGetUniformLocation(m_shader, "tex"), 1);
    glUniform1i(glGetUniformLocation(m_shader, "useTexture"), 1);
    glUniform1i(glGetUniformLocation(m_shader, "textureWidth"), 150);
    glUniform1i(glGetUniformLocation(m_shader, "textureHeight"), 52);
    glUniform1i(glGetUniformLocation(m_shader, "useCelShading"), GL_TRUE);

    glUseProgram(0);

//    glUseProgram(m_plantshader);
//    glUniform1i(glGetUniformLocation(m_plantshader, "useLighting"), GL_TRUE);
//    glUniform3f(glGetUniformLocation(m_plantshader, "ambient_color"), 0.2, 0.2, 0.2);
//    glUniform3f(glGetUniformLocation(m_plantshader, "lightPosition_worldSpace"), 3.0, 1.0, 3.0);
//    glUniform1i(glGetUniformLocation(m_plantshader, "smoothShading"), GL_TRUE);
//    Transforms cylindertrans = m_transform;
//    cylindertrans.model = glm::translate(glm::mat4x4(), glm::vec3(0.0f, -1.0f, 0.0f));

//    glUniform1f(glGetUniformLocation(m_plantshader, "radius"), 2.0f);


//    glUniform3f(glGetUniformLocation(m_plantshader, "color"), 0, 1, 0);
//    glUniformMatrix4fv(glGetUniformLocation(m_plantshader, "v"), 1, GL_FALSE, &cylindertrans.view[0][0]);

//    m_plant->render(m_plantshader, cylindertrans);


//    glUseProgram(0);

    glUseProgram(m_shader);

    ///////////layer 1
    m_level.draw(m_shader, d1, m_param_x_1, m_param_y_1, m_size_1, m_transform);
    ///////////layer 2
    m_level.draw(m_shader, d2, m_param_x_2, m_param_y_2, m_size_2, m_transform);
    ///////////layer 3
    m_level.draw(m_shader, d3, m_param_x_3, m_param_y_3, m_size_3, m_transform);
    ///////////layer 4
    m_level.draw(m_shader, d4, m_param_x_4, m_param_y_4, m_size_4, m_transform);

    m_camera.eye[0] = 1.4*sin(2*m_pos_y/M_PI);
    m_camera.eye[2] = 1.4*cos(2*m_pos_y/M_PI);
    if (d1 >1.9) {
        m_param_x_1 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_param_y_1 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_size_1 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    }

    if (d2 >1.9) {
        m_param_x_2 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_param_y_2 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_size_2 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    }

    if (d3 >1.9) {
        m_param_x_3 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_param_y_3 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_size_3 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    }

    if (d4 >1.9) {
        m_param_x_4 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_param_y_4 = static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
        m_size_4 = 0.7 + static_cast <float> (rand()/6) / (static_cast <float> (RAND_MAX));
    }

    m_theta+=(m_dir_theta/(float) 60);
    m_camera.eye.y = 0;
    m_camera.eye.x = 1.5*sin(2*m_theta/M_PI);
    m_camera.eye.z = 1.5*cos(2*m_theta/M_PI);
    m_camera.center.x = -sin(2*m_theta/M_PI);
    m_camera.center.z = -cos(2*m_theta/M_PI);
    m_camera.center.y = 0;
    updateCamera();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    updateCamera();
}

void View::mousePressEvent(QMouseEvent *event)
{
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;
    if (!deltaX && !deltaY) return;
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    m_dir_theta=max(min(m_dir_theta+((float) deltaX/80.0),4.0),-4.0);
    m_dir_y=max(min(m_dir_y-((float) deltaY/240.0),1.8),-1.8);
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
    switch(event->key()) {
    case Qt::Key_Up:
        //m_phi += -M_PI / 24;
        m_dir_y++;
        break;
    case Qt::Key_Down:
        //m_phi += M_PI / 24;
        m_dir_y--;
        break;
    case Qt::Key_Left:
        //m_theta += -M_PI / 24;
        m_dir_theta--;
        break;
    case Qt::Key_Right:
        //m_theta += M_PI / 24;
        m_dir_theta++;
        break;
    }

    //m_camera.eye = 5.0f * glm::vec3(glm::cos(m_theta) * glm::sin(m_phi), glm::cos(m_phi), glm::sin(m_theta) * glm::sin(m_phi));
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::updateCamera()
{
    float w = width();
    float h = height();
    float aspectRatio = 1.0f * w / h;
    if (aspectRatio > 0) m_transform.projection = glm::perspective(m_camera.fovy, aspectRatio, m_camera.near, m_camera.far);
    if (aspectRatio > 0) m_transform.view = glm::lookAt(m_camera.eye, m_camera.center, m_camera.up);
}

//void View::drawCylinder()
//{

//}
