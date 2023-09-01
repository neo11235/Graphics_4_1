#define _USE_MATH_DEFINES

//#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <assert.h>
#include <cmath>
#include <cstring>
#include <iomanip>
#include "1805045_Point.cpp"
#include "1805045_objects.cpp"
#include "1805045_bitmap_image.hpp"
#define vec Point
#define toRad(x) (((x) * M_PI) / 180)
using namespace std;


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
Point pos;   // position of the eye
Point l;     // look/forward direction
Point r;     // right direction
Point u;     // up direction

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

/* Draw a cube centered at the origin */
void drawCube() {
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Top face (y = 1.0f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( 1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f,  1.0f);
        glVertex3f( 1.0f, 1.0f,  1.0f);

        // Bottom face (y = -1.0f)
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);

        // Front face  (z = 1.0f)
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f( 1.0f, -1.0f, 1.0f);

        // Back face (z = -1.0f)
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);

        // Left face (x = -1.0f)
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);

        // Right face (x = 1.0f)
        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(1.0f,  1.0f, -1.0f);
        glVertex3f(1.0f,  1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
}

/* Draw a pyramid centered at the origin */
void drawPyramid() {
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(0.0f,1.0f,1.0f);       // Cyan
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
}

void drawCircle(double radius, int segments) {
    glBegin(GL_POLYGON);
        for (int i = 0; i < segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            glVertex3f(radius * cos(theta), radius * sin(theta), 0);
        }
    glEnd();
}

void drawCylinder(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2+cos(theta))/3;
            glColor3f(c,c,c);
            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

void drawCone(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_TRIANGLES);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2+cos(theta))/3;
            glColor3f(c,c,c);
            glVertex3f(0, 0, height/2);
            glVertex3f(currx, curry, -height/2);
            glVertex3f(tempx, tempy, -height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

void drawCircle_v2(double radius, int segments) {
    Point points[segments];

    for (int i = 0; i < segments; i++) {
        double theta = i * 2.0 * M_PI / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
        points[i].z = 0;
    }

    glBegin(GL_POLYGON);
        for (int i = 0; i < segments; i++) {
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
    glEnd();
}

void drawCylinder_v2(double height, double radius, int segments) {
    Point points[segments+1];

    for (int i = 0; i < segments+1; i++) {
        double theta = i * 2.0 * M_PI / segments;
        points[i].x = radius * cos(theta);
        points[i].y = radius * sin(theta);
    }

    glBegin(GL_QUADS);
        for (int i = 0; i < segments; i++) {
            GLfloat c = (2 + cos(i * 2.0 * M_PI / segments)) / 3;
            glColor3f(c,c,c);
            glVertex3f(points[i].x, points[i].y, height/2);
            glVertex3f(points[i].x, points[i].y, -height/2);
            glVertex3f(points[i+1].x, points[i+1].y, -height/2);
            glVertex3f(points[i+1].x, points[i+1].y, height/2);
        }
    glEnd();
}

void drawSphere(double radius, int stacks, int slices) {
    Point points[stacks+1][slices+1];
    for (int j = 0; j <= stacks; j++) {
        double phi = -M_PI / 2.0 + j * M_PI / stacks;
        double r = radius * cos(phi);
        double h = radius * sin(phi);
        for (int i = 0; i < slices+1; i++) {
            double theta = i * 2.0 * M_PI / slices;
            points[j][i].x = r * cos(theta);
            points[j][i].y = r * sin(theta);
            points[j][i].z = h;
        }
    }

    glBegin(GL_QUADS);
        for (int j = 0; j < stacks; j++) {
            for (int i = 0; i < slices; i++) {
                GLfloat c = (2+cos((i+j) * 2.0 * M_PI / slices)) / 3;
                glColor3f(c,c,c);
                glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
            }
        }
    glEnd();
}


GLdouble near, far, fovY, aspectRatio;
int screenWidth, screenHeight;
int levelOfRecursion;
Checkerboard checkerboard;
vector<void*> objects;
int numberOfNormalLight;
int numberOfSpotLight;

vector<NormalLight> nlights;
vector<SpotLight> splights;

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
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);
    // draw
    // drawSphere(5,100,100);
    drawAxes();
    checkerboard.draw();
    for(void * vp : objects){
        ((Object*)(vp))->draw();
    }
    for(NormalLight nlight:nlights){
        nlight.draw();
    }
    for(SpotLight slight : splights){
        slight.draw();
    }
    glutSwapBuffers();  // Render now
}

// GLdouble near, far, fovY, aspectRatio;
// int screenWidth, screenHeight;
// int levelOfRecursion;
// Checkerboard checkerboard;
// vector<void*> objects; 
// int numberOfNormalLight;
// int numberOfSpotLight;

// vector<NormalLight> nlights;
// vector<SpotLight> splights;

void fileReader()
{
    ifstream fin;
    fin.open("description.txt");
    if(!fin.is_open())
    {
        cout << "Cannot open description.txt file\n";
        exit(0);
    }
    fin >> near >> far
    >>fovY >> aspectRatio;
    fin >> levelOfRecursion >> screenHeight;
    screenWidth = screenHeight;
    fin >> checkerboard.width >> checkerboard.surface.ambient 
    >> checkerboard.surface.diffuse >> checkerboard.surface.reflection;

    //checkerboard.width = 1;
    checkerboard.iteration = 50;
    
    int numberOfObjects;
    fin >> numberOfObjects;
    cout << "debug " << numberOfObjects << endl;
    for(int i = 0; i < numberOfObjects; ++i){
        string type;
        fin >> type;
        if(type == "sphere")
        {
            Sphere *sphere = new Sphere();
            fin >> sphere->center.x >> sphere->center.y >> sphere->center.z
            >> sphere->radius 
            >> sphere->color.r >> sphere->color.g >> sphere->color.b
            >> sphere->surface.ambient >> sphere->surface.diffuse
            >> sphere->surface.specular >> sphere->surface.reflection
            >> sphere->surface.shininess;
            objects.push_back(sphere);
        }
        else if(type == "pyramid")
        {
            Pyramid *pyr = new Pyramid();
            fin >> pyr->corner.x >> pyr->corner.y >> pyr->corner.z 
            >> pyr->width >> pyr->height
            >> pyr->color.r >> pyr->color.g >> pyr->color.b
            >> pyr->surface.ambient >> pyr->surface.diffuse 
            >> pyr->surface.specular >> pyr->surface.reflection
            >> pyr->surface.shininess;
            objects.push_back(pyr);
        }
        else if(type == "cube")
        {
            Cube *cube = new Cube();
            fin >> cube->corner.x >> cube->corner.y >> cube->corner.z
            >> cube->side
            >> cube->color.r >> cube->color.g >> cube->color.b
            >> cube->surface.ambient >> cube->surface.diffuse 
            >> cube->surface.specular >> cube->surface.reflection
            >> cube->surface.shininess;

            // cube->corner.x = -10;
            // cube->corner.y = -10;
            // cube->corner.z = 10;
            // cube->side = 1;
           objects.push_back(cube);
        }
        else
        {
            cout << "Unknown object type\'" << type << "\'\n";
            cout << i << endl;
            exit(0);
        }
    }
    fin >> numberOfNormalLight;
    nlights.resize(numberOfNormalLight);
    for(int i = 0;i < numberOfNormalLight; ++i)
    {
        fin >> nlights[i].position.x >> nlights[i].position.y >> nlights[i].position.z
        >> nlights[i].fallOff;
    }
    fin >> numberOfSpotLight;
    splights.resize(numberOfSpotLight);
    for(int i = 0; i < numberOfSpotLight; ++i){
        fin >> splights[i].position.x >> splights[i].position.y >> splights[i].position.z
        >> splights[i].fallOff 
        >> splights[i].look.x >> splights[i].look.y >> splights[i].look.z
        >> splights[i].cutoffAngle;
        splights[i].cutoffAngle = toRad(splights[i].cutoffAngle);
    }
    cout << "Succesfully read light sources" << endl;

    fin.close();

}

Point pointBuffer[1000][1000];
void buildPointBuffer()
{
    r = unit(r);
    l = unit(l);
    u = unit(u);
    Point center = pos + l * near;
    GLdouble realHeight = near * tan(toRad(fovY) / 2);
    GLdouble realWidth = near * tan(toRad(fovY) * aspectRatio / 2);

    
    GLdouble di = 2 * realHeight / screenHeight;
    GLdouble dj = 2 * realWidth / screenWidth;
    
    for(int i = 0; i < screenHeight; ++i)
    {
        for(int j = 0; j < screenWidth; ++j){

            pointBuffer[i][j] = center + u * (screenHeight / 2 - i) * di
            + r * (-screenWidth / 2 + j) * dj;
        }
    }
}
Color imgColor[1000][1000];
const string outputPath = "";
const string outputImgFileName = "out.bmp"; 
void saveBitmapFile(){
    cout << screenHeight << ' ' << screenWidth << endl;
    bitmap_image image(screenHeight, screenWidth);
    for(int x = 0; x < screenHeight; ++x){
        for(int y = 0; y < screenWidth; ++y){
            
            image.set_pixel(y,x,imgColor[x][y].r * 255, imgColor[x][y].g * 255, imgColor[x][y].b * 255);
        }
    }
    image.save_image(outputPath + outputImgFileName);
}
bool findMinIntersect(Ray ray, Color & rcolor, SurfaceProperty &rsurface,
    Point & normal, Point & sect
){
    GLdouble curMin = 1e15;
    Point tsect, tnormal;
    bool check = false;
    for(void * vp : objects){
        Object * cur = (Object*)vp;
        if(cur->intersect(ray, tnormal, tsect)){
            if(length(tsect - ray.u) < curMin){
                curMin = length(tsect - ray.u);
                sect = tsect;
                normal = tnormal;
                rcolor = cur->color;
                rsurface = cur->surface;
                check = true;
            }
        }
    }
    Color tcolor;
    if(checkerboard.intersect(ray, tnormal, tsect, tcolor))
    {
        if(length(tsect - ray.u) < curMin){
            curMin = length(tsect - ray.u);
            rcolor = tcolor;
            normal = tnormal;
            sect = tsect;
            rsurface = checkerboard.surface;
            check = true;
        }
    }
    return check;
}

Color rayTrace(Ray ray, int rdepth){
    if(rdepth == 0){
        return Color(0,0,0);
    }
    Color rcolor;
    SurfaceProperty rsurface;
    
    Point sect, normal;
    
    bool check = findMinIntersect(ray, rcolor, rsurface, normal, sect);
    
    if(!check){
        return Color(0, 0, 0);
    }  
    Color res = rcolor * rsurface.ambient;
    
    Ray reflectedRay = reflect(ray, normal, sect);
    Color reflectedColor = rayTrace(reflectedRay, rdepth - 1);

    res = res + reflectedColor * rsurface.reflection;

    Color dColor;
    SurfaceProperty dSurface;
    Point dsect, dnormal;

    GLdouble lambert = 0, phong = 0;
    for(NormalLight nlight : nlights){

        Point ps = unit(nlight.position - sect);

        Ray tmpray;
        tmpray.u = sect;
        tmpray.v = unit(nlight.position - sect);
        tmpray.u = tmpray.u + tmpray.v * 0.1; 
        if(findMinIntersect(tmpray, dColor, dSurface, dnormal, dsect))
            continue;

        GLdouble dis = length(nlight.position - sect);
        GLdouble sf = exp(-dis * dis * nlight.fallOff);
        lambert += dot(ps, normal) * sf;
        phong += pow(dot(reflectedRay.v, ps), rsurface.shininess) * sf;
    }
    for(SpotLight slight: splights){
        Point ps = unit(slight.position - sect);

        GLdouble angle = acos(normInvT(dot(ps * -1, slight.look - slight.position) 
        / (length(ps) * length(slight.look - slight.position))));
        if(angle > slight.cutoffAngle)
            continue;

        Ray tmpray;
        tmpray.u = sect;
        tmpray.v = unit(slight.position - sect);
        tmpray.u = tmpray.u + tmpray.v * 0.1; 
        if(findMinIntersect(tmpray, dColor, dSurface, dnormal, dsect))
            continue;

        GLdouble dis = length(slight.position - sect);
        GLdouble sf = exp(-dis * dis * slight.fallOff);
        lambert += dot(ps, normal) * sf;
        phong += pow(dot(reflectedRay.v, ps), rsurface.shininess) * sf;
    }
    res = res + rcolor * rsurface.diffuse * lambert + rcolor * rsurface.specular * phong;

    return res;

    
}
void startRayTracing(void){
    for(int i = 0; i < screenHeight; ++i){
        for(int j = 0; j < screenWidth; ++j)
        {
            Ray cur;
            cur.u = pointBuffer[i][j];
            cur.v = unit(pointBuffer[i][j] - pos);
            imgColor[i][j] = rayTrace(cur, 3);
        }
    }
}
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
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
    gluPerspective(fovY, aspect, near, far);
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.01;
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;
        case '0':
            cout << "Building point buffer" << endl;
            buildPointBuffer();
            cout << "End building point buffer" << endl;
            cout << "Starting ray tracing " << endl;
            startRayTracing();
            cout << "Ended ray tracing " << endl;
            cout << "Saving file " << endl;
            saveBitmapFile();
            cout << "Saved bmp file " << endl;
            
            break;

		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    pos.x=0;pos.y=0;pos.z=20;

    l.x=0;l.y=0;l.z=-1;
    u.x=0;u.y=1;u.z=0;
    r.x=1;r.y=0;r.z=0;
    fileReader();

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(screenHeight, screenWidth);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Ray Tracing");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;
}
