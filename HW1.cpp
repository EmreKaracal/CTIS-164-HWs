/*********
   CTIS164 - Template Source Program
----------
STUDENT : Emre Karaçal - 21903232
SECTION : 01
HOMEWORK: #1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
- Can change the bus color with Page up button.
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

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R      0.0174532

// Modes
#define STOP             0
#define START            1

//ON / OFF TV
#define ON   1
#define OFF  2

// COLORS
#define COLOR1          1
#define COLOR2          2
#define COLOR3          3


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int screen;
int Xshape, Yshape;       // Assigned shape locations
int mode = STOP;          // START: Start Moving, STOP: stop
int ColorMode = COLOR1;      // Color Modes
bool Timer = false; // Start timer to move shape
int r = 0;
double xCloud, xSun;

typedef struct {
    int  sx, sy;
}shape_t;

shape_t ShapesCrdnt[2];

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
void TV()
{
    //TV_main
    glColor3ub(104, 104, 107);
    glBegin(GL_QUADS);
    glVertex2f(-440, 240);
    glVertex2f(440, 240);
    glVertex2f(440, -380);
    glVertex2f(-440, -380);
    glEnd();

    //TV_addition
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-410, 207);
    glVertex2f(410, 207);
    glVertex2f(410, -310);
    glVertex2f(-410, -310);
    glEnd();

    //Red Light
    glColor3ub(254, 0, 0);
    circle(0, -350, 10);

    //buttons_ON / OFF
    glColor3ub(255, 92, 0);
    glBegin(GL_QUADS);
    glVertex2f(-240, -320);
    glVertex2f(-120, -320);
    glVertex2f(-120, -350);
    glVertex2f(-240, -350);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-235, -325);
    glVertex2f(-125, -325);
    glVertex2f(-125, -345);
    glVertex2f(-235, -345);
    glEnd();

    glColor3ub(255, 92, 0);
    vprint(-215, -340, GLUT_BITMAP_9_BY_15, "ON / OFF");

    //buttons_START / STOP
    glColor3ub(255, 92, 0);
    glBegin(GL_QUADS);
    glVertex2f(240, -320);
    glVertex2f(120, -320);
    glVertex2f(120, -350);
    glVertex2f(240, -350);
    glEnd();
   
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(235, -325);
    glVertex2f(125, -325);
    glVertex2f(125, -345);
    glVertex2f(235, -345);
    glEnd();

    glColor3ub(255, 92, 0);
    vprint(140, -340, GLUT_BITMAP_9_BY_15, "START/STOP");

}
void displayBackground_On() {

    if (200 + xSun > 310)
        xSun = -310;
    if (100 + xCloud > 283)
        xCloud = -310;

    //Sky
    glColor3ub(107, 225, 243);
    glBegin(GL_QUADS);
    glVertex2f(-400, 200);
    glVertex2f(400, 200);
    glColor3ub(3, 177, 255);
    glVertex2f(400, -200);
    glVertex2f(-400, -200);
    glEnd();

    // Sun
    glColor3ub(255, 255, 0);
    circle(200 + xSun, 100, 90);

    // Cloud
    glColor3ub(255, 255, 255);
    circle(100 + xCloud, 120, 50);
    circle(100 + xCloud, 80, 50);
    circle(40 + xCloud, 100, 50);
    circle(170 + xCloud, 100, 50);

    // Road
    glColor3ub(64, 64, 64);
    glRectf(-400, -150, 400, -300);

    //Lines
    glColor3f(1, 1, 1);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2f(-400, -160);
    glVertex2f(400, -160);
    glVertex2f(-400, -300);
    glVertex2f(400, -300);
    glEnd();

    //BUS_STOP
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(+90, -150);
    glVertex2f(-50, -150);
    glVertex2f(-50, -70);
    glVertex2f(+90, -70);
    glEnd();

    glColor3ub(168, 111, 47);
    glBegin(GL_QUADS);
    glVertex2f(+70, -120);
    glVertex2f(-30, -120);
    glVertex2f(-30, -55);
    glVertex2f(+70, -55);
    glEnd();

    glColor3ub(19, 71, 0);
    glBegin(GL_QUADS);
    glVertex2f(+80, -150);
    glVertex2f(-40, -150);
    glVertex2f(-40, -80);
    glVertex2f(+80, -80);
    glEnd();

    glColor3ub(168, 111, 47);
    glBegin(GL_QUADS);
    glVertex2f(+70, -150);
    glVertex2f(-30, -150);
    glVertex2f(-30, -130);
    glVertex2f(+70, -130);
    glEnd();

    glColor3ub(255, 255, 255);
    vprint(-2, -73, GLUT_BITMAP_9_BY_15, "TUNUS");


    //BUS
    for (int q = 1; q <= 2; q++) {
        Xshape = (ShapesCrdnt + q - 1)->sx;
        Yshape = (ShapesCrdnt + q - 1)->sy;

        if (!(Xshape >= ShapesCrdnt[q].sx && Xshape <= ShapesCrdnt[q].sx && Yshape >= ShapesCrdnt[q].sy && Yshape <= ShapesCrdnt[q].sy))
        {
            //BUS_BODY
            switch (ColorMode) {
            case COLOR1: glColor3ub(0, 255, 245);
                break;
            case COLOR2: glColor3ub(44, 255, 0);
                break;
            case COLOR3: glColor3ub(255, 255, 255);

            }
            glBegin(GL_POLYGON);
            glVertex2f(Xshape - 140, Yshape - 215);
            glVertex2f(Xshape - 140, Yshape - 110);
            glVertex2f(Xshape + 140, Yshape - 110);
            glVertex2f(Xshape + 140, Yshape - 215);
            glVertex2f(Xshape + 140, Yshape - 110);
            glEnd();

            // WHEELS
            glColor3ub(0, 0, 0);
            circle(Xshape - 70, Yshape - 220, 20);
            glColor3ub(0, 0, 0);
            circle(Xshape + 70, Yshape - 220, 20);

            //RIMS
            glColor3ub(255, 255, 255);
            circle(Xshape - 70, Yshape - 220, 10);
            glColor3ub(255, 255, 255);
            circle(Xshape + 70, Yshape - 220, 10);
            glColor3ub(0, 0, 0);
            circle(Xshape - 70, Yshape - 220, 4);
            glColor3ub(0, 0, 0);
            circle(Xshape + 70, Yshape - 220, 4);

            //WINDOWS
            glBegin(GL_POLYGON);
            glColor3ub(0, 0, 0);
            glVertex2f(Xshape - 132, Yshape - 160);
            glVertex2f(Xshape - 132, Yshape - 120);
            glVertex2f(Xshape - 82, Yshape - 120);
            glVertex2f(Xshape - 82, Yshape - 160);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3ub(0, 0, 0);
            glVertex2f(Xshape - 76, Yshape - 160);
            glVertex2f(Xshape - 76, Yshape - 120);
            glVertex2f(Xshape - 26, Yshape - 120);
            glVertex2f(Xshape - 26, Yshape - 160);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3ub(0, 0, 0);
            glVertex2f(Xshape - 20, Yshape - 160);
            glVertex2f(Xshape - 20, Yshape - 120);
            glVertex2f(Xshape + 30, Yshape - 120);
            glVertex2f(Xshape + 30, Yshape - 160);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3ub(0, 0, 0);
            glVertex2f(Xshape + 36, Yshape - 160);
            glVertex2f(Xshape + 36, Yshape - 120);
            glVertex2f(Xshape + 86, Yshape - 120);
            glVertex2f(Xshape + 86, Yshape - 160);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3ub(0, 0, 0);
            glVertex2f(Xshape + 92, Yshape - 160);
            glVertex2f(Xshape + 92, Yshape - 120);
            glVertex2f(Xshape + 136, Yshape - 120);
            glVertex2f(Xshape + 136, Yshape - 160);
            glEnd();

            // DOOR
            glBegin(GL_POLYGON);
            glColor3ub(137, 141, 138);
            glVertex2f(Xshape - 10, Yshape - 210);
            glVertex2f(Xshape - 10, Yshape - 170);
            glVertex2f(Xshape + 20, Yshape - 170);
            glVertex2f(Xshape + 20, Yshape - 210);
            glEnd();
            glColor3ub(0, 0, 0);
            circle(Xshape + 0, Yshape - 190, 4);

            // FRONT LIGHT
            glBegin(GL_POLYGON);
            glColor3ub(229, 255, 0);
            glVertex2f(Xshape + 135, Yshape - 200);
            glVertex2f(Xshape + 135, Yshape - 175);
            glVertex2f(Xshape + 140, Yshape - 175);
            glVertex2f(Xshape + 140, Yshape - 200);
            glEnd();

            //REAR LIGHT
            glBegin(GL_POLYGON);
            glColor3ub(255, 0, 0);
            glVertex2f(Xshape - 135, Yshape - 200);
            glVertex2f(Xshape - 135, Yshape - 175);
            glVertex2f(Xshape - 140, Yshape - 175);
            glVertex2f(Xshape - 140, Yshape - 200);
            glEnd();

        }
    }

}
void displayBackground_Off() {



    // Sky
    glColor3ub(107, 225, 243);
    glBegin(GL_QUADS);
    glVertex2f(-400, 200);
    glVertex2f(400, 200);
    glColor3ub(3, 177, 255);
    glVertex2f(400, -200);
    glVertex2f(-400, -200);
    glEnd();


    // Sun
    glColor3ub(255, 255, 0);
    circle(200 + xSun, 100, 90);

    // Cloud
    glColor3ub(255, 255, 255);
    circle(100 + xCloud, 120, 50);
    circle(100 + xCloud, 80, 50);
    circle(40 + xCloud, 100, 50);
    circle(170 + xCloud, 100, 50);


    // Road
    glColor3ub(64, 64, 64);
    glRectf(-400, -150, 400, -300);

    //Lines
    glColor3f(1, 1, 1);
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2f(-400, -160);
    glVertex2f(400, -160);
    glVertex2f(-400, -300);
    glVertex2f(400, -300);
    glEnd();


    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);//ekranýn kapanmasý
    TV();
}

void IDbox() {

    glColor3ub(255, 92, 0);
    vprint(340, 370, GLUT_BITMAP_9_BY_15, "Emre Karacal");
    vprint(360, 340, GLUT_BITMAP_9_BY_15, "21903232");
}

void MENU() {
    glColor3ub(255, 92, 0);
    vprint(-350, 350, GLUT_BITMAP_9_BY_15, "PAGE UP: Change the Bus Color");
    
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //


    TV();
    switch (screen)
    {
    case ON: displayBackground_On();
        break;
    case OFF: displayBackground_Off();
        break;
    }
    for (int y = 0; y < 2; y++) {
        if (ShapesCrdnt[y].sx > 270)
            ShapesCrdnt[y].sx = -270;

        if (ShapesCrdnt[y].sy > 300)
            ShapesCrdnt[y].sy = -400;
    }

    IDbox();
    MENU();


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


    if (key == GLUT_KEY_PAGE_UP) {
        if (ColorMode == COLOR3)
            ColorMode = COLOR1;
        else
            ColorMode++;
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x <= 380 && x >= 260 && y >= 720 && y <= 750)
    {
        if (screen == ON)
            screen = OFF;
        else
            screen = ON;
    }



    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x <= 740 && x >= 620 && y >= 720 && y <= 750) {
        if (mode == STOP) {
            mode = START;
            Timer = true;
        }
        else {
            mode = STOP;
            Timer = false;
        }
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
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    xSun += 0.30;
    xCloud += 0.50;

    if (Timer)
        for (int q = 0; q < 2; q++)
            ShapesCrdnt[q].sx += 5;






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
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Television / BUS ");

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