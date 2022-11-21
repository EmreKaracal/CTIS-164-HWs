/*********
   CTIS164 - Template Source Program
----------
STUDENT : Emre Karacal
SECTION : 01
HOMEWORK: HW3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
- Moon (moving)
- Stars
----------
- I sent only my cpp code, my project includes both vec.cpp and vec.h too =)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"

#define WINDOW_WIDTH  1100
#define WINDOW_HEIGHT  700

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532	   // Degree to Radian conversion


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
    float r, g, b;
}color_t;

typedef struct {
    vec_t   pos;
    color_t color;
    vec_t   vel;
}light_t;

typedef struct {
    vec_t pos,
        vel;
}bullet_t;

light_t Ship = { { 0, 150}, {242 / 255., 52 / 255., 32 / 255.}, {6, 1.5} };
bullet_t bullet;
double mx, my;
bool launch = false;
double xMoon;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void Background() {
    if (200 + xMoon > 910)
        xMoon = -1010;

    //Sky
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, winHeight / 2);
    glColor3ub(58, 7, 109);
    glVertex2f(winWidth / 2, -winHeight / 2);
    glVertex2f(-winWidth / 2, -winHeight / 2);
    glEnd();

    //stars
    glColor3ub(255, 255, 255);
    vprint2(600, 300, 0.3, "*");
    vprint2(200, 300, 0.2, "*");
    vprint2(-200, 300, 0.3, "*");
    vprint2(-600, 300, 0.2, "*");
    vprint2(400, 100, 0.3, "*");
    vprint2(0, 100, 0.2, "*");
    vprint2(-400, 100, 0.3, "*");
    vprint2(600, -100, 0.2, "*");
    vprint2(200, -100, 0.3, "*");
    vprint2(-200, -100, 0.2, "*");
    vprint2(-600, -100, 0.3, "*");
    vprint2(400, -300, 0.2, "*");
    vprint2(0, -300, 0.3, "*");
    vprint2(-400, -300, 0.2, "*");

    //Moon
    glColor3ub(216, 208, 208);
    circle(250 + xMoon, 220, 160);
    glColor3ub(165, 159, 159);
    circle(160 + xMoon, 220, 50);
    circle(180 + xMoon, 260, 30);
    circle(290 + xMoon, 300, 30);
    circle(300 + xMoon, 160, 60);
    circle(230 + xMoon, 350, 20);

}


void DrawShip(light_t s) {
    //Body of the Ship
    glBegin(GL_TRIANGLES);
    glColor3ub(9, 255, 0);
    glVertex2f(Ship.pos.x - 15, Ship.pos.y);
    glVertex2f(Ship.pos.x, Ship.pos.y + 50);
    glVertex2f(Ship.pos.x + 15, Ship.pos.y);

    glVertex2f(Ship.pos.x - 30, Ship.pos.y - 40);
    glVertex2f(Ship.pos.x, Ship.pos.y + 10);
    glVertex2f(Ship.pos.x + 30, Ship.pos.y - 40);

    glVertex2f(Ship.pos.x, Ship.pos.y + 15);
    glVertex2f(Ship.pos.x + 50, Ship.pos.y - 15);
    glVertex2f(Ship.pos.x, Ship.pos.y - 15);



    glVertex2f(Ship.pos.x, Ship.pos.y + 15);
    glVertex2f(Ship.pos.x - 50, Ship.pos.y - 15);
    glVertex2f(Ship.pos.x, Ship.pos.y - 15);

    glColor3ub(255, 255, 255);
    glVertex2f(Ship.pos.x - 5, Ship.pos.y + 10);
    glVertex2f(Ship.pos.x, Ship.pos.y + 40);
    glVertex2f(Ship.pos.x + 5, Ship.pos.y + 10);
    glEnd();

    // Ship Engine
    glColor3ub(255, 251, 0);
    glBegin(GL_POLYGON);
    glVertex2f(Ship.pos.x - 5, Ship.pos.y - 40);
    glVertex2f(Ship.pos.x - 5, Ship.pos.y - 70);
    glVertex2f(Ship.pos.x + 5, Ship.pos.y - 70);
    glVertex2f(Ship.pos.x + 5, Ship.pos.y - 40);
    glEnd();

    glColor3ub(154, 154, 153);
    glBegin(GL_POLYGON);
    glVertex2f(Ship.pos.x - 7, Ship.pos.y - 40);
    glVertex2f(Ship.pos.x - 7, Ship.pos.y - 50);
    glVertex2f(Ship.pos.x + 7, Ship.pos.y - 50);
    glVertex2f(Ship.pos.x + 7, Ship.pos.y - 40);
    glEnd();

}
void InfoBar() {

    glColor3f(255, 0, 0);
    vprint(-500, 300, GLUT_BITMAP_9_BY_15, "EMRE KARACAL");
    vprint(-500, 270, GLUT_BITMAP_9_BY_15, "21903232 - HW3");

}

bool hit() {
    double dx = Ship.pos.x - bullet.pos.x;
    double dy = Ship.pos.y - bullet.pos.y;
    double d = sqrt(dx * dx + dy * dy);
    return d <= 20;
}

void drawBullet(double x, double y) {
    if (launch) {

        glColor3ub(255, 0, 0);
        glBegin(GL_POLYGON);
        glVertex2f(x - 15, y + 2);
        glVertex2f(x + 15, y + 2);
        glVertex2f(x + 15, y - 2);
        glVertex2f(x - 15, y - 2);
        glEnd();

        glColor3ub(255, 0, 0);
        glBegin(GL_POLYGON);
        glVertex2f(x - 2, y + 15);
        glVertex2f(x + 2, y + 15);
        glVertex2f(x + 2, y - 15);
        glVertex2f(x - 2, y - 15);
        glEnd();

        glColor3ub(0, 64, 255);
        circle(x, y, 6);

    }
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    Background();
    //Ship
    DrawShip(Ship);
    drawBullet(bullet.pos.x, bullet.pos.y);
    glColor3f(0, 0, 0);
    InfoBar();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT && stat == GLUT_DOWN && !launch) {
        bullet.pos = { mx, my };
        launch = true;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   mx = x - winWidth / 2;
//   my = winHeight / 2 - y;
void onMove(int x, int y) {
    // Write your codes here.

    mx = x - winWidth / 2.;
    my = winHeight / 2. - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    xMoon += 0.5;

    bullet.vel = mulV(6, unitV(subV(Ship.pos, bullet.pos)));
    vec_t prevPos = bullet.pos;

    bullet.pos = addV(bullet.pos, bullet.vel);

    if (bullet.pos.x > winWidth / 2 || bullet.pos.x <= -winWidth / 2 ||
        bullet.pos.y > winHeight / 2 || bullet.pos.y <= -winHeight / 2)
        bullet.pos = prevPos;


    if (Ship.pos.x < winWidth / 2. - 550)
        Ship.pos.y += 1;
    else
        Ship.pos.y -= 1;

    Ship.pos.x += 4;
    if (Ship.pos.x >= winWidth / 2)
        Ship.pos.x = -winWidth / 2. - 35;

    if (hit())
        launch = false;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 0);
    glutCreateWindow("HW3 - Chasing the ship by Emre Karacal");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();


}