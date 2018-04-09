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
