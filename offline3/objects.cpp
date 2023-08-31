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
    GLdouble ambient, diffuse, reflection, specular, shininess;

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
        glBegin(GL_QUADS);


        glEnd();
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
int __dx[] = {0, 1, 1, 0, 0, 1, 1, 0};
int __dy[] = {0, 0, 1, 1, 0, 0, 1, 1};
int __dz[] = {0, 0, 0, 0, 1, 1, 1, 1};
struct Cube:Object
{
    
    Point corner;
    GLdouble side;
    Cube(){}


    virtual void draw(){
        std::cout << "debug cube draw " << corner.x << ' ' << corner.y << ' ' << corner.z << std::endl;
        std::cout << side << std::endl;
        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);
        drawHelper(0, 1, 2, 3);
        glColor3f(0, color.g, color.b);
        drawHelper(4, 5, 6, 7);
        glColor3f(color.r, color.g, color.b);
        drawHelper(0, 1, 5, 4);
        glColor3f(color.r, color.g, color.b);
        
        drawHelper(3, 2, 6, 7);
        glColor3f(0, color.g, color.b);
        
        drawHelper(0, 1, 7, 3);
        glColor3f(color.r, color.g, color.b);
        
        drawHelper(1, 5, 6, 2);
        glEnd();
    }
    private:
    void drawHelper(int i, int j, int k, int l)
    {
        glVertex3f(corner.x + __dx[i] * side, corner.y + __dy[i] * side, corner.z + __dz[i] * side);
        glVertex3f(corner.x + __dx[j] * side, corner.y + __dy[j] * side, corner.z + __dz[j] * side);
        glVertex3f(corner.x + __dx[k] * side, corner.y + __dy[k] * side, corner.z + __dz[k] * side);
        glVertex3f(corner.x + __dx[l] * side, corner.y + __dy[l] * side, corner.z + __dz[l] * side);

        // glVertex3f(corner.x + __dx[k] * side, corner.y + __dy[k] * side, corner.z + __dz[k] * side);
        // glVertex3f(corner.x + __dx[l] * side, corner.y + __dy[l] * side, corner.z + __dz[l] * side);
        // glVertex3f(corner.x + __dx[i] * side, corner.y + __dy[i] * side, corner.z + __dz[i] * side);
        // std::cout << corner.x + __dx[i] * side << ' ' << corner.y + __dy[i] * side << ' ' <<  corner.z + __dz[i] * side << std:: endl;
        // std::cout << corner.x + __dx[j] * side << ' ' << corner.y + __dy[j] * side << ' ' <<  corner.z + __dz[j] * side << std:: endl;
        // std::cout << corner.x + __dx[k] * side << ' ' << corner.y + __dy[k] * side << ' ' <<  corner.z + __dz[k] * side << std:: endl;
        // std::cout << corner.x + __dx[l] * side << ' ' << corner.y + __dy[l] * side << ' ' <<  corner.z + __dz[l] * side << std:: endl;
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