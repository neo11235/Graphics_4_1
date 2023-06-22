#include <iostream> 
#include <GL/glut.h>

GLdouble x = 0, y = 0, vx = 0.1;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2d(0, 0.5);
    glVertex2d(0.5, 0.5);
    glVertex2d(.5, 0);
    glVertex2d(0, 0);
    glEnd();
    glFlush();

}
void reshape(int width, int height)
{
    // std::cout << width << ' ' < < height <<'\n';
    glViewport(0,0, width, height);
    
    if(width == 0)
        width = 1;
    if( height == 0) 
        height = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width >= height)
        gluOrtho2D(-width * 1.0 / height , width * 1.0 / height, -1, 1);
    else
        gluOrtho2D(-1, 1, -height * 1.0 / width, height * 1.0 / width);
}
int main(int argc, char ** argv)
{
    std::cout << "test" << '\n';
    glutInit(&argc,argv);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Hlw openGl");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}