/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  Header file for the SciTech Nucleus Graphics Architecture
*				device driver Hardware Abstraction Layer.
*
*				This header file contains all the macros, type definitions
*				and function prototypes for the generic Nucleus based
*				OpenGL compatible GL3D device driver interface.
*
****************************************************************************/

#ifndef	__NUCLEUS_GL3D_H
#define	__NUCLEUS_GL3D_H

#ifndef	__NUCLEUS_GA3D_H
#include "nucleus/ga3d.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

/****************************************************************************
REMARKS:
This enumeration defines the identifiers used to obtain the device context
function group pointer structures. As new features and capabilities are
added to the future versions of the specification, new identifiers will
be added to extract new function pointers from the drivers.

HEADER:
nucleus/gl3d.h

MEMBERS:
GA_GET_GLINITFUNCS		- Get GA_glInitFuncs structure
GA_GET_GLFUNCS			- Get GA_glFuncs structure
****************************************************************************/
typedef enum {
	GA_GET_GLINITFUNCS,
	GA_GET_GLFUNCS,
	} GA_glFuncGroupsType;

/****************************************************************************
REMARKS:
Structure representing the information passed to the Nucleus GL3D binding
functions to both choose an appropriate visual that is supported by the
GL3D implementation and to pass in the information for the visual when
a rendering context is created.

HEADER:
nucleus/gl3d.h

MEMBERS:
dwSize			- Set to size of structure in bytes
ColorIndex		- True if the visual is color index
DoubleBuffer	- True if the visual is double buffered
Stereo			- True if the visual is stereo enabled
DepthBits      	- Size of depth buffer in bits (0 for none)
AlphaBits		- Size of alpha buffer in bits (0 for none)
StencilBits    	- Size of stencil buffer in bits (0 for none)
AccumBits      	- Size of accumulation buffer in bits (0 for none)
****************************************************************************/
typedef struct {
	N_uint32		dwSize;
	N_int32			ColorIndex;
	N_int32			DoubleBuffer;
	N_int32			Stereo;
	N_int32			DepthBits;
	N_int32			AlphaBits;
	N_int32			StencilBits;
	N_int32			AccumBits;
	} GA_glVisual;

/* {secret} Type definition for internal GL3D rendering context object */
typedef struct GA_glContext GA_HGLRC;

/****************************************************************************
REMARKS:
Main GL3D device driver structure. This structure consists of a header
block that contains configuration information about the opengl device driver.

The Signature member is filled with the null terminated string 'GL3D\0'
by the driver implementation. This can be used to verify that the file loaded
really is an GL3D device driver.

The Version member is a BCD value which specifies what revision level of the
device driver specification is implemented in the driver. The high byte
specifies the major version number and the low byte specifies the minor
version number. For example, the BCD value for version 1.0 is 0x100 and the
BCD value for version 2.2 would be 0x202.

The DriverRev member specifies the driver revision level, and is used by the
driver configuration software.

The OemVendorName member contains the name of the vendor that developed the
device driver implementation, and can be up to 80 characters in length.

The OemCopyright member contains a copyright string for the vendor that
developed the device driver implementation and may be up to 80 characters
in length.

HEADER:
nucleus/gl3d.h

MEMBERS:
Signature      		- 'GL3D\0' 20 byte signature
Version            	- Driver Interface Version (BCD)
DriverRev          	- Driver revision number
OemVendorName  		- Vendor Name string
OemCopyright   		- Vendor Copyright string
****************************************************************************/
typedef struct {
	/*------------------------------------------------------------------*/
	/* Device driver header block                                       */
	/*------------------------------------------------------------------*/
	char        Signature[20];
	N_uint32   	Version;
	N_uint32   	DriverRev;
	char        OemVendorName[80];
	char        OemCopyright[80];
	N_uint32   	res1[20];

	/*------------------------------------------------------------------*/
	/* Driver initialization functions 									*/
	/*------------------------------------------------------------------*/
	ibool		(NAPIP QueryFunctions)(N_uint32 id,N_int32 safetyLevel,void *funcs);
	void		(NAPIP MapShared)(void);
	} GA_glCtx;

/****************************************************************************
REMARKS:
Main device driver init functions, including all mode information, setup and
initialisation functions.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;
	void 		(NAPIP ChooseVisual)(GA_3DSurface *surf,GA_glVisual *visual);
	ibool		(NAPIP CheckVisual)(GA_3DSurface *surf,GA_glVisual *visual);
	GA_HGLRC *	(NAPIP CreateContext)(GA_3DSurface *surf,GA_glVisual *visual);
	ibool		(NAPIP DeleteContext)(GA_HGLRC *ctx);
	ibool		(NAPIP MakeCurrent)(GA_HGLRC *ctx);
	void *		(NAPIP GetProcAddress)(const char *procName);
	} GA_glInitFuncs;

/****************************************************************************
REMARKS:
Main structure containing all public OpenGL compatible functions supported.
{secret}
****************************************************************************/
typedef struct {
	N_uint32	dwSize;

	/* Regular OpenGL functions */
	void 		(APIENTRYP glAccum) (GLenum op, GLfloat value);
	void 		(APIENTRYP glAlphaFunc) (GLenum func, GLclampf ref);
	GLboolean 	(APIENTRYP glAreTexturesResident) (GLsizei n, const GLuint *textures, GLboolean *residences);
	void 		(APIENTRYP glArrayElement) (GLint i);
	void 		(APIENTRYP glBegin) (GLenum mode);
	void 		(APIENTRYP glBindTexture) (GLenum target, GLuint texture);
	void 		(APIENTRYP glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
	void 		(APIENTRYP glBlendFunc) (GLenum sfactor, GLenum dfactor);
	void 		(APIENTRYP glCallList) (GLuint list);
	void 		(APIENTRYP glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
	void 		(APIENTRYP glClear) (GLbitfield mask);
	void 		(APIENTRYP glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void 		(APIENTRYP glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void 		(APIENTRYP glClearDepth) (GLclampd depth);
	void 		(APIENTRYP glClearIndex) (GLfloat c);
	void 		(APIENTRYP glClearStencil) (GLint s);
	void 		(APIENTRYP glClipPlane) (GLenum plane, const GLdouble *equation);
	void 		(APIENTRYP glColor3b) (GLbyte red, GLbyte green, GLbyte blue);
	void 		(APIENTRYP glColor3bv) (const GLbyte *v);
	void 		(APIENTRYP glColor3d) (GLdouble red, GLdouble green, GLdouble blue);
	void 		(APIENTRYP glColor3dv) (const GLdouble *v);
	void 		(APIENTRYP glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
	void        (APIENTRYP glColor3fv) (const GLfloat *v);
	void        (APIENTRYP glColor3i) (GLint red, GLint green, GLint blue);
	void        (APIENTRYP glColor3iv) (const GLint *v);
	void        (APIENTRYP glColor3s) (GLshort red, GLshort green, GLshort blue);
	void        (APIENTRYP glColor3sv) (const GLshort *v);
	void        (APIENTRYP glColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
	void        (APIENTRYP glColor3ubv) (const GLubyte *v);
	void        (APIENTRYP glColor3ui) (GLuint red, GLuint green, GLuint blue);
	void        (APIENTRYP glColor3uiv) (const GLuint *v);
	void        (APIENTRYP glColor3us) (GLushort red, GLushort green, GLushort blue);
	void        (APIENTRYP glColor3usv) (const GLushort *v);
	void        (APIENTRYP glColor4b) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
	void        (APIENTRYP glColor4bv) (const GLbyte *v);
	void        (APIENTRYP glColor4d) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
	void        (APIENTRYP glColor4dv) (const GLdouble *v);
	void        (APIENTRYP glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void        (APIENTRYP glColor4fv) (const GLfloat *v);
	void        (APIENTRYP glColor4i) (GLint red, GLint green, GLint blue, GLint alpha);
	void        (APIENTRYP glColor4iv) (const GLint *v);
	void        (APIENTRYP glColor4s) (GLshort red, GLshort green, GLshort blue, GLshort alpha);
	void        (APIENTRYP glColor4sv) (const GLshort *v);
	void        (APIENTRYP glColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
	void        (APIENTRYP glColor4ubv) (const GLubyte *v);
	void        (APIENTRYP glColor4ui) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
	void        (APIENTRYP glColor4uiv) (const GLuint *v);
	void        (APIENTRYP glColor4us) (GLushort red, GLushort green, GLushort blue, GLushort alpha);
	void        (APIENTRYP glColor4usv) (const GLushort *v);
	void        (APIENTRYP glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void        (APIENTRYP glColorMaterial) (GLenum face, GLenum mode);
	void        (APIENTRYP glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void        (APIENTRYP glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
	void        (APIENTRYP glCopyTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	void        (APIENTRYP glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	void        (APIENTRYP glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	void        (APIENTRYP glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	void        (APIENTRYP glCullFace) (GLenum mode);
	void        (APIENTRYP glDeleteLists) (GLuint list, GLsizei range);
	void        (APIENTRYP glDeleteTextures) (GLsizei n, const GLuint *textures);
	void        (APIENTRYP glDepthFunc) (GLenum func);
	void        (APIENTRYP glDepthMask) (GLboolean flag);
	void        (APIENTRYP glDepthRange) (GLclampd zNear, GLclampd zFar);
	void        (APIENTRYP glDisable) (GLenum cap);
	void        (APIENTRYP glDisableClientState) (GLenum array);
	void        (APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count);
	void        (APIENTRYP glDrawBuffer) (GLenum mode);
	void        (APIENTRYP glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void        (APIENTRYP glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void        (APIENTRYP glEdgeFlag) (GLboolean flag);
	void        (APIENTRYP glEdgeFlagPointer) (GLsizei stride, const GLboolean *pointer);
	void        (APIENTRYP glEdgeFlagv) (const GLboolean *flag);
	void        (APIENTRYP glEnable) (GLenum cap);
	void        (APIENTRYP glEnableClientState) (GLenum array);
	void        (APIENTRYP glEnd) (void);
	void        (APIENTRYP glEndList) (void);
	void        (APIENTRYP glEvalCoord1d) (GLdouble u);
	void        (APIENTRYP glEvalCoord1dv) (const GLdouble *u);
	void        (APIENTRYP glEvalCoord1f) (GLfloat u);
	void        (APIENTRYP glEvalCoord1fv) (const GLfloat *u);
	void        (APIENTRYP glEvalCoord2d) (GLdouble u, GLdouble v);
	void        (APIENTRYP glEvalCoord2dv) (const GLdouble *u);
	void        (APIENTRYP glEvalCoord2f) (GLfloat u, GLfloat v);
	void        (APIENTRYP glEvalCoord2fv) (const GLfloat *u);
	void        (APIENTRYP glEvalMesh1) (GLenum mode, GLint i1, GLint i2);
	void        (APIENTRYP glEvalMesh2) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
	void        (APIENTRYP glEvalPoint1) (GLint i);
	void        (APIENTRYP glEvalPoint2) (GLint i, GLint j);
	void        (APIENTRYP glFeedbackBuffer) (GLsizei size, GLenum type, GLfloat *buffer);
	void        (APIENTRYP glFinish) (void);
	void        (APIENTRYP glFlush) (void);
	void        (APIENTRYP glFogf) (GLenum pname, GLfloat param);
	void        (APIENTRYP glFogfv) (GLenum pname, const GLfloat *params);
	void        (APIENTRYP glFogi) (GLenum pname, GLint param);
	void        (APIENTRYP glFogiv) (GLenum pname, const GLint *params);
	void        (APIENTRYP glFrontFace) (GLenum mode);
	void        (APIENTRYP glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	GLuint 		(APIENTRYP glGenLists) (GLsizei range);
	void        (APIENTRYP glGenTextures) (GLsizei n, GLuint *textures);
	void        (APIENTRYP glGetBooleanv) (GLenum pname, GLboolean *params);
	void        (APIENTRYP glGetClipPlane) (GLenum plane, GLdouble *equation);
	void        (APIENTRYP glGetDoublev) (GLenum pname, GLdouble *params);
	GLenum 		(APIENTRYP glGetError) (void);
	void        (APIENTRYP glGetFloatv) (GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetIntegerv) (GLenum pname, GLint *params);
	void        (APIENTRYP glGetLightfv) (GLenum light, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetLightiv) (GLenum light, GLenum pname, GLint *params);
	void        (APIENTRYP glGetMapdv) (GLenum target, GLenum query, GLdouble *v);
	void        (APIENTRYP glGetMapfv) (GLenum target, GLenum query, GLfloat *v);
	void        (APIENTRYP glGetMapiv) (GLenum target, GLenum query, GLint *v);
	void        (APIENTRYP glGetMaterialfv) (GLenum face, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetMaterialiv) (GLenum face, GLenum pname, GLint *params);
	void        (APIENTRYP glGetPixelMapfv) (GLenum map, GLfloat *values);
	void        (APIENTRYP glGetPixelMapuiv) (GLenum map, GLuint *values);
	void        (APIENTRYP glGetPixelMapusv) (GLenum map, GLushort *values);
	void        (APIENTRYP glGetPointerv) (GLenum pname, GLvoid* *params);
	void        (APIENTRYP glGetPolygonStipple) (GLubyte *mask);
	const GLubyte * (APIENTRYP glGetString) (GLenum name);
	void        (APIENTRYP glGetTexEnvfv) (GLenum target, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetTexEnviv) (GLenum target, GLenum pname, GLint *params);
	void        (APIENTRYP glGetTexGendv) (GLenum coord, GLenum pname, GLdouble *params);
	void        (APIENTRYP glGetTexGenfv) (GLenum coord, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetTexGeniv) (GLenum coord, GLenum pname, GLint *params);
	void        (APIENTRYP glGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
	void        (APIENTRYP glGetTexLevelParameterfv) (GLenum target, GLint level, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params);
	void        (APIENTRYP glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat *params);
	void        (APIENTRYP glGetTexParameteriv) (GLenum target, GLenum pname, GLint *params);
	void        (APIENTRYP glHint) (GLenum target, GLenum mode);
	void        (APIENTRYP glIndexMask) (GLuint mask);
	void        (APIENTRYP glIndexPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
	void        (APIENTRYP glIndexd) (GLdouble c);
	void        (APIENTRYP glIndexdv) (const GLdouble *c);
	void        (APIENTRYP glIndexf) (GLfloat c);
	void        (APIENTRYP glIndexfv) (const GLfloat *c);
	void        (APIENTRYP glIndexi) (GLint c);
	void        (APIENTRYP glIndexiv) (const GLint *c);
	void        (APIENTRYP glIndexs) (GLshort c);
	void        (APIENTRYP glIndexsv) (const GLshort *c);
	void        (APIENTRYP glIndexub) (GLubyte c);
	void        (APIENTRYP glIndexubv) (const GLubyte *c);
	void        (APIENTRYP glInitNames) (void);
	void        (APIENTRYP glInterleavedArrays) (GLenum format, GLsizei stride, const GLvoid *pointer);
	GLboolean 	(APIENTRYP glIsEnabled) (GLenum cap);
	GLboolean 	(APIENTRYP glIsList) (GLuint list);
	GLboolean 	(APIENTRYP glIsTexture) (GLuint texture);
	void        (APIENTRYP glLightModelf) (GLenum pname, GLfloat param);
	void        (APIENTRYP glLightModelfv) (GLenum pname, const GLfloat *params);
	void        (APIENTRYP glLightModeli) (GLenum pname, GLint param);
	void        (APIENTRYP glLightModeliv) (GLenum pname, const GLint *params);
	void        (APIENTRYP glLightf) (GLenum light, GLenum pname, GLfloat param);
	void        (APIENTRYP glLightfv) (GLenum light, GLenum pname, const GLfloat *params);
	void        (APIENTRYP glLighti) (GLenum light, GLenum pname, GLint param);
	void        (APIENTRYP glLightiv) (GLenum light, GLenum pname, const GLint *params);
	void        (APIENTRYP glLineStipple) (GLint factor, GLushort pattern);
	void        (APIENTRYP glLineWidth) (GLfloat width);
	void        (APIENTRYP glListBase) (GLuint base);
	void        (APIENTRYP glLoadIdentity) (void);
	void        (APIENTRYP glLoadMatrixd) (const GLdouble *m);
	void        (APIENTRYP glLoadMatrixf) (const GLfloat *m);
	void        (APIENTRYP glLoadName) (GLuint name);
	void        (APIENTRYP glLogicOp) (GLenum opcode);
	void        (APIENTRYP glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
	void        (APIENTRYP glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
	void        (APIENTRYP glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
	void        (APIENTRYP glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
	void        (APIENTRYP glMapGrid1d) (GLint un, GLdouble u1, GLdouble u2);
	void        (APIENTRYP glMapGrid1f) (GLint un, GLfloat u1, GLfloat u2);
	void        (APIENTRYP glMapGrid2d) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
	void        (APIENTRYP glMapGrid2f) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
	void        (APIENTRYP glMaterialf) (GLenum face, GLenum pname, GLfloat param);
	void        (APIENTRYP glMaterialfv) (GLenum face, GLenum pname, const GLfloat *params);
	void        (APIENTRYP glMateriali) (GLenum face, GLenum pname, GLint param);
	void        (APIENTRYP glMaterialiv) (GLenum face, GLenum pname, const GLint *params);
	void        (APIENTRYP glMatrixMode) (GLenum mode);
	void        (APIENTRYP glMultMatrixd) (const GLdouble *m);
	void        (APIENTRYP glMultMatrixf) (const GLfloat *m);
	void        (APIENTRYP glNewList) (GLuint list, GLenum mode);
	void        (APIENTRYP glNormal3b) (GLbyte nx, GLbyte ny, GLbyte nz);
	void        (APIENTRYP glNormal3bv) (const GLbyte *v);
	void        (APIENTRYP glNormal3d) (GLdouble nx, GLdouble ny, GLdouble nz);
	void        (APIENTRYP glNormal3dv) (const GLdouble *v);
	void        (APIENTRYP glNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz);
	void        (APIENTRYP glNormal3fv) (const GLfloat *v);
	void        (APIENTRYP glNormal3i) (GLint nx, GLint ny, GLint nz);
	void        (APIENTRYP glNormal3iv) (const GLint *v);
	void        (APIENTRYP glNormal3s) (GLshort nx, GLshort ny, GLshort nz);
	void        (APIENTRYP glNormal3sv) (const GLshort *v);
	void        (APIENTRYP glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
	void        (APIENTRYP glOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	void        (APIENTRYP glPassThrough) (GLfloat token);
	void        (APIENTRYP glPixelMapfv) (GLenum map, GLint mapsize, const GLfloat *values);
	void        (APIENTRYP glPixelMapuiv) (GLenum map, GLint mapsize, const GLuint *values);
	void        (APIENTRYP glPixelMapusv) (GLenum map, GLint mapsize, const GLushort *values);
	void        (APIENTRYP glPixelStoref) (GLenum pname, GLfloat param);
	void        (APIENTRYP glPixelStorei) (GLenum pname, GLint param);
	void        (APIENTRYP glPixelTransferf) (GLenum pname, GLfloat param);
	void        (APIENTRYP glPixelTransferi) (GLenum pname, GLint param);
	void        (APIENTRYP glPixelZoom) (GLfloat xfactor, GLfloat yfactor);
	void        (APIENTRYP glPointSize) (GLfloat size);
	void        (APIENTRYP glPolygonMode) (GLenum face, GLenum mode);
	void        (APIENTRYP glPolygonOffset) (GLfloat factor, GLfloat units);
	void        (APIENTRYP glPolygonStipple) (const GLubyte *mask);
	void        (APIENTRYP glPopAttrib) (void);
	void        (APIENTRYP glPopClientAttrib) (void);
	void        (APIENTRYP glPopMatrix) (void);
	void        (APIENTRYP glPopName) (void);
	void        (APIENTRYP glPrioritizeTextures) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
	void        (APIENTRYP glPushAttrib) (GLbitfield mask);
	void        (APIENTRYP glPushClientAttrib) (GLbitfield mask);
	void        (APIENTRYP glPushMatrix) (void);
	void        (APIENTRYP glPushName) (GLuint name);
	void        (APIENTRYP glRasterPos2d) (GLdouble x, GLdouble y);
	void        (APIENTRYP glRasterPos2dv) (const GLdouble *v);
	void        (APIENTRYP glRasterPos2f) (GLfloat x, GLfloat y);
	void        (APIENTRYP glRasterPos2fv) (const GLfloat *v);
	void        (APIENTRYP glRasterPos2i) (GLint x, GLint y);
	void        (APIENTRYP glRasterPos2iv) (const GLint *v);
	void        (APIENTRYP glRasterPos2s) (GLshort x, GLshort y);
	void        (APIENTRYP glRasterPos2sv) (const GLshort *v);
	void        (APIENTRYP glRasterPos3d) (GLdouble x, GLdouble y, GLdouble z);
	void        (APIENTRYP glRasterPos3dv) (const GLdouble *v);
	void        (APIENTRYP glRasterPos3f) (GLfloat x, GLfloat y, GLfloat z);
	void        (APIENTRYP glRasterPos3fv) (const GLfloat *v);
	void        (APIENTRYP glRasterPos3i) (GLint x, GLint y, GLint z);
	void        (APIENTRYP glRasterPos3iv) (const GLint *v);
	void        (APIENTRYP glRasterPos3s) (GLshort x, GLshort y, GLshort z);
	void        (APIENTRYP glRasterPos3sv) (const GLshort *v);
	void        (APIENTRYP glRasterPos4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	void        (APIENTRYP glRasterPos4dv) (const GLdouble *v);
	void        (APIENTRYP glRasterPos4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void        (APIENTRYP glRasterPos4fv) (const GLfloat *v);
	void        (APIENTRYP glRasterPos4i) (GLint x, GLint y, GLint z, GLint w);
	void        (APIENTRYP glRasterPos4iv) (const GLint *v);
	void        (APIENTRYP glRasterPos4s) (GLshort x, GLshort y, GLshort z, GLshort w);
	void        (APIENTRYP glRasterPos4sv) (const GLshort *v);
	void        (APIENTRYP glReadBuffer) (GLenum mode);
	void        (APIENTRYP glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	void        (APIENTRYP glRectd) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
	void        (APIENTRYP glRectdv) (const GLdouble *v1, const GLdouble *v2);
	void        (APIENTRYP glRectf) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	void        (APIENTRYP glRectfv) (const GLfloat *v1, const GLfloat *v2);
	void        (APIENTRYP glRecti) (GLint x1, GLint y1, GLint x2, GLint y2);
	void        (APIENTRYP glRectiv) (const GLint *v1, const GLint *v2);
	void        (APIENTRYP glRects) (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
	void        (APIENTRYP glRectsv) (const GLshort *v1, const GLshort *v2);
	GLint 		(APIENTRYP glRenderMode) (GLenum mode);
	void        (APIENTRYP glRotated) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
	void        (APIENTRYP glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void        (APIENTRYP glScaled) (GLdouble x, GLdouble y, GLdouble z);
	void        (APIENTRYP glScalef) (GLfloat x, GLfloat y, GLfloat z);
	void        (APIENTRYP glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
	void        (APIENTRYP glSelectBuffer) (GLsizei size, GLuint *buffer);
	void        (APIENTRYP glShadeModel) (GLenum mode);
	void        (APIENTRYP glStencilFunc) (GLenum func, GLint ref, GLuint mask);
	void        (APIENTRYP glStencilMask) (GLuint mask);
	void        (APIENTRYP glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
	void        (APIENTRYP glTexCoord1d) (GLdouble s);
	void        (APIENTRYP glTexCoord1dv) (const GLdouble *v);
	void        (APIENTRYP glTexCoord1f) (GLfloat s);
	void        (APIENTRYP glTexCoord1fv) (const GLfloat *v);
	void        (APIENTRYP glTexCoord1i) (GLint s);
	void        (APIENTRYP glTexCoord1iv) (const GLint *v);
	void        (APIENTRYP glTexCoord1s) (GLshort s);
	void        (APIENTRYP glTexCoord1sv) (const GLshort *v);
	void        (APIENTRYP glTexCoord2d) (GLdouble s, GLdouble t);
	void        (APIENTRYP glTexCoord2dv) (const GLdouble *v);
	void        (APIENTRYP glTexCoord2f) (GLfloat s, GLfloat t);
	void        (APIENTRYP glTexCoord2fv) (const GLfloat *v);
	void        (APIENTRYP glTexCoord2i) (GLint s, GLint t);
	void        (APIENTRYP glTexCoord2iv) (const GLint *v);
	void        (APIENTRYP glTexCoord2s) (GLshort s, GLshort t);
	void        (APIENTRYP glTexCoord2sv) (const GLshort *v);
	void        (APIENTRYP glTexCoord3d) (GLdouble s, GLdouble t, GLdouble r);
	void        (APIENTRYP glTexCoord3dv) (const GLdouble *v);
	void        (APIENTRYP glTexCoord3f) (GLfloat s, GLfloat t, GLfloat r);
	void        (APIENTRYP glTexCoord3fv) (const GLfloat *v);
	void        (APIENTRYP glTexCoord3i) (GLint s, GLint t, GLint r);
	void        (APIENTRYP glTexCoord3iv) (const GLint *v);
	void        (APIENTRYP glTexCoord3s) (GLshort s, GLshort t, GLshort r);
	void        (APIENTRYP glTexCoord3sv) (const GLshort *v);
	void        (APIENTRYP glTexCoord4d) (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	void        (APIENTRYP glTexCoord4dv) (const GLdouble *v);
	void        (APIENTRYP glTexCoord4f) (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
	void        (APIENTRYP glTexCoord4fv) (const GLfloat *v);
	void        (APIENTRYP glTexCoord4i) (GLint s, GLint t, GLint r, GLint q);
	void        (APIENTRYP glTexCoord4iv) (const GLint *v);
	void        (APIENTRYP glTexCoord4s) (GLshort s, GLshort t, GLshort r, GLshort q);
	void        (APIENTRYP glTexCoord4sv) (const GLshort *v);
	void        (APIENTRYP glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void        (APIENTRYP glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
	void        (APIENTRYP glTexEnvfv) (GLenum target, GLenum pname, const GLfloat *params);
	void        (APIENTRYP glTexEnvi) (GLenum target, GLenum pname, GLint param);
	void        (APIENTRYP glTexEnviv) (GLenum target, GLenum pname, const GLint *params);
	void        (APIENTRYP glTexGend) (GLenum coord, GLenum pname, GLdouble param);
	void        (APIENTRYP glTexGendv) (GLenum coord, GLenum pname, const GLdouble *params);
	void        (APIENTRYP glTexGenf) (GLenum coord, GLenum pname, GLfloat param);
	void        (APIENTRYP glTexGenfv) (GLenum coord, GLenum pname, const GLfloat *params);
	void        (APIENTRYP glTexGeni) (GLenum coord, GLenum pname, GLint param);
	void        (APIENTRYP glTexGeniv) (GLenum coord, GLenum pname, const GLint *params);
	void        (APIENTRYP glTexImage1D) (GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void        (APIENTRYP glTexImage2D) (GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void        (APIENTRYP glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
	void        (APIENTRYP glTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
	void        (APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param);
	void        (APIENTRYP glTexParameteriv) (GLenum target, GLenum pname, const GLint *params);
	void        (APIENTRYP glTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
	void        (APIENTRYP glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void        (APIENTRYP glTranslated) (GLdouble x, GLdouble y, GLdouble z);
	void        (APIENTRYP glTranslatef) (GLfloat x, GLfloat y, GLfloat z);
	void        (APIENTRYP glVertex2d) (GLdouble x, GLdouble y);
	void        (APIENTRYP glVertex2dv) (const GLdouble *v);
	void        (APIENTRYP glVertex2f) (GLfloat x, GLfloat y);
	void        (APIENTRYP glVertex2fv) (const GLfloat *v);
	void        (APIENTRYP glVertex2i) (GLint x, GLint y);
	void        (APIENTRYP glVertex2iv) (const GLint *v);
	void        (APIENTRYP glVertex2s) (GLshort x, GLshort y);
	void        (APIENTRYP glVertex2sv) (const GLshort *v);
	void        (APIENTRYP glVertex3d) (GLdouble x, GLdouble y, GLdouble z);
	void        (APIENTRYP glVertex3dv) (const GLdouble *v);
	void        (APIENTRYP glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
	void        (APIENTRYP glVertex3fv) (const GLfloat *v);
	void        (APIENTRYP glVertex3i) (GLint x, GLint y, GLint z);
	void        (APIENTRYP glVertex3iv) (const GLint *v);
	void        (APIENTRYP glVertex3s) (GLshort x, GLshort y, GLshort z);
	void        (APIENTRYP glVertex3sv) (const GLshort *v);
	void        (APIENTRYP glVertex4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	void        (APIENTRYP glVertex4dv) (const GLdouble *v);
	void        (APIENTRYP glVertex4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void        (APIENTRYP glVertex4fv) (const GLfloat *v);
	void        (APIENTRYP glVertex4i) (GLint x, GLint y, GLint z, GLint w);
	void        (APIENTRYP glVertex4iv) (const GLint *v);
	void        (APIENTRYP glVertex4s) (GLshort x, GLshort y, GLshort z, GLshort w);
	void        (APIENTRYP glVertex4sv) (const GLshort *v);
	void        (APIENTRYP glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void        (APIENTRYP glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

	/* OpenGL 1.2 additional functions */
	void 		(APIENTRYP glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	void 		(APIENTRYP glTexImage3D)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void 		(APIENTRYP glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
	void 		(APIENTRYP glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	} GA_glFuncs;

/****************************************************************************
REMARKS:
Function pointer type for the Binary Portable DLL initialisation entry point.
{secret}
****************************************************************************/
typedef GA_glCtx * (NAPIP GA_gl3DInitLibrary_t)(ibool shared,PM_imports *pmImp,GA_devCtx *hwCtx);

#pragma pack()

/*------------------------- Function Prototypes ---------------------------*/

#ifdef	__cplusplus
extern "C" {
#endif

/* Functions to load and unload the GL3D device driver */

GA_glCtx *	NAPI GA_glLoadDriver(GA_devCtx *hwCtx,ibool shared);
void		NAPI GA_glUnloadDriver(GA_glCtx *glCtx);

#ifdef	__cplusplus
}
#endif	/* __cplusplus */

#endif	/* __NUCLEUS_GL3D_H */
