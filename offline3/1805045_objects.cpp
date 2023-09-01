#ifndef __OBJECT__h
#define __OBJECT__h
#include "1805045_Point.cpp"
#include "1805045_bitmap_image.hpp"
#include <GL/glut.h>
#include <vector>
#include <string>
const GLdouble PI = acos(-1.0);
#define toDeg(x) ((x) * 180 / PI)
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
const GLdouble EPS = 1e-2;
struct Color
{
    GLdouble r, g, b;
    Color(){}
    Color(GLdouble r, GLdouble g, GLdouble b):r(r), g(g), b(b){}
};
Color operator*(Color a, GLdouble b){
    a.r *= b;
    a.g *= b;
    a.b *= b;
    return a;
}
Color operator+(Color a, Color b){
    a.r += b.r;
    a.g += b.g;
    a.b += b.b;
    return a;
}
struct Ray  
{
    Point u, v;
    Ray(){}
    Ray(Point u, Point v):u(u), v(v){}
};
struct NormalLight
{
    Point position;
    GLdouble fallOff;
    NormalLight(){}
    void draw(){
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslated(position.x, position.y, position.z);
        glutSolidSphere(5, 100, 100);
        glPopMatrix();
    }
};
GLdouble normInvT(GLdouble a){
    if(a > 1)
        return 1;
    if(a < -1)
        return -1;
    return a;
}
struct SpotLight
{
    Point position;
    GLdouble fallOff;
    Point look;
    GLdouble cutoffAngle;
    SpotLight(){}
    void draw(){
        GLdouble height = 10;

        glPushMatrix();
        glColor3f(1, 1, 1);
        Point bpos = position + unit(look - position) * 10;
        Point axis = unit(cross(Point(0, 0, 1), position - bpos));
        glTranslated(bpos.x, bpos.y, bpos.z);
        GLdouble angle = acos(normInvT(dot(axis, Point(0,0,1)) / length(axis)));
        GLdouble base = height * sin(angle);
        glRotated(toDeg(angle), axis.x, axis.y, axis.z);
        glutSolidCone(base,height,50,50);
        glPopMatrix();
    }
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
struct Triangle{
    Point vertex[3];
    Triangle(){}
    Triangle(Point a, Point b, Point c){
        vertex[0] = a;
        vertex[1] = b;
        vertex[2] = c;
    }
    bool intersect(Ray ray, Point& normal, Point& sect){
        Point pnormal = cross(vertex[1] - vertex[0], vertex[2] - vertex[0]);
        pnormal = unit(pnormal);
        if(abs(dot(ray.v , pnormal)) < EPS)
            return false;
        
        GLdouble t = dot(vertex[0], pnormal) - dot(ray.u, pnormal);
        t /= dot(ray.v, pnormal);
        if(t < 0)
            return false;
        sect = ray.u + ray.v * t;
        if(!inside(sect))
            return false;
        normal = pnormal;
        if(dot(pnormal, ray.v) > 0)
        {
            normal = normal * -1;
        }
        return true;
    } 
    bool inside(Point p){
        GLdouble tarea = length(cross(vertex[1] - vertex[0], vertex[2] - vertex[0]));
        GLdouble ar1 = length(cross(vertex[1] - vertex[0], p - vertex[0]));
        GLdouble ar2 = length(cross(vertex[2] - vertex[0], p - vertex[0]));
        GLdouble ar3 = length(cross(p - vertex[1], vertex[2] - vertex[1]));
        if(abs(ar1 + ar2 + ar3 - tarea) < EPS)
            return true;
        return false;
    }
    
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
    virtual bool intersect(Ray ray, Point& normal, Point& sect){
        Triangle tmp;
        bool res = false;
        GLdouble curDis = 1e15;
        Point top(corner.x + width / 2, corner.y + width / 2, corner.z);
        top = top + Point(0,0,height);

        Point tnormal, tsect;

        tmp.vertex[0] = {top.x, top.y, top.z};
        tmp.vertex[1] = {corner.x, corner.y, corner.z};
        tmp.vertex[2] = {corner.x + width, corner.y, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }

        tmp.vertex[0] = {top.x, top.y, top.z};
        tmp.vertex[1] = {corner.x + width, corner.y, corner.z};
        tmp.vertex[2] = {corner.x + width, corner.y + width, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }

        tmp.vertex[0] = {top.x, top.y, top.z};
        tmp.vertex[1] = {corner.x + width, corner.y + width, corner.z};
        tmp.vertex[2] = {corner.x, corner.y + width, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }

        tmp.vertex[0] = {top.x, top.y, top.z};
        tmp.vertex[1] = {corner.x, corner.y + width, corner.z};
        tmp.vertex[2] = {corner.x, corner.y, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }

        //sqr
        tmp.vertex[0] = {corner.x, corner.y, corner.z};
        tmp.vertex[1] = {corner.x + width, corner.y, corner.z};
        tmp.vertex[2] = {corner.x + width, corner.y + width, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }

        tmp.vertex[0] = {corner.x, corner.y, corner.z};
        tmp.vertex[1] = {corner.x + width, corner.y + width, corner.z};
        tmp.vertex[2] = {corner.x, corner.y + width, corner.z};

        if(tmp.intersect(ray, tnormal, tsect)){
            GLdouble tdis = length(tsect - ray.u);
            if(tdis < curDis){
                curDis = tdis;
                normal = tnormal;
                sect = tsect;
                res = true;
            }
        }
        return res;
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
        ray.v = unit(ray.v);
        Point uc = center - ray.u;
        if(length(ray.v) < 1e-6){
            std::cout << "problem at intersect of sphere, ray direction too small" << std::endl;
            return false;
        }
        GLdouble base = dot(uc, ray.v);

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
        if(t1 < 0)t1 = 1e15;
        if(t2 < 0)t2 = 1e15;
        t = std::min(t1, t2);
        if(abs(t - 1e15) < EPS)
            return false;

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
    
    std::vector<Triangle> triangulate(){
        std::vector<Triangle> res;
        triangulateHelper(res, 0, 1, 2, 3);
        triangulateHelper(res, 4, 5, 6, 7);
        triangulateHelper(res, 0, 1, 5, 4);
        triangulateHelper(res, 3, 2, 6, 7);
        triangulateHelper(res, 0, 4, 7, 3);
        triangulateHelper(res, 1, 5, 6, 2);
        return res;
    }
    virtual void draw(){
        // std::cout << "debug cube draw " << corner.x << ' ' << corner.y << ' ' << corner.z << std::endl;
        // std::cout << side << std::endl;
        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);
        drawHelper(0, 1, 2, 3);
        drawHelper(4, 5, 6, 7);
        drawHelper(0, 1, 5, 4);
        drawHelper(3, 2, 6, 7);
        drawHelper(0, 4, 7, 3);
        drawHelper(1, 5, 6, 2);
        glEnd();
    }
    private:
    void triangulateHelper(std::vector<Triangle> & out, int i, int j, int k, int l){
        Triangle tmp;
        tmp.vertex[0] = {corner.x + __dx[i] * side, corner.y + __dy[i] * side, corner.z + __dz[i] * side};
        tmp.vertex[1] = {corner.x + __dx[j] * side, corner.y + __dy[j] * side, corner.z + __dz[j] * side};
        tmp.vertex[2] = {corner.x + __dx[k] * side, corner.y + __dy[k] * side, corner.z + __dz[k] * side};
        out.push_back(tmp);

        tmp.vertex[0] = {corner.x + __dx[i] * side, corner.y + __dy[i] * side, corner.z + __dz[i] * side};
        tmp.vertex[1] = {corner.x + __dx[k] * side, corner.y + __dy[k] * side, corner.z + __dz[k] * side};
        tmp.vertex[2] = {corner.x + __dx[l] * side, corner.y + __dy[l] * side, corner.z + __dz[l] * side};
        out.push_back(tmp);
    }
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
    virtual bool intersect(Ray ray, Point& normal, Point& sect){
        std::vector<Triangle> trs = triangulate();
        Point tnormal, tsect;
        bool res = false;
        GLdouble curMin = 1e15;
        for(Triangle triangle : trs){
            if(triangle.intersect(ray, tnormal, tsect))
            {
                if(length(tsect - ray.u) < curMin){
                    curMin = length(tsect - ray.u);
                    normal = tnormal;
                    sect = tsect;
                    res = true;
                }
            }
        }
        return res;
    }

};
// const std::string filename1 = "texture_w.bmp";
// const std::string filename2 = "texture_b.bmp";
struct Checkerboard
{
    GLdouble width;
    SurfaceProperty surface;
    Color color1 = {0.0,0.0,0.0}; // black
    Color color2 = {1.0, 1.0, 1.0}; // white
    bitmap_image image1;
    bitmap_image image2;
    bool texture;
    int iteration;
    Checkerboard(){
        image1 = bitmap_image("texture_w.bmp");
        image2 = bitmap_image("texture_b.bmp");
        image2.save_image("texture_b_2.bmp");

        texture = false;
    }

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
    bool intersect(Ray ray, Point& normal, Point &sect, Color & color){
        if(abs(ray.v.z) < EPS)
            return false;
        ray.v = unit(ray.v);
        GLdouble t = - ray.u.z / ray.v.z;
        if(t < 0) 
            return false;
        sect = ray.u + ray.v * t;
        if(abs(sect.z) > 1e5){
            return false;
        }
        if(ray.v.z < 0)
            normal = Point(0, 0 ,1);
        else
            normal = Point(0, 0, -1);
        int i = floor(sect.x / width);
        int j = floor(sect.y / width);
        if(!texture){
            if((i + j) % 2 == 0)
                color = color1;
            else
                color = color2;
        }
        else
        { 
            if((i + j) % 2 != 0)
            {
                int x, y;
                int imwi = image1.width();
                int imhi = image1.height();
                x = ((sect.x - i * width) / width) * imwi;
                y = ((sect.y - j * width) / width) * imhi;
                // std::cout << x << ' ' << y << std::endl;
                assert(x >= 0 && x < imwi);
                assert(y >= 0 && y < imhi);
                rgb_t pxcolor = image1.get_pixel(x, y);
                color = Color(pxcolor.red/255.0, pxcolor.green/255.0, pxcolor.blue/255.0);
            }
            else
            {
                int x, y;
                int imwi = image2.width();
                int imhi = image2.height();
                x = ((sect.x - i * width) / width) * imwi;
                y = ((sect.y - j * width) / width) * imhi;
                // std::cout << x << ' ' << y << std::endl;
                assert(x >= 0 && x < imwi);
                assert(y >= 0 && y < imhi);
                rgb_t pxcolor = image2.get_pixel(x, y);
                color = Color(pxcolor.red/255.0, pxcolor.green/255.0, pxcolor.blue/255.0);
            }
        }
        return true;
    }
};

Ray reflect(Ray incoming, Point normal, Point sect)
{
    normal = unit(normal);
    Point cu = incoming.u - sect;
    normal = normal * dot(normal, cu);
    Ray res;
    res.u = sect;
    res.v = unit(incoming.u + (sect - incoming.u + normal) * 2 - sect) ;
    res.u = res.u + res.v * .1;
    return res;
}





#endif