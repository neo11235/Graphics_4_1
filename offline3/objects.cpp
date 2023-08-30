#ifndef __OBJECT__.h
#define __OBJECT__.h
#include "Point.cpp"
#include <GL/glut.h>
struct Color
{
    GLdouble r, g, b;
    Color(){}
    Color(GLdouble r, GLdouble g, GLdouble b):r(r), g(g), b(b){}
};

struct Ray  
{
    Point u, v;
    Ray(){}
    Ray(Point u, Point v):u(u), v(v){}
};
struct SurfaceProperty
{
    GLdouble ambient, diffuse, reflection, shininess;

    SurfaceProperty(){
        ambient = diffuse = reflection = 0;
    }
    SurfaceProperty(GLdouble ambient, GLdouble diffuse, GLdouble reflection):
    ambient(ambient),diffuse(diffuse),reflection(reflection)
    {}
    SurfaceProperty(GLdouble ambient, GLdouble diffuse, GLdouble reflection, GLdouble shininess):
    ambient(ambient), diffuse(diffuse), reflection(reflection), shininess(shininess)
    {}
};
struct Object
{
    Color color;
    SurfaceProperty surface;
    virtual void draw() = 0;
    Object(){}
};

struct Pyramid:Object
{
    Point corner;
    GLdouble widht;
    GLdouble height;
    Pyramid(){}
    virtual void draw(){

    }
};
struct Sphere:Object
{
    Point center;
    GLdouble radius;
    Sphere(){}
    virtual void draw(){

    }
    
};
struct Cube:Object
{
    Point corner;
    GLdouble side;
    Cube(){}
    virtual void draw(){

    }
};
struct Checkerboard
{
    GLdouble width;
    SurfaceProperty surface;
    Color color1 = {0.0,0.0,0.0}; // white
    Color color2 = {1.0, 1.0, 1.0}; // black
    int iteration;
    Checkerboard(){}

    void draw(){
        glBegin(GL_QUADS);
        for(int i = -iteration; i < iteration; ++i)
        {
            for(int j = -iteration; j < iteration; ++j)
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

struct Triangle{
    Point vertex[3];
    Triangle(){}  
};




#endif