QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm lib
DEPENDPATH += glm lib

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    sphere.cpp \
    lib/ResourceLoader.cpp \
    lib/transforms.cpp \
    plant.cpp \
    lsys.cpp \
    cylinder.cpp \
    shape.cpp \
    ivy.cpp


HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h \
    plant.h \
    lsys.h \
    cylinder.h \
    shape.h \
    ivy.h

FORMS += mainwindow.ui

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert \
    shaders/plant.vert \
    shaders/plant.frag

RESOURCES += \
    shaders/shaders.qrc

LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include
