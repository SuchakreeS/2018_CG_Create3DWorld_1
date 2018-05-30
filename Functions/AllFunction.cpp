
//|____________________________________________________________________
//! Draws Cat Body.
//|____________________________________________________________________

void DrawCatBody(const float width, const float length, const float height)
{
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

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

}

//|____________________________________________________________________
//! Draws Cat Head.
//|____________________________________________________________________

void DrawCatHead(const float width, const float length, const float height)
{
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	glColor3f(0.31f, 0.13f, 0.33f);
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

}

//|____________________________________________________________________
//! Draws Cat Ear.
//|____________________________________________________________________

void DrawCatEar(const float width, const float length, const float height)
{
	float x = -width * 0.15f;
	float y = width * 0.02f;
	float z = width * 0.25f;
	float scal = width * 0.75f;

	float x2 = 0;
	float y2 = 0;
	float z2 = 0;
	float scal2 = width * 0.50f;

	glColor3f(0.31f, 0.13f, 0.33f);
	// R5
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glVertex3f((0.00 * scal) + x, (0.85 * scal) + y, (0.35 * scal) + z);
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
	glVertex3f((0.30 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
	glEnd();

	// L19
	glBegin(GL_TRIANGLES);
	glVertex3f((0.47 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.44 * scal) + x, (0.85 * scal) + y, (0.09 * scal) + z);
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


	// Up ear left
	glBegin(GL_TRIANGLES);
	glVertex3f((-0.03 * scal) + x, (1.00 * scal) + y, (0.22 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.16 * scal) + z);
	glVertex3f((0.14 * scal) + x, (0.94 * scal) + y, (0.28 * scal) + z);
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

}

//|____________________________________________________________________
//! Draws Broom.
//|____________________________________________________________________

void DrawBroom(const float width, const float length, const float height)
{
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

}

//|____________________________________________________________________
