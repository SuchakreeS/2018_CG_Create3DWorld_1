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
