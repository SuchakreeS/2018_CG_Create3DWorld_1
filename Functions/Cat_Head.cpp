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
