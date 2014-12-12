#include "plant.h"

Plant::Plant()
{
    m_scenegraph = new QList<Node>();
    m_initialized = 0;
    m_cyl = new Cylinder(50,50,0);

    m_system = new LSys();
    char *ab = "ab";
    Rule *r1 = new StochasticRule('x', "a[-x][+x][~x]", "0", 0.70f);
    Rule *r2 = new Rule('a', "aa");

    m_system->addRule(r1);
    m_system->addRule(r2);

    m_initial = new char[2];
    m_initial[0] = 'x';
    m_initial[1] = '\0';
}

Plant::~Plant() {
    delete m_scenegraph;
    delete m_cyl;
    delete[] m_initial;
}

void Plant::parseSystem(int level, GLuint vertexLocation, GLuint normalLocation, GLuint tangentLocation, GLuint textureLocation) {
    m_factor = 1.0f / pow(1.33f, (float)level);

    glm::vec3 color1 = glm::vec3(0.408f, 0.259f, 0.102f); //Brown
    glm::vec3 color2 = glm::vec3(0.161f, 0.647f, 0.29f) / 2.0f; //Green
    glm::vec3 colordelta = color2 - color1; //vector in direction of color


    char *lsys = m_system->generate(level, m_initial);
    int syslength = strlen(lsys);
    QStack<glm::mat4> rotationstack = QStack<glm::mat4>();
    QStack<glm::mat4> scalestack = QStack<glm::mat4>();
    QStack<glm::vec3> tstack = QStack<glm::vec3>();

    glm::vec3 translation = glm::vec3(0.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_factor, m_factor * 2.0f, m_factor));

    glm::mat4 scalemat = glm::scale(glm::mat4(1.0f), glm::vec3(0.70f, 1.0f, 0.70f));

    int currentlevel = 0;

    m_scenegraph->clear();
    for(int i = 0; i < syslength; i++) {
        Node n;
        switch(lsys[i]) {
        case 'a':
            n.model = glm::translate(glm::mat4(1.0f), translation) * rotation * scale;
            translation = translation + (glm::vec3(rotation * scale * glm::vec4(0.0f, m_factor, 0.0f, 0.0f)));
            n.color = color1 + ((colordelta / (float)level) * (float)currentlevel);
//            scale = scale * scalemat;
            m_scenegraph->append(n);
            break;
        case '+':
            rotation = glm::rotate(rotation, (float)M_PI / 6.0f, glm::vec3(0.0f, 0.0f, -1.0f));
            break;
        case '-':
            rotation = glm::rotate(rotation, (float)M_PI / 6.0f, glm::vec3(-0.5f, 0.0f, 0.5f));
            break;
        case '~':
            rotation = glm::rotate(rotation, (float)M_PI / 6.0f, glm::vec3(0.5f, 0.0f, 0.5f));
            break;
        case '[':
            rotationstack.push(rotation);
            tstack.push(translation);
            scalestack.push(scale);
            scale = scale * scalemat;
            currentlevel++;
            break;
        case ']':
            rotation = rotationstack.pop();
            translation = tstack.pop();
            scale = scalestack.pop();
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

void Plant::render(GLuint shader, Transforms t) {

    Transforms planttrans = t;


    int s = m_scenegraph->size();

    for(int i = 0; i < s; i++) {
        Node n = m_scenegraph->at(i);

        planttrans.model = t.model * n.model;

        glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, &planttrans.getTransform()[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, &planttrans.model[0][0]);
        glUniform3f(glGetUniformLocation(shader, "color"), n.color.r, n.color.g, n.color.b);

        m_cyl->draw();

    }
}
