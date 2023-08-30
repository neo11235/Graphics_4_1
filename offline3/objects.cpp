#ifndef __OBJECT__.h
#define __OBJECT__.h
#include "Point.cpp"
#include <GL/glut.h>
struct Color1
{
    GLdouble r, g, b;
    Color1(){}
    Color1(GLdouble r, GLdouble g, GLdouble b):r(r), g(g), b(b){}
};

struct Ray  
{
    Point u, v;
    Ray(){}
    Ray(Point u, Point v):u(u), v(v){}
};
struct SurfaceProperty
{
    GLdouble ambient, diffuse, reflection;
    SurfaceProperty(){
        ambient = diffuse = reflection = 0;
    }
    SurfaceProperty(GLdouble ambient, GLdouble diffuse, GLdouble reflection):
    ambient(ambient),diffuse(diffuse),reflection(reflection)
    {}
};

struct Triangle{
    Point vertex[3];
    Triangle(){}
    
};

struct Pyramid
{
    Point vertex[4];
    Pyramid(){}
};
struct Sphere
{
    Point center;
    GLdouble radius;
    Sphere(){}
    
};
struct Qube
{
    Point corners[2];
    Qube(){}
};
struct Checkerboard
{
    GLdouble width;
    SurfaceProperty surface;
    Color1 color1 = {0.0,0.0,0.0};
    Color1 color2 = {1.0, 1.0, 1.0};
    Checkerboard(){}

    void glDraw(){
        glBegin(GL_QUADS);
        for(int i = -20; i < 20; ++i)
        {
            for(int j = -20; j < 20; ++j)
            {
                GLdouble x = i * width, y = j *width;
                if((i + j) % 2 == 0)
                {
                    glColor3f(color1.r, color1.g, color1.b);
                }
                else
                {
                    glColor3f(color2.r, color2.g, color2.b);
                }
                glVertex3f(x, y, 0);
                glVertex3f(x + width, y, 0);
                glVertex3f(x + width, y + width, 0);
                glVertex3f(x, y + width, 0);

                
            }
        }
        glEnd();
    }

};





#endif