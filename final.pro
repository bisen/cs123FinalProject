QT += core gui opengl
TARGET = final
TEMPLATE = app
# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib
DEPENDPATH += glm lib
SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    cone.cpp \
    lib/ResourceLoader.cpp \
    lib/transforms.cpp \
    plant.cpp \
    lsys.cpp \
    cylinder.cpp \
    shape.cpp \
    ivy.cpp \
    skybox.cpp \
    level.cpp


HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    ivy.h \
    plant.h \
    lsys.h \
    cylinder.h \
    shape.h \
    ivy.h \
    skybox.h \
    lib/transforms.h \
    level.h

FORMS += mainwindow.ui
OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert \
    shaders/plant.vert \
    shaders/plant.frag \
    shaders/skybox.vert \
    shaders/skybox.frag \
    assets/PosZ.png \
    assets/PosY.png \
    assets/PosX.png \
    assets/NegZ.png \
    assets/NegY.png \
    assets/NegX.png
RESOURCES += \
shaders/shaders.qrc
LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include

