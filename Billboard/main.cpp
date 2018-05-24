/*____________________________________________________________________
|
| File: main.cpp
|
| Description: Demonstration of view-oriented billboards and blending
|  effects.
|
| Functions: 
|
| (C) Copyright 2007 Mores Prachyabrued, Dr. Christoph Borst
|___________________________________________________________________*/

/*___________________
|
| Include Files
|__________________*/

#include "first_header.h"

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <GL/glut.h>

/*___________________
|
| Constants
|__________________*/

const int WIN_WIDTH_INIT					= 800;
const int WIN_HEIGHT_INIT					= 600;

const float TZ_INIT								= 500.0f;
const float RX_INIT								= 0.0f;
const float RY_INIT								= 0.0f;

const float FORWARD								= 10.0f;
const float ROT_X									= 4.0f;
const float ROT_Y									= 4.0f;

/*___________________
|
| Global variables
|__________________*/

// camera defined w.r.t. world
float cam_tz						= TZ_INIT;
float cam_rx						= RX_INIT;
float cam_ry						= RY_INIT;

GLuint texture;

bool follow_cam = false;

/*___________________
|
| Function Prototypes
|__________________*/

void Init() ;
void Display_Func();
void Keyboard_Func(unsigned char key, int x, int y);
void Reshape_Func(int width, int height);
void Draw_World_Axes();
void LoadPPM(char *fname, unsigned int *w, unsigned int *h, unsigned char **data, int mallocflag);
void Draw_Billboard(float r, float g, float b);

char smoketex[50] = "smoketex.ppm";

/*____________________________________________________________________
|
| Function: main
|
| Input:
| Output: Program entry point.
|___________________________________________________________________*/

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIN_WIDTH_INIT, WIN_HEIGHT_INIT);
	glutCreateWindow("View-oriented billboards and blending effects");

	glutDisplayFunc(Display_Func);
	glutKeyboardFunc(Keyboard_Func);
	glutReshapeFunc(Reshape_Func);

	Init();

	glutMainLoop();

	return 0;
}

/*____________________________________________________________________
|
| Function: Init
|
| Input:
| Output: Initialization routine.
|___________________________________________________________________*/

void Init() 
{
  unsigned int texwidth, texheight;
  unsigned char *imagedata;

	glClearColor(0, 0, 0.5, 1); 
	glEnable(GL_DEPTH_TEST); 
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);

/*____________________________________________________________________
|
| Load texture
|___________________________________________________________________*/
	  
	// describe how data will be stored in memory
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// generate a new "texture object" and select it for setup
	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture);

	// load an image into memory
	LoadPPM(smoketex, &texwidth, &texheight, &imagedata, 1);

	// describe the image to the graphics system as a texture map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
		GL_RGB, GL_UNSIGNED_BYTE, imagedata);
  free(imagedata);

	// select methods for "scaling" a texture region to a pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// select the method for combining texture color with the lighting equation
	// (look up the third parameter)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	  
	// NOTE: to have another texture map, generate another texture object and
	//       repeat the setup steps. To select which texture is being applied 
	//       during drawing, use glBindTexture() to select.
}

/*____________________________________________________________________
|
| Function: Display_Func
|
| Input:
| Output: GLUT display callback function.
|___________________________________________________________________*/

void Display_Func(void) 
{
	int i;

	// Clear screen & depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Add view matrix, V = Tz(-d)*Rx(-e)*Ry(-a)
	glTranslatef(0, 0, -cam_tz);
	glRotatef(-cam_rx, 1, 0, 0);
	glRotatef(-cam_ry, 0, 1, 0);

	Draw_World_Axes();

	// Billboard 1, no stack
	glPushMatrix();
		glTranslatef(50, 50, 0);
		Draw_Billboard(1, 0 ,0);
	glPopMatrix();

	// Billboard 2, stack
  for(i=0; i<3; i++) {
    glPushMatrix();
      glTranslatef(-50, -50, 0);
      Draw_Billboard(1, 1 ,1);
    glPopMatrix();
  }

	// Display!
	glutSwapBuffers();
}

/*____________________________________________________________________
|
| Function: Keyboard_Func
|
| Input:
| Output: GLUT keyboard callback function.
|___________________________________________________________________*/

void Keyboard_Func(unsigned char key, int x, int y)
{
	switch ((char)key) {

		// camera distance
		case 'q':
		case 'Q':
			cam_tz += FORWARD;
			break;
		case 'a':
		case 'A':
			cam_tz -= FORWARD;
			break;
		// camera elevation
		case 's':
		case 'S':
			cam_rx += ROT_X;
			while(cam_rx >= 360) {
				cam_rx -= 360;
			}
			break;
		case 'w':
		case 'W':
			cam_rx -= ROT_X;
			while(cam_rx <= -360) {
				cam_rx += 360;
			}
			break;
		// camera azimuth
		case 'e':
		case 'E':
			cam_ry += ROT_Y;
			while(cam_ry >= 360) {
				cam_ry -= 360;
			}
			break;
		case 'd':
		case 'D':
			cam_ry -= ROT_Y;
			while(cam_ry <= -360) {
				cam_ry += 360;
			}
			break;

		// Orient billboards to follow camera?
		case 'f':
		case 'F':
			follow_cam = !follow_cam;
			break;

		default:
			break;
	}

	glutPostRedisplay();
}

/*____________________________________________________________________
|
| Function: Reshape_Func
|
| Input:
| Output: GLUT reshape callback function.
|___________________________________________________________________*/

void Reshape_Func(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ((float)width)/height, 1.0f, 1000.0f);
}

/*____________________________________________________________________
|
| Function: Draw_World_Axes
|
| Input:
| Output: Draw world axes.
|___________________________________________________________________*/

void Draw_World_Axes()
{
	glBegin(GL_LINES);
		// x-axis is red
		glColor3f(1.0, 0.0, 0.0); 
		glVertex3f(  0.0,  0.0, 0.0);
		glVertex3f(100.0,  0.0, 0.0);
		// y-axis is green
		glColor3f(0.0, 1.0, 0.0); 
		glVertex3f( 0.0,   0.0, 0.0);
		glVertex3f( 0.0, 100.0, 0.0);
		// z-axis is blue
		glColor3f(0.0, 0.0, 1.0); 
		glVertex3f( 0.0, 0.0,   0.0);
		glVertex3f( 0.0, 0.0, 100.0);
	glEnd();
}

/*____________________________________________________________________
|
| Function: LoadPPM
|
| Input:
| Output: Draw world axes.
|  LoadPPM - a minimal Portable Pixelformat image file loader
|  fname: name of file to load (input)
|  w: width of loaded image in pixels (output)
|  h: height of loaded image in pixels (output)
|  data: image data address (input or output depending on mallocflag)
|  mallocflag: 1 if memory not pre-allocated, 0 if data already points
|              to allocated memory that can hold the image. Note that 
|              if new memory is allocated, free() should be used to 
|              deallocate when it is no longer needed.
|___________________________________________________________________*/

void LoadPPM(char *fname, unsigned int *w, unsigned int *h, 
					unsigned char **data, int mallocflag)
{
  FILE *fp;
  char P, num;
  int max;
  char s[1000];

  if (!(fp = fopen(fname, "rb")))
   {
    perror("cannot open image file\n");
    exit(0);
   }

  fscanf(fp, "%c%c\n", &P, &num);
  if ((P != 'P') || (num != '6'))
   {
    perror("unknown file format for image\n");
    exit(0);
   }

  do
   {
    fgets(s, 999, fp);
   }
  while (s[0] == '#');


  sscanf(s, "%d%d", w, h);
  fgets(s, 999, fp);
  sscanf(s, "%d", &max);

  if (mallocflag)
    if (!(*data = (unsigned char *)malloc(*w * *h * 3)))
     {
      perror("cannot allocate memory for image data\n");
      exit(0);
     }

  fread(*data, 3, *w * *h, fp);

  fclose(fp);
}

/*____________________________________________________________________
|
| Function: Draw_Billboard
|
| Input:
| Output: Draw a billboard.
|___________________________________________________________________*/

void Draw_Billboard(float r, float g, float b)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Orient billboards to face camera	
	if (follow_cam) {
		glRotatef(cam_ry, 0, 1, 0);
		glRotatef(cam_rx, 1, 0, 0);
	}

	glBegin(GL_QUADS);
		glColor3f(r, g, b);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(0.0,  0.0, 0.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(50.0,  0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(50.0, 50.0, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(0.0, 50.0, 0.0);

		//glTexCoord2f(1.0, 1.0);
		//glVertex3f(0.0, 50.0, 0.0);
		//glTexCoord2f(0.0, 1.0);
		//glVertex3f(50.0, 50.0, 0.0);
		//glTexCoord2f(0.0, 0.0);
		//glVertex3f(50.0,  0.0, 0.0);
		//glTexCoord2f(1.0, 0.0);
		//glVertex3f(0.0,  0.0, 0.0);
	glEnd();

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glDisable(GL_TEXTURE_2D);
}