#define _USE_MATH_DEFINES
#include <iostream> 
#include <GL/glut.h>
#include <chrono>
#include <ctime>
#include <vector>
#include <cmath>
struct Point{
    GLdouble x,y;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(GLdouble x, GLdouble y):x(x),y(y){}
    Point operator+(Point b)
    {
        return {x + b.x, y + b.y};
    }
    Point operator-(Point b)
    {
        return {x - b.x, y - b.y};
    }
    Point operator*(Point b)//complex multiply
    {
        return {x * b.x - y * b.y, y * b.x + x * b.y};
    }
};
Point rotate(Point p , GLdouble angle) // angle in degree,counterclockwise
{
        angle *= M_PI / 180.0;
        return p * (Point(cos(angle), sin(angle)));
}
struct ClockShape{
    std::vector<Point> lines; // consists of 2*n points
    Point center; // center of 2 circle
    GLdouble r1, r2 ; // center of 2 circle
    std::vector<Point> hour,minute,sec; 
    GLdouble pr1, pr2; // pendulam circle radius 1 and 2
    GLdouble plen; // pendulam arm length
    Point penCenter1;
    Point penCenter2;
    std::vector<Point> penRec;
};
ClockShape staticData;
ClockShape currentShape;
const GLdouble scWidth = 640 ;
const GLdouble scHeight = 640 ;
const GLdouble clWidth = 400 ;
const GLdouble clHeight = 480 ;
const GLdouble tHeight = 150 ;
const GLdouble paddingX = 120 ;
const GLdouble paddingY = 5 ;
const GLdouble clCenY = 110 ;
const GLdouble radius1 = 160;
const GLdouble radius2 = 190;
const GLdouble smallLine = 10 ;
const GLdouble largeLine = 20 ;

const GLdouble hourBase = 6;
const GLdouble hourDim1 = 90;
const GLdouble hourDim2 = 40;

const GLdouble minBase = 4;
const GLdouble minDim1 = 110;
const GLdouble minDim2 = 40;

const GLdouble secBase = 2;
const GLdouble secDim1 = 120;
const GLdouble secDim2 = 40;

const GLdouble penR1 = 10;
const GLdouble penR2 = 30;
const GLdouble penArmLen = 150; 
const GLdouble penArmWidth = 10;

const GLdouble maxTheta = 45;
const GLdouble cosC = M_PI;


const int frameRate = 30;
int refreshMillis = 1000 / frameRate;
volatile int counter = 0;

void fillStaticData(){
    GLdouble x = clWidth / scWidth ; 
    GLdouble y = 1.0 - (2 * paddingY) / scHeight;


    //outer shape
    staticData.lines.push_back({-x, y});
    staticData.lines.push_back({x, y});

    staticData.lines.push_back({x, y});
    staticData.lines.push_back({x, y - (2 * clHeight) / scHeight});

    staticData.lines.push_back({x, y - (2 * clHeight) / scHeight});
    staticData.lines.push_back({0, y - (2 * (clHeight + tHeight)) 
                                    / scHeight});

    staticData.lines.push_back({-x, y});
    staticData.lines.push_back({-x, y - (2 * clHeight) / scHeight});

    staticData.lines.push_back({-x, y - (2 * clHeight) / scHeight});
    staticData.lines.push_back({0, y - (2 * (clHeight + tHeight)) 
                                    / scHeight});
    
    //circles
    staticData.center = {0.0, 2 * clCenY / scHeight };
    staticData.r1 = 2 * radius1 / scHeight ;
    staticData.r2 = 2 * radius2 / scHeight ; 

    //time markings
    Point outer(2 * radius1 / scHeight, 0);
    Point inner(2 * (radius1 - largeLine) / scHeight, 0);
    //larger markings
    for(int i = 0; i < 4; ++i)
    {
        Point tmp = rotate(outer, i * 90);
        staticData.lines.push_back(tmp + staticData.center);
        
        tmp = rotate(inner, i * 90);
        staticData.lines.push_back(tmp + staticData.center);
    }

    //smaller markers
    inner = {2 * (radius1 - smallLine) / scHeight, 0};
    for(int i = 0; i < 12; ++i)if(i%3)
    {
        Point tmp = rotate(outer, i * 30);
        staticData.lines.push_back(tmp + staticData.center);
        
        tmp = rotate(inner, i * 30);
        staticData.lines.push_back(tmp + staticData.center);
    }

    //non static data
    x = hourBase / scWidth ;

    staticData.hour.push_back({x, 0});
    y = hourDim1 / scHeight;
    staticData.hour.push_back({x, y});
    y = (hourDim1 + hourDim2) / scHeight;
    staticData.hour.push_back({0, y});
    y = hourDim1 / scHeight;
    staticData.hour.push_back({-x,y});
    staticData.hour.push_back({-x,0});

    x = minBase / scWidth ;

    staticData.minute.push_back({x, 0});
    y = minDim1 / scHeight;
    staticData.minute.push_back({x, y});
    y = (minDim1 + minDim2) / scHeight;
    staticData.minute.push_back({0, y});
    y = minDim1 / scHeight;
    staticData.minute.push_back({-x,y});
    staticData.minute.push_back({-x,0});

    x = secBase / scWidth ;

    staticData.sec.push_back({x, 0});
    y = secDim1 / scHeight;
    staticData.sec.push_back({x, y});
    y = (secDim1 + secDim2) / scHeight;
    staticData.sec.push_back({0, y});
    y = secDim1 / scHeight;
    staticData.sec.push_back({-x,y});
    staticData.sec.push_back({-x,0});
    
    staticData.pr1 = 2 * penR1 / scHeight;
    staticData.pr2 = 2 * penR2 / scHeight; 
    staticData.plen = 2 * penArmLen / scHeight;
    staticData.penCenter1 = Point(0.0, 0.0);
    staticData.penCenter2 = staticData.penCenter1;
    staticData.penCenter2.y -= (penArmLen * 2) / scHeight;

    x = penArmWidth / scWidth;
    staticData.penRec.push_back({-x, 0});
    y = staticData.penCenter2.y;
    staticData.penRec.push_back({-x, y});
    staticData.penRec.push_back({x, y});
    staticData.penRec.push_back({x, 0});


}
void rotateVector(std::vector<Point> & v, GLdouble angle)
{
    for (auto &e : v)
        e = rotate(e, angle);
}
void translateVector(std::vector<Point> & v,Point tr)
{
    for (auto &e : v)
        e = e + tr;
}
GLdouble timeToAngle(GLdouble curTime)//in degree
{
    return -6.0 * curTime;
}
ClockShape getCurrentClockShape(int tick)
{
    ClockShape res = staticData;
    auto now = std::chrono::system_clock::now();
    std::time_t cur = std::chrono::system_clock::to_time_t(now);
    std::tm curTime = *std::localtime(std::addressof(cur));
    // std::cout << curTime.tm_hour << ' ' << curTime.tm_min 
    // << ' ' << curTime.tm_sec<<std::endl;

    GLdouble secPosition = timeToAngle(curTime.tm_sec);
    GLdouble minPosition = timeToAngle(curTime.tm_min);
    minPosition += secPosition / 60.0;
    GLdouble hPosition = timeToAngle((curTime.tm_hour%12) * 5);
    hPosition += minPosition / 12.0;

    rotateVector(res.hour, hPosition);
    translateVector(res.hour, res.center);
    
    rotateVector(res.minute, minPosition);
    translateVector(res.minute, res.center);

    rotateVector(res.sec, secPosition);
    translateVector(res.sec, res.center);

    
    GLdouble angle = - maxTheta * cos( cosC * tick / frameRate);

    res.penCenter1 = rotate(res.penCenter1, angle);
    res.penCenter2 = rotate(res.penCenter2, angle);
    for (Point &e: res.penRec)
        e = rotate(e, angle);

    Point tmp = {0, - res.r2 + res.center.y};
    res.penCenter1 = res.penCenter1 + tmp;
    res.penCenter2 = res.penCenter2 + tmp;
    for (Point &e: res.penRec)
        e = e + tmp;
    
    return res;//for now
}

void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
}
void drawCircle(Point center, GLdouble radius)
{
    Point unit(radius,0), prv(radius,0);
    glBegin(GL_LINES);
    for(int i = 0; i <= 360 ; ++i)
    {
        Point actual = prv + center;
        glVertex2f(actual.x, actual.y);
        prv = rotate(unit, i);
        actual = prv + center;
        glVertex2f(actual.x, actual.y);
    }
    glEnd();
}
void drawFilledCircle(Point center, GLdouble radius)
{
    Point unit(radius,0), prv(radius,0);
    glBegin(GL_POLYGON);
    glVertex2f(center.x, center.y);    
    for(int i = 0; i <= 360 ; ++i)
    {
        Point actual = prv + center;
        glVertex2f(actual.x, actual.y);
        prv = rotate(unit, i);
        actual = prv + center;
        glVertex2f(actual.x, actual.y);
    }
    glEnd();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(13);
    glBegin(GL_LINES);
        for(Point p : currentShape.lines)
            glVertex2f(p.x, p.y);

    glEnd();
    
    glPointSize(10);
    glBegin(GL_POINTS);
        
        glVertex2d(currentShape.center.x, currentShape.center.y);
    glEnd();

    glBegin(GL_POLYGON);
        for(Point p: currentShape.hour)
            glVertex2d(p.x, p.y);
    glEnd();

    glBegin(GL_POLYGON);
        for(Point p: currentShape.minute)
            glVertex2d(p.x, p.y);
    glEnd();

    glBegin(GL_POLYGON);
        for(Point p: currentShape.sec)
            glVertex2d(p.x, p.y);
    glEnd();
    
    glBegin(GL_POLYGON);
        for(Point p: currentShape.penRec)
            glVertex2d(p.x, p.y);
    glEnd();

    drawCircle(currentShape.center, currentShape.r1);
    drawCircle(currentShape.center, currentShape.r2);
    
    drawFilledCircle(currentShape.penCenter1, currentShape.pr1);
    drawFilledCircle(currentShape.penCenter2, currentShape.pr2);
    glFlush();

}
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    if (width == 0 ) width = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}


void timerListener(int value)
{
    counter = (counter + 1) % (2 * frameRate);

    currentShape = getCurrentClockShape(counter);
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, timerListener, 0);
}

int main(int argc, char ** argv)
{
    fillStaticData();
    glutInit(&argc,argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Clock");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timerListener, 0);
    initGL();
    glutMainLoop();
    return 0;
}