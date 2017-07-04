#include <GL/glut.h>

void display();

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Hello World");
    glutDisplayFunc(display);
    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glVertex2f(0.0, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);
    glEnd();
    glFlush();
}
