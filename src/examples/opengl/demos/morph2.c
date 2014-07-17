/*
** Morph.c (renamed by Akka from original StereoSample.c)
** Morph2.c (adapted and renamed by DaveM for GLUT stereo example)
**
** OpenGL Stereo Sample
**
** ELSA AG Aachen (Germany)
**
** Simple OpenGL stereo implementation for graphics hardware
** with Per-Window stereo support.
**
** NOTE THAT PROGRAM WILL RUN EVEN IF STEREO IS NOT SUPPORTED.
** To verify stereo support, look for "GL_STEREO == GL_TRUE" in introductory
** diagnostic dialog box.
**
** Program interface:
**   - Left mouse-button changes camera separation
**   - Right mouse-button changes parallax balance
**   - <enter> restores stereo settings to default values
**   - <space> bar toggles morph & motion (added by Akka)
**   - '1' enables backface cull
**   - '2' disables backface cull
**   - <escape> quits
**
** 27.11.97 dt: Ripped from own StereoObjects Demo for Comdex Fall '97
**
** Modified 8/18/98 by Bob Akka, StereoGraphics:
**   - Stereo camera separation changes no longer affect parallax balance
**   - Morph & motion can now be stopped using the <space> bar
**   - Stereo camera separation & parallax balance default values changed
**   - Stereo settings adjustments are now less sensitive
**   - Negative camera separation no longer allowed
**   - Positive parallax balance var value no longer allowed
**   - Camera separation var may not exceed 4x default value
**   - Parallax balance var may not go beyond 2x default value
**   - Introductory diagnostic dialog box text changed
**   - Background changed from black to gray to reduce ghosting
**   - #define variables "X" and "Z" renamed "XVAL" and "ZVAL"
**   - #includes of <stdio.h> and <stdlib.h> commented out
**
** Modified 9/6/99 by Dave Milici,
** for testing stereo-enabled GLDirect OpenGL drivers from SciTech Software.
**
** Modified 11/20/99 by DaveM, for use with GLUT instead of native WIN32.
**
*/



/* ##### INCLUDES 	*/

// #include <windows.h>// WIN32 support deleted by DaveM
#include <gl\glut.h>   // GLUT support added by DaveM
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>  // commented out by Akka
#include <stdlib.h> // commented out by Akka
#include <string.h>	// added by DaveM
#include <math.h>

#ifndef glutExit
#define glutExit()	exit(0)
#endif

/* ##### DEFINES 	*/

#define XVAL 0.525731112119133606f  // var renamed by Akka
#define ZVAL 0.850650808352039932f  // var renamed by Akka

#define MORPH_MIN 0.25f
#define MORPH_MAX 1.5f

// some very good looking defaults for this demo
// changed by Akka; original defaults: 0.0358333, -0.0123611
#define EYE_OFFSET  0.08f  // default viewpoint separation
#define EYE_ADJUST -0.04f  // default horizontal image shift adjustment

#define PULL_BACK 2.2f          // z distance from object center

/* ##### TYPEDEFS	*/

#ifndef POINT
typedef struct {int x, y;} POINT;
#endif

/* ##### VARIABLES */

char szAppTitle[] = "OpenGL Stereo Sample";

char szMessage[2048];
char szInfo[256];

GLboolean bStereo = GL_FALSE;

int bLButtonDown = GL_FALSE;
int bRButtonDown = GL_FALSE;

int bRotation = GL_TRUE;  // added by Akka

int nWidth;
int nHeight;
float aspectViewport = 1.0f;

GLfloat eyeOffset = EYE_OFFSET; // init stereo separation with default settings
GLfloat eyeAdjust = EYE_ADJUST;


POINT ptLast;
POINT ptCurrent;


GLfloat   angleX = 0.0f;
GLfloat   angleY = 0.0f;
GLfloat   angleZ = 0.0f;
GLfloat   translateZ = 0.0f;
GLfloat   incZ = 0.005f;

// Object data

GLfloat   factorMorph = 1.0;
GLfloat   incMorph    = 0.015f;

static GLfloat vIco[12][3] = 
{  
  {  -XVAL, 0.0f,    ZVAL},
  {   XVAL, 0.0f,    ZVAL},
  {  -XVAL, 0.0f,   -ZVAL},
  {   XVAL, 0.0f,   -ZVAL},
  {0.0f,    ZVAL,    XVAL},
  {0.0f,    ZVAL,   -XVAL},
  {0.0f,   -ZVAL,    XVAL},
  {0.0f,   -ZVAL,   -XVAL},
  {   ZVAL,    XVAL, 0.0f},
  {  -ZVAL,    XVAL, 0.0f},
  {   ZVAL,   -XVAL, 0.0f},
  {  -ZVAL,   -XVAL, 0.0f}
};

static int idxIco[20][3] = 
{
  { 0,  1,  4}, //  0
  { 0,  4,  9}, //  1
  { 0,  9, 11}, //  2
  { 0,  6,  1}, //  3
  { 0, 11,  6}, //  4
  { 1,  6, 10}, //  5
  { 1, 10,  8}, //  6
  { 1,  8,  4}, //  7
  { 2,  3,  7}, //  8
  { 2,  5,  3}, //  9
  { 2,  9,  5}, // 10
  { 2, 11,  9}, // 11
  { 2,  7, 11}, // 12
  { 3,  5,  8}, // 13
  { 3,  8, 10}, // 14
  { 3, 10,  7}, // 15
  { 4,  5,  9}, // 16
  { 4,  8,  5}, // 17
  { 6,  7, 10}, // 18
  { 6, 11,  7}  // 19
};

static GLfloat vDode[20][3];

static int idxDode[12][5] =
{
  { 0,  1,  2,  4,  3}, //  0
  { 0,  3,  5,  6,  7}, //  1
  { 9,  8, 12, 11, 10}, //  2
  { 9, 13, 14, 15,  8}, //  3
  { 0,  7, 17, 16,  1}, //  4
  { 9, 10, 16, 17, 13}, //  5
  {18,  5,  3,  4, 19}, //  6
  {15, 18, 19, 12,  8}, //  7
  { 6, 14, 13, 17,  7}, //  8
  { 1, 16, 10, 11,  2}, //  9
  { 5, 18, 15, 14,  6}, // 10
  { 2, 11, 12, 19,  4}  // 11
};

/* #################### PROTOTYPES 	*/

void Init(void);
void Reshape(int width, int height);
void Display(void);
void Lists(void);

void MaterialCreate(void);
void LightCreate(void);
void Icosahedron(void);

void InitDodecahedron(void);
void Dodecahedron(void);
void DodecahedronMorph(void);


/* #################### FUNCTIONS 	*/

// Main drawing routine.
// Contains the code for both eyes.
void Display(void)
{
	   // added by Akka; "frustumAdjust" replaces "eyeAdjust" in
	   //    remainder of this routine
  float frustumAdjust = eyeAdjust * eyeOffset / EYE_OFFSET;

  // glViewport() call is done in WM_SIZE message

  // Clear both back buffers in one step.
  // And this is helpful for non-stereo boards
  // which display both stereo images on the left buffer then
  // because we do onbly one clear.
  glDrawBuffer(GL_BACK);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw the image for the left eye.
  glDrawBuffer(GL_BACK_LEFT);

  // Setup the projection for the left eye
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-aspectViewport * 0.75 - frustumAdjust,
			 aspectViewport * 0.75 - frustumAdjust,
			-0.75, 0.75, 0.65, 4.0);
  glTranslatef(eyeOffset, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -PULL_BACK);


  // Setup the transformation matrix for the object.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
  glRotatef(angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(angleX, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, translateZ);


  DodecahedronMorph();
  glCallList(1);

	   // Note that, in some OpenGL stereoscopy implementations, there
	   //    are two back buffers and two front buffers, but only one
	   //    z-buffer. If possible, software should assume that only one
	   //    z-buffer is available, and should clear that z-buffer before
	   //    rendering each new stereo view. If your application software
	   //    renders left- and right-eye stereo pair elements concurrently,
	   //    for performance reasons, perhaps, your software will only
	   //    do non-wireframe rendering properly on systems that implement
	   //    stereo using separate left- and right-eye z-buffers.
  glClear (GL_DEPTH_BUFFER_BIT);

  // Select back right buffer to recieve drawing
  glDrawBuffer(GL_BACK_RIGHT);

  // Draw the image for the right eye.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-aspectViewport * 0.75 + frustumAdjust,
			 aspectViewport * 0.75 + frustumAdjust,
			-0.75, 0.75, 0.65, 4.0);
  glTranslatef(-eyeOffset, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -PULL_BACK);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
  glRotatef(angleY, 0.0f, 1.0f, 0.0f);
  glRotatef(angleX, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, translateZ);

  DodecahedronMorph();
  glCallList(1);

  // Update stereo buffers for display (DaveM)
  glutSwapBuffers();
}

static void TriangleNormalVector(GLfloat a[3], GLfloat b[3], GLfloat c[3], GLfloat n[3])
{
  int i;
  GLfloat d;
  GLfloat v1[3];
  GLfloat v2[3];

  for (i = 0; i < 3; i++)
  {
    v1[i] =  a[i] - b[i];
    v2[i] =  b[i] - c[i];
  }

  n[0] = v1[1] * v2[2] - v1[2] * v2[1];
  n[1] = v1[2] * v2[0] - v1[0] * v2[2];
  n[2] = v1[0] * v2[1] - v1[1] * v2[0];

  d = (GLfloat) sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
  if (fabs(d) > 1.0e-8)
  {
    d = 1.0f / d;
    n[0] *= d;
	n[1] *= d;
    n[2] *= d;
  }
}

void Icosahedron(void)
{
  int i;
  GLfloat norm[3];

  glNewList(1, GL_COMPILE);
    glBegin(GL_TRIANGLES);
	for (i = 0; i < 20; i++)
	{
	  TriangleNormalVector(vIco[idxIco[i][0]], vIco[idxIco[i][1]], vIco[idxIco[i][2]], norm);
      glNormal3fv(norm);
      glVertex3fv(vIco[idxIco[i][0]]);
      glVertex3fv(vIco[idxIco[i][1]]);
	  glVertex3fv(vIco[idxIco[i][2]]);
    }
    glEnd();
  glEndList();
}


void InitDodecahedron(void)
{
  int     i;
  GLfloat n[3];
  GLfloat d;

  for (i = 0; i < 20; i++)
  {
    n[0] = vIco[idxIco[i][0]][0] + vIco[idxIco[i][1]][0] + vIco[idxIco[i][2]][0];
	n[1] = vIco[idxIco[i][0]][1] + vIco[idxIco[i][1]][1] + vIco[idxIco[i][2]][1];
    n[2] = vIco[idxIco[i][0]][2] + vIco[idxIco[i][1]][2] + vIco[idxIco[i][2]][2];
    
    d = (GLfloat) sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    if (fabs(d) > 1.0e-8)
    {
      d = 1.0f / d;
      n[0] *= d;
      n[1] *= d;
      n[2] *= d;
    }
    vDode[i][0] = n[0];
    vDode[i][1] = n[1];
    vDode[i][2] = n[2];
  }
}


void Dodecahedron(void)
{
  int i;

  for (i = 0; i < 12; i++)
  {
	glBegin(GL_POLYGON);
	  glNormal3fv(vIco[i]);                // Icosahedron coordinate is exactly the normal vector.
      glVertex3fv(vDode[idxDode[i][0]]);
      glVertex3fv(vDode[idxDode[i][1]]);
      glVertex3fv(vDode[idxDode[i][2]]);
	  glVertex3fv(vDode[idxDode[i][3]]);
      glVertex3fv(vDode[idxDode[i][4]]);
    glEnd();
  }
}

void DodecahedronMorph(void)
{
  int i;
  GLfloat v[3];
  GLfloat norm[3];

  for (i = 0; i < 12; i++)
  {
    glBegin(GL_TRIANGLE_FAN);
      v[0] = factorMorph * vIco[i][0];
	  v[1] = factorMorph * vIco[i][1];
      v[2] = factorMorph * vIco[i][2];
      TriangleNormalVector(v, vDode[idxDode[i][0]], vDode[idxDode[i][1]], norm);
      glNormal3fv(norm);
      glVertex3fv(v); // center
      glVertex3fv(vDode[idxDode[i][0]]);
      glVertex3fv(vDode[idxDode[i][1]]);
      TriangleNormalVector(v, vDode[idxDode[i][1]], vDode[idxDode[i][2]], norm);
      glNormal3fv(norm);
      glVertex3fv(vDode[idxDode[i][2]]);
      TriangleNormalVector(v, vDode[idxDode[i][2]], vDode[idxDode[i][3]], norm);
      glNormal3fv(norm);
      glVertex3fv(vDode[idxDode[i][3]]);
      TriangleNormalVector(v, vDode[idxDode[i][3]], vDode[idxDode[i][4]], norm);
      glNormal3fv(norm);
      glVertex3fv(vDode[idxDode[i][4]]);
      TriangleNormalVector(v, vDode[idxDode[i][4]], vDode[idxDode[i][0]], norm);
      glNormal3fv(norm);
      glVertex3fv(vDode[idxDode[i][0]]);
    glEnd();
  }
}



void MaterialCreate(void)
{
  GLfloat ambientGold[]  = {0.1f, 0.05f, 0.0f, 1.0f};
  GLfloat diffuseGold[]  = {0.65f, 0.55f, 0.15f, 1.0f};
  GLfloat specularGold[] = {0.85f, 0.75f, 0.45f, 1.0f};

  GLfloat ambientSilver[]  = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat diffuseSilver[]  = {0.6f, 0.6f, 0.6f, 1.0f};
  GLfloat specularSilver[] = {0.9f, 0.9f, 0.9f, 1.0f};

  GLfloat shininess = 100.0f;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientGold);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseGold);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specularGold);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// Create directional light
void LightCreate(void)
{
  GLfloat light0_ambient[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0_position[] = {-5.0f, 5.0f, 5.0f, 1.0f};

  GLfloat light1_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat light1_diffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat light1_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat light1_position[] = {5.0f, 0.0f, 0.0f, 1.0f};
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  glEnable(GL_LIGHTING);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
}

// GLUT functions instead of WIN32

void Init(void)
{
	// OPENGL DRIVER QUERY START

	sprintf(szMessage,  // one '\n' removed by Akka
		"Current OpenGL Driver:\n%s\n%s\n%s\n%s\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION),
		glGetString(GL_EXTENSIONS));

	glGetBooleanv(GL_STEREO, &bStereo); // query stereo support!
	if (!bStereo)
	{
		strcat(szMessage, "\nGL_STEREO == GL_FALSE\n");
	}
	else
	{
		strcat(szMessage, "\nGL_STEREO == GL_TRUE\n");
	}

	// interface instructions added by Akka
	strcat(szMessage, "\n- Change stereo settings using mouse-buttons\n");
	strcat(szMessage, "- Halt motion using space-bar\n");

	// OPENGL DRIVER QUERY END

	// OPENGL INITIALIZATION START

	// IcosahedronCreate();

	// changed from black (to reduce ghosting) by Akka
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

	LightCreate();
	Icosahedron();
	MaterialCreate();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// glFrontFace(GL_CCW);  // default
	// glCullFace(GL_BACK);  // default
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_FLAT);

	InitDodecahedron();

	// OPENGL INITIALIZATION END
}

// case WM_TIMER:
void UpdateHandler(void)
{
	  if (bRotation)  {  // condition added by Akka
		  angleX += 0.6f;
		  if (angleX > 360.0f)
			angleX -= 360.0f;

		  angleY += 1.0f;
		  if (angleY > 360.0f)
			angleY -= 360.0f;

		  angleZ += 0.4f;
		  if (angleZ > 360.0f)
			angleZ -= 360.0f;

		  translateZ += incZ;
		  if (translateZ > 0.5f)
		  {
			translateZ = 0.5f;
			incZ = -incZ;
		  }
		  else if (translateZ < -0.5f)
		  {
			translateZ = -0.5f;
			incZ = -incZ;
		  }

		  factorMorph += incMorph;
		  if (factorMorph > MORPH_MAX)
		  {
			factorMorph = MORPH_MAX;
			incMorph = -incMorph;
		  }
		  else if (factorMorph < MORPH_MIN)
		  {
			factorMorph = MORPH_MIN;
			incMorph = -incMorph;
		  }
	  }
	  glutPostRedisplay();
}

// case WM_SIZE:
void ReshapeHandler(int nWidth, int nHeight)
{
	  glViewport(0, 0, nWidth, nHeight);
	  if (nHeight > 0)
	  {
		aspectViewport = (float) nWidth / (float) nHeight;
	  }
	  else
	  {
		aspectViewport = 1.0f;
	  }
	  glutPostRedisplay();
}

// case WM_COMMAND:
void KeyDownHandler(unsigned char key, int x, int y)
{
  switch (key)
  {
	case '1':
	  glEnable(GL_CULL_FACE);
	  break;

	case '2':
	  glDisable(GL_CULL_FACE);
	  break;

	case '\r':
	  eyeAdjust = EYE_ADJUST;
	  eyeOffset = EYE_OFFSET;
	  glutPostRedisplay();
	  break;

	case 0x1B:
	  glutExit();
	  break;

	case ' ':  // added by Akka
	  bRotation = !bRotation;
	  break;
  }
}

void ExtKeyDownHandler(int key, int x, int y)
{
}

// case WM_LBUTTONDOWN:
// case WM_RBUTTONDOWN:
void MouseDownHandler(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
		bLButtonDown = (state == GLUT_DOWN) ? GL_TRUE : GL_FALSE;
	if (button == GLUT_RIGHT_BUTTON)
		bRButtonDown = (state == GLUT_DOWN) ? GL_TRUE : GL_FALSE;

	if (bLButtonDown || bRButtonDown) {
		ptLast.x = x;
		ptLast.y = y;
	}
}

// case WM_MOUSEMOVE:
void MouseMoveHandler(int x, int y)
{
	int offset;

	  if (bLButtonDown)
	  {
		ptCurrent.x = x;
		ptCurrent.y = y;
		offset = ptCurrent.x - ptLast.x;
		if (nWidth > 0)
		{
		  // changed by Akka; originally += 0.2f ...
		  eyeOffset += 0.1f * (float) offset / (float) nWidth;
		  if (eyeOffset < 0.0f)  // added by Akka
			  eyeOffset = 0.0f;  // added by Akka
		  if (eyeOffset > EYE_OFFSET * 4)  // added by Akka
			  eyeOffset = EYE_OFFSET * 4;  // added by Akka
		}
		ptLast = ptCurrent;
		glutPostRedisplay();
	  }
	  else if (bRButtonDown)
	  {
		ptCurrent.x = x;
		ptCurrent.y = y;
		offset = ptCurrent.x - ptLast.x;
		if (nWidth > 0)
		{
		  // changed by Akka; originally += 0.2f ...
		  eyeAdjust += 0.02f * (float) offset / (float) nWidth;
		  if (eyeAdjust > 0.0f)  // added by Akka
			  eyeAdjust = 0.0f;  // added by Akka
		  if (eyeAdjust < EYE_ADJUST * 2)  // added by Akka
			  eyeAdjust = EYE_ADJUST * 2;  // added by Akka
		}
		ptLast = ptCurrent;
		glutPostRedisplay();
	  }
}

int main(int argc, char *argv[])
{
	glutInit(&argc, &*argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STEREO | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("OpenGL Stereo Morph");
	glutDisplayFunc(Display);
	glutIdleFunc(UpdateHandler);
	glutKeyboardFunc(KeyDownHandler);
	glutSpecialFunc(ExtKeyDownHandler);
	glutMouseFunc(MouseDownHandler);
	glutMotionFunc(MouseMoveHandler);
	glutReshapeFunc(ReshapeHandler);

	Init();
	glutMainLoop();

	return 0;             /* ANSI C requires main to return int. */
}
