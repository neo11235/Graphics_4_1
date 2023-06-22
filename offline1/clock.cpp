#include <iostream> 
#include <GL/glut.h>

GLdouble x = 0, y = 0, vx = 0.1;
void display()
{
    std::cout << "Display called\n"<<std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize( 10);
    glBegin(GL_POINTS);
        // glColor3d(1,1,1);
        glVertex2d(x,y);
        
        // glVertex2d(1,0);
        // glVertex2d(0,1);
    glEnd();

    // glBegin(GL_LINE_STRIP);
    //     glVertex2d(0,0);
    //     glVertex2d(1,0);
    //     glVertex2d(0,1);
    // glEnd();
    // glLineWidth(5);
    // glBegin(GL_POLYGON);
    //     glColor3d(0,0,1);
    //     glVertex2d(0,0);
    //     glColor3d(0,1,0);
    //     glVertex2d(1,0);
    //     glColor3d(1,0,0);
    //     glVertex2d(0,1);
    //     glVertex2d(-.5,-.5);
    // glEnd();
    glFlush();

}
void idle(){
    std::cout << "idle called " << x << std::endl;
    x += vx ;
    if(x > 1 || x < -1)
        vx = -vx;
      glutPostRedisplay();
    // display();
}
void keyboardFunc(unsigned char k, int a, int b)
{
    std::cout << "keyboardFunc called " << x << std::endl;
    switch (k)
    {
    case 'a':
        /* code */
        x -= vx;
        break;
    case 'f':
        x += vx;
        break;
    }
      glutPostRedisplay();
    // display();
    
}
void specialKey(int key,int a,int b)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        /* code */
        x -= vx;
        break;
    case GLUT_KEY_RIGHT:
        x += vx;
        break;
    }
      glutPostRedisplay();
    // display();
    
}
void mouse(int button,int state,int a,int b)
{
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        x = -1 + a * 2.0 / 640;
        y = 1 - 2.0 * b / 800;
    }
    glutPostRedisplay();
}
int main(int argc, char ** argv)
{
    std::cout << "test" << '\n';
    glutInit(&argc,argv);
    glutInitWindowSize(640,800);
    glutInitWindowPosition(100,50);
    glutCreateWindow("Hlw openGl");
    glutDisplayFunc(display);
    // glutIdleFunc(idle);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}