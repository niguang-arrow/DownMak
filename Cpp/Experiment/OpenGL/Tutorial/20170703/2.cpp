#include <GL/glut.h>

double th = 0;

void display();
void special(int, int, int);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Hello World");
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(th, 0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0); glVertex2f(0.0, 0.5);
    glColor3f(0.0, 1.0, 0.0); glVertex2f(0.5, -0.5);
    glColor3f(0.0, 0.0, 1.0); glVertex2f(-0.5, -0.5);
    glEnd();
    glTranslatef(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0); glVertex2f(0.0, 0.5);
    glColor3f(0.0, 1.0, 1.0); glVertex2f(0.5, -0.5);
    glColor3f(1.0, 0.0, 1.0); glVertex2f(-0.5, -0.5);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    glutPostRedisplay();
}
