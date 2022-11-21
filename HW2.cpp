/*********
   CTIS164 - Template Source Program
----------
STUDENT : EMRE KARACAL
SECTION : 01
HOMEWORK: #2
----------
PROBLEMS:
- UFO's are not spawning in random locations with random colors.
- After hit two UFO's in order to see UFO's again we need to start F1 button.
----------
ADDITIONAL FEATURES:
  - We can aim the gun.
  PRESS F1 : Reset the game
  PAGE UP: change the jet color
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532	   // Degree to Radian conversion

// COLORS
#define COLOR1          1
#define COLOR2          2
#define COLOR3          3

#define FPS 60
#define MAX_FIRE 1000
#define SINGLE   1
#define DOUBLE   2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
    double r, g, b; //colors
}color_t;
typedef struct {
    double x, y;
}point_t;
typedef struct {
    point_t pos;
    double angle;
}player_t;
typedef struct {   //colors and cordinations for UFO1
    point_t pos;
    color_t color;
    double radius;
    bool hit;
    double x, y;
}ufo1_t;
typedef struct {  ////colors and cordinations for UFO2
    double x, y;
    bool hit;
}ufo2_t;
typedef struct {
    point_t pos;
    float angle;
    bool active;
}fire_t;

player_t p = { {0,-250}, 0 };
ufo1_t ufo1 = { -40, false };
ufo2_t ufo2 = { -240, false };
fire_t fire[MAX_FIRE];
point_t c;
bool spacebar = false;
int fire_rate = 0,
mode = SINGLE,
FIRERATE = 15,
UfoDif = 3,
ShipDif = 7,
fSpeed = 8,
ColorMode = COLOR1;
double xEarth;

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

void vertex(point_t P, point_t Tr, double angle) {
    float xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    float yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2f(xp, yp);
}

void drawBackground() {

    if (200 + xEarth > 910)
        xEarth = -1010;

    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, -winHeight / 2);
    glVertex2f(-winWidth / 2, -winHeight / 2);
    glEnd();

    //stars
    glColor3ub(255, 255, 255);
    vprint2(600, 300, 0.2, "*");

    vprint2(200, 300, 0.2, "*");

    vprint2(-200, 300, 0.2, "*");

    vprint2(-600, 300, 0.2, "*");


    vprint2(400, 100, 0.2, "*");

    vprint2(0, 100, 0.2, "*");

    vprint2(-400, 100, 0.2, "*");


    vprint2(600, -100, 0.2, "*");

    vprint2(200, -100, 0.2, "*");

    vprint2(-200, -100, 0.2, "*");

    vprint2(-600, -100, 0.2, "*");

    //Eearth
    glColor3ub(0, 207, 255);
    circle(250 + xEarth, 220, 160);
    glColor3ub(25, 158, 45);
    circle(160 + xEarth, 220, 50);
    glColor3ub(25, 158, 45);
    circle(180 + xEarth, 260, 30);
    glColor3ub(25, 158, 45);
    circle(290 + xEarth, 300, 30);
    glColor3ub(25, 158, 45);
    circle(300 + xEarth, 160, 60);
    glColor3ub(25, 158, 45);
    circle(230 + xEarth, 350, 20);



}
void drawLabel()
{
    glColor3ub(255, 92, 0);
    vprint(-480, 350, GLUT_BITMAP_9_BY_15, "HW2 BY EMRE KARACAL");
}
void Settings() {

    glColor3f(255, 0, 0);
    vprint(-200, 350, GLUT_BITMAP_9_BY_15, " !!!!  MISSION : PROTECT THE EARTH AGAINST ALLIENS !!!!");
    glColor3f(68, 255, 0);
    vprint(-450, -380, GLUT_BITMAP_9_BY_15, "'PAGE UP': CHANGE THE COLOR OF THE JET");
    vprint(-35, -380, GLUT_BITMAP_9_BY_15, "SPACEBAR: FIRE");
    vprint(150, -380, GLUT_BITMAP_9_BY_15, "UP/DOWN: AIM GUN");
    vprint(320, -380, GLUT_BITMAP_9_BY_15, "LEFT/RIGHT:  MOVE JET");

}

void FighterJet() {
    float angle = p.angle * D2R;

    //Main Body of Jet
    switch (ColorMode) {
    case COLOR1: glColor3ub(0, 140, 42);
        break;
    case COLOR2: glColor3ub(255, 0, 0);
        break;
    case COLOR3: glColor3ub(0, 72, 155);

    }
    glBegin(GL_TRIANGLES);
    vertex({ 0  ,  40 }, p.pos, angle);
    vertex({ -60, -30 }, p.pos, angle);
    vertex({ +60, -30 }, p.pos, angle);

    vertex({ 0  ,  0 }, p.pos, angle);
    vertex({ -30, -50 }, p.pos, angle);
    vertex({ +30, -50 }, p.pos, angle);

    vertex({ 0  ,  60 }, p.pos, angle);
    vertex({ -35, -20 }, p.pos, angle);
    vertex({ +35, -20 }, p.pos, angle);

    glColor3ub(255, 255, 255);
    vertex({ 0  ,  45 }, p.pos, angle);
    vertex({ -10 ,  10 }, p.pos, angle);
    vertex({ +10 ,  10 }, p.pos, angle);
    glEnd();



}

void UFO1() {

    glColor4ub(204, 255, 255, 200);
    circle(ufo1.x, ufo1.y, 20);
    glColor3ub(255, 251, 0);
    glBegin(GL_POLYGON);
    glVertex2f(ufo1.x - 18, -3);
    glVertex2f(ufo1.x - 40, -25);
    glVertex2f(ufo1.x + 40, -25);
    glVertex2f(ufo1.x + 18, -3);
    glEnd();
}
void UFO2() {

    //main body
    glColor4ub(204, 255, 255, 200);
    circle(ufo2.x, ufo2.y + 300, 20);
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(ufo2.x + 42, 280);
    glVertex2f(ufo2.x + 20, 301);
    glVertex2f(ufo2.x - 20, 301);
    glVertex2f(ufo2.x - 42, 280);
    glEnd();


}
void Fires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active && mode == SINGLE) {
            glColor3ub(0, 249, 255);
            circle(fire[i].pos.x, fire[i].pos.y, 5);
        }

    }
}

int AvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active == false) return i;
    }
    return -1;
}

bool UFO1_hit(fire_t fire, ufo1_t ufo1) {
    if (fire.pos.x <= ufo1.x + 90 && fire.pos.x >= ufo1.x &&
        fire.pos.y >= -5 && fire.pos.y <= 17)
        return true;
    else
        return false;
}

bool UFO2_hit(fire_t fire, ufo2_t ufo2) {
    if (fire.pos.x <= ufo2.x + 90 && fire.pos.x >= ufo2.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
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

    drawBackground();
    drawLabel();
    Fires();
    FighterJet();
    Settings();
    if (!ufo1.hit) UFO1();
    if (!ufo2.hit) UFO2();

    if (ufo1.hit == 1 && ufo2.hit == 1)
    {
        glColor3ub(255, 0, 254);
        vprint2(-300, 0, 0.2, "!!! CONGRATULATIONS YOU SAVED THE EARTH !!!");
        vprint2(-300, -50, 0.2, "        PRESS F1 TO RESTART");

    }
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

    switch (key) {
    case ' ': spacebar = true; break;
        mode = SINGLE;

    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        spacebar = false;
    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y) {
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1:
        if (ufo2.hit == true) ufo2.x = -240;
        ufo1.hit = false; ufo2.hit = false;
        break;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
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

    if (key == GLUT_KEY_PAGE_UP) {
        if (ColorMode == COLOR3)
            ColorMode = COLOR1;
        else
            ColorMode++;
    }
    // to refresh the window it calls display() function
    //glutPostRedisplay();
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



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    xEarth += 0.2;
    // Moving ship and Aimin the gun
    if (right) p.pos.x += 5;
    if (left) p.pos.x -= 5;
    if (down) p.angle += 3;
    if (up) p.angle -= 3;

    if (p.pos.x + 40 >= winWidth / 2) p.pos.x -= 5;
    if (p.pos.x - 40 <= -winWidth / 2) p.pos.x += 5;
    if (p.angle > 45)p.angle -= 3;
    if (p.angle < -45)p.angle += 3;

    ufo2.x += ShipDif;
    if (ufo2.x >= winWidth / 2)
        ufo2.x = -winWidth / 2 - 120;

    ufo1.x += ShipDif;
    if (ufo1.x >= winWidth / 2)
        ufo1.x = -winWidth / 2 - 120;

    if (spacebar && fire_rate == 0) {
        int availablefire = AvailableFire();
        if (availablefire != -1) {
            fire[availablefire].pos = p.pos;
            fire[availablefire].angle = p.angle + 90;
            fire[availablefire].active = true;
            fire_rate = FIRERATE;
        }
    }

    if (fire_rate > 0)
        fire_rate--;

    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active) {
            fire[i].pos.x += fSpeed * cos(fire[i].angle * D2R);
            fire[i].pos.y += fSpeed * sin(fire[i].angle * D2R);

            if (UFO1_hit(fire[i], ufo1))
                ufo1.hit = true;

            if (UFO2_hit(fire[i], ufo2))
                ufo2.hit = true;
        }
    }

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
    glutInitWindowPosition(250, 120);
    glutCreateWindow("HW2: \"GUARRDIAN OF THE GALAXY\" by Emre Karaçal");

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