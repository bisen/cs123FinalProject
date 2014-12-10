#include "plant.h"

Plant::Plant()
{
    m_scenegraph = new QList<Node>();
    m_initialized = 0;
    m_cyl = new Cylinder(50,50,0);

    m_system = new LSys();
    char *ab = "ab";
    Rule *r1 = new Rule('x', "a[-x][+x]");
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

void Plant::parseSystem(int level, GLuint vertexLocation, GLuint normalLocation) {
    m_factor = 1.0f / pow(1.9f, (float)level);
    char *lsys = m_system->generate(level, m_initial);
    int syslength = strlen(lsys);
    QStack<glm::mat3> modelstack = QStack<glm::mat3>();
    QStack<glm::vec3> tstack = QStack<glm::vec3>();
    glm::vec3 translation = glm::vec3(0.0f);
    glm::mat3 model = glm::mat3(1.0f);

    glm::mat3 plusmatrix = glm::mat3x3();
    glm::mat3 minusmatrix = glm::mat3x3();

    plusmatrix[0] = glm::vec3(glm::cos((float)M_PI / 6.0f), glm::sin((float)M_PI / 6.0f), 0.0f);
    plusmatrix[1] = glm::vec3(-glm::sin((float)M_PI / 6.0f), glm::cos((float)M_PI / 6.0f), 0.0f);
    plusmatrix[2] = glm::vec3(0.0f, 0.0f, 1.0f);

    minusmatrix[0] = glm::vec3(glm::cos((float)-M_PI / 6.0f), glm::sin((float)-M_PI / 6.0f), 0.0f);
    minusmatrix[1] = glm::vec3(-glm::sin((float)-M_PI / 6.0f), glm::cos((float)-M_PI / 6.0f), 0.0f);
    minusmatrix[2] = glm::vec3(0.0f, 0.0f, 1.0f);

    m_scenegraph->clear();
    for(int i = 0; i < syslength; i++) {
        Node n;
        switch(lsys[i]) {
        case 'a':
//            n.model = glm::translate(glm::mat4(1.0f), translation) * model;
//            translation = translation + glm::vec3(model * glm::vec4(0.0f, 0.1f, 0.0f, 0.0f));
            n.rotation = model;
            n.translation = translation;
            translation = translation + (model * glm::vec3(0.0f, m_factor, 0.0f));
            m_scenegraph->append(n);
            break;
        case '+':
            model = model * plusmatrix;

            break;
        case '-':
            model = model * minusmatrix;
            break;
        case '[':
            modelstack.push(model);
            tstack.push(translation);
            break;
        case ']':
            model = modelstack.pop();
            translation = tstack.pop();
            break;
        default:
            break;
        }
    }

    m_cyl->tesselate(50,50,0);
    m_cyl->init(vertexLocation, normalLocation);
    m_initialized = 1;
}

void Plant::render(GLuint shader) {
//    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
//    glm::vec3 diffuse = glm::vec3(0.7f, 0.3f, 0.0f);
//    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

//    glUniform3fv(glGetAttribLocation(shader, "ambient_color"), 1, &ambient.r);
//    glUniform3fv(glGetAttribLocation(shader, "diffuse_color"), 1, &diffuse.r);
//    glUniform3fv(glGetAttribLocation(shader, "specular_color"), 1, &specular.r);
//    glUniform1f(glGetAttribLocation(shader, "shininess"), 10);

    int s = m_scenegraph->size();

    glm::vec3 initialpos = glm::vec3(0.0f, -0.8f, 0.0f);
    glm::vec3 p1, p2;

    for(int i = 0; i < s; i++) {
        Node n = m_scenegraph->at(i);

//        glUniformMatrix4fv(glGetAttribLocation(shader, "m"), 1, GL_FALSE, &n.model[0][0]);
//        m_cyl->draw();

        p1 = n.translation + initialpos;
        p2 = p1 + (n.rotation * glm::vec3(0.0, m_factor, 0.0));

//        printVertex(pos2);

        glBegin(GL_LINES);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
        glEnd();

    }
}
