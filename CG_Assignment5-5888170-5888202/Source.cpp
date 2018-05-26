/*____________________________________________________________________
|
| File: main.cpp
|
| Description: Hermite curve drawing.
|
| Functions:
|
| (C) Copyright 2007 Mores Prachyabrued
|___________________________________________________________________*/

// Enable older CRT functions (such as strcpy) without warnings from vc8 (vc 2005 .net)
#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE				// shut up the vc8 compiler
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

/*___________________
|
| Include Files
|__________________*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <gmtl/gmtl.h>

#include <GL/glut.h>

/*___________________
|
| Constants
|__________________*/

const int WIN_WIDTH_INIT = 800;
const int WIN_HEIGHT_INIT = 600;

const float C_TSTEP = 0.0005f;            // Curve time step (for rendering curve)

const gmtl::Vec3f WORLD_UP(0, 1, 0);                  // World up axis (Y)

const int PT_NB = 6;                                  // Number of input points (equals #tangents and #segments)
const gmtl::Point3f input_pts[PT_NB] =
{
	gmtl::Point3f(-180, -25, 150),
	gmtl::Point3f(0, -50, 0),
	gmtl::Point3f(-180, 0, -150),
	gmtl::Point3f(0, 25, -150),
	gmtl::Point3f(-180, -10, 0),
	gmtl::Point3f(0, -50, 150)
};

gmtl::Matrix44f MMAT;                                 // Basis matrix for Hermite curve form (const)

const float R_MAX = 2.5f*1200.0f;                     // Expected maximum magnitude of local rightward component of plane's acceleration

const int PARTICLE_NB = 40;                    		  // Number of particles

const float VMAG_MEAN = 100.0f;                       // Velocity
const float VMAG_STD = 25.0f;
const float VDIR_STD = 0.25f;

const int TTL_BASE = 200;                          // Time to live
const int TTL_OFFSET = 50;

const float SMOKE_SIZE = 15;                         // Smoke size

const float S_TSTEP = 0.001f;                          // Simulation time step (for particle update)

const gmtl::Vec3f GRAVITY(0, -100.0f, 0);                 // w.r.t. world, can be upward for smoke

const gmtl::Vec3f V_WIND(100, 0, 0);                  // Wind velocity and drag coefficient (K)
const float K_COEF = 1.0f;

// Keyboard modifiers
enum KeyModifier { KM_SHIFT = 0, KM_CTRL, KM_ALT };

const float ROT_AMOUNT = gmtl::Math::deg2Rad(1.0f);
const float COSTHETA = cos(ROT_AMOUNT);
gmtl::Matrix44f yrotp_mat;
gmtl::Point4f p0, p1, p2, p3, p4, p5, p6, p7, p8;
const float P_WIDTH = 3;
const float P_LENGTH = 3;
const float P_HEIGHT = 1.5f;

/*___________________
|
| Type Definitions
|__________________*/

// Particle structure storing position, velocity, and other properties
typedef struct _MyParticle {
	gmtl::Point3f p;      // Position
	gmtl::Vec3f v;        // Velocity
	float m;              // Mass
	int ttl;              // Time to live, decremented each iteration
	int ttl_init;	  			// Initial ttl, used to fade color as the age increases
} MyParticle;

/*___________________
|
| Global variables
|__________________*/

// camera w.r.t. plane
float distance = 100.0f;
float elevation = -15.0f;                 // In degs
float azimuth = 180.0f;                 // In degs

										// Mouse & keyboard
int mx_prev = 0, my_prev = 0;
bool mbuttons[3] = { false, false, false };
bool kmodifiers[3] = { false, false, false };

gmtl::Vec3f tangents[PT_NB];                          // Tangent at each input point
float s_tan = 1.0f;                                   // A scaling factor for tangents

gmtl::Matrix44f Cmats[PT_NB];                         // Coefficient matrix (C) for each Hermite curve segment (It's actually 4x3, ignore last column)

gmtl::Matrix44f ppose;                                // The plane's pose
gmtl::Matrix44f pposeadj;                             // Adjusted plane coordinate system that the (plane) camera will be attached to
gmtl::Matrix44f pposeadj_inv;                         // Adjusted plane coordinate system (plane's camera is attached to this frame), inverted 
int ps = 0;                                        // The segment in which the plane currently belongs
float pt = 0;                                        // The current t-value for the plane 
float pdt = 0.02f;                                    // delta_t for the plane

MyParticle particles[PARTICLE_NB];    			          // Array of particles

GLuint texture;

// Rendering option
bool render_curve = true;
bool render_constraint = false;

enum TextureID { TID_SKYBACK, TID_SKYBACK_2, TID_SKYLEFT, TID_SKYBOTTOM, TID_SKYTOP, TID_SKYRIGHT, TID_SKYFRONT, TEXTURE_NB };  // Texture IDs, with the last ID indicating the total number of textures
const GLfloat NO_LIGHT[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat AMBIENT_LIGHT[] = { 0.9, 0.9, 0.9, 1.0 };
const GLfloat DIFFUSE_LIGHT[] = { 3.0, 3.0, 3.0, 3.0 };
const GLfloat SPECULAR_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
GLuint textures[TEXTURE_NB];                           // Textures
const float SB_SIZE = 1100.0f;                     // Skybox dimension


												   // Materials
const GLfloat DARKRED_COL[] = { 0.1, 0.0, 0.0, 1.0 };
const GLfloat BRIGHTRED_COL[] = { 0.7, 0.0, 0.0, 1.0 };
const GLfloat DARKBLUE_COL[] = { 0.0, 0.0, 0.1, 1.0 };
const GLfloat BRIGHTBLUE_COL[] = { 0.0, 0.0, 0.7, 1.0 };
const GLfloat DARK_COL[] = { 0.1, 0.1, 0.1, 1.0 };
const GLfloat MEDIUMWHITE_COL[] = { 0.7, 0.7, 0.7, 1.0 };
const GLfloat SPECULAR_COL[] = { 0.7, 0.7, 0.7, 1.0 };

const GLfloat BRIGHTPURPLE_COL[] = { 0.7, 0.0, 0.7, 1.0 };
const GLfloat DARKPURPLE_COL[] = { 0.1, 0.0, 0.1, 1.0 };
const GLfloat BRIGHTYELLOW_COL[] = { 1.0, 0.85, 0.05, 1.0 };
const GLfloat DARKYELLOW_COL[] = { 0.95, 0.75, 0.05, 1.0 };
const GLfloat BRIGHTBROWN_COL[] = { 0.9, 0.55, 0.0, 1.0 };
const GLfloat DARKBROWN_COL[] = { 0.4, 0.05, 0.0, 1.0 };
const GLfloat BRIGHTBROWN2_COL[] = { 0.55, 0.25, 0.0, 1.0 };
const GLfloat DARKBROWN2_COL[] = { 0.05, 0.01, 0.0, 1.0 };

/*___________________
|
| Function Prototypes
|__________________*/

void Init();
void Init_AParticle(MyParticle &par);
void Compute_Tangents();
void Compute_Coefficiences();
void Display_Func();
void Idle_Func();
void Update_Plane();
void Update_PSystem();
void Keyboard_Func(unsigned char key, int x, int y);
void Reshape_Func(int width, int height);
void Mouse_Func(int button, int state, int x, int y);
void Motion_Func(int x, int y);
void Draw_World_Axes();
void Draw_Path();
void Draw_Rocket();
void Draw_Particles();
float FastGauss(float mean, float std);
void LoadPPM(char *fname, unsigned int *w, unsigned int *h, unsigned char **data, int mallocflag);

void DrawBroom(const float width, const float length, const float height);
void DrawCatHead(const float width, const float length, const float height);
void DrawCurcle(const float width, const float length, const float height);

void SetLight(const gmtl::Point4f &pos, const bool is_ambient, const bool is_diffuse, const bool is_specular);

void DrawSkybox(const float s);

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
	glutCreateWindow("Hermite Curve Drawing");

	glutDisplayFunc(Display_Func);
	glutIdleFunc(Idle_Func);
	glutKeyboardFunc(Keyboard_Func);
	glutReshapeFunc(Reshape_Func);
	glutMouseFunc(Mouse_Func);
	glutMotionFunc(Motion_Func);

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
	int i;
	unsigned int texwidth, texheight;
	unsigned char *imagedata;
	char smoketex[50] = "smoketex_3.ppm";
	// OpenGL
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Inits Hermite basis matrix
	MMAT.set(
		2, -2, 1, 1,
		-3, 3, -2, -1,
		0, 0, 1, 0,
		1, 0, 0, 0
	);

	// Inits tangents and coefficient matrices
	Compute_Tangents();
	Compute_Coefficiences();

	// Init adjusted plane-coordinate-system that the camera is attached to (HACK! by calling Update_Plane here)
	Update_Plane();

	// Inits particle system
	for (i = 0; i<PARTICLE_NB; i++) {
		Init_AParticle(particles[i]);
	}

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
| Function: Init_AParticle
|
| Input:
| Output: Init a single particle.
|___________________________________________________________________*/

void Init_AParticle(MyParticle &par)
{
	gmtl::Vec3f v_dir;

	// position (= plane position)
	par.p.set(ppose[0][3], ppose[1][3], ppose[2][3]);

	// velocity (consider plane's -Z axis as mean velocity direction)
	v_dir.set(-ppose[0][2], -ppose[1][2], -ppose[2][2]);
	par.v.set(
		FastGauss(v_dir[0], VDIR_STD),
		FastGauss(v_dir[1], VDIR_STD),
		FastGauss(v_dir[2], VDIR_STD)
	);
	par.v = FastGauss(VMAG_MEAN, VMAG_STD) * par.v;

	// mass
	par.m = 1;

	// ttl
	par.ttl = par.ttl_init = (rand() % TTL_BASE) + TTL_OFFSET;

	Compute_Tangents();
	Compute_Coefficiences();

	unsigned char *img_data;               // Texture image data
	unsigned int  width;                   // Texture width
	unsigned int  height;                  // Texture height


	char skyBack[50] = "skybox_back.ppm";
	char skyBack2[50] = "skybox_back_2.ppm";
	char skyLeft[50] = "skybox_left_2.ppm";
	char skyBottom[50] = "skybox_down_3.ppm";
	char skyFront[50] = "skybox_front_2.ppm";
	char skyUp[50] = "skybox_top_3.ppm";
	char skyRight[50] = "skybox_right_2.ppm";

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//|___________________________________________________________________
	//|
	//| Setup lighting
	//|___________________________________________________________________

	// Disable global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NO_LIGHT);

	// NOTE: for specular reflections, the "local viewer" model produces better
	// results than the default, but is slower. The default would not use the correct
	// vertex-to-eyepoint vector, treating it as always parallel to Z.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Enable two sided lighting
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//|___________________________________________________________________
	//|
	//| Setup texturing
	//|___________________________________________________________________

	// Describe how data will be stored in memory
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select the method for combining texture color with the lighting equation
	// (look up the third parameter)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Generate and setup texture objects
	glGenTextures(TEXTURE_NB, textures);

	// Skybox back wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBACK]);
	LoadPPM(skyBack, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Skybox back2 wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBACK_2]);
	LoadPPM(skyBack2, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Skybox left wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYLEFT]);
	LoadPPM(skyLeft, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Skybox Bottom wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBOTTOM]);
	LoadPPM(skyBottom, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Skybox Top wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYTOP]);
	LoadPPM(skyUp, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Skybox Right wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYRIGHT]);
	LoadPPM(skyRight, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Skybox Front wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYFRONT]);
	LoadPPM(skyFront, &width, &height, &img_data, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
	free(img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

/*____________________________________________________________________
|
| Function: Compute_Tangents
|
| Input:
| Output: Computes tangents from input points and the scale factor
|___________________________________________________________________*/

void Compute_Tangents()
{
	int i;
	int prev, next;       // Adjacent points

	for (i = 0; i<PT_NB; i++) {
		// Compute adjacent points
		if (i == 0) { // First point
			prev = PT_NB - 1;
			next = i + 1;
		}
		else
			if (i == PT_NB - 1) { // Last point
				prev = i - 1;
				next = 0;
			}
			else { // Interior point
				prev = i - 1;
				next = i + 1;
			}

			tangents[i] = s_tan * (input_pts[next] - input_pts[prev]);
	}
}

/*____________________________________________________________________
|
| Function: Compute_Coefficiences
|
| Input:
| Output: Computes coefficiences matrices for curve segments
|___________________________________________________________________*/

void Compute_Coefficiences()
{
	int i;
	int n;                               // Next input point (index)
	gmtl::Matrix44f Gmat;                // Geometry matrix (It's actually 4x3, ignore last column)

	for (i = 0; i<PT_NB; i++) {
		if (i == PT_NB - 1) {
			n = 0;
		}
		else {
			n = i + 1;
		}

		Gmat.set(
			input_pts[i][0], input_pts[i][1], input_pts[i][2], 0,
			input_pts[n][0], input_pts[n][1], input_pts[n][2], 0,
			tangents[i][0], tangents[i][1], tangents[i][2], 0,
			tangents[n][0], tangents[n][1], tangents[n][2], 0
		);

		Cmats[i] = MMAT * Gmat;
	}
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
	// Clear screen & depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Add view matrix
	glTranslatef(0, 0, -distance);
	glRotatef(-elevation, 1, 0, 0);
	glRotatef(-azimuth, 0, 1, 0);
	glMultMatrixf(pposeadj_inv.mData);

	Draw_World_Axes();
	Draw_Path();
	//Draw_Rocket();
	DrawBroom(2.0f, 2.0f, 2.0f);
	DrawCatHead(2.0f, 2.0f, 2.0f);
	Draw_Particles();
	DrawSkybox(SB_SIZE);
	DrawCurcle(P_WIDTH + 2, P_LENGTH, P_HEIGHT);
	DrawCurcle(P_WIDTH + 2 + 700, P_LENGTH + 300, P_HEIGHT);
	// Display!
	glutSwapBuffers();
}

/*____________________________________________________________________
|
| Function: Idle_Func
|
| Input:
| Output: GLUT idle callback function. Called for each simulation step.
|___________________________________________________________________*/

void Idle_Func()
{
	Update_Plane();
	Update_PSystem();

	glutPostRedisplay();
}

/*____________________________________________________________________
|
| Function: Update_Plane
|
| Input:
| Output: Called every simulation step to update the plane's pose.
|___________________________________________________________________*/

void Update_Plane()
{
	float x, y, z;                    // Plane's coordinates
	gmtl::Vec3f xa, ya, za;           // Plane's axes
	gmtl::Vec3f ac;                   // Acceleration vector
	float r;                          // Local rightward component of acceleration
	float ra;                         // Roll angle
	gmtl::AxisAnglef roll_aa;         // Local roll in axis angle and matrix forms
	gmtl::Matrix44f roll_mat;
	gmtl::Vec3f zadj;

	/*____________________________________________________________________
	|
	| Update t and possibly current segment
	|___________________________________________________________________*/

	pt += pdt;
	if (pt > 1) {
		pt -= 1;
		ps++;
		if (ps == PT_NB) {
			ps = 0;
		}
	}

	/*____________________________________________________________________
	|
	| Compute plane's new position by evaluating the polynomials (use Horner's rule for slight speedup)
	|___________________________________________________________________*/

	x = ((Cmats[ps][0][0] * pt + Cmats[ps][1][0])*pt + Cmats[ps][2][0])*pt + Cmats[ps][3][0];
	y = ((Cmats[ps][0][1] * pt + Cmats[ps][1][1])*pt + Cmats[ps][2][1])*pt + Cmats[ps][3][1];
	z = ((Cmats[ps][0][2] * pt + Cmats[ps][1][2])*pt + Cmats[ps][2][2])*pt + Cmats[ps][3][2];

	/*____________________________________________________________________
	|
	| Compute plane's orientation
	|___________________________________________________________________*/

	// Compute direction of motion (z) by evaluating the polynomial derivatives (use Horner's rule for slight speedup)
	za[0] = (3 * Cmats[ps][0][0] * pt + 2 * Cmats[ps][1][0])*pt + Cmats[ps][2][0];
	za[1] = (3 * Cmats[ps][0][1] * pt + 2 * Cmats[ps][1][1])*pt + Cmats[ps][2][1];
	za[2] = (3 * Cmats[ps][0][2] * pt + 2 * Cmats[ps][1][2])*pt + Cmats[ps][2][2];
	gmtl::normalize(za);

	// Compute lateral axis (x)
	gmtl::cross(xa, WORLD_UP, za);
	gmtl::normalize(xa);

	// Compute up axis (x)
	gmtl::cross(ya, za, xa);
	gmtl::normalize(ya);

	/*____________________________________________________________________
	|
	| Compute banked turn (local roll)
	|___________________________________________________________________*/

	// Compute acceleration vector
	ac[0] = 6 * Cmats[ps][0][0] * pt + 2 * Cmats[ps][1][0];
	ac[1] = 6 * Cmats[ps][0][1] * pt + 2 * Cmats[ps][1][1];
	ac[2] = 6 * Cmats[ps][0][2] * pt + 2 * Cmats[ps][1][2];

	// Compute local rightward component of acceleration
	r = gmtl::dot(ac, xa);

	// Clamp r to R_MAX
	if (r > R_MAX) {
		r = R_MAX;
	}
	else
		if (r < -R_MAX) {
			r = -R_MAX;
		}

	// Compute roll angle
	ra = asin(r / R_MAX);

	//printf("%.2f\n", r);

	/*____________________________________________________________________
	|
	| Update plane's pose
	|___________________________________________________________________*/

	ppose.set(
		xa[0], ya[0], za[0], x,
		xa[1], ya[1], za[1], y,
		xa[2], ya[2], za[2], z,
		0, 0, 0, 1
	);
	ppose.setState(gmtl::Matrix44f::AFFINE);

	// Compute local roll (rotation about longitudinal axis (z))
	roll_aa.set(ra, za[0], za[1], za[2]);
	gmtl::set(roll_mat, roll_aa);

	// Apply local roll
	ppose = ppose * roll_mat;

	/*____________________________________________________________________
	|
	| Compute adjusted plane coordinate system and its inverse
	|___________________________________________________________________*/

	gmtl::cross(zadj, xa, WORLD_UP);
	gmtl::normalize(zadj);

	pposeadj.set(
		xa[0], WORLD_UP[0], zadj[0], x,
		xa[1], WORLD_UP[1], zadj[1], y,
		xa[2], WORLD_UP[2], zadj[2], z,
		0, 0, 0, 1
	);
	pposeadj.setState(gmtl::Matrix44f::AFFINE);

	gmtl::invert(pposeadj_inv, pposeadj);
}

/*____________________________________________________________________
|
| Function: Update_PSystem
|
| Input:
| Output: Called every simulation step to update the particle system.
|___________________________________________________________________*/

void Update_PSystem()
{
	int i;
	gmtl::Vec3f F;             // Net force
	gmtl::Vec3f a;             // Acceleration

	for (i = 0; i<PARTICLE_NB; i++) {
		// Life is shorten by one time unit
		particles[i].ttl--;

		if (particles[i].ttl > 0) { // Still active
									// Update position
			particles[i].p += S_TSTEP * particles[i].v;

			// Compute net force from gravity and vicous drag (from wind)
			F = (particles[i].m * GRAVITY) - (K_COEF*(particles[i].v - V_WIND));

			// Calculate acceleration from froce
			a = F / particles[i].m;

			// Update velocity
			particles[i].v += S_TSTEP * a;
		}
		else {  // Died, make it reborn
			Init_AParticle(particles[i]);
		}
	}
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
	case 'c':       // Toggle curve drawing
		render_curve = !render_curve;
		break;
	case 'x':       // Toggle constraint drawing
		render_constraint = !render_constraint;
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
	gluPerspective(45.0f, ((float)width) / height, 1.0f, 10000.0f);
}

//|____________________________________________________________________
//|
//| Function: Mouse_Func
//|
//! \param button     [in] one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
//! \param state      [in] one of GLUT_UP (event is due to release) or GLUT_DOWN (press).
//! \param x          [in] X-coordinate of mouse when an event occured.
//! \param y          [in] Y-coordinate of mouse when an event occured.
//! \return None.
//!
//! GLUT mouse-callback function: called for each mouse click.
//|____________________________________________________________________

void Mouse_Func(int button, int state, int x, int y)
{
	int km_state;

	// Updates button's sate and mouse coordinates
	if (state == GLUT_DOWN) {
		mbuttons[button] = true;
		mx_prev = x;
		my_prev = y;
	}
	else {
		mbuttons[button] = false;
	}

	// Updates keyboard modifiers
	km_state = glutGetModifiers();
	kmodifiers[KM_SHIFT] = km_state & GLUT_ACTIVE_SHIFT ? true : false;
	kmodifiers[KM_CTRL] = km_state & GLUT_ACTIVE_CTRL ? true : false;
	kmodifiers[KM_ALT] = km_state & GLUT_ACTIVE_ALT ? true : false;

	//glutPostRedisplay();      // Asks GLUT to redraw the screen
}

//|____________________________________________________________________
//|
//| Function: Motion_Func
//|
//! \param x      [in] X-coordinate of mouse when an event occured.
//! \param y      [in] Y-coordinate of mouse when an event occured.
//! \return None.
//!
//! GLUT motion-callback function: called for each mouse motion.
//|____________________________________________________________________

void Motion_Func(int x, int y)
{
	int dx, dy, d;

	if (mbuttons[GLUT_LEFT_BUTTON] || mbuttons[GLUT_RIGHT_BUTTON]) {
		// Computes distances the mouse has moved
		dx = x - mx_prev;
		dy = y - my_prev;

		// Updates mouse coordinates
		mx_prev = x;
		my_prev = y;

		// Hold left button to rotate camera
		if (mbuttons[GLUT_LEFT_BUTTON]) {
			if (!kmodifiers[KM_CTRL]) {
				elevation += dy;            // Elevation update
			}
			if (!kmodifiers[KM_SHIFT]) {
				azimuth += dx;              // Azimuth update
			}
		}

		// Hold right button to zoom
		if (mbuttons[GLUT_RIGHT_BUTTON]) {
			if (abs(dx) >= abs(dy)) {
				d = dx;
			}
			else {
				d = -dy;
			}
			distance += d;
		}

		glutPostRedisplay();      // Asks GLUT to redraw the screen
	}
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
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	// y-axis is green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	// z-axis is blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 100.0);
	glEnd();
}

/*____________________________________________________________________
|
| Function: Draw_Path
|
| Input:
| Output: Draws a path.
|___________________________________________________________________*/

void Draw_Path()
{
	int i;
	float x, y, z, t;

	/*____________________________________________________________________
	|
	| Draw input points and tangents
	|___________________________________________________________________*/

	if (render_constraint) {
		for (i = 0; i<PT_NB; i++) {
			glPushMatrix();
			glTranslatef(input_pts[i][0], input_pts[i][1], input_pts[i][2]);
			glColor3f(1, 0, 0);
			glutSolidSphere(5, 16, 16);
			glColor3f(1, 1, 0);
			glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(tangents[i][0], tangents[i][1], tangents[i][2]);
			glEnd();
			glPopMatrix();
		}
	}

	/*____________________________________________________________________
	|
	| Draw Hermite curve segments using line strips
	|___________________________________________________________________*/

	if (render_curve) {
		glColor3f(0, 1, 0);
		for (i = 0; i<PT_NB; i++) {
			// Draw each segment
			glBegin(GL_LINE_STRIP);
			for (t = 0; t <= 1; t += C_TSTEP) {
				// Simple polynomial evaluation
				//float t2 = t*t;
				//float t3 = t2*t;
				//x = Cmats[i][0][0]*t3 + Cmats[i][1][0]*t2 + Cmats[i][2][0]*t + Cmats[i][3][0];
				//y = Cmats[i][0][1]*t3 + Cmats[i][1][1]*t2 + Cmats[i][2][1]*t + Cmats[i][3][1];
				//z = Cmats[i][0][2]*t3 + Cmats[i][1][2]*t2 + Cmats[i][2][2]*t + Cmats[i][3][2];

				// Use Horner's rule for slight speedup
				x = ((Cmats[i][0][0] * t + Cmats[i][1][0])*t + Cmats[i][2][0])*t + Cmats[i][3][0];
				y = ((Cmats[i][0][1] * t + Cmats[i][1][1])*t + Cmats[i][2][1])*t + Cmats[i][3][1];
				z = ((Cmats[i][0][2] * t + Cmats[i][1][2])*t + Cmats[i][2][2])*t + Cmats[i][3][2];
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
}

/*____________________________________________________________________
|
| Function: Draw_Rocket
|
| Input:
| Output: Draw a rocket (longitudial = z, lateral = x, up = y).
|___________________________________________________________________*/

void Draw_Rocket()
{
	glPushMatrix();

	glMultMatrixf(ppose.mData);
	glScalef(4, 4, 4);          // If lighting enabled here, be sure to use GL_RESCALE_NORMAL

								// Draw body
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.0, 0.0); // back face are RED
							  // back face
	glVertex3f(-2.5, 2.5, -5.0);
	glVertex3f(2.5, 2.5, -5.0);
	glVertex3f(2.5, -2.5, -5.0);
	glVertex3f(-2.5, -2.5, -5.0);

	glColor3f(0.0, 0.75, 0.0); // left and right faces are GREEN
							   // left face
	glVertex3f(-2.5, 2.5, -5.0);
	glVertex3f(-2.5, -2.5, -5.0);
	glVertex3f(-2.5, -2.5, 5.0);
	glVertex3f(-2.5, 2.5, 5.0);
	// right face
	glVertex3f(2.5, 2.5, -5.0);
	glVertex3f(2.5, 2.5, 5.0);
	glVertex3f(2.5, -2.5, 5.0);
	glVertex3f(2.5, -2.5, -5.0);

	glColor3f(0.0, 0.0, 0.75); // top and bottom faces are BLUE
							   // top face
	glVertex3f(2.5, 2.5, 5.0);
	glVertex3f(2.5, 2.5, -5.0);
	glVertex3f(-2.5, 2.5, -5.0);
	glVertex3f(-2.5, 2.5, 5.0);
	// bottom face
	glVertex3f(2.5, -2.5, -5.0);
	glVertex3f(2.5, -2.5, 5.0);
	glVertex3f(-2.5, -2.5, 5.0);
	glVertex3f(-2.5, -2.5, -5.0);
	glEnd();

	// Draw head
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.0, 0.0); // top and bottom faces are RED
							  // top face
	glVertex3f(2.5, 2.5, 5.0);
	glVertex3f(-2.5, 2.5, 5.0);
	glVertex3f(0, -2.5, 10.0);
	// bottom face
	glVertex3f(2.5, -2.5, 5.0);
	glVertex3f(0, -2.5, 10.0);
	glVertex3f(-2.5, -2.5, 5.0);

	glColor3f(0.0, 0.5, 0.0); // left and right faces are GREEN
							  // left face
	glVertex3f(-2.5, 2.5, 5.0);
	glVertex3f(-2.5, -2.5, 5.0);
	glVertex3f(0, -2.5, 10.0);
	// right face
	glVertex3f(2.5, 2.5, 5.0);
	glVertex3f(0, -2.5, 10.0);
	glVertex3f(2.5, -2.5, 5.0);
	glEnd();

	// Draw tail wing
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.5); // wing is BLUE
	glVertex3f(0, 2.5, 0);
	glVertex3f(0, 5.0, -5.0);
	glVertex3f(-1.25, 2.5, -5.0);
	glVertex3f(0, 2.5, 0);
	glVertex3f(1.25, 2.5, -5.0);
	glVertex3f(0, 5.0, -5.0);
	glVertex3f(0, 5.0, -5.0);
	glVertex3f(1.25, 2.5, -5.0);
	glVertex3f(-1.25, 2.5, -5.0);
	glEnd();

	// Draw side wings
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.5, 0.0); // wings are GREEN
							  // left
	glVertex3f(-2.5, 0, -5.0);
	glVertex3f(-5.0, 0, -5.0);
	glVertex3f(-2.5, 0, 0);
	glVertex3f(-2.5, 0, 0);
	glVertex3f(-2.5, 2.5, -5.0);
	glVertex3f(-5.0, 0, -5.0);
	glVertex3f(-5.0, 0, -5.0);
	glVertex3f(-2.5, 2.5, -5.0);
	glVertex3f(-2.5, 0, -5.0);
	// right
	glVertex3f(2.5, 0, 0);
	glVertex3f(5.0, 0, -5.0);
	glVertex3f(2.5, 0, -5.0);
	glVertex3f(5.0, 0, -5.0);
	glVertex3f(2.5, 2.5, -5.0);
	glVertex3f(2.5, 0, 0);
	glVertex3f(5.0, 0, -5.0);
	glVertex3f(2.5, 0, -5.0);
	glVertex3f(2.5, 2.5, -5.0);
	glEnd();

	glPopMatrix();
}

/*____________________________________________________________________
|
| Function: Draw_Particles
|
| Input:
| Output: Draw particles as view-oriented billboards.
|___________________________________________________________________*/

void Draw_Particles()
{
	int i;
	gmtl::Matrix44f cam_mat;                    // Camera matrix
	gmtl::Matrix44f dt_mat, el_mat, az_mat;     // distance, elevation, and azimuth matrices, initialized to IDENTITY
	float age_scale;                            // Age factor

												/*____________________________________________________________________
												|
												| Enable texturing and blending
												|___________________________________________________________________*/

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	/*____________________________________________________________________
	|
	| Orient billboards to face the camera
	|___________________________________________________________________*/

	// Set distance matrix
	dt_mat[2][3] = distance;

	// Set elevation matrix
	gmtl::set(el_mat,
		gmtl::AxisAnglef(gmtl::Math::deg2Rad(elevation), 1, 0, 0)
	);

	// Set azimuth matrix
	gmtl::set(az_mat,
		gmtl::AxisAnglef(gmtl::Math::deg2Rad(azimuth), 0, 1, 0)
	);

	// Compute camera w.r.t. world and discard translation
	cam_mat = pposeadj * az_mat * el_mat * dt_mat;
	cam_mat[0][3] = cam_mat[1][3] = cam_mat[2][3] = 0;

	/*____________________________________________________________________
	|
	| Render particles as billboards
	|___________________________________________________________________*/

	for (i = 0; i<PARTICLE_NB; i++) {
		glPushMatrix();
		glTranslatef(particles[i].p[0], particles[i].p[1], particles[i].p[2]);
		glMultMatrixf(cam_mat.mData);       // Orient billboards to face camera
		glBegin(GL_QUADS);
		age_scale = ((float)particles[i].ttl) / particles[i].ttl_init;
		glColor3f(age_scale, age_scale, age_scale);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-SMOKE_SIZE, -SMOKE_SIZE, 0.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(SMOKE_SIZE, -SMOKE_SIZE, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(SMOKE_SIZE, SMOKE_SIZE, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-SMOKE_SIZE, SMOKE_SIZE, 0.0);
		glEnd();
		glPopMatrix();
	}

	/*____________________________________________________________________
	|
	| Restore rendering states
	|___________________________________________________________________*/

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);
}

/*
Random number generator by Donald H. House.
Modified to compile on Visual Studio.Net 2003
*/
float FastGauss(float mean, float std)
{
#define RESOLUTION 2500
	static float lookup[RESOLUTION + 1];

#define itblmax    20    
	/* length - 1 of table describing F inverse */
#define didu    40.0    
	/* delta table position / delta ind. variable           itblmax / 0.5 */

	static float tbl[] =
	{ 0.00000E+00, 6.27500E-02, 1.25641E-01, 1.89000E-01,
		2.53333E-01, 3.18684E-01, 3.85405E-01, 4.53889E-01,
		5.24412E-01, 5.97647E-01, 6.74375E-01, 7.55333E-01,
		8.41482E-01, 9.34615E-01, 1.03652E+00, 1.15048E+00,
		1.28167E+00, 1.43933E+00, 1.64500E+00, 1.96000E+00,
		3.87000E+00 };

	static int hereb4;
	//static struct timeval tv;

	float u, di, delta/*, result*/;
	int i, index, minus = 1;

	if (!hereb4) {
		for (i = 0; i <= RESOLUTION; i++) {
			if ((u = i / (float)RESOLUTION) > 0.5) {
				minus = 0;
				u -= 0.5;
			}

			/* interpolate gaussian random number using table */

			index = (int)(di = (didu * u));
			di -= (float)index;
			delta = tbl[index] + (tbl[index + 1] - tbl[index]) * di;
			lookup[i] = (minus ? -delta : delta);
		}

		/*gettimeofday(&tv, NULL);
		srand((unsigned int)tv.tv_usec);*/
		srand((unsigned)time(NULL));
		hereb4 = 1;
	}

	i = rand() / (RAND_MAX / RESOLUTION) + 1;
	if (i > RESOLUTION) {
		i = RESOLUTION;
	}

	return(mean + std * lookup[i]);
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

void LoadPPM(char *fname, unsigned int *w, unsigned int *h, unsigned char **data, int mallocflag)
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
	} while (s[0] == '#');


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



void DrawSkybox(const float s)
{
	float s2 = s / 2;

	// Turn on texture mapping and disable lighting
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);



	// Left wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYLEFT]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-s2 + 4.2, -s2, s2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-s2 + 4.2, -s2, -s2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-s2 + 4.2, s2, -s2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-s2 + 4.2, s2, s2);
	glEnd();

	// Back2 wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBACK_2]);  // Specify which texture will be used   
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-s2, -s2, -s2 + 4.7);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(s2, -s2, -s2 + 4.7);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(s2, s2, -s2 + 4.7);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-s2, s2, -s2 + 4.7);
	glEnd();

	// Bottom wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBOTTOM]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-s2, -s2, s2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(s2, -s2, s2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(s2, -s2, -s2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-s2, -s2, -s2);
	glEnd();

	// Top wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYTOP]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-s2, s2 - 2, -s2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(s2, s2 - 2, -s2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(s2, s2 - 2, s2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-s2, s2 - 2, s2);
	glEnd();

	// Right wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYRIGHT]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(s2 - 4.6, -s2, -s2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(s2 - 4.6, -s2, s2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(s2 - 4.6, s2, s2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(s2 - 4.6, s2, -s2);
	glEnd();

	// Front wall
	glBindTexture(GL_TEXTURE_2D, textures[TID_SKYFRONT]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(s2, -s2, s2 - 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-s2, -s2, s2 - 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-s2, s2, s2 - 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(s2, s2, s2 - 2);
	glEnd();

	// Turn off texture mapping and enable lighting
	//glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}


void DrawBroom(const float width, const float length, const float height)
{
	glPushMatrix();

	glMultMatrixf(ppose.mData);
	glScalef(4, 4, 4);          // If lighting enabled here, be sure to use GL_RESCALE_NORM
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	//------- Meaw -----------------------------------------
	glBegin(GL_TRIANGLES);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKBROWN_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTBROWN_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);

	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);

	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	// (-0.05, 0.05, 8) (0.05, 0.05, 8) AB

	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // A
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // G
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);  //B
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2);  // H

																			 // (0.05, 0.05, 8) (0.08, 0, 8) BC

	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // B
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // H
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // C
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2);    // I

																			// (0.08, 0, 8) (0.05, -0.05, 8) CD

	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // C
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // I
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // D
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // J

																			 // (0.05, -0.05, 8) (-0.05, -0.05, 8) DE

	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // D
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // J
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // E
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // K

																			  // (-0.05, -0.05, 8) (-0.08, 0, 8) EF

	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // E
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // K
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // F
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // L

																		  // (-0.08, 0, 8) (-0.05, 0.05, 8) FA

	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);  // F
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // L
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // A
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // G

	glEnd();

	glBegin(GL_QUAD_STRIP);

	// 1
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKBROWN2_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTBROWN2_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glVertex3f((-0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2); // 1
	glVertex3f((0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2);  // 2
	glVertex3f((-0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2); // 6
	glVertex3f((0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2);  // 5

	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f((0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2);  // 2
	glVertex3f((0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // 3
	glVertex3f((0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2);  // 5
	glVertex3f((0.15 * scal2) + x2, (-0.05* scal2) + y2, (0 * scal2) + z2); // 4

	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2); // 1
	glVertex3f((-0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2);  // 8
	glVertex3f((-0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2); // 6
	glVertex3f((-0.15 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2);  // 7

	glEnd();

	glBegin(GL_QUAD_STRIP);
	// 2
	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2); // 1
	glVertex3f((0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2);  // 2
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 1
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 2

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2);  // 2
	glVertex3f((0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // 3
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 2
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2); // 3

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (0.1 * scal2) + y2, (0 * scal2) + z2); // 1
	glVertex3f((-0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2);  // 8
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 1
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 8

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2); // 6
	glVertex3f((0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2);  // 5
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 6
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 5

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2);  // 5
	glVertex3f((0.15 * scal2) + x2, (-0.05* scal2) + y2, (0 * scal2) + z2); // 4
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 5
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.12 * scal2) + z2); // 4

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (-0.1 * scal2) + y2, (0 * scal2) + z2); // 6
	glVertex3f((-0.15 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2);  // 7
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 6
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 7

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.15 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2);  // 7
	glVertex3f((-0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2);  // 8
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 7
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 8

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.15 * scal2) + x2, (-0.05* scal2) + y2, (0 * scal2) + z2); // 4
	glVertex3f((0.15 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // 3
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.12 * scal2) + z2); // 4
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2); // 3

	glEnd();

	glBegin(GL_QUAD_STRIP);
	// 3

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 1
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 2
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 1
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 2

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 2
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2); // 3
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 2
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 3

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 1
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 8
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 1
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 8

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 6
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 5
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 6
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 5

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2);  // 5
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.12 * scal2) + z2); // 4
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 5
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.2 * scal2) + z2); // 4

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.12 * scal2) + z2); // 6
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 7
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 6
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 7

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 7
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2);  // 8
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 7
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 8

	glColor3f(0.5f, 0.2f, 0.0f);
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.12 * scal2) + z2); // 4
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.12 * scal2) + z2); // 3
	glVertex3f((0.1 * scal2) + x2, (-0.05* scal2) + y2, (-0.2 * scal2) + z2); // 4
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 3

	glEnd();

	glBegin(GL_QUAD_STRIP);
	// 4
	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 1
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 2
	glVertex3f((-0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2); // 1
	glVertex3f((0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);  // 2

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 2
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 3
	glVertex3f((0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);  // 2
	glVertex3f((0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2); // 3

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 1
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 8
	glVertex3f((-0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2); // 1
	glVertex3f((-0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2);  // 8

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 6
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 5
	glVertex3f((-0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2); // 6
	glVertex3f((0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);  // 5

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2);  // 5
	glVertex3f((0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 4
	glVertex3f((0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);  // 5
	glVertex3f((0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2); // 4

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.06 * scal2) + x2, (-0.06 * scal2) + y2, (-0.2 * scal2) + z2); // 6
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 7
	glVertex3f((-0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2); // 6
	glVertex3f((-0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2);  // 7

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((-0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 7
	glVertex3f((-0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2);  // 8
	glVertex3f((-0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2);  // 7
	glVertex3f((-0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2);  // 8

	glColor3f(0.8f, 0.65f, 0.0f);
	glVertex3f((0.1 * scal2) + x2, (-0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 4
	glVertex3f((0.1 * scal2) + x2, (0.05 * scal2) + y2, (-0.2 * scal2) + z2); // 3
	glVertex3f((0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2); // 4
	glVertex3f((0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2); // 3

	glEnd();

	glBegin(GL_QUAD_STRIP);

	// end
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f((-0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 1
	glVertex3f((0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 2
	glVertex3f((-0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 6
	glVertex3f((0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 5

	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f((0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 2
	glVertex3f((0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2);	// 3
	glVertex3f((0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 5
	glVertex3f((0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2);	// 4

	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3f((-0.18 * scal2) + x2, (0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 1
	glVertex3f((-0.23 * scal2) + x2, (0.13 * scal2) + y2, (-0.9 * scal2) + z2);	// 8
	glVertex3f((-0.18 * scal2) + x2, (-0.18 * scal2) + y2, (-0.9 * scal2) + z2);	// 6
	glVertex3f((-0.23 * scal2) + x2, (-0.13 * scal2) + y2, (-0.9 * scal2) + z2);	// 7

	glEnd();
	glPopMatrix();
}



void DrawCatHead(const float width, const float length, const float height)
{
	glPushMatrix();

	glMultMatrixf(ppose.mData);
	glScalef(4, 4, 4);          // If lighting enabled here, be sure to use GL_RESCALE_NORMAL
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKPURPLE_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTPURPLE_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.51f, 0.33f, 0.53f);
	// R3
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glEnd();
	// R4
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// B8
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glEnd();
	// B9
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glEnd();

	// B11
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();

	// L17
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glEnd();
	// L18
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// B22
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glEnd();
	// B23
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.64 * scal) + y, (0.35 * scal) + z);
	glEnd();
	// B24
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// Eye left
	glColor3f(0.00f, 0.00f, 0.00f);
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.12 * scal) + x, (0.80 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.80 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.12 * scal) + x, (0.75 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.75 * scal) + y, (0.36 * scal) + z);
	glEnd();

	// Eye right
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.30 * scal) + x, (0.80 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.32 * scal) + x, (0.80 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.75 * scal) + y, (0.36 * scal) + z);
	glVertex3f((0.32 * scal) + x, (0.75 * scal) + y, (0.36 * scal) + z);
	glEnd();

	glColor3f(0.31f, 0.13f, 0.33f);
	// Up head
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();

	// Up head Left
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// Up head Right
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	//------- Ear ----------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
	// R5
	glBegin(GL_TRIANGLES);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glEnd();

	// B10
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();

	// B12
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glEnd();

	// L19
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();

	// B25
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();
	// B26
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();
	// B25 - in ear
	glColor3f(0.51f, 0.33f, 0.53f);
	glBegin(GL_TRIANGLES);
	glVertex3f((0.00 * scal) + x, (0.97 * scal) + y, (0.23 * scal) + z);
	glVertex3f((0.11 * scal) + x, (0.91 * scal) + y, (0.29 * scal) + z);
	glVertex3f((0.03 * scal) + x, (0.88 * scal) + y, (0.34 * scal) + z);
	glEnd();
	// B26 - in ear
	glBegin(GL_TRIANGLES);
	glVertex3f((0.44 * scal) + x, (0.97 * scal) + y, (0.23 * scal) + z);
	glVertex3f((0.41 * scal) + x, (0.88 * scal) + y, (0.34 * scal) + z);
	glVertex3f((0.33 * scal) + x, (0.91 * scal) + y, (0.29 * scal) + z);
	glEnd();


	// Up ear left
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glEnd();
	// Up ear right
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();

	// Up head Left
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// Up head Right
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	// R1
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glEnd();
	// R2
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glEnd();

	// B6
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glEnd();
	// B7
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glEnd();

	// L15
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glEnd();
	// L16
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.00 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glEnd();

	// B20
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glEnd();
	// B21
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.10 * scal) + y, (0.44 * scal) + z);
	glEnd();

	//------Down--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glEnd();

	//------Up body--------------------------------------------
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.53 * scal) + y, (0.28 * scal) + z);
	glEnd();

	glPopMatrix();
}


void SetLight(const gmtl::Point4f &pos, const bool is_ambient, const bool is_diffuse, const bool is_specular)
{
	glLightfv(GL_LIGHT0, GL_POSITION, pos.mData);
	if (is_ambient) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT_LIGHT);
	}
	else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, NO_LIGHT);
	}
	if (is_diffuse) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE_LIGHT);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, NO_LIGHT);
	}
	if (is_specular) {
		glLightfv(GL_LIGHT0, GL_SPECULAR, SPECULAR_LIGHT);
	}
	else {
		glLightfv(GL_LIGHT0, GL_SPECULAR, NO_LIGHT);
	}
}

void DrawCurcle(const float width, const float length, const float height) {
	float x = -width * 0.15f + 30.2;
	float y = width * 0.02f - 150.2;
	float z = width * 0.25f - 30.8;
	float scal = width * 2.f;
	float h = 150.f;
	const float COSTHETA = cos(ROT_AMOUNT);
	const float SINTHETA = sin(ROT_AMOUNT);
	yrotp_mat.set(COSTHETA, 0, SINTHETA, 0,
		0, 1, 0, 0,
		-SINTHETA, 0, COSTHETA, 0,
		0, 0, 0, 1);
	yrotp_mat.setState(gmtl::Matrix44f::ORTHOGONAL);

	//float p0[3], p1[3], p2[3], p3[3], p4[3], p5[3], temp[3];
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKPURPLE_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTPURPLE_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.51f, 0.33f, 0.53f);
	p0.set(0, 0, 0, 1);
	p8.set(0, h + 50, 0, 1);
	p1.set(20, 0, 0, 1);
	p6.set(30, h, 0, 1);
	for (int i = 0; i < 360; i++) {
		p2 = yrotp_mat * p1;
		p7 = yrotp_mat * p6;
		p3.set(p1[0], p1[1] + h, p1[2], p1[3]);
		p4.set(p2[0], p2[1] + h, p2[2], p2[3]);
		p5.set(p0[0], p0[1] + h, p0[2], p0[3]);

		glBegin(GL_TRIANGLES);
		glVertex3f(p0[0] + x, p0[1] + y, p0[2] + z);
		glVertex3f(p1[0] + x, p1[1] + y, p1[2] + z);
		glVertex3f(p2[0] + x, p2[1] + y, p2[2] + z);
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glVertex3f(p1[0] + x, p1[1] + y, p1[2] + z);
		glVertex3f(p3[0] + x, p3[1] + y, p3[2] + z);
		glVertex3f(p2[0] + x, p2[1] + y, p2[2] + z);
		glVertex3f(p4[0] + x, p4[1] + y, p4[2] + z);
		glEnd();
		/*
		glBegin(GL_TRIANGLES);
		glVertex3f(p5[0] + x, p5[1] + y, p5[2] + z);
		glVertex3f(p3[0] + x, p3[1] + y, p3[2] + z);
		glVertex3f(p4[0] + x, p4[1] + y, p4[2] + z);
		glEnd();
		*/

		glBegin(GL_QUAD_STRIP);
		glVertex3f(p3[0] + x, p3[1] + y, p3[2] + z);
		glVertex3f(p4[0] + x, p4[1] + y, p4[2] + z);
		glVertex3f(p6[0] + x, p6[1] + y, p6[2] + z);
		glVertex3f(p7[0] + x, p7[1] + y, p7[2] + z);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3f(p8[0] + x, p8[1] + y, p8[2] + z);
		glVertex3f(p6[0] + x, p6[1] + y, p6[2] + z);
		glVertex3f(p7[0] + x, p7[1] + y, p7[2] + z);
		glEnd();

		p1 = p2;
		p6 = p7;
	}


	/*
	glBegin(GL_TRIANGLES);
	glVertex3f((0.00 * scal) + x, (0.00 * scal) + y, (0.20 * scal) + z);
	glVertex3f((0.20 * scal) + x, (0.00 * scal) + y, (0.20 * scal) + z);
	glVertex3f((0.10 * scal) + x, (-0.40 * scal) + y, (0.10 * scal) + z);
	glEnd();
	*/
}