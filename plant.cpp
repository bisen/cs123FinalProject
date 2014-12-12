#include "plant.h"
#include "cylinder.h"

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

    m_num_vert = 0;
}

Plant::~Plant() {
    delete m_scenegraph;
    delete m_cyl;
    delete[] m_initial;

    if(m_initialized) {
        //delete[] m_buf;
    }
}

void Plant::parseSystem(int level, GLuint vertexLocation, GLuint normalLocation) {
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
    m_cyl->init(vertexLocation, normalLocation);
    init(vertexLocation, normalLocation);
}

void Plant::render(GLuint shader, Transforms t) {

//    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, &t.view[0][0]);
//    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, &t.model[0][0]);
//    glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, &t.getTransform()[0][0]);


    glBindVertexArray(m_vaoid);
//    printf("%s", glewGetErrorString(glGetError()));
    glDrawArrays(GL_TRIANGLES, 0, m_num_vert);

    glBindVertexArray(0);


}

void Plant::copyAndMult(GLfloat *buf, int &index, glm::mat4 matrix, glm::mat4 inverseMat) {
    glm::vec4 newVec = matrix * glm::vec4(buf[index], buf[index+1], buf[index+2], 1.0f);
    glm::vec4 newNorm = inverseMat * glm::vec4(buf[index+3], buf[index+4], buf[index+5], 0.0f);

    m_buf[index] = newVec.x;
    m_buf[index+1] = newVec.y;
    m_buf[index+2] = newVec.z;

    m_buf[index+3] = newNorm.x;
    m_buf[index+4] = newNorm.y;
    m_buf[index+5] = newNorm.z;

    index += 6;

}

void Plant::init(GLuint vertexLocation, GLuint normalLocation) {

    int base_bufsize = m_cyl->getNumVert();

    int s = m_scenegraph->size();
    glm::vec3 **positions = m_cyl->getPositions();
    glm::vec3 **normals = m_cyl->getNormals();

    GLfloat *cyl_buf = m_cyl->getBuf();

    int count = 0;

    m_buf = new GLfloat[base_bufsize * 6 * s];
    m_num_vert = base_bufsize * s;

    for(int i = 0; i < s; i++) {
        Node n = m_scenegraph->at(i);

        glm::mat4 curmodel = n.model;
        glm::mat4 inverseModel = glm::inverse(n.model);

        //For each of the three cylinder faces
        for(int x = 0; x < CYL_FACES; x++) {
            //Multiply each vertex by the model matrix and add it to the buffer
            for(int k = 0; k < base_bufsize; k++) {
                copyAndMult(cyl_buf, count, curmodel, inverseModel);
            }
        }
    }

    glGenVertexArrays(1, &m_vaoid);
    glBindVertexArray(m_vaoid);

    glGenBuffers(1, &m_vboid);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboid);
    glBufferData(GL_ARRAY_BUFFER, m_num_vert * 6 * sizeof(GLfloat), m_buf, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);
    glEnableVertexAttribArray(normalLocation);

    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*) 0);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_TRUE,  6*sizeof(GLfloat), (void*) (sizeof(GLfloat)*3));
    printf("%d\n", glGetError());


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_initialized = 1;
}
