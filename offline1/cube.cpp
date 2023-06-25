#define _USE_MATH_DEFINES
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <vector>
#include <iostream>
#define toRad(x) (((x) * M_PI) / 180)

struct Point {
  GLdouble x, y, z;
  Point() : x(0), y(0), z(0) {}
  Point(GLdouble X, GLdouble Y, GLdouble Z) :
      x(X), y(Y), z(Z) {}
  Point operator + (const Point& u) const {
    return Point(x + u.x, y + u.y, z + u.z); }
  Point operator - (const Point& u) const {
    return Point(x - u.x, y - u.y, z - u.z); }
  Point operator * (const GLdouble u) const {
    return Point(x * u, y * u, z * u); }
  Point operator / (const GLdouble u) const {
    return Point(x / u, y / u, z / u); }
};
GLdouble dot(Point a, Point b) {
  return a.x * b.x + a.y * b.y + a.z * b.z; }
Point cross(Point a, Point b) {
  return Point(a.y*b.z - a.z*b.y,
     a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
GLdouble length(Point a) { return sqrt(dot(a, a));}
GLdouble distance(Point a, Point b) {
  return length(a-b); }
Point unit(const Point &p) { return p/length(p); }
// Rotate p around axis x, with angle radians.
Point rotate(Point p, Point axis, GLdouble angle) {
  axis = unit(axis);Point comp1 = p * cos(angle);
  Point comp2 = axis*(1-cos(angle))*dot(axis,p);
  Point comp3 = cross(axis, p) * sin(angle);
  return comp1 + comp2 + comp3;
}
/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}



const GLdouble tlen = 1;
std::vector<Point> dTriangle = {Point(tlen,0,tlen),Point(0,sqrt(2.0)*tlen,0),Point(-tlen,0,tlen)};
std::vector<Point> triangleColor 
={Point(1,0,0),Point(0,1,0),Point(1,0,0),Point(0,1,0),
Point(0,1,0),Point(1,0,0),Point(0,1,0),Point(1,0,0)};
std::vector<Point> circleColor 
= {Point(0.5,1.0,1.0),Point(1.0,0.0,1.0),
Point(1.0f, 0.5f, 0.0f), Point(0.5f, 0.5f, 0.5f),
Point(0.0f, 0.5f, 0.5f), Point(0.0f, 1.0f, 0.0f)
};
std::vector<Point> transformedTriangle = dTriangle;
std::vector<Point> circularSegment(4);
std::vector<Point> cylinder(4);

GLdouble curAngle = 0.0;
GLdouble dAngle = 1.0;
const GLdouble maxAngle = asin(sqrt(2.0/3.0))*180/M_PI;
const GLdouble topAngle = asin(1.0/sqrt(3.0));
const GLdouble smallestLength = .01;
const GLdouble rotationSpeed = M_PI/36.0;
void rotateTTriangle(GLdouble angle)
{
    dTriangle[0] = rotate(dTriangle[0],dTriangle[1],angle);
    dTriangle[2] = rotate(dTriangle[2],dTriangle[1],angle);
}
void transformTriangle(GLdouble angle)
{
   // angle must be >= 0 and less than 45 
   if(angle < 0){
        std::cout << "Warning angle less than 0:"
        << angle << std::endl;
        transformedTriangle = dTriangle;
        return;
   }
   if(angle > maxAngle)
   {
       std::cout << "Warning greater than max angle:"
        << angle << std::endl; 
        angle = maxAngle;
   }
   angle = toRad(angle);
   GLdouble len = tlen * sqrt(2.0/3.0) / sin(angle + topAngle);
   

//    Point tmp = dTriangle[0] + dTriangle[2];
//    Point raxis = cross(tmp, dTriangle[1]);
//    tmp = rotate(dTriangle[1], raxis, angle);
//    transformedTriangle[1] = unit(tmp) * len;

   for(int i = 0; i < 3; ++i)
   {
        Point tmp = dTriangle[(i+1)%3] + dTriangle[(i+2)%3];
        Point raxis = cross(tmp, dTriangle[i]);
        tmp = rotate(dTriangle[i], raxis, -angle);
        transformedTriangle[i] = unit(tmp) * len;
   }
   circularSegment[0] = transformedTriangle[0];
   circularSegment[3] = rotate(transformedTriangle[2],Point(0,1,0),M_PI/2);
   circularSegment[1] = circularSegment[0];
   circularSegment[1].y = -circularSegment[1].y;
   circularSegment[2] = circularSegment[3];
   circularSegment[2].y = -circularSegment[2].y;

   cylinder[0] = transformedTriangle[0];
   cylinder[1] = circularSegment[3];
   cylinder[2] = rotate(transformedTriangle[1],Point(0,1,0),M_PI/2);
   cylinder[3] = transformedTriangle[1];

}

void drawTriangle()
{
    glBegin(GL_TRIANGLES);
        for(Point p : transformedTriangle)
            glVertex3f(p.x,p.y,p.z);
    glEnd();
}
//given in counterclockwise order
//GL_QUAD must be called before calling it
void drawCylinderHelper(Point p1, Point p2, Point p3, Point p4)
{
    if(length(p1 - p2) < smallestLength)
    {
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glVertex3f(p4.x,p4.y,p4.z);
        return;
    }
    Point mid1 = (p1 + p2);
    Point mid2 = (p3 + p4);
    GLdouble len1 = length(p1);
    mid1 = unit(mid1) * len1;
    mid2 = unit(mid2) * len1;
    drawCylinderHelper(p1, mid1, mid2, p4);
    drawCylinderHelper(mid1, p2, p3, mid2);
}
void drawCylinder()
{
    glBegin(GL_QUADS);
        drawCylinderHelper(
            cylinder[0],cylinder[1],cylinder[2],cylinder[3]
        );
    glEnd();
}
//given in counterclockwise order
//GL_QUAD must be called before calling it
void drawCircularSegmentHelper(Point p1,Point p2,Point p3,Point p4)
{
    if(length(p1 - p2) < smallestLength)
    {
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glVertex3f(p4.x,p4.y,p4.z);
        return;
    }
    GLdouble len1 = length(p1);
    Point m1,m2,m3,m4,mm;
    m1 = unit(p1 + p2) * len1;
    m2 = unit(p2 + p3) * len1;
    m3 = unit(p3 + p4) * len1;
    m4 = unit(p4 + p1) * len1;
    mm = unit(m1 + m3) * len1;
    drawCircularSegmentHelper(p1,m1,mm,m4);
    drawCircularSegmentHelper(m1,p2,m2,mm);
    drawCircularSegmentHelper(mm,m2,p3,m3);
    drawCircularSegmentHelper(m4,mm,m3,p4);
}
void drawCircularSegment()
{
    glBegin(GL_QUADS);
        drawCircularSegmentHelper(
            circularSegment[0],
            circularSegment[1],
            circularSegment[2],
            circularSegment[3]
        );
    glEnd();
}



/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    // draw
    // if (isAxes) drawAxes();
    // if (isCube) drawCube();
    // if (isPyramid) drawPyramid();
    //drawAxes();
    Point ax = unit(dTriangle[0] + dTriangle[1]);
    for(int i = 0; i < 4 ; ++i)
    {
        glPushMatrix();
        glColor3f(triangleColor[i].x,triangleColor[i].y,triangleColor[i].z);
        
        glRotated(i*90,0,1,0);
        
        drawTriangle();
        glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
        drawCylinder();
        glColor3f(circleColor[i].x,circleColor[i].y,circleColor[i].z);
        drawCircularSegment();
        
        glRotated(2*90,1,0,0);
        
        glColor3f(triangleColor[i+4].x,triangleColor[i+4].y,triangleColor[i+4].z);
        drawTriangle();
        glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
        drawCylinder();
        glPopMatrix();
    }
    glPushMatrix();
        ax = dTriangle[2];
        glRotated(90,ax.x,ax.y,ax.z);
        glColor3f(circleColor[4].x,circleColor[4].y,circleColor[4].z);
        drawCircularSegment();
        glRotated(180,ax.x,ax.y,ax.z);
        glColor3f(circleColor[5].x,circleColor[5].y,circleColor[5].z);
        drawCircularSegment();
    glPopMatrix();

    glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
        glRotated(90,dTriangle[0].x,dTriangle[0].y,dTriangle[0].z);
        drawCylinder();
        glRotated(90*2,dTriangle[0].x,dTriangle[0].y,dTriangle[0].z);
        drawCylinder();
        glRotated(90*2,dTriangle[2].x,dTriangle[2].y,dTriangle[2].z);
        drawCylinder();
        ax = Point(0,0,0) -dTriangle[0];
        glRotated(90*2,ax.x,ax.y,ax.z); 
        drawCylinder();
    glPopMatrix();




    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // control eyex
    case '1':
        eyex += v;
        break;
    case '2':
        eyex -= v;
        break;
    // control eyey
    case '3':
        eyey += v;
        break;
    case '4':
        eyey -= v;
        break;
    // control eyez
    case '5':
        eyez += v;
        break;
    case '6':
        eyez -= v;
        break;

    // Control center (location where the eye is looking at)
    // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
    // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
    // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

    // Control what is shown
    case 'a':
        rotateTTriangle(-rotationSpeed);
        transformTriangle(curAngle);
        break;
    case 'd':
        rotateTTriangle(rotationSpeed);
        transformTriangle(curAngle);
        break;

    case '.':
        curAngle = std::max(curAngle - dAngle, 0.0);
        transformTriangle(curAngle);
        break;
    case ',':
        curAngle = std::min(curAngle + dAngle, maxAngle);
        transformTriangle(curAngle);
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_RIGHT:
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_UP:
        eyey += v;
        break;
    case GLUT_KEY_DOWN:
        eyey -= v;
        break;
    
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    transformTriangle(0.0);
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
