#include "level.h"

Level::Level(Cylinder *cylinder, Cone *cone)
{
    m_cylinder = cylinder;
    m_cone = cone;
}

void Level::draw(GLuint shader, GLfloat d, GLfloat param_x, GLfloat param_y, GLfloat size, Transforms transform)
{
    transform.model=glm::translate(transform.model, glm::vec3(param_x, d, param_y));
    transform.model=glm::scale(transform.model, glm::vec3(size, 1.0, size));
    glUniform3f(glGetUniformLocation(shader, "color"), 1, 0, 0);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, &transform.getTransform()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, &transform.model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, &transform.view[0][0]);
    m_cylinder->draw();
    //drawIvy(shader, transform);

    transform.model=glm::rotate(transform.model, (float) (-M_PI/2.0), glm::vec3(1.0, 0.0, 0.0));
    transform.model=glm::translate(transform.model, glm::vec3(0, 0, 1.5));
    glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, &transform.getTransform()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, &transform.model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, &transform.view[0][0]);
    m_cone->draw();

    transform.model=glm::translate(transform.model, glm::vec3(0, 0, -d-1));
    transform.model=glm::rotate(transform.model, (float) (M_PI), glm::vec3(1.0, 0.0, 0.0));
    transform.model=glm::translate(transform.model, glm::vec3(0, 0, -d+1));
    glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, &transform.getTransform()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, &transform.model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, &transform.view[0][0]);
    m_cone->draw();
}

void Level::drawIvy(GLuint shader, Transforms t) {
    glUniform1i(glGetUniformLocation(shader, "wrap"), 1);
    glUniform3f(glGetUniformLocation(shader, "center"), 0, 0, 0);
    glUniform1f(glGetUniformLocation(shader, "radius"), 0.5f);

    t.model = glm::scale(t.model, glm::vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "v"), 1, GL_FALSE, &t.view[0][0]);
    m_ivy->render(shader, t);

    glUniform1i(glGetUniformLocation(shader, "wrap"), 0);

}
