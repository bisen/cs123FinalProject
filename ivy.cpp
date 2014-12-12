#include "ivy.h"

Ivy::Ivy()
{
    m_scenegraph = new QList<Node>();
    m_initialized = 0;
    m_cyl = new Cylinder(50,50,0);

    m_system = new LSys();
    Rule *r1 = new Rule('x', "ab");
    Rule *r2 = new StochasticRule('b', "+x", "+a[+x][-x]", 0.5f);

    m_system->addRule(r1);
    m_system->addRule(r2);

    m_initial = new char[2];
    m_initial[0] = 'x';
    m_initial[1] = '\0';
}

void Ivy::parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation) {
    m_factor = 1.0f;// / pow(1.33f, (float)level);

    glm::vec3 color1 = glm::vec3(0.408f, 0.259f, 0.102f); //Brown
    glm::vec3 color2 = glm::vec3(0.161f, 0.647f, 0.29f) / 2.0f; //Green
    glm::vec3 colordelta = color2 - color1; //vector in direction of color


    char *lsys = m_system->generate(level, m_initial);
    int syslength = strlen(lsys);

    glm::mat4 rotation = glm::mat4x4();// glm::rotate(glm::mat4(1.0f), (float)-M_PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 rotation_plus = glm::rotate(glm::mat4(1.0f), (float)M_PI / 12.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation_minus = glm::rotate(glm::mat4(1.0f), (float)-M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));


    QStack<glm::mat4> rot_stack = QStack<glm::mat4>();
    QStack<glm::vec3> trans_stack = QStack<glm::vec3>();

    int currentlevel = 0;

    m_scenegraph->clear();
    for(int i = 0; i < syslength; i++) {
        Node n;
        switch(lsys[i]) {
        case 'a':
            n.model = glm::translate(glm::mat4(1.0f), translation) * rotation * glm::rotate(glm::mat4(1.0f), (float)-M_PI / 2, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 1.05f, 0.25f));
            translation = translation + glm::vec3(rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            printVertex(glm::vec4(translation, 1.0f));
            n.color = color2;
            m_scenegraph->append(n);
            break;
        case '+':
            rotation = rotation_plus * rotation;
            break;
        case '-':
            rotation = rotation_minus * rotation;
            break;
        case '[':
            rot_stack.push(rotation);
            trans_stack.push(translation);
            currentlevel++;
            break;
        case ']':
            rotation = rot_stack.pop();
            translation = trans_stack.pop();
            currentlevel--;
            break;
        default:
            break;
        }
    }

    m_cyl->tesselate(15,15,0);
    m_cyl->init(vertexLocation, normalLocation, tangentLocation, textureLocation);
    m_initialized = 1;
}
