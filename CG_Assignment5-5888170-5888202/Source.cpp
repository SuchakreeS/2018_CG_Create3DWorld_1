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

#include <math.h>

#include <gmtl/gmtl.h>

#include <GL/glut.h>

/*___________________
|
| Constants
|__________________*/

const int WIN_WIDTH_INIT = 800;
const int WIN_HEIGHT_INIT = 600;

const float T_STEP = 0.001f;

const gmtl::Vec3f WORLD_UP(0, 1, 0);                  // World up axis (Y)

													  // Textures
enum TextureID { TID_SKYBACK, TID_SKYBACK_2, TID_SKYLEFT, TID_SKYBOTTOM, TID_SKYTOP, TID_SKYRIGHT, TID_SKYFRONT, TEXTURE_NB };  // Texture IDs, with the last ID indicating the total number of textures
const GLfloat NO_LIGHT[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat AMBIENT_LIGHT[] = { 0.1, 0.1, 0.1, 1.0 };
const GLfloat DIFFUSE_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat SPECULAR_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
GLuint textures[TEXTURE_NB];                           // Textures
const float SB_SIZE = 1500.0f;                     // Skybox dimension

												   // Textures
enum TextureID { TID_SKYBACK, TID_SKYBACK_2, TID_SKYLEFT, TID_SKYBOTTOM, TID_SKYTOP, TID_SKYRIGHT, TID_SKYFRONT, TEXTURE_NB };  // Texture IDs, with the last ID indicating the total number of textures
const GLfloat NO_LIGHT[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat AMBIENT_LIGHT[] = { 0.1, 0.1, 0.1, 1.0 };
const GLfloat DIFFUSE_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat SPECULAR_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
GLuint textures[TEXTURE_NB];                           // Textures
const float SB_SIZE = 1500.0f;                     // Skybox dimension

const GLfloat NO_LIGHT[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat AMBIENT_LIGHT[] = { 0.1, 0.1, 0.1, 1.0 };
const GLfloat DIFFUSE_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat SPECULAR_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };

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

													  // Keyboard modifiers
enum KeyModifier { KM_SHIFT = 0, KM_CTRL, KM_ALT };

/*___________________
|
| Global variables
|__________________*/

// camera w.r.t. plane
float distance = 600.0f;
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
gmtl::Matrix44f pposeadj_inv;                         // Adjusted plane coordinate system (plane's camera is attached to this frame), inverted 
int ps = 0;                                        // The segment in which the plane currently belongs
float pt = 0;                                        // The current t-value for the plane 
float pdt = 0.01f;                                    // delta_t for the plane

													  // Rendering option
bool render_curve = true;
bool render_constraint = false;

/*___________________
|
| Function Prototypes
|__________________*/

void Init();
void Compute_Tangents();
void Compute_Coefficiences();
void Display_Func();
void Idle_Func();
void Update_Plane();
void Keyboard_Func(unsigned char key, int x, int y);
void Reshape_Func(int width, int height);
void Mouse_Func(int button, int state, int x, int y);
void Motion_Func(int x, int y);
void Draw_World_Axes();
void Draw_Path();
void Draw_Rocket();
void DrawBroom(const float width, const float length, const float height);
void DrawCatBody(const float width, const float length, const float height);
void DrawCatHead(const float width, const float length, const float height);
void DrawCatEar(const float width, const float length, const float height);



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

	//Draw_World_Axes();
	Draw_Path();
	//Draw_Rocket();
	DrawBroom(20.0f, 20.0f, 20.0f);
	DrawCatHead(20.0f, 20.0f, 20.0f);
	DrawCatEar(20.0f, 20.0f, 20.0f);
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
	// Update_PSystem();

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
	gmtl::Matrix44f pposeadj;         // Adjusted plane coordinate system that the (plane) camera will be attached to
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
			for (t = 0; t <= 1; t += T_STEP) {
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




void DrawCatBody(const float width, const float length, const float height)
{
	float x = -width * 0.15f;
	float y = width * 0.02f - 3;
	float z = width * 0.25f - 2.2;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKPURPLE_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTPURPLE_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);
	

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

//|____________________________________________________________________
//! Draws Cat Ear.
//|____________________________________________________________________

void DrawCatEar(const float width, const float length, const float height)
{
	glPushMatrix();

	glMultMatrixf(ppose.mData);
	glScalef(4, 4, 4);          // If lighting enabled here, be sure to use GL_RESCALE_NORMAL
	float x = -width * 0.15f;
	float y = width * 0.02f - 3;
	float z = width * 0.25f - 2.2;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKPURPLE_COL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BRIGHTPURPLE_COL);
	glNormal3f(0.0f, 1.0f, 0.0f);

	glPopMatrix();
}


