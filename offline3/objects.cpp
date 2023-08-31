#ifndef __OBJECT__.h
#define __OBJECT__.h
#include "Point.cpp"
#include <GL/glut.h>
GLdouble sqrNorm(GLdouble v)
{
    if(v < 0)
    {
        std::cout << "warning value less than 0 " << v << std::endl;
        return 1e-9;
    }
    if(v > 1e15){
        std::cout << "warning very high value " << v << std::endl;
    }
    return v;
}
const GLdouble EPS = 1e-6;
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
    virtual bool intersect(Ray ray, Point& normal, Point& sect) = 0; 
    Object(){}
};

struct Pyramid:Object
{
    Point corner;
    GLdouble width;
    GLdouble height;
    Pyramid(){}
    virtual void draw(){
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
            glVertex3f(corner.x, corner.y, corner.z);
            glVertex3f(corner.x + width, corner.y, corner.z);
            glVertex3f(corner.x + width, corner.y + width, corner.z);
            glVertex3f(corner.x, corner.y + width, corner.z);
        glEnd();
        Point top(corner.x + width / 2, corner.y + width / 2, corner.z);
        top = top + Point(0,0,height);
        glBegin(GL_TRIANGLES);
            glVertex3f(top.x, top.y, top.z);
            glVertex3f(corner.x, corner.y, corner.z);
            glVertex3f(corner.x + width, corner.y, corner.z);

            glVertex3f(top.x, top.y, top.z);
            glVertex3f(corner.x + width, corner.y, corner.z);
            glVertex3f(corner.x + width, corner.y + width, corner.z);

            glVertex3f(top.x, top.y, top.z);
            glVertex3f(corner.x + width, corner.y + width, corner.z);
            glVertex3f(corner.x, corner.y + width, corner.z);

            glVertex3f(top.x, top.y, top.z);
            glVertex3f(corner.x, corner.y + width, corner.z);
            glVertex3f(corner.x, corner.y, corner.z);
        glEnd();
    }
};
struct Sphere:Object
{
    Point center;
    GLdouble radius;
    Sphere(){}
    virtual void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glTranslated(center.x, center.y, center.z);
        glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }
    virtual bool intersect(Ray ray, Point& normal, Point& sect){
        Point uc = center - ray.u;
        if(length(ray.v) < 1e-6){
            cout << "problem at intersect of sphere, ray direction too small\n";
            return false;
        }
        GLdouble base = dot(uc, ray.v) / length(ray.v);

        GLdouble distance = sqrt(sqrNorm(dot(uc, uc) - base * base));
        if(distance > radius + EPS){
            return false;
        }
        GLdouble a, b, c;
        c = dot(uc, uc) - radius * radius;
        b = 2 * uc.x * ray.v.x + 2 * uc.y * ray.v.y + 2 * uc.z * ray.v.z;
        b = -b;
        a = dot(ray.v, ray.v);
        if(b * b - 4 * a * c < 0)
            return false;
        GLdouble t1 = (-b  - sqrt(b * b - 4 * a * c)) / (2 * a);
        GLdouble t2 = (-b  + sqrt(b * b - 4 * a * c)) / (2 * a);
        GLdouble t;
        if(t1 < 0)
        {
            if(t2 < 0)
                return false;
            t = t2;
        }
        else
            t = t1;
        
        sect = ray.u + ray.v * t;
        normal = unit(sect - center);
        return true;
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
        // std::cout << "debug cube draw " << corner.x << ' ' << corner.y << ' ' << corner.z << std::endl;
        // std::cout << side << std::endl;
        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);
        drawHelper(0, 1, 2, 3);
        drawHelper(4, 5, 6, 7);
        drawHelper(0, 1, 5, 4);
        drawHelper(3, 2, 6, 7);
        drawHelper(0, 1, 7, 3);
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
    Point intersect(Ray ray, Point& normal){

    }
};

struct Triangle{
    Point vertex[3];
    Triangle(){}  
};




#endif