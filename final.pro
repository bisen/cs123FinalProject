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
    Cylinder.cpp \
    cone.cpp \
    plant.cpp \
    lsys.cpp \
    Shape.cpp \
    lib/ResourceLoader.cpp \
    lib/transforms.cpp


HEADERS += mainwindow.h \
    view.h \
    lib/ResourceLoader.h

FORMS += mainwindow.ui

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert

RESOURCES += \
    shaders/shaders.qrc

LIBS += -L/course/cs123/lib/glew/glew-1.10.0/include -lGLEW
INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include
