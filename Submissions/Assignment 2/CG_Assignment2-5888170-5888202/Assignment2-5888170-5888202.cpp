
//|___________________________________________
//|
//| Team Member:
//|	Suchakree	Sawangwong		5888170 Sec.1
//|	Amonnat		Tengputtipong	5888202	Sec.1
//|___________________________________________


//|___________________
//|
//| Includes
//|___________________

#include <math.h>

#include <gmtl/gmtl.h>

#include <GL/glut.h>

//|___________________
//|
//| Constants
//|___________________

// Plane dimensions
const float P_WIDTH = 3;
const float P_LENGTH = 3;
const float P_HEIGHT = 1.5;

// Camera's view frustum 
const float CAM_FOV = 60.0f;     // Field of view in degs

								 //|___________________
								 //|
								 //| Global Variables
								 //|___________________

								 // Track window dimensions, initialized to 800x600
int w_width = 800;
int w_height = 600;

// Plane pose (position & orientation)
gmtl::Matrix44f plane_pose; // T, as defined in the handout, initialized to IDENTITY by default

							// Camera pose
gmtl::Matrix44f cam_pose;   // C, as defined in the handout
gmtl::Matrix44f cam_pose_2;
gmtl::Matrix44f cam_pose_3;
gmtl::Matrix44f view_mat;   // View transform is C^-1 (inverse of the camera transform C)
gmtl::Matrix44f view_mat_2;
gmtl::Matrix44f view_mat_3;

// Transformation matrices applied to plane and camera poses
gmtl::Matrix44f xtransp_mat;
gmtl::Matrix44f xtransn_mat;
gmtl::Matrix44f xrotp_mat;
gmtl::Matrix44f xrotp_mat_2;
gmtl::Matrix44f xrotn_mat;

gmtl::Matrix44f ytransp_mat;
gmtl::Matrix44f ytransn_mat;
gmtl::Matrix44f yrotp_mat;
gmtl::Matrix44f yrotp_mat_2;
gmtl::Matrix44f yrotn_mat;

gmtl::Matrix44f ztransp_mat;
gmtl::Matrix44f ztransn_mat;
gmtl::Matrix44f zrotp_mat;
gmtl::Matrix44f zrotn_mat;



//|___________________
//|
//| Function Prototypes
//|___________________

void InitMatrices();
void InitGL(void);
void DisplayFunc(void);
void KeyboardFunc(unsigned char key, int x, int y);
void ReshapeFunc(int w, int h);
void DrawCoordinateFrame(const float l);
void DrawPlane(const float width, const float length, const float height);

//|____________________________________________________________________
//|
//| Function: InitMatrices
//|
//! \param None.
//! \return None.
//!
//! Initializes all the matrices
//|____________________________________________________________________

void InitMatrices()
{
	const float TRANS_AMOUNT = 1.0f;
	const float ROT_AMOUNT = gmtl::Math::deg2Rad(5.0f); // specified in degs, but get converted to radians
	const float ROT_AMOUNT_2 = gmtl::Math::deg2Rad(90.0f); // specified in degs, but get converted to radians

	const float COSTHETA = cos(ROT_AMOUNT);
	const float SINTHETA = sin(ROT_AMOUNT);
	const float COSTHETA_2 = cos(ROT_AMOUNT_2);
	const float SINTHETA_2 = sin(ROT_AMOUNT_2);

	// Positive X-Translation
	xtransp_mat.set(1, 0, 0, TRANS_AMOUNT,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	xtransp_mat.setState(gmtl::Matrix44f::TRANS);

	gmtl::invert(xtransn_mat, xtransp_mat);

	// Positive X-rotation (roll)
	xrotp_mat.set(1, 0, 0, 0,
		0, COSTHETA, -SINTHETA, 0,
		0, SINTHETA, COSTHETA, 0,
		0, 0, 0, 1);
	xrotp_mat_2.set(1, 0, 0, 0,
		0, COSTHETA_2, -SINTHETA_2, 0,
		0, SINTHETA_2, COSTHETA_2, 0,
		0, 0, 0, 1);
	xrotp_mat.setState(gmtl::Matrix44f::ORTHOGONAL);

	// Negative X-rotation (roll)
	gmtl::invert(xrotn_mat, xrotp_mat);

	// Positive Y-Translation
	ytransp_mat.set(1, 0, 0, 0,
		0, 1, 0, TRANS_AMOUNT,
		0, 0, 1, 0,
		0, 0, 0, 1);
	ytransp_mat.setState(gmtl::Matrix44f::TRANS);

	gmtl::invert(ytransn_mat, ytransp_mat);

	// Positive Y-rotation (roll)
	yrotp_mat.set(COSTHETA, 0, SINTHETA, 0,
		0, 1, 0, 0,
		-SINTHETA, 0, COSTHETA, 0,
		0, 0, 0, 1);
	yrotp_mat_2.set(COSTHETA_2, 0, SINTHETA_2, 0,
		0, 1, 0, 0,
		-SINTHETA_2, 0, COSTHETA_2, 0,
		0, 0, 0, 1);
	yrotp_mat.setState(gmtl::Matrix44f::ORTHOGONAL);

	// Negative Y-rotation (roll)
	gmtl::invert(yrotn_mat, yrotp_mat);

	// Positive Z-Translation
	ztransp_mat.set(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, TRANS_AMOUNT,
		0, 0, 0, 1);
	ztransp_mat.setState(gmtl::Matrix44f::TRANS);

	gmtl::invert(ztransn_mat, ztransp_mat);

	// Positive Z-rotation (roll)
	zrotp_mat.set(COSTHETA, -SINTHETA, 0, 0,
		SINTHETA, COSTHETA, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	zrotp_mat.setState(gmtl::Matrix44f::ORTHOGONAL);

	// Negative Z-rotation (roll)
	gmtl::invert(zrotn_mat, zrotp_mat);

	// Inits plane pose
	plane_pose.set(1, 0, 0, 1.0f,
		0, 1, 0, 0.0f,
		0, 0, 1, 0.0f,
		0, 0, 0, 1.0f);
	plane_pose.setState(gmtl::Matrix44f::AFFINE);     // AFFINE because the plane pose can contain both translation and rotation         

													  // Inits camera pose and view transform

	cam_pose.set(1, 0, 0, 2.0f,
		0, 1, 0, 1.0f,
		0, 0, 1, 15.0f,
		0, 0, 0, 1.0f);
	cam_pose_2.set(1, 0, 0, 0.0f,
		0, 1, 0, 20.0f,
		0, 0, 1, 0.0f,
		0, 0, 0, 1.0f);
	cam_pose_3.set(1, 0, 0, 20.0f,
		0, 1, 0, 0.0f,
		0, 0, 1, 0.0f,
		0, 0, 0, 1.0f);
	cam_pose.setState(gmtl::Matrix44f::AFFINE);
	cam_pose_2.setState(gmtl::Matrix44f::AFFINE);
	cam_pose_3.setState(gmtl::Matrix44f::AFFINE);
	gmtl::invert(view_mat, cam_pose);                 // View transform is the inverse of the camera pose
	gmtl::invert(view_mat_2, cam_pose_2);
	gmtl::invert(view_mat_3, cam_pose_3);

}

//|____________________________________________________________________
//|
//| Function: InitGL
//|
//! \param None.
//! \return None.
//!
//! OpenGL initializations
//|____________________________________________________________________

void InitGL(void)
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

//|____________________________________________________________________
//|
//| Function: DisplayFunc
//|
//! \param None.
//! \return None.
//!
//! GLUT display callback function: called for every redraw event.
//|____________________________________________________________________

void DisplayFunc(void)
{
	// Modelview matrix
	gmtl::Matrix44f modelview_mat;        // M, as defined in the handout

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//|____________________________________________________________________
	//|
	//| Viewport 1 rendering: shows the moving camera's view
	//|____________________________________________________________________

	glViewport(0, 0, (GLsizei)w_width / 2, (GLsizei)w_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(CAM_FOV, (float)w_width / (2 * w_height), 0.1f, 100.0f);     // Check MSDN: google "gluPerspective msdn"

																				// Approach1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                          // A good practice for beginner

											   // Draws world coordinate frame
	modelview_mat = view_mat;                  // M = C^-1
	glLoadMatrixf(modelview_mat.mData);
	DrawCoordinateFrame(10);

	// Draws plane and its local frame
	modelview_mat *= plane_pose;               // M = C^-1 * T
	glLoadMatrixf(modelview_mat.mData);
	DrawPlane(P_WIDTH, P_LENGTH, P_HEIGHT);
	DrawCoordinateFrame(3);

	/*
	// Approach 2 (gives the same results as the approach 1)
	glMatrixMode(GL_MODELVIEW);

	// Draws world coordinate frame
	glLoadMatrixf(view_mat.mData);             // M = C^-1
	DrawCoordinateFrame(10);

	// Draws plane and its local frame
	glMultMatrixf(plane_pose.mData);           // M = C^-1 * T (OpenGL calls build transforms in left-to-right order)
	DrawPlane(P_WIDTH, P_LENGTH, P_HEIGHT);
	DrawCoordinateFrame(3);
	*/

	//|____________________________________________________________________
	//|
	//| TODO: Viewport 2 rendering: shows the fixed top-down view
	//|____________________________________________________________________

	// glViewport...
	glViewport((GLsizei)w_width / 2, (GLsizei)w_height/ 30, (GLsizei)w_width / 2.5, (GLsizei)w_height / 1.25);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(CAM_FOV, (float)w_width / (2 * w_height), 0.1f, 100.0f);     // Check MSDN: google "gluPerspective msdn"

																				// Approach1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                          // A good practice for beginner

											   // Draws world coordinate frame
	modelview_mat = view_mat * xrotp_mat_2;                  // M = C^-1
	glLoadMatrixf(modelview_mat.mData);
	//glMultMatrixf(xrotp_mat_2.mData);
	DrawCoordinateFrame(10);
	// Draws plane and its local frame
	modelview_mat *= plane_pose;               // M = C^-1 * T
	glLoadMatrixf(modelview_mat.mData);
	DrawPlane(P_WIDTH, P_LENGTH, P_HEIGHT);
	DrawCoordinateFrame(3);

	//|____________________________________________________________________
	//|
	//| TODO: Viewport 3 rendering: shows the fixed top-down view
	//|____________________________________________________________________

	// glViewport...
	glViewport((GLsizei)w_width / 2, (GLsizei)w_height / 5, (GLsizei)w_width / 2.5, (GLsizei)w_height / 1.25);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(CAM_FOV, (float)w_width / (2 * w_height), 0.1f, 100.0f);     // Check MSDN: google "gluPerspective msdn"

																				// Approach1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                          // A good practice for beginner

											   // Draws world coordinate frame
	modelview_mat = view_mat * yrotp_mat_2;                  // M = C^-1
	glLoadMatrixf(modelview_mat.mData);
	//glMultMatrixf(xrotp_mat_2.mData);
	DrawCoordinateFrame(10);
	// Draws plane and its local frame
	modelview_mat *= plane_pose;               // M = C^-1 * T
	glLoadMatrixf(modelview_mat.mData);
	DrawPlane(P_WIDTH, P_LENGTH, P_HEIGHT);
	DrawCoordinateFrame(3);





	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(CAM_FOV, (float)w_width / (2 * w_height), 0.1f, 100.0f);

	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity(); 
	// ...

	glFlush();
}

//|____________________________________________________________________
//|
//| Function: KeyboardFunc
//|
//! \param None.
//! \return None.
//!
//! GLUT keyboard callback function: called for every key press event.
//|____________________________________________________________________

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
		//---------- X-Controlor -------------------------------------------------

	case 'q': // Forward translation of the plane (positive X-translation)
		plane_pose = plane_pose * xtransp_mat;
		break;
	case 'w': // Backward translation of the plane
		plane_pose = plane_pose * xtransn_mat;
		break;
	case 'e': // Rolls the plane (+ X-rot)
		plane_pose = plane_pose * xrotp_mat;
		break;
	case 'r': // Rolls the plane (- X-rot)
		plane_pose = plane_pose * xrotn_mat;
		break;

		//---------- Y-Controlor -------------------------------------------------

	case 'a': // Forward translation of the plane (positive Y-translation)
		plane_pose = plane_pose * ytransp_mat;
		break;
	case 's': // Backward translation of the plane
		plane_pose = plane_pose * ytransn_mat;
		break;
	case 'd': // Rolls the plane (+ Y-rot)
		plane_pose = plane_pose * yrotp_mat;
		break;
	case 'f': // Rolls the plane (- Y-rot)
		plane_pose = plane_pose * yrotn_mat;
		break;

		//---------- Z-Controlor -------------------------------------------------

	case 'z': // Forward translation of the plane (positive Z-translation)
		plane_pose = plane_pose * ztransp_mat;
		break;
	case 'x': // Backward translation of the plane
		plane_pose = plane_pose * ztransn_mat;
		break;
	case 'c': // Rolls the plane (+ Z-rot)
		plane_pose = plane_pose * zrotp_mat;
		break;
	case 'v': // Rolls the plane (- Z-rot)
		plane_pose = plane_pose * zrotn_mat;
		break;


		//---------- Camera Controlor -------------------------------------------------
	case 't': // Forward translation of the camera (negative X-translation - cameras looks in its (local) -X direction)
		cam_pose = cam_pose * xtransn_mat;
		break;
	case 'y': // Backward translation of the camera
		cam_pose = cam_pose * xtransp_mat;
		break;
	case 'g': // Forward translation of the camera (negative Y-translation - cameras looks in its (local) -Y direction)
		cam_pose = cam_pose * ytransn_mat;
		break;
	case 'h': // Backward translation of the camera
		cam_pose = cam_pose * ytransp_mat;
		break;
	case 'b': // Forward translation of the camera (negative Z-translation - cameras looks in its (local) -Z direction)
		cam_pose = cam_pose * ztransn_mat;
		break;
	case 'n': // Backward translation of the camera
		cam_pose = cam_pose * ztransp_mat;
		break;
	}

	gmtl::invert(view_mat, cam_pose);       // Updates view transform to reflect the change in camera transform
	glutPostRedisplay();                    // Asks GLUT to redraw the screen
}

//|____________________________________________________________________
//|
//| Function: ReshapeFunc
//|
//! \param None.
//! \return None.
//!
//! GLUT reshape callback function: called everytime the window is resized.
//|____________________________________________________________________

void ReshapeFunc(int w, int h)
{
	// Track the current window dimensions
	w_width = w;
	w_height = h;
}

//|____________________________________________________________________
//|
//| Function: DrawCoordinateFrame
//|
//! \param l      [in] length of the three axes.
//! \return None.
//!
//! Draws coordinate frame consisting of the three principal axes.
//|____________________________________________________________________

void DrawCoordinateFrame(const float l)
{
	glBegin(GL_LINES);
	// X axis is red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(l, 0.0f, 0.0f);

	// Y axis is green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, l, 0.0f);

	// Z axis is blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, l);
	glEnd();
}

//|____________________________________________________________________
//|
//| Function: DrawPlane
//|
//! \param width       [in] Width  of the plane.
//! \param length      [in] Length of the plane.
//! \param height      [in] Height of the plane.
//! \return None.
//!
//! Draws the plane.
//|____________________________________________________________________

void DrawPlane(const float width, const float length, const float height)
{
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	/*
	glBegin(GL_TRIANGLES);
	glBegin(GL_QUAD_STRIP);
	*/
	//------Right--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
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
	// R5
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	//------Back--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
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
	// B10
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();
	// B11
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();
	// B12
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();

	//------Left--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
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
	// L19
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();

	//------Front--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
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
	// B25
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
	glEnd();
	// B26
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
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
	glVertex3f((0.33 * scal) + x, (0.91 * scal) + y, (0.29 * scal) + z);
	glVertex3f((0.41 * scal) + x, (0.88 * scal) + y, (0.34 * scal) + z);
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
	//------Up--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
	// B24
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();

	// B25
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();
	// B26
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
	glEnd();

	//------Down--------------------------------------------
	glColor3f(0.31f, 0.13f, 0.33f);
	glBegin(GL_QUAD_STRIP);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.05 * scal) + z);
	glVertex3f((0.05 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glVertex3f((0.37 * scal) + x, (0.00 * scal) + y, (0.39 * scal) + z);
	glEnd();


	//------- Meaw -----------------------------------------


	//glScalef(1.0, 2.0f, 1);
	glBegin(GL_TRIANGLES);
	// (-0.05, 0.05, 8) (0.05, 0.05, 8) (0.08, 0, 8) (0.05, -0.05, 8) (-0.05, -0.05, 8) (-0.08, 0, 8)
	glColor3f(0.2f, 0.1f, 0.0f);
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);

	glColor3f(0.2f, 0.1f, 0.0f);
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);

	glEnd();

	glBegin(GL_QUAD_STRIP);
	glColor3f(0.2f, 0.1f, 0.0f);
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	// (-0.05, 0.05, 8) (0.05, 0.05, 8) AB
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // A
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // G
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2);  //B
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2);  // H

																			 // (0.05, 0.05, 8) (0.08, 0, 8) BC
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // B
	glVertex3f((0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // H
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // C
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2);    // I

																			// (0.08, 0, 8) (0.05, -0.05, 8) CD
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // C
	glVertex3f((0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // I
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // D
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // J

																			 // (0.05, -0.05, 8) (-0.05, -0.05, 8) DE
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // D
	glVertex3f((0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // J
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // E
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // K

																			  // (-0.05, -0.05, 8) (-0.08, 0, 8) EF
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (2 * scal2) + z2); // E
	glVertex3f((-0.05 * scal2) + x2, (-0.05 * scal2) + y2, (0 * scal2) + z2); // K
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2); // F
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // L

																		  // (-0.08, 0, 8) (-0.05, 0.05, 8) FA
	glColor3f(0.3f, 0.1f, 0.0f);
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (1.98 * scal2) + z2);  // F
	glVertex3f((-0.08 * scal2) + x2, (0 * scal2) + y2, (0 * scal2) + z2); // L
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (2 * scal2) + z2); // A
	glVertex3f((-0.05 * scal2) + x2, (0.05 * scal2) + y2, (0 * scal2) + z2); // G

	glEnd();

	glBegin(GL_QUAD_STRIP);

	// 1
	glColor3f(0.8f, 0.5f, 0.0f);
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
	//------- Meaw -----------------------------------------
}

//|____________________________________________________________________
//|
//| Function: main
//|
//! \param None.
//! \return None.
//!
//! Program entry point
//|____________________________________________________________________

int main(int argc, char **argv)
{
	InitMatrices();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w_width, w_height);

	glutCreateWindow("Plane Episode 1");

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);

	InitGL();

	glutMainLoop();

	return 0;
}