//
//  main.cpp
//  CyberCat
//
//  Created by Goran Stojanovic on 06/02/2020.
//  Copyright © 2020 embware. All rights reserved.
//
#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

#define CATSIM 
#include "CyberCat.h"
#include "Model.h"


/* This line is for Max OSX  */
#include <GLUT/glut.h>


/*! GLUT display callback function */
void display(void);
/*! GLUT window reshape callback function */
void reshape(int, int);



void vertex(Coord val)
{
    glVertex2f(val.x,val.y);
}

std::mutex mutex_angle;

Len ground_dist = 405; // cm

int width = 500, height = 500;
Len bone_length = 80;
Len cat_h = 80;

Model backLeft {{150,200}, 0,0, bone_length};
Model backRight {{150,200}, 0,0, bone_length};

Model frontLeft {{350,200}, 0,0, bone_length};
Model frontRight {{350,200}, 0,0, bone_length};



void thread_update_angle (Degree* angle,Degree value, int delayTimePerDegree)
{
    while (*angle != value)
    {
        
        if (*angle < value)
        {
            (*angle)++;
            
        }
        else if (*angle > value)
        {
            (*angle)--;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayTimePerDegree));
    }
}

struct GraphServoDriver : ServoDriver
{
    std::thread thread[SERVO_NO];
    
    GraphServoDriver(Degree startAngle[SERVO_NO]) : ServoDriver(1000,startAngle)
    {
    }
    
    
    
    inline Milliseconds setServoAngle(uint8 servoIndex, Degree angle)
    {
        if (angle >= config[servoIndex].angleMin && angle<= config[servoIndex].angleMax)
        {
           // first engage (write to) servo to minimize delay
            Milliseconds delayTime = abs((Milliseconds)(angle - servoAngle[servoIndex])) * servoSpeed/60;
            
            thread[servoIndex].swap(*new std::thread {thread_update_angle, &servoAngle[servoIndex], angle, servoSpeed/90});
                  
            return delayTime;
        }
        else
        {
            LOG2("ERROR: Servo %d Angle %d out of valid range",servoIndex,angle);
            return 0;
        }
    }
    
    ~GraphServoDriver()
    {
        for (int id = 0; id < SERVO_NO; id++)
        {
            thread[id].join();
        }
    }
};


Degree startAngles[8] {150,150,150,150,60,60,60,60};
GraphServoDriver driver {startAngles};
CyberCat cat {driver};


void updateModel(Model& frontLeft,Model& frontRight,Model& backLeft, Model& backRight)
{
   frontLeft.set(driver.servoAngle[FLS],driver.servoAngle[FLK]);
   frontRight.set(driver.servoAngle[FRS],driver.servoAngle[FRK]);
   backLeft.set(driver.servoAngle[BLS],driver.servoAngle[BLK]);
   backRight.set(driver.servoAngle[BRS],driver.servoAngle[BRK]);
}

void idle()
{
    // std::lock_guard<std::mutex>  lock{mutex_angle};
    //cat.updateModel(frontLeft,frontRight,backLeft,backRight);
    
    Coord last = backRight.posShoulder;
    Coord front = frontRight.posShoulder;
    
    last.y += ground_dist - std::max(backRight.posFoot.y, backLeft.posFoot.y);
    front.y +=  ground_dist - std::max(frontRight.posFoot.y, frontLeft.posFoot.y);
    
    backLeft.posShoulder = (last);
    backRight.posShoulder = (last);
    frontLeft.posShoulder = (front);
    frontRight.posShoulder = (front);
    
    glutPostRedisplay();
}

void specialKey(int key, int x,int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            cat.up();
            break;
        case GLUT_KEY_DOWN:
            cat.down();
            break;
        case GLUT_KEY_LEFT:
            cat.walk();
            break;
        case GLUT_KEY_RIGHT:
            cat.run();
            break;
            
        
        case GLUT_KEY_F1:
            cat.bounce();
            break;
        case GLUT_KEY_F2:
            cat.dinamic(sqrt(2));
            break;
        case GLUT_KEY_F3:
            cat.height(50);
           
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    
}

void driverThread(ServoDriver* driver)
{
    for(;;)
    {
        driver->actuate();
    }
}

int main(int argc, char* argv[])
{
    // http://www.linuxhowtos.org/data/6/server.c
    // socket server link
    
    std::cout << "CyberCat started";
    std::thread catThread {driverThread, &(cat.driver)};
    
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(width, height);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("Cyber Cat Simulation");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    glutIdleFunc(idle);
    glutSpecialFunc(specialKey);
    //glutMouseFunc(mouse);
    
    /* set the default background color to black */
    glClearColor(0,0,0,.9);
    
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
    
    return 0;
}



/*! glut display callback function.  Every time the window needs to be drawn,
 glut will call this function.  This includes when the window size
 changes, or when another window covering part of this window is
 moved so this window is uncovered.
 */

void drawLimb(Model &limb)
{
    Coord shoulder = limb.posShoulder;
    Coord knee = limb.posKnee;
    Coord foot = limb.posFoot;
    // limb
    
    vertex(shoulder);
    vertex(knee);
    
    vertex(knee);
    vertex(foot);
}

void drawFoot(Coord foot)
{
    const int size = 2.5;
    //foot
    glBegin(GL_QUADS);
    glVertex2f(foot.x - size, foot.y - size);
    glVertex2f(foot.x + size, foot.y - size);
    glVertex2f(foot.x + size, foot.y + size);
    glVertex2f(foot.x - size, foot.y + size);
    
    glEnd();
}

void drawBody()
{
    Coord start = backLeft.posShoulder;
    Coord end = frontLeft.posShoulder;
    
    glBegin(GL_LINES);
    
    glColor3f(0, 1, 0);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    
    glVertex2f(end.x, end.y);
    glVertex2f(end.x, end.y - 20);
    
    glVertex2f(end.x, end.y - 20);
    glVertex2f(start.x, start.y - 20);
    
    glVertex2f(start.x, start.y - 20);
    glVertex2f(start.x, start.y);
    
    // head
    glColor3f(1, 1, 0);  //yellow
    
    glVertex2f(end.x, end.y - 20);
    glVertex2f(end.x, end.y - 50);
    
    glVertex2f(end.x, end.y - 50);
    glVertex2f(end.x - 30, end.y - 50);
    
    glVertex2f(end.x - 30, end.y - 50);
    glVertex2f(end.x- 30, end.y - 20);
    
    glVertex2f(end.x - 30, end.y - 20);
    glVertex2f(end.x, end.y - 20);
    
    glEnd();
    
    glColor3f(1, 0, 0); // Red
    drawFoot(backLeft.posFoot);
    drawFoot(frontLeft.posFoot);
    
    glColor3f(0, 0, 1); // blue
    drawFoot(backRight.posFoot);
    drawFoot(frontRight.posFoot);
}



void drawString(int x, int y, char *string)
{
    glRasterPos2i(x,y);
    
    for (char* c = string; *c != '\0'; c++) {
        //glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void display()
{
    updateModel(frontLeft,frontRight,backLeft,backRight);
    /* clear the color buffer (resets everything to black) */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* start drawing */
    
    
    drawBody();
    
    glBegin(GL_LINES);
    
    // center line
    glColor3f(.2, .2, .2);  //white
    glVertex2f(width/2, height);
    glVertex2f(width/2, 0);
    glVertex2f(0, height/2);
    glVertex2f(width, height/2);
    
    
    // ground
    glColor3f(.5, .5, .5);  //white
    
    glVertex2f(0, ground_dist);
    glVertex2f(width, ground_dist);
    glVertex2f(0,  ground_dist + 3);
    glVertex2f(height, ground_dist + 3);
    
    // limb
    
    glColor3f(0, 0, 1); // blue
    drawLimb(backLeft);
    glColor3f(.9, 0, 0); //red
    drawLimb(backRight);
    
    glColor3f(0, 0, 1); // blue
    drawLimb(frontLeft);
    glColor3f(.9, 0, 0); //red
    drawLimb(frontRight);
    
    /* tell OpenGL we're done drawing triangles */
    glEnd();
    glutSwapBuffers();
}

/*! glut reshape callback function.  GLUT calls this function whenever
 the window is resized, including the first time it is created.
 You can use variables to keep track the current window size.
 */
void reshape(int width, int height)
{
    /* tell OpenGL we want to display in a recangle that is the
     same size as the window
     */
    int pad =20;
    glViewport(pad,pad,2*width -2*pad,2*height -2*pad);
    
    /* switch to the projection matrix */
    glMatrixMode(GL_PROJECTION);
    
    /* clear the projection matrix */
    glLoadIdentity();
    
    /* set the camera view, orthographic projection in 2D */
    //gluOrtho2D(-1, width,-1,height);
    
    // gluOrtho2D(-1, width,height,0);
    gluOrtho2D(0,512,512,0);
    
    
    /* switch back to the model view matrix */
    glMatrixMode(GL_MODELVIEW);
    
}
