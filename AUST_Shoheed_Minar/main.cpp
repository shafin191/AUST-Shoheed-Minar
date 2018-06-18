#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#define pi 3.14159265358979323846264

#include "imageloader.h"
#include "text3d.h"


using namespace std;

float angle = 180.0f;
float posL1=0.8f;
float posL2=0.8f;
float posL3=0.8f;
float R=0,G=0.4,B=1.0;
int night = 0;
int text_flag = 0;
int rotate_Flag = 0;
int sun_moon_flag = 0;
double PI = acos(-1);
float z = -10.0;
float _angle = -30.0f;
float _scale;
float skyAngle = 360;
//The four strings that are drawn
const char* STRS[3] = {"Ahsanullah University of", "Science and Technology","Bangladesh"};

GLuint _textureIdGrass; //The id of the grass texture
GLuint _textureIdRedTile1; //The id of the Red Tile texture
GLuint _textureIdRedBreak;
GLuint _textureIdSkyDay;
GLuint _textureIdSkyNight;


//Computes a scaling value so that the strings
float computeScale(const char* strs[3]) {
	float maxWidth = 0;
	for(int i = 0; i < 3; i++) {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}

	return 2.6f / maxWidth;
}



void cleanup() {
	t3dCleanup();
}

void my_mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
           {
                if (text_flag == 0) {
                text_flag = 1;
                }
                else {
                text_flag = 0;
            }
           }
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
			//glutIdleFunc(spinDisplay_right);
          //glutIdleFunc(NULL); // make idle function inactive
         break;
      default:
         break;
   }
}


void keyboardkey(int key, int x, int y)
{

	if(key==GLUT_KEY_DOWN)
    {
        R = 0.0; G = 0.4; B = 1.0;
        posL1=0.8f; posL2=0.8f; posL3=0.8f;
            night = 0;
    }
	else if(key==GLUT_KEY_UP)
    {
        R = 0.0; G = 0.0; B = 0.0;
        posL1=0.2f; posL2=0.2f; posL3=0.2f;
              night = 1;
    }

    glutPostRedisplay();
}


void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
            break;
        case 'r':
        case 'R':
            angle += 5.0f;
            if (angle > 360) {
                angle -= 360;
            }
            break;
        case 'd':
        case 'D':
            R = 0.0; G = 0.4; B = 1.0;
            posL1=0.8f; posL2=0.8f; posL3=0.8f;
            night = 0;
            break;
        case 'n':
        case 'N':
            R = 0.0; G = 0.0; B = 0.0;
            posL1=0.2f; posL2=0.2f; posL3=0.2f;
            night = 1;
            break;
        case 't':
        case 'T':
            if (text_flag == 0) {
                text_flag = 1;
            } else {
                text_flag = 0;
            }
            break;
        case 'z':
                z= z+1.0;
                break;
        case 'x':
                z = z-1.0;
                break;


        case 's':
        case 'S':
        case 'm':
        case 'M':
            if (sun_moon_flag == 0) {
                sun_moon_flag = 1;
            } else {
                sun_moon_flag = 0;
            }
            break;
        case ' ':
            if (rotate_Flag == 0) {
                rotate_Flag = 1;
            } else {
                rotate_Flag = 0;
            }
            break;


	}
	glutPostRedisplay();
}

void drawScene()
{


    GLUquadric *quadric;
    quadric = gluNewQuadric();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear before draw somethings
    glClearColor(R,G,B,1); // for background -- day night
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity();

	glTranslatef(0.5f, 0.0f, z);



    // Light 0
    GLfloat lightColor0[] = {posL1, posL2, posL3, 1.0f};
	GLfloat lightPos0[] = {0.0f, 8.0f, -8.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);


	glRotatef(10, 1.0f, 0.0f, 0.0f);
	//glRotatef(-10, 0.0f, 0.0f, 1.0f);
	glRotatef(angle,0.0f, 1.0f, 0.0f);


	//Draw the strings along the sides of a square
	glPushMatrix();
    glScalef(_scale*1.2, _scale*1.2, _scale*1.2);
    glColor3f(1.0f, 0.0f, 0.1f);
    for(int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(180.0,0.0,1.0,0.0);
        glTranslatef(-2, -i*1+15.0, -7+1.5f / _scale);
        if (text_flag == 1) {
            t3dDraw3D(STRS[i], 0, 0, 0.2f);
        }
        glPopMatrix();
    }
    glPopMatrix();


        glPushMatrix();
        glRotatef(0.0,0.0,0.0,1.0); // orbits the planet around the sun
        glTranslatef(1.40,0.0,0.0);        // sets the radius of the orbit
        glRotatef(skyAngle,0.0,1.0,1.0); // revolves the planet on its axis
        glColor3f(4.0, 4.0, 4.0);          // green

        glEnable(GL_TEXTURE_2D);
        if (night == 1) {
            glBindTexture(GL_TEXTURE_2D, _textureIdSkyNight);
        } else {
            glBindTexture(GL_TEXTURE_2D, _textureIdSkyDay);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gluQuadricTexture(quadric, 1);
        //glutWireSphere(0.16,20,20);        // Earth
        gluSphere(quadric,40,20,20);
        glDisable(GL_TEXTURE_2D);


        glPopMatrix();


    /// left
    glBegin(GL_QUADS);



    glColor3f(1.0, 1.0, 1.0);


    //1st col
    // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(3.70f, .10f, 0.8f);
	glVertex3f(3.60f, .10f, 0.8f);
	glVertex3f(3.60f, -1.10f, 0.8f);
    glVertex3f(3.70f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(3.70f, .10f, 0.9f);
	glVertex3f(3.60f, .10f, 0.9f);
	glVertex3f(3.60f, -1.10f, 0.9f);
    glVertex3f(3.70f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(3.60f, .10f, 0.8f);
    glVertex3f(3.60f, .10f, 0.9f);
    glVertex3f(3.60f, -1.10f, 0.9f);
    glVertex3f(3.60f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(3.70f, .10f, 0.8f);
    glVertex3f(3.70f, .10f, 0.9f);
    glVertex3f(3.70f, -1.10f, 0.9f);
    glVertex3f(3.70f, -1.10f, 0.8f);



    //2nd col
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(3.50f, .40f, 0.8f);
	glVertex3f(3.40f, .40f, 0.8f);
	glVertex3f(3.40f, -1.10f, 0.8f);
    glVertex3f(3.50f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(3.50f, .40f, 0.9f);
	glVertex3f(3.40f, .40f, 0.9f);
	glVertex3f(3.40f, -1.10f, 0.9f);
    glVertex3f(3.50f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(3.40f, .40f, 0.8f);
    glVertex3f(3.40f, .40f, 0.9f);
    glVertex3f(3.40f, -1.10f, 0.9f);
    glVertex3f(3.40f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(3.50f, .40f, 0.8f);
    glVertex3f(3.50f, .40f, 0.9f);
    glVertex3f(3.50f, -1.10f, 0.9f);
    glVertex3f(3.50f, -1.10f, 0.8f);


    //3rd col

    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(3.30f, .55f, 0.8f);
	glVertex3f(3.20f, .55f, 0.8f);
	glVertex3f(3.20f, -1.10f, 0.8f);
    glVertex3f(3.30f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(3.30f, .55f, 0.9f);
	glVertex3f(3.20f, .55f, 0.9f);
	glVertex3f(3.20f, -1.10f, 0.9f);
    glVertex3f(3.30f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(3.20f, .55f, 0.8f);
    glVertex3f(3.20f, .55f, 0.9f);
    glVertex3f(3.20f, -1.10f, 0.9f);
    glVertex3f(3.20f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(3.30f, .55f, 0.8f);
    glVertex3f(3.30f, .55f, 0.9f);
    glVertex3f(3.30f, -1.10f, 0.9f);
    glVertex3f(3.30f, -1.10f, 0.8f);


    //4th col

    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(3.10f, 1.0f, 0.8f);
	glVertex3f(3.00f, 1.0f, 0.8f);
	glVertex3f(3.00f, -1.10f, 0.8f);
    glVertex3f(3.10f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(3.10f, 1.0f, 0.9f);
	glVertex3f(3.00f, 1.0f, 0.9f);
	glVertex3f(3.00f, -1.10f, 0.9f);
    glVertex3f(3.10f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(3.00f, 1.0f, 0.8f);
    glVertex3f(3.00f, 1.0f, 0.9f);
    glVertex3f(3.00f, -1.10f, 0.9f);
    glVertex3f(3.00f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(3.10f, 1.0f, 0.8f);
    glVertex3f(3.10f, 1.0f, 0.9f);
    glVertex3f(3.10f, -1.10f, 0.9f);
    glVertex3f(3.10f, -1.10f, 0.8f);


    //5th col
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(2.90f, .55f, 0.8f);
	glVertex3f(2.80f, .55f, 0.8f);
	glVertex3f(2.80f, -1.10f, 0.8f);
    glVertex3f(2.90f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(2.90f, .55f, 0.9f);
	glVertex3f(2.80f, .55f, 0.9f);
	glVertex3f(2.80f, -1.10f, 0.9f);
    glVertex3f(2.90f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(2.80f, .55f, 0.8f);
    glVertex3f(2.80f, .55f, 0.9f);
    glVertex3f(2.80f, -1.10f, 0.9f);
    glVertex3f(2.80f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(2.90f, .55f, 0.8f);
    glVertex3f(2.90f, .55f, 0.9f);
    glVertex3f(2.90f, -1.10f, 0.9f);
    glVertex3f(2.90f, -1.10f, 0.8f);

    //6th col

    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(2.70f, .40f, 0.8f);
	glVertex3f(2.60f, .40f, 0.8f);
	glVertex3f(2.60f, -1.10f, 0.8f);
    glVertex3f(2.70f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(2.70f, .40f, 0.9f);
	glVertex3f(2.60f, .40f, 0.9f);
	glVertex3f(2.60f, -1.10f, 0.9f);
    glVertex3f(2.70f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(2.60f, .40f, 0.8f);
    glVertex3f(2.60f, .40f, 0.9f);
    glVertex3f(2.60f, -1.10f, 0.9f);
    glVertex3f(2.60f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(2.70f, .40f, 0.8f);
    glVertex3f(2.70f, .40f, 0.9f);
    glVertex3f(2.70f, -1.10f, 0.9f);
    glVertex3f(2.70f, -1.10f, 0.8f);


    //7th col
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(2.50f, .10f, 0.8f);
	glVertex3f(2.40f, .10f, 0.8f);
	glVertex3f(2.40f, -1.10f, 0.8f);
    glVertex3f(2.50f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(2.50f, .10f, 0.9f);
	glVertex3f(2.40f, .10f, 0.9f);
	glVertex3f(2.50f, -1.10f, 0.9f);
    glVertex3f(2.40f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(2.40f, .10f, 0.8f);
    glVertex3f(2.40f, .10f, 0.9f);
    glVertex3f(2.40f, -1.10f, 0.9f);
    glVertex3f(2.40f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(2.50f, .10f, 0.8f);
    glVertex3f(2.50f, .10f, 0.9f);
    glVertex3f(2.50f, -1.10f, 0.9f);
    glVertex3f(2.50f, -1.10f, 0.8f);


    ///---------------------------------------box-----------------------------------------

    //box

    glColor3f(0.5,0.5,0.5);
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(2.00f, .12f, 0.5f);
	glVertex3f(-1.0f, .12f, 0.5f);
	glVertex3f(-1.0f, -1.12f, 0.5f);
    glVertex3f(2.00f, -1.12f, 0.5f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(2.00f, .10f, 1.0f);
	glVertex3f(-1.00f, .10f, 1.0f);
	glVertex3f(-1.00f, -1.10f, 1.0f);
    glVertex3f(2.00f, -1.10f, 1.0f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(2.0f, .10f, 0.5f);
    glVertex3f(2.0f, .10f, 1.0f);
    glVertex3f(2.0f, -1.10f, 1.0f);
    glVertex3f(2.0f, -1.10f, 0.5f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-1.00f, .10f, 0.5f);
    glVertex3f(-1.00f, .10f, 01.0f);
    glVertex3f(-1.00f, -1.10f, 01.0f);
    glVertex3f(-1.00f, -1.10f, 0.5f);

    // top
    glNormal3f(0.0, 1.0f, 0.0f);
    glVertex3f(2.00f, .10f, 0.5f);
    glVertex3f(-1.00f, .10f, 0.50f);
    glVertex3f(-1.00f, .10f, 01.0f);
    glVertex3f(2.00f, .10f, 1.0f);

    glEnd();


    glPushMatrix(); //set where to start the current object transformations
    glTranslatef(-0.35, 0, 0); //move cube1 to the right
    glBegin(GL_QUADS);
    ///--------------------------------------------mother-----------------------------
    //1st

    glColor3f(0.0,0.0,0.0);
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(1.50f, 1.05f, 0.8f);
	glVertex3f(1.40f, 1.05f, 0.8f);
	glVertex3f(1.40f, -1.10f, 0.8f);
    glVertex3f(1.50f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(1.50f, 1.05f, 0.9f);
	glVertex3f(1.40f, 1.05f, 0.9f);
	glVertex3f(1.40f, -1.10f, 0.9f);
    glVertex3f(1.50f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(1.40f, 1.05f, 0.8f);
    glVertex3f(1.40f, 1.05f, 0.9f);
    glVertex3f(1.40f, -1.10f, 0.9f);
    glVertex3f(1.40f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(1.50f, 1.05f, 0.8f);
    glVertex3f(1.50f, 1.05f, 0.9f);
    glVertex3f(1.50f, -1.10f, 0.9f);
    glVertex3f(1.50f, -1.10f, 0.8f);

    //2nd
    //back

     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(1.30f, 1.35f, 0.8f);
	glVertex3f(1.20f, 1.35f, 0.8f);
	glVertex3f(1.20f, -1.10f, 0.8f);
    glVertex3f(1.30f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(1.30f, 1.35f, 0.9f);
	glVertex3f(1.20f, 1.35f, 0.9f);
	glVertex3f(1.20f, -1.10f, 0.9f);
    glVertex3f(1.30f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(1.20f, 1.35f, 0.8f);
    glVertex3f(1.20f, 1.35f, 0.9f);
    glVertex3f(1.20f, -1.10f, 0.9f);
    glVertex3f(1.20f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(1.30f, 1.35f, 0.8f);
    glVertex3f(1.30f, 1.35f, 0.9f);
    glVertex3f(1.30f, -1.10f, 0.9f);
    glVertex3f(1.30f, -1.10f, 0.8f);


    //3rd
    //back
     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(1.10f, 1.50f, 0.8f);
	glVertex3f(1.00f, 1.50f, 0.8f);
	glVertex3f(1.00f, -1.10f, 0.8f);
    glVertex3f(1.10f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(1.10f, 1.50f, 0.9f);
	glVertex3f(1.00f, 1.50f, 0.9f);
	glVertex3f(1.00f, -1.10f, 0.9f);
    glVertex3f(1.10f, -1.10f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(1.00f, 1.50f, 0.8f);
    glVertex3f(1.00f, 1.50f, 0.9f);
    glVertex3f(1.00f, -1.10f, 0.9f);
    glVertex3f(1.00f, -1.10f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(1.10f, 1.50f, 0.8f);
    glVertex3f(1.10f, 1.50f, 0.9f);
    glVertex3f(1.10f, -1.10f, 0.9f);
    glVertex3f(1.10f, -1.10f, 0.8f);


    //4th
    //back
     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(0.90f, 1.95f, 0.8f);
	glVertex3f(0.80f, 1.95f, 0.8f);
	glVertex3f(0.80f, -0.90f, 0.8f);
    glVertex3f(0.90f, -0.90f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(0.90f, 1.95f, 0.9f);
	glVertex3f(0.80f, 1.95f, 0.9f);
	glVertex3f(0.80f, -0.90f, 0.9f);
    glVertex3f(0.90f, -0.90f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.80f, 1.95f, 0.8f);
    glVertex3f(0.80f, 1.95f, 0.9f);
    glVertex3f(0.80f, -0.90f, 0.9f);
    glVertex3f(0.80f, -0.90f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(0.90f, 1.95f, 0.8f);
    glVertex3f(0.90f, 1.95f, 0.9f);
    glVertex3f(0.90f, -0.90f, 0.9f);
    glVertex3f(0.90f, -0.90f, 0.8f);

    //5th
    //back
     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(0.70f, 1.50f, 0.8f);
	glVertex3f(0.60f, 1.50f, 0.8f);
	glVertex3f(0.60f, -0.70f, 0.8f);
    glVertex3f(0.70f, -0.70f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(0.70f, 1.50f, 0.9f);
	glVertex3f(0.60f, 1.50f, 0.9f);
	glVertex3f(0.60f, -0.70f, 0.9f);
    glVertex3f(0.70f, -0.70f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.60f, 1.50f, 0.8f);
    glVertex3f(0.60f, 1.50f, 0.9f);
    glVertex3f(0.60f, -0.70f, 0.9f);
    glVertex3f(0.60f, -0.70f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(0.70f, 1.50f, 0.8f);
    glVertex3f(0.70f, 1.50f, 0.9f);
    glVertex3f(0.70f, -0.70f, 0.9f);
    glVertex3f(0.70f, -0.70f, 0.8f);

    //6th
    //back
     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(0.50f, 1.35f, 0.8f);
	glVertex3f(0.40f, 1.35f, 0.8f);
	glVertex3f(0.40f, -0.50f, 0.8f);
    glVertex3f(0.50f, -0.50f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(0.50f, 1.35f, 0.9f);
	glVertex3f(0.40f, 1.35f, 0.9f);
	glVertex3f(0.40f, -0.50f, 0.9f);
    glVertex3f(0.50f, -0.50f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.40f, 1.35f, 0.8f);
    glVertex3f(0.40f, 1.35f, 0.9f);
    glVertex3f(0.40f, -0.50f, 0.9f);
    glVertex3f(0.40f, -0.50f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(0.50f, 1.35f, 0.8f);
    glVertex3f(0.50f, 1.35f, 0.9f);
    glVertex3f(0.50f, -0.50f, 0.9f);
    glVertex3f(0.50f, -0.50f, 0.8f);

    //7th
    //back
     glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(0.30f, 1.05f, 0.8f);
	glVertex3f(0.20f, 1.05f, 0.8f);
	glVertex3f(0.20f, -0.30f, 0.8f);
    glVertex3f(0.30f, -0.30f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(0.30f, 1.05f, 0.9f);
	glVertex3f(0.20f, 1.05f, 0.9f);
	glVertex3f(0.20f, -0.30f, 0.9f);
    glVertex3f(0.30f, -0.30f, 0.9f);

    // left
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.20f, 1.05f, 0.8f);
    glVertex3f(0.20f, 1.05f, 0.9f);
    glVertex3f(0.20f, -0.30f, 0.9f);
    glVertex3f(0.20f, -0.30f, 0.8f);

    // right
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(0.30f, 1.05f, 0.8f);
    glVertex3f(0.30f, 1.05f, 0.9f);
    glVertex3f(0.30f, -0.30f, 0.9f);
    glVertex3f(0.30f, -0.30f, 0.8f);




    glEnd();
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);


    ///-------------------------------right--------------------------------
    //1st col right
    // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-2.70f, .10f, 0.8f);
	glVertex3f(-2.60f, .10f, 0.8f);
	glVertex3f(-2.60f, -1.10f, 0.8f);
    glVertex3f(-2.7f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-2.70f, .10f, 0.9f);
	glVertex3f(-2.60f, .10f, 0.9f);
	glVertex3f(-2.60f, -1.10f, 0.9f);
    glVertex3f(-2.70f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-2.60f, .10f, 0.8f);
    glVertex3f(-2.60f, .10f, 0.9f);
    glVertex3f(-2.60f, -1.10f, 0.9f);
    glVertex3f(-2.60f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-2.70f, .10f, 0.8f);
    glVertex3f(-2.70f, .10f, 0.9f);
    glVertex3f(-2.70f, -1.10f, 0.9f);
    glVertex3f(-2.70f, -1.10f, 0.8f);



    //2nd col
    // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-2.50f, .40f, 0.8f);
	glVertex3f(-2.40f, .40f, 0.8f);
	glVertex3f(-2.40f, -1.40f, 0.8f);
    glVertex3f(-2.50f, -1.40f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-2.50f, .40f, 0.9f);
	glVertex3f(-2.40f, .40f, 0.9f);
	glVertex3f(-2.40f, -1.40f, 0.9f);
    glVertex3f(-2.50f, -1.40f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-2.40f, .40f, 0.8f);
    glVertex3f(-2.40f, .40f, 0.9f);
    glVertex3f(-2.40f, -1.40f, 0.9f);
    glVertex3f(-2.40f, -1.40f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-2.50f, .40f, 0.8f);
    glVertex3f(-2.50f, .40f, 0.9f);
    glVertex3f(-2.50f, -1.40f, 0.9f);
    glVertex3f(-2.50f, -1.40f, 0.8f);




    //3rd col

   // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-2.30f, .55f, 0.8f);
	glVertex3f(-2.20f, .55f, 0.8f);
	glVertex3f(-2.20f, -1.10f, 0.8f);
    glVertex3f(-2.30f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-2.30f, .55f, 0.9f);
	glVertex3f(-2.20f, .55f, 0.9f);
	glVertex3f(-2.20f, -1.10f, 0.9f);
    glVertex3f(-2.30f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-2.20f, .55f, 0.8f);
    glVertex3f(-2.20f, .55f, 0.9f);
    glVertex3f(-2.20f, -1.10f, 0.9f);
    glVertex3f(-2.20f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-2.30f, .55f, 0.8f);
    glVertex3f(-2.30f, .55f, 0.9f);
    glVertex3f(-2.30f, -1.10f, 0.9f);
    glVertex3f(-2.30f, -1.10f, 0.8f);



    //4th col

   // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-2.10f, 1.0f, 0.8f);
	glVertex3f(-2.00f, 1.0f, 0.8f);
	glVertex3f(-2.00f, -1.10f, 0.8f);
    glVertex3f(-2.10f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-2.10f, 1.0f, 0.9f);
	glVertex3f(-2.00f, 1.0f, 0.9f);
	glVertex3f(-2.00f, -1.10f, 0.9f);
    glVertex3f(-2.10f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-2.00f, 1.0f, 0.8f);
    glVertex3f(-2.00f, 1.0f, 0.9f);
    glVertex3f(-2.00f, -1.10f, 0.9f);
    glVertex3f(-2.00f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-2.10f, 1.0f, 0.8f);
    glVertex3f(-2.10f, 1.0f, 0.9f);
    glVertex3f(-2.10f, -1.10f, 0.9f);
    glVertex3f(-2.10f, -1.10f, 0.8f);



    //5th col
       // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-1.90f, .55f, 0.8f);
	glVertex3f(-1.80f, .55f, 0.8f);
	glVertex3f(-1.80f, -1.10f, 0.8f);
    glVertex3f(-1.90f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-1.90f, .55f, 0.9f);
	glVertex3f(-1.80f, .55f, 0.9f);
	glVertex3f(-1.80f, -1.10f, 0.9f);
    glVertex3f(-1.90f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-1.80f, .55f, 0.8f);
    glVertex3f(-1.80f, .55f, 0.9f);
    glVertex3f(-1.80f, -1.10f, 0.9f);
    glVertex3f(-1.80f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-1.90f, .55f, 0.8f);
    glVertex3f(-1.90f, .55f, 0.9f);
    glVertex3f(-1.90f, -1.10f, 0.9f);
    glVertex3f(-1.90f, -1.10f, 0.8f);


    //6th col

      // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-1.70f, .40f, 0.8f);
	glVertex3f(-1.60f, .40f, 0.8f);
	glVertex3f(-1.60f, -1.10f, 0.8f);
    glVertex3f(-1.70f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-1.70f, .40f, 0.9f);
	glVertex3f(-1.60f, .40f, 0.9f);
	glVertex3f(-1.60f, -1.10f, 0.9f);
    glVertex3f(-1.70f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-1.60f, .40f, 0.8f);
    glVertex3f(-1.60f, .40f, 0.9f);
    glVertex3f(-1.60f, -1.10f, 0.9f);
    glVertex3f(-1.60f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-1.70f, .40f, 0.8f);
    glVertex3f(-1.70f, .40f, 0.9f);
    glVertex3f(-1.70f, -1.10f, 0.9f);
    glVertex3f(-1.70f, -1.10f, 0.8f);



    //7th col
       // back
    glNormal3f(0.0, 0.0f, 1.0f);
    glVertex3f(-1.50f, .10f, 0.8f);
	glVertex3f(-1.40f, .10f, 0.8f);
	glVertex3f(-1.40f, -1.10f, 0.8f);
    glVertex3f(-1.50f, -1.10f, 0.8f);

    // front
    glNormal3f(0.0, 0.0f, -1.0f);
    glVertex3f(-1.50f, .10f, 0.9f);
	glVertex3f(-1.40f, .10f, 0.9f);
	glVertex3f(-1.40f, -1.10f, 0.9f);
    glVertex3f(-1.50f, -1.10f, 0.9f);

    // left
    glNormal3f(-1.0, 0.0f, 0.0f);
    glVertex3f(-1.40f, .10f, 0.8f);
    glVertex3f(-1.40f, .10f, 0.9f);
    glVertex3f(-1.40f, -1.10f, 0.9f);
    glVertex3f(-1.40f, -1.10f, 0.8f);

    // right
    glNormal3f(1.0, 0.0f, 0.0f);
    glVertex3f(-1.50f, .10f, 0.8f);
    glVertex3f(-1.50f, .10f, 0.9f);
    glVertex3f(-1.50f, -1.10f, 0.9f);
    glVertex3f(-1.50f, -1.10f, 0.8f);





    glEnd();



    // red floor 1

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGrass);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glPushMatrix();
	// top
	glTranslatef(0.0f, -0.81f, 0.0f);

    glColor3f(0.83,0.83,0.83);

    glBegin(GL_QUADS);


    glNormal3f(0.0, 1.0f, 0.0f);


    glTexCoord2f(-10.0f, 0.0f);
    glVertex3f(-3.0f, 0.0f, 4.0f);
    glTexCoord2f(30.0f, 0.0f);
    glVertex3f(4.0f, 0.0f, 4.0f);
    glTexCoord2f(30.0f, 20.0f);
    glVertex3f(4.0f, 0.0f, -2.6f);
    glTexCoord2f(-10.0f, 20.0f);
    glVertex3f(-3.0f, 0.0f, -2.6f);

    glEnd();

        // front

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 4.0f);

        glColor3f(0.83,0.83,0.83);

        glBegin(GL_QUADS);


        glNormal3f(0.0, 0.0f, -1.0f);


        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-3.0f, 0.0f, 0.0f);
        glTexCoord2f(20.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glTexCoord2f(20.0f, 0.5f);
        glVertex3f(4.0f, -0.30f, 0.0f);
        glTexCoord2f(0.0f, 0.5f);
        glVertex3f(-3.0f, -0.30f, 0.0f);

        glEnd();
        glPopMatrix();

        // back

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -2.6f);

        glColor3f(0.83,0.83,0.83);

        glBegin(GL_QUADS);


        glNormal3f(0.0, 0.0f, 1.0f);


        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-3.0f, 0.0f, 0.0f);
        glTexCoord2f(20.0f, 0.0f);
        glVertex3f(4.0f, 0.0f, 0.0f);
        glTexCoord2f(20.0f, 0.5f);
        glVertex3f(4.0f, -0.30f, 0.0f);
        glTexCoord2f(0.0f, 0.5f);
        glVertex3f(-3.0f, -0.30f, 0.0f);

        glEnd();
        glPopMatrix();

        // left


        glPushMatrix();
        glTranslatef(-3.0f, 0.0f, 0.0f);

         glColor3f(0.83,0.83,0.83);

        glBegin(GL_QUADS);


        glNormal3f(1.0, 0.0f, 0.0f);


        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 4.0f);
        glTexCoord2f(20.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -2.6f);
        glTexCoord2f(20.0f, 0.5f);
        glVertex3f(0.0f, -0.30f, -2.6f);
        glTexCoord2f(0.0f, 0.5f);
        glVertex3f(0.0f, -0.30f, 4.0f);

        glEnd();
        glPopMatrix();

        // right


        glPushMatrix();
        glTranslatef(4.0f, 0.0f, 0.0f);

        glColor3f(0.83,0.83,0.83);

        glBegin(GL_QUADS);


        glNormal3f(-1.0, 0.0f, 0.0f);


        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 4.0f);
        glTexCoord2f(20.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -2.6f);
        glTexCoord2f(20.0f, 0.5f);
        glVertex3f(0.0f, -0.30f, -2.6f);
        glTexCoord2f(0.0f, 0.5f);
        glVertex3f(0.0f, -0.30f, 4.0f);

        glEnd();
        glPopMatrix();



    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    //Grass texture floor

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdRedTile1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPushMatrix();
	glTranslatef(0.5f, -1.11f, 0.0f);

    glColor3f(0.8,0.0,0.0);
    glBegin(GL_QUADS);

    glNormal3f(0.0, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, 20.0f);
    glTexCoord2f(40.0f, 0.0f);
    glVertex3f(20.0f, 0.0f, 20.0f);
    glTexCoord2f(40.0f, 40.0f);
    glVertex3f(20.0f, 0.0f, -20.0f);
    glTexCoord2f(0.0f, 40.0f);
    glVertex3f(-20.0f, 0.0f, -20.0f);

    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdRedBreak);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPushMatrix();
	glTranslatef(0.5f, -1.11f, 0.0f);

	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);

    glNormal3f(0.0, 0.0f, -1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, 20.0f);
    glTexCoord2f(6.0f, 0.0f);
    glVertex3f(20.0f, 0.0f, 20.0f);
    glTexCoord2f(6.0f, 1.0f);
    glVertex3f(20.0f, 2.0f, 20.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-20.0f, 2.0f, 20.0f);


    glNormal3f(0.0, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, -20.0f);
    glTexCoord2f(6.0f, 0.0f);
    glVertex3f(20.0f, 0.0f, -20.0f);
    glTexCoord2f(6.0f, 1.0f);
    glVertex3f(20.0f, 2.0f, -20.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-20.0f, 2.0f, -20.0f);

    glNormal3f(1.0, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, -20.0f);
    glTexCoord2f(6.0f, 0.0f);
    glVertex3f(-20.0f, 0.0f, 20.0f);
    glTexCoord2f(6.0f, 1.0f);
    glVertex3f(-20.0f, 2.0f, 20.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-20.0f, 2.0f, -20.0f);

    glNormal3f(-1.0, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(20.0f, 0.0f, -20.0f);
    glTexCoord2f(6.0f, 0.0f);
    glVertex3f(20.0f, 0.0f, 20.0f);
    glTexCoord2f(6.0f, 1.0f);
    glVertex3f(20.0f, 2.0f, 20.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(20.0f, 2.0f, -20.0f);


	glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);









    // back red circle

    // front
    glPushMatrix();
    glNormal3f(0.0, 0.0f, -1.0f);
    glColor3f(1,0,0);
    glTranslatef(0.5f, 0.9f, 1.3f); //Move forward 5 units
    glBegin(GL_POLYGON);
    float radius = 0.7f;
    for(double i = 0; i < 2 * PI; i += PI / 10) //<-- Change this Value
    glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
    glEnd();
    glPopMatrix();

    // back
    glPushMatrix();
    glNormal3f(0.0, 0.0f, 1.0f);
    glColor3f(1,0,0);
    glTranslatef(0.5f, 0.9f, 1.29f); //Move forward 5 units
    glBegin(GL_POLYGON);
    radius = 0.7f;
    for(double i = 0; i < 2 * PI; i += PI / 10) //<-- Change this Value
    glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
    glEnd();
    glPopMatrix();



	glutSwapBuffers();
}

void timer(int value) {
	if (rotate_Flag == 1) angle += 0.25f;
	if (angle > 360) {
		angle -= 360;
	}

	skyAngle += 0.03;
	if (skyAngle > 360) {
		skyAngle -= 360;
	}

	if(text_flag==1)
    {
        text_flag=0;
    }



	glutPostRedisplay();
	glutTimerFunc(5000, timer, 0);
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}


void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);


	Image* image = loadBMP("grass_texture.bmp");
	_textureIdGrass = loadTexture(image);
	delete image;


	image = loadBMP("red_tile_2.bmp");
	_textureIdRedTile1 = loadTexture(image);
	delete image;

	image = loadBMP("red_break.bmp");
	_textureIdRedBreak = loadTexture(image);
	delete image;

	image = loadBMP("ls1.bmp");
	_textureIdSkyDay = loadTexture(image);
	delete image;

	image = loadBMP("sky_night.bmp");
	_textureIdSkyNight = loadTexture(image);
	delete image;


	t3dInit();
}




int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);

    R = 0.0; G = 0.4; B = 1.0;

	glutCreateWindow("AUST SHAHEED MINAR");
	initRendering();

	_scale = computeScale(STRS);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(keyboardkey);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(my_mouse);


	glutTimerFunc(25, timer, 0);
	glutMainLoop();
	return 0;
}
