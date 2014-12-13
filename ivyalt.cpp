#include "ivyalt.h"

IvyAlt::IvyAlt()
{
    m_scenegraph = new QList<Node>();
    m_initialized = 0;
    m_cyl = new Cylinder(15,15,0);

    m_system = new LSys();
    Rule *r1 = new Rule('x', "ab");
    //Sometimes three branches, sometimes two
    Rule *r2 = new StochasticRule('b', "+x", "+a[+x][-x][~x]", 0.80f);
    Rule *r3 = new StochasticRule('b', "+x", "+a[+x][-x]", 0.90f);

    //Sometimes the vine wiggles the other direction
    Rule *r4 = new StochasticRule('+', "$", "+", 0.10f);

    m_system->addRule(r1);
    m_system->addRule(r2);
    m_system->addRule(r3);
    m_system->addRule(r4);

    m_initial = new char[2];
    m_initial[0] = 'x';
    m_initial[1] = '\0';

    m_level = 8;
}

int IvyAlt::parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation) {
    m_factor = 1.0f;
    char *lsys = m_system->generate(m_level, m_initial);
    int syslength = strlen(lsys);

    glm::mat4 rotation = glm::mat4x4();
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 rotation_plus = glm::rotate(glm::mat4(1.0f), (float)M_PI / 12.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation_minus = glm::rotate(glm::mat4(1.0f), (float)-M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation_tilde = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation_dollar = glm::rotate(glm::mat4(1.0f), (float)-M_PI / 12.0f, glm::vec3(0.0f, 0.0f, 1.0f));




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
            m_scenegraph->append(n);
            break;
        case '+':
            rotation = rotation_plus * rotation;
            break;
        case '-':
            rotation = rotation_minus * rotation;
            break;
        case '~':
            rotation = rotation_tilde * rotation;
            break;
        case '$':
            rotation = rotation_dollar * rotation;
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
    this->init(vertexLocation, normalLocation, tangentLocation, textureLocation);

    delete[] lsys;
    return 1;
}
