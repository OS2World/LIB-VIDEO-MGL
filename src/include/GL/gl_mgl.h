/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	Any
*
* Description:  MGL specific OpenGL header that contains the OpenGL entry
*				points table for both the GL and GLU functions. We include
*				this in a separate header so that we can include either
*				GL\gl.h or GL\glu.h without the other header and things will
*				still work correctly.
*
****************************************************************************/

#ifndef __gl_mgl_h_
#define __gl_mgl_h_

#ifdef __cplusplus
extern "C" {
#endif

/* Structure containing all public OpenGL functions supported by the MGL */

#pragma pack(1)
typedef struct {
	long		dwSize;

	/* Regular OpenGL functions */
	void (APIENTRYP glAccum) (GLenum op, GLfloat value);
	void (APIENTRYP glAlphaFunc) (GLenum func, GLclampf ref);
	GLboolean (APIENTRYP glAreTexturesResident) (GLsizei n, const GLuint *textures, GLboolean *residences);
	void (APIENTRYP glArrayElement) (GLint i);
	void (APIENTRYP glBegin) (GLenum mode);
	void (APIENTRYP glBindTexture) (GLenum target, GLuint texture);
	void (APIENTRYP glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
	void (APIENTRYP glBlendFunc) (GLenum sfactor, GLenum dfactor);
	void (APIENTRYP glCallList) (GLuint list);
	void (APIENTRYP glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
	void (APIENTRYP glClear) (GLbitfield mask);
	void (APIENTRYP glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRYP glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void (APIENTRYP glClearDepth) (GLclampd depth);
	void (APIENTRYP glClearIndex) (GLfloat c);
	void (APIENTRYP glClearStencil) (GLint s);
	void (APIENTRYP glClipPlane) (GLenum plane, const GLdouble *equation);
	void (APIENTRYP glColor3b) (GLbyte red, GLbyte green, GLbyte blue);
	void (APIENTRYP glColor3bv) (const GLbyte *v);
	void (APIENTRYP glColor3d) (GLdouble red, GLdouble green, GLdouble blue);
	void (APIENTRYP glColor3dv) (const GLdouble *v);
	void (APIENTRYP glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
	void (APIENTRYP glColor3fv) (const GLfloat *v);
	void (APIENTRYP glColor3i) (GLint red, GLint green, GLint blue);
	void (APIENTRYP glColor3iv) (const GLint *v);
	void (APIENTRYP glColor3s) (GLshort red, GLshort green, GLshort blue);
	void (APIENTRYP glColor3sv) (const GLshort *v);
	void (APIENTRYP glColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
	void (APIENTRYP glColor3ubv) (const GLubyte *v);
	void (APIENTRYP glColor3ui) (GLuint red, GLuint green, GLuint blue);
	void (APIENTRYP glColor3uiv) (const GLuint *v);
	void (APIENTRYP glColor3us) (GLushort red, GLushort green, GLushort blue);
	void (APIENTRYP glColor3usv) (const GLushort *v);
	void (APIENTRYP glColor4b) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
	void (APIENTRYP glColor4bv) (const GLbyte *v);
	void (APIENTRYP glColor4d) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
	void (APIENTRYP glColor4dv) (const GLdouble *v);
	void (APIENTRYP glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void (APIENTRYP glColor4fv) (const GLfloat *v);
	void (APIENTRYP glColor4i) (GLint red, GLint green, GLint blue, GLint alpha);
	void (APIENTRYP glColor4iv) (const GLint *v);
	void (APIENTRYP glColor4s) (GLshort red, GLshort green, GLshort blue, GLshort alpha);
	void (APIENTRYP glColor4sv) (const GLshort *v);
	void (APIENTRYP glColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
	void (APIENTRYP glColor4ubv) (const GLubyte *v);
	void (APIENTRYP glColor4ui) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
	void (APIENTRYP glColor4uiv) (const GLuint *v);
	void (APIENTRYP glColor4us) (GLushort red, GLushort green, GLushort blue, GLushort alpha);
	void (APIENTRYP glColor4usv) (const GLushort *v);
	void (APIENTRYP glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void (APIENTRYP glColorMaterial) (GLenum face, GLenum mode);
	void (APIENTRYP glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRYP glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
	void (APIENTRYP glCopyTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	void (APIENTRYP glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	void (APIENTRYP glCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	void (APIENTRYP glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	void (APIENTRYP glCullFace) (GLenum mode);
	void (APIENTRYP glDeleteLists) (GLuint list, GLsizei range);
	void (APIENTRYP glDeleteTextures) (GLsizei n, const GLuint *textures);
	void (APIENTRYP glDepthFunc) (GLenum func);
	void (APIENTRYP glDepthMask) (GLboolean flag);
	void (APIENTRYP glDepthRange) (GLclampd zNear, GLclampd zFar);
	void (APIENTRYP glDisable) (GLenum cap);
	void (APIENTRYP glDisableClientState) (GLenum array);
	void (APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count);
	void (APIENTRYP glDrawBuffer) (GLenum mode);
	void (APIENTRYP glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void (APIENTRYP glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRYP glEdgeFlag) (GLboolean flag);
	void (APIENTRYP glEdgeFlagPointer) (GLsizei stride, const GLboolean *pointer);
	void (APIENTRYP glEdgeFlagv) (const GLboolean *flag);
	void (APIENTRYP glEnable) (GLenum cap);
	void (APIENTRYP glEnableClientState) (GLenum array);
	void (APIENTRYP glEnd) (void);
	void (APIENTRYP glEndList) (void);
	void (APIENTRYP glEvalCoord1d) (GLdouble u);
	void (APIENTRYP glEvalCoord1dv) (const GLdouble *u);
	void (APIENTRYP glEvalCoord1f) (GLfloat u);
	void (APIENTRYP glEvalCoord1fv) (const GLfloat *u);
	void (APIENTRYP glEvalCoord2d) (GLdouble u, GLdouble v);
	void (APIENTRYP glEvalCoord2dv) (const GLdouble *u);
	void (APIENTRYP glEvalCoord2f) (GLfloat u, GLfloat v);
	void (APIENTRYP glEvalCoord2fv) (const GLfloat *u);
	void (APIENTRYP glEvalMesh1) (GLenum mode, GLint i1, GLint i2);
	void (APIENTRYP glEvalMesh2) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
	void (APIENTRYP glEvalPoint1) (GLint i);
	void (APIENTRYP glEvalPoint2) (GLint i, GLint j);
	void (APIENTRYP glFeedbackBuffer) (GLsizei size, GLenum type, GLfloat *buffer);
	void (APIENTRYP glFinish) (void);
	void (APIENTRYP glFlush) (void);
	void (APIENTRYP glFogf) (GLenum pname, GLfloat param);
	void (APIENTRYP glFogfv) (GLenum pname, const GLfloat *params);
	void (APIENTRYP glFogi) (GLenum pname, GLint param);
	void (APIENTRYP glFogiv) (GLenum pname, const GLint *params);
	void (APIENTRYP glFrontFace) (GLenum mode);
	void (APIENTRYP glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	GLuint (APIENTRYP glGenLists) (GLsizei range);
	void (APIENTRYP glGenTextures) (GLsizei n, GLuint *textures);
	void (APIENTRYP glGetBooleanv) (GLenum pname, GLboolean *params);
	void (APIENTRYP glGetClipPlane) (GLenum plane, GLdouble *equation);
	void (APIENTRYP glGetDoublev) (GLenum pname, GLdouble *params);
	GLenum (APIENTRYP glGetError) (void);
	void (APIENTRYP glGetFloatv) (GLenum pname, GLfloat *params);
	void (APIENTRYP glGetIntegerv) (GLenum pname, GLint *params);
	void (APIENTRYP glGetLightfv) (GLenum light, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetLightiv) (GLenum light, GLenum pname, GLint *params);
	void (APIENTRYP glGetMapdv) (GLenum target, GLenum query, GLdouble *v);
	void (APIENTRYP glGetMapfv) (GLenum target, GLenum query, GLfloat *v);
	void (APIENTRYP glGetMapiv) (GLenum target, GLenum query, GLint *v);
	void (APIENTRYP glGetMaterialfv) (GLenum face, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetMaterialiv) (GLenum face, GLenum pname, GLint *params);
	void (APIENTRYP glGetPixelMapfv) (GLenum map, GLfloat *values);
	void (APIENTRYP glGetPixelMapuiv) (GLenum map, GLuint *values);
	void (APIENTRYP glGetPixelMapusv) (GLenum map, GLushort *values);
	void (APIENTRYP glGetPointerv) (GLenum pname, GLvoid* *params);
	void (APIENTRYP glGetPolygonStipple) (GLubyte *mask);
	const GLubyte * (APIENTRYP glGetString) (GLenum name);
	void (APIENTRYP glGetTexEnvfv) (GLenum target, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetTexEnviv) (GLenum target, GLenum pname, GLint *params);
	void (APIENTRYP glGetTexGendv) (GLenum coord, GLenum pname, GLdouble *params);
	void (APIENTRYP glGetTexGenfv) (GLenum coord, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetTexGeniv) (GLenum coord, GLenum pname, GLint *params);
	void (APIENTRYP glGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
	void (APIENTRYP glGetTexLevelParameterfv) (GLenum target, GLint level, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params);
	void (APIENTRYP glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat *params);
	void (APIENTRYP glGetTexParameteriv) (GLenum target, GLenum pname, GLint *params);
	void (APIENTRYP glHint) (GLenum target, GLenum mode);
	void (APIENTRYP glIndexMask) (GLuint mask);
	void (APIENTRYP glIndexPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRYP glIndexd) (GLdouble c);
	void (APIENTRYP glIndexdv) (const GLdouble *c);
	void (APIENTRYP glIndexf) (GLfloat c);
	void (APIENTRYP glIndexfv) (const GLfloat *c);
	void (APIENTRYP glIndexi) (GLint c);
	void (APIENTRYP glIndexiv) (const GLint *c);
	void (APIENTRYP glIndexs) (GLshort c);
	void (APIENTRYP glIndexsv) (const GLshort *c);
	void (APIENTRYP glIndexub) (GLubyte c);
	void (APIENTRYP glIndexubv) (const GLubyte *c);
	void (APIENTRYP glInitNames) (void);
	void (APIENTRYP glInterleavedArrays) (GLenum format, GLsizei stride, const GLvoid *pointer);
	GLboolean (APIENTRYP glIsEnabled) (GLenum cap);
	GLboolean (APIENTRYP glIsList) (GLuint list);
	GLboolean (APIENTRYP glIsTexture) (GLuint texture);
	void (APIENTRYP glLightModelf) (GLenum pname, GLfloat param);
	void (APIENTRYP glLightModelfv) (GLenum pname, const GLfloat *params);
	void (APIENTRYP glLightModeli) (GLenum pname, GLint param);
	void (APIENTRYP glLightModeliv) (GLenum pname, const GLint *params);
	void (APIENTRYP glLightf) (GLenum light, GLenum pname, GLfloat param);
	void (APIENTRYP glLightfv) (GLenum light, GLenum pname, const GLfloat *params);
	void (APIENTRYP glLighti) (GLenum light, GLenum pname, GLint param);
	void (APIENTRYP glLightiv) (GLenum light, GLenum pname, const GLint *params);
	void (APIENTRYP glLineStipple) (GLint factor, GLushort pattern);
	void (APIENTRYP glLineWidth) (GLfloat width);
	void (APIENTRYP glListBase) (GLuint base);
	void (APIENTRYP glLoadIdentity) (void);
	void (APIENTRYP glLoadMatrixd) (const GLdouble *m);
	void (APIENTRYP glLoadMatrixf) (const GLfloat *m);
	void (APIENTRYP glLoadName) (GLuint name);
	void (APIENTRYP glLogicOp) (GLenum opcode);
	void (APIENTRYP glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
	void (APIENTRYP glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
	void (APIENTRYP glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
	void (APIENTRYP glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
	void (APIENTRYP glMapGrid1d) (GLint un, GLdouble u1, GLdouble u2);
	void (APIENTRYP glMapGrid1f) (GLint un, GLfloat u1, GLfloat u2);
	void (APIENTRYP glMapGrid2d) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
	void (APIENTRYP glMapGrid2f) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
	void (APIENTRYP glMaterialf) (GLenum face, GLenum pname, GLfloat param);
	void (APIENTRYP glMaterialfv) (GLenum face, GLenum pname, const GLfloat *params);
	void (APIENTRYP glMateriali) (GLenum face, GLenum pname, GLint param);
	void (APIENTRYP glMaterialiv) (GLenum face, GLenum pname, const GLint *params);
	void (APIENTRYP glMatrixMode) (GLenum mode);
	void (APIENTRYP glMultMatrixd) (const GLdouble *m);
	void (APIENTRYP glMultMatrixf) (const GLfloat *m);
	void (APIENTRYP glNewList) (GLuint list, GLenum mode);
	void (APIENTRYP glNormal3b) (GLbyte nx, GLbyte ny, GLbyte nz);
	void (APIENTRYP glNormal3bv) (const GLbyte *v);
	void (APIENTRYP glNormal3d) (GLdouble nx, GLdouble ny, GLdouble nz);
	void (APIENTRYP glNormal3dv) (const GLdouble *v);
	void (APIENTRYP glNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz);
	void (APIENTRYP glNormal3fv) (const GLfloat *v);
	void (APIENTRYP glNormal3i) (GLint nx, GLint ny, GLint nz);
	void (APIENTRYP glNormal3iv) (const GLint *v);
	void (APIENTRYP glNormal3s) (GLshort nx, GLshort ny, GLshort nz);
	void (APIENTRYP glNormal3sv) (const GLshort *v);
	void (APIENTRYP glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRYP glOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	void (APIENTRYP glPassThrough) (GLfloat token);
	void (APIENTRYP glPixelMapfv) (GLenum map, GLint mapsize, const GLfloat *values);
	void (APIENTRYP glPixelMapuiv) (GLenum map, GLint mapsize, const GLuint *values);
	void (APIENTRYP glPixelMapusv) (GLenum map, GLint mapsize, const GLushort *values);
	void (APIENTRYP glPixelStoref) (GLenum pname, GLfloat param);
	void (APIENTRYP glPixelStorei) (GLenum pname, GLint param);
	void (APIENTRYP glPixelTransferf) (GLenum pname, GLfloat param);
	void (APIENTRYP glPixelTransferi) (GLenum pname, GLint param);
	void (APIENTRYP glPixelZoom) (GLfloat xfactor, GLfloat yfactor);
	void (APIENTRYP glPointSize) (GLfloat size);
	void (APIENTRYP glPolygonMode) (GLenum face, GLenum mode);
	void (APIENTRYP glPolygonOffset) (GLfloat factor, GLfloat units);
	void (APIENTRYP glPolygonStipple) (const GLubyte *mask);
	void (APIENTRYP glPopAttrib) (void);
	void (APIENTRYP glPopClientAttrib) (void);
	void (APIENTRYP glPopMatrix) (void);
	void (APIENTRYP glPopName) (void);
	void (APIENTRYP glPrioritizeTextures) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
	void (APIENTRYP glPushAttrib) (GLbitfield mask);
	void (APIENTRYP glPushClientAttrib) (GLbitfield mask);
	void (APIENTRYP glPushMatrix) (void);
	void (APIENTRYP glPushName) (GLuint name);
	void (APIENTRYP glRasterPos2d) (GLdouble x, GLdouble y);
	void (APIENTRYP glRasterPos2dv) (const GLdouble *v);
	void (APIENTRYP glRasterPos2f) (GLfloat x, GLfloat y);
	void (APIENTRYP glRasterPos2fv) (const GLfloat *v);
	void (APIENTRYP glRasterPos2i) (GLint x, GLint y);
	void (APIENTRYP glRasterPos2iv) (const GLint *v);
	void (APIENTRYP glRasterPos2s) (GLshort x, GLshort y);
	void (APIENTRYP glRasterPos2sv) (const GLshort *v);
	void (APIENTRYP glRasterPos3d) (GLdouble x, GLdouble y, GLdouble z);
	void (APIENTRYP glRasterPos3dv) (const GLdouble *v);
	void (APIENTRYP glRasterPos3f) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRYP glRasterPos3fv) (const GLfloat *v);
	void (APIENTRYP glRasterPos3i) (GLint x, GLint y, GLint z);
	void (APIENTRYP glRasterPos3iv) (const GLint *v);
	void (APIENTRYP glRasterPos3s) (GLshort x, GLshort y, GLshort z);
	void (APIENTRYP glRasterPos3sv) (const GLshort *v);
	void (APIENTRYP glRasterPos4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	void (APIENTRYP glRasterPos4dv) (const GLdouble *v);
	void (APIENTRYP glRasterPos4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void (APIENTRYP glRasterPos4fv) (const GLfloat *v);
	void (APIENTRYP glRasterPos4i) (GLint x, GLint y, GLint z, GLint w);
	void (APIENTRYP glRasterPos4iv) (const GLint *v);
	void (APIENTRYP glRasterPos4s) (GLshort x, GLshort y, GLshort z, GLshort w);
	void (APIENTRYP glRasterPos4sv) (const GLshort *v);
	void (APIENTRYP glReadBuffer) (GLenum mode);
	void (APIENTRYP glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	void (APIENTRYP glRectd) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
	void (APIENTRYP glRectdv) (const GLdouble *v1, const GLdouble *v2);
	void (APIENTRYP glRectf) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	void (APIENTRYP glRectfv) (const GLfloat *v1, const GLfloat *v2);
	void (APIENTRYP glRecti) (GLint x1, GLint y1, GLint x2, GLint y2);
	void (APIENTRYP glRectiv) (const GLint *v1, const GLint *v2);
	void (APIENTRYP glRects) (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
	void (APIENTRYP glRectsv) (const GLshort *v1, const GLshort *v2);
	GLint (APIENTRYP glRenderMode) (GLenum mode);
	void (APIENTRYP glRotated) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
	void (APIENTRYP glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRYP glScaled) (GLdouble x, GLdouble y, GLdouble z);
	void (APIENTRYP glScalef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRYP glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
	void (APIENTRYP glSelectBuffer) (GLsizei size, GLuint *buffer);
	void (APIENTRYP glShadeModel) (GLenum mode);
	void (APIENTRYP glStencilFunc) (GLenum func, GLint ref, GLuint mask);
	void (APIENTRYP glStencilMask) (GLuint mask);
	void (APIENTRYP glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
	void (APIENTRYP glTexCoord1d) (GLdouble s);
	void (APIENTRYP glTexCoord1dv) (const GLdouble *v);
	void (APIENTRYP glTexCoord1f) (GLfloat s);
	void (APIENTRYP glTexCoord1fv) (const GLfloat *v);
	void (APIENTRYP glTexCoord1i) (GLint s);
	void (APIENTRYP glTexCoord1iv) (const GLint *v);
	void (APIENTRYP glTexCoord1s) (GLshort s);
	void (APIENTRYP glTexCoord1sv) (const GLshort *v);
	void (APIENTRYP glTexCoord2d) (GLdouble s, GLdouble t);
	void (APIENTRYP glTexCoord2dv) (const GLdouble *v);
	void (APIENTRYP glTexCoord2f) (GLfloat s, GLfloat t);
	void (APIENTRYP glTexCoord2fv) (const GLfloat *v);
	void (APIENTRYP glTexCoord2i) (GLint s, GLint t);
	void (APIENTRYP glTexCoord2iv) (const GLint *v);
	void (APIENTRYP glTexCoord2s) (GLshort s, GLshort t);
	void (APIENTRYP glTexCoord2sv) (const GLshort *v);
	void (APIENTRYP glTexCoord3d) (GLdouble s, GLdouble t, GLdouble r);
	void (APIENTRYP glTexCoord3dv) (const GLdouble *v);
	void (APIENTRYP glTexCoord3f) (GLfloat s, GLfloat t, GLfloat r);
	void (APIENTRYP glTexCoord3fv) (const GLfloat *v);
	void (APIENTRYP glTexCoord3i) (GLint s, GLint t, GLint r);
	void (APIENTRYP glTexCoord3iv) (const GLint *v);
	void (APIENTRYP glTexCoord3s) (GLshort s, GLshort t, GLshort r);
	void (APIENTRYP glTexCoord3sv) (const GLshort *v);
	void (APIENTRYP glTexCoord4d) (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	void (APIENTRYP glTexCoord4dv) (const GLdouble *v);
	void (APIENTRYP glTexCoord4f) (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
	void (APIENTRYP glTexCoord4fv) (const GLfloat *v);
	void (APIENTRYP glTexCoord4i) (GLint s, GLint t, GLint r, GLint q);
	void (APIENTRYP glTexCoord4iv) (const GLint *v);
	void (APIENTRYP glTexCoord4s) (GLshort s, GLshort t, GLshort r, GLshort q);
	void (APIENTRYP glTexCoord4sv) (const GLshort *v);
	void (APIENTRYP glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRYP glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
	void (APIENTRYP glTexEnvfv) (GLenum target, GLenum pname, const GLfloat *params);
	void (APIENTRYP glTexEnvi) (GLenum target, GLenum pname, GLint param);
	void (APIENTRYP glTexEnviv) (GLenum target, GLenum pname, const GLint *params);
	void (APIENTRYP glTexGend) (GLenum coord, GLenum pname, GLdouble param);
	void (APIENTRYP glTexGendv) (GLenum coord, GLenum pname, const GLdouble *params);
	void (APIENTRYP glTexGenf) (GLenum coord, GLenum pname, GLfloat param);
	void (APIENTRYP glTexGenfv) (GLenum coord, GLenum pname, const GLfloat *params);
	void (APIENTRYP glTexGeni) (GLenum coord, GLenum pname, GLint param);
	void (APIENTRYP glTexGeniv) (GLenum coord, GLenum pname, const GLint *params);
	void (APIENTRYP glTexImage1D) (GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRYP glTexImage2D) (GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRYP glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
	void (APIENTRYP glTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
	void (APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param);
	void (APIENTRYP glTexParameteriv) (GLenum target, GLenum pname, const GLint *params);
	void (APIENTRYP glTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRYP glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void (APIENTRYP glTranslated) (GLdouble x, GLdouble y, GLdouble z);
	void (APIENTRYP glTranslatef) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRYP glVertex2d) (GLdouble x, GLdouble y);
	void (APIENTRYP glVertex2dv) (const GLdouble *v);
	void (APIENTRYP glVertex2f) (GLfloat x, GLfloat y);
	void (APIENTRYP glVertex2fv) (const GLfloat *v);
	void (APIENTRYP glVertex2i) (GLint x, GLint y);
	void (APIENTRYP glVertex2iv) (const GLint *v);
	void (APIENTRYP glVertex2s) (GLshort x, GLshort y);
	void (APIENTRYP glVertex2sv) (const GLshort *v);
	void (APIENTRYP glVertex3d) (GLdouble x, GLdouble y, GLdouble z);
	void (APIENTRYP glVertex3dv) (const GLdouble *v);
	void (APIENTRYP glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
	void (APIENTRYP glVertex3fv) (const GLfloat *v);
	void (APIENTRYP glVertex3i) (GLint x, GLint y, GLint z);
	void (APIENTRYP glVertex3iv) (const GLint *v);
	void (APIENTRYP glVertex3s) (GLshort x, GLshort y, GLshort z);
	void (APIENTRYP glVertex3sv) (const GLshort *v);
	void (APIENTRYP glVertex4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	void (APIENTRYP glVertex4dv) (const GLdouble *v);
	void (APIENTRYP glVertex4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void (APIENTRYP glVertex4fv) (const GLfloat *v);
	void (APIENTRYP glVertex4i) (GLint x, GLint y, GLint z, GLint w);
	void (APIENTRYP glVertex4iv) (const GLint *v);
	void (APIENTRYP glVertex4s) (GLshort x, GLshort y, GLshort z, GLshort w);
	void (APIENTRYP glVertex4sv) (const GLshort *v);
	void (APIENTRYP glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void (APIENTRYP glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

	/* OpenGL 1.2 additional functions */
	void 	(APIENTRYP glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	void 	(APIENTRYP glTexImage3D)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	void 	(APIENTRYP glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
	void 	(APIENTRYP glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	} MGL_glFuncs;
#pragma pack()

#if !defined(NO_GL_MACROS)

/* Macros to call the OpenGL functions via a function pointer */

#define glAccum(op,value)                                                               _MGL_glFuncs.glAccum(op,value)
#define glAlphaFunc(func,ref)                                                           _MGL_glFuncs.glAlphaFunc(func,ref)
#define glAreTexturesResident(n,textures,residences)                                    _MGL_glFuncs.glAreTexturesResident(n,textures,residences)
#define glArrayElement(i)                                                               _MGL_glFuncs.glArrayElement(i)
#define glBegin(mode)                                                                   _MGL_glFuncs.glBegin(mode)
#define glBindTexture(target,texture)                                                   _MGL_glFuncs.glBindTexture(target,texture)
#define glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap)                           _MGL_glFuncs.glBitmap(width,height,xorig,yorig,xmove,ymove,bitmap)
#define glBlendFunc(sfactor,dfactor)                                                    _MGL_glFuncs.glBlendFunc(sfactor,dfactor)
#define glCallList(list)                                                                _MGL_glFuncs.glCallList(list)
#define glCallLists(n,type,lists)                                                       _MGL_glFuncs.glCallLists(n,type,lists)
#define glClear(mask)                                                                   _MGL_glFuncs.glClear(mask)
#define glClearAccum(red,green,blue,alpha)                                              _MGL_glFuncs.glClearAccum(red,green,blue,alpha)
#define glClearColor(red,green,blue,alpha)                                              _MGL_glFuncs.glClearColor(red,green,blue,alpha)
#define glClearDepth(depth)                                                             _MGL_glFuncs.glClearDepth(depth)
#define glClearIndex(c)                                                                 _MGL_glFuncs.glClearIndex(c)
#define glClearStencil(s)                                                               _MGL_glFuncs.glClearStencil(s)
#define glClipPlane(plane,equation)                                                     _MGL_glFuncs.glClipPlane(plane,equation)
#define glColor3b(red,green,blue)                                                       _MGL_glFuncs.glColor3b(red,green,blue)
#define glColor3bv(v)                                                                   _MGL_glFuncs.glColor3bv(v)
#define glColor3d(red,green,blue)                                                       _MGL_glFuncs.glColor3d(red,green,blue)
#define glColor3dv(v)                                                                   _MGL_glFuncs.glColor3dv(v)
#define glColor3f(red,green,blue)                                                       _MGL_glFuncs.glColor3f(red,green,blue)
#define glColor3fv(v)                                                                   _MGL_glFuncs.glColor3fv(v)
#define glColor3i(red,green,blue)                                                       _MGL_glFuncs.glColor3i(red,green,blue)
#define glColor3iv(v)                                                                   _MGL_glFuncs.glColor3iv(v)
#define glColor3s(red,green,blue)                                                       _MGL_glFuncs.glColor3s(red,green,blue)
#define glColor3sv(v)                                                                   _MGL_glFuncs.glColor3sv(v)
#define glColor3ub(red,green,blue)                                                      _MGL_glFuncs.glColor3ub(red,green,blue)
#define glColor3ubv(v)                                                                  _MGL_glFuncs.glColor3ubv(v)
#define glColor3ui(red,green,blue)                                                      _MGL_glFuncs.glColor3ui(red,green,blue)
#define glColor3uiv(v)                                                                  _MGL_glFuncs.glColor3uiv(v)
#define glColor3us(red,green,blue)                                                      _MGL_glFuncs.glColor3us(red,green,blue)
#define glColor3usv(v)                                                                  _MGL_glFuncs.glColor3usv(v)
#define glColor4b(red,green,blue,alpha)                                                 _MGL_glFuncs.glColor4b(red,green,blue,alpha)
#define glColor4bv(v)                                                                   _MGL_glFuncs.glColor4bv(v)
#define glColor4d(red,green,blue,alpha)                                                 _MGL_glFuncs.glColor4d(red,green,blue,alpha)
#define glColor4dv(v)                                                                   _MGL_glFuncs.glColor4dv(v)
#define glColor4f(red,green,blue,alpha)                                                 _MGL_glFuncs.glColor4f(red,green,blue,alpha)
#define glColor4fv(v)                                                                   _MGL_glFuncs.glColor4fv(v)
#define glColor4i(red,green,blue,alpha)                                                 _MGL_glFuncs.glColor4i(red,green,blue,alpha)
#define glColor4iv(v)                                                                   _MGL_glFuncs.glColor4iv(v)
#define glColor4s(red,green,blue,alpha)                                                 _MGL_glFuncs.glColor4s(red,green,blue,alpha)
#define glColor4sv(v)                                                                   _MGL_glFuncs.glColor4sv(v)
#define glColor4ub(red,green,blue,alpha)                                                _MGL_glFuncs.glColor4ub(red,green,blue,alpha)
#define glColor4ubv(v)                                                                  _MGL_glFuncs.glColor4ubv(v)
#define glColor4ui(red,green,blue,alpha)                                                _MGL_glFuncs.glColor4ui(red,green,blue,alpha)
#define glColor4uiv(v)                                                                  _MGL_glFuncs.glColor4uiv(v)
#define glColor4us(red,green,blue,alpha)                                                _MGL_glFuncs.glColor4us(red,green,blue,alpha)
#define glColor4usv(v)                                                                  _MGL_glFuncs.glColor4usv(v)
#define glColorMask(red,green,blue,alpha)                                               _MGL_glFuncs.glColorMask(red,green,blue,alpha)
#define glColorMaterial(face,mode)                                                      _MGL_glFuncs.glColorMaterial(face,mode)
#define glColorPointer(size,type,stride,pointer)                                        _MGL_glFuncs.glColorPointer(size,type,stride,pointer)
#define glCopyPixels(x,y,width,height,type)                                             _MGL_glFuncs.glCopyPixels(x,y,width,height,type)
#define glCopyTexImage1D(target,level,internalformat,x,y,width,border)                  _MGL_glFuncs.glCopyTexImage1D(target,level,internalformat,x,y,width,border)
#define glCopyTexImage2D(target,level,internalformat,x,y,width,height,border)           _MGL_glFuncs.glCopyTexImage2D(target,level,internalformat,x,y,width,height,border)
#define glCopyTexSubImage1D(target,level,xoffset,x,y,width)                             _MGL_glFuncs.glCopyTexSubImage1D(target,level,xoffset,x,y,width)
#define glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height)              _MGL_glFuncs.glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height)
#define glCullFace(mode)                                                                _MGL_glFuncs.glCullFace(mode)
#define glDeleteLists(list,range)                                                       _MGL_glFuncs.glDeleteLists(list,range)
#define glDeleteTextures(n,textures)                                                    _MGL_glFuncs.glDeleteTextures(n,textures)
#define glDepthFunc(func)                                                               _MGL_glFuncs.glDepthFunc(func)
#define glDepthMask(flag)                                                               _MGL_glFuncs.glDepthMask(flag)
#define glDepthRange(zNear,zFar)                                                        _MGL_glFuncs.glDepthRange(zNear,zFar)
#define glDisable(cap)                                                                  _MGL_glFuncs.glDisable(cap)
#define glDisableClientState(array)                                                     _MGL_glFuncs.glDisableClientState(array)
#define glDrawArrays(mode,first,count)                                                  _MGL_glFuncs.glDrawArrays(mode,first,count)
#define glDrawElements(mode,count,type,indices)                                         _MGL_glFuncs.glDrawElements(mode,count,type,indices)
#define glDrawPixels(width,height,format,type,pixels)                                   _MGL_glFuncs.glDrawPixels(width,height,format,type,pixels)
#define glEdgeFlag(flag)                                                                _MGL_glFuncs.glEdgeFlag(flag)
#define glEdgeFlagPointer(stride,pointer)                                               _MGL_glFuncs.glEdgeFlagPointer(stride,pointer)
#define glEdgeFlagv(flag)                                                               _MGL_glFuncs.glEdgeFlagv(flag)
#define glEnable(cap)                                                                   _MGL_glFuncs.glEnable(cap)
#define glEnableClientState(array)                                                      _MGL_glFuncs.glEnableClientState(array)
#define glEnd()                                                                     _MGL_glFuncs.glEnd()
#define glEndList()                                                                 _MGL_glFuncs.glEndList()
#define glEvalCoord1d(u)                                                                _MGL_glFuncs.glEvalCoord1d(u)
#define glEvalCoord1dv(u)                                                               _MGL_glFuncs.glEvalCoord1dv(u)
#define glEvalCoord1f(u)                                                                _MGL_glFuncs.glEvalCoord1f(u)
#define glEvalCoord1fv(u)                                                               _MGL_glFuncs.glEvalCoord1fv(u)
#define glEvalCoord2d(u,v)                                                              _MGL_glFuncs.glEvalCoord2d(u,v)
#define glEvalCoord2dv(u)                                                               _MGL_glFuncs.glEvalCoord2dv(u)
#define glEvalCoord2f(u,v)                                                              _MGL_glFuncs.glEvalCoord2f(u,v)
#define glEvalCoord2fv(u)                                                               _MGL_glFuncs.glEvalCoord2fv(u)
#define glEvalMesh1(mode,i1,i2)                                                         _MGL_glFuncs.glEvalMesh1(mode,i1,i2)
#define glEvalMesh2(mode,i1,i2,j1,j2)                                                   _MGL_glFuncs.glEvalMesh2(mode,i1,i2,j1,j2)
#define glEvalPoint1(i)                                                                 _MGL_glFuncs.glEvalPoint1(i)
#define glEvalPoint2(i,j)                                                               _MGL_glFuncs.glEvalPoint2(i,j)
#define glFeedbackBuffer(size,type,buffer)                                              _MGL_glFuncs.glFeedbackBuffer(size,type,buffer)
#define glFinish()                                                                  _MGL_glFuncs.glFinish()
#define glFlush()                                                                   _MGL_glFuncs.glFlush()
#define glFogf(pname,param)                                                             _MGL_glFuncs.glFogf(pname,param)
#define glFogfv(pname,params)                                                           _MGL_glFuncs.glFogfv(pname,params)
#define glFogi(pname,param)                                                             _MGL_glFuncs.glFogi(pname,param)
#define glFogiv(pname,params)                                                           _MGL_glFuncs.glFogiv(pname,params)
#define glFrontFace(mode)                                                               _MGL_glFuncs.glFrontFace(mode)
#define glFrustum(left,right,bottom,top,zNear,zFar)                                     _MGL_glFuncs.glFrustum(left,right,bottom,top,zNear,zFar)
#define glGenLists(range)                                                               _MGL_glFuncs.glGenLists(range)
#define glGenTextures(n,textures)                                                       _MGL_glFuncs.glGenTextures(n,textures)
#define glGetBooleanv(pname,params)                                                     _MGL_glFuncs.glGetBooleanv(pname,params)
#define glGetClipPlane(plane,equation)                                                  _MGL_glFuncs.glGetClipPlane(plane,equation)
#define glGetDoublev(pname,params)                                                      _MGL_glFuncs.glGetDoublev(pname,params)
#define glGetError()                                                                _MGL_glFuncs.glGetError()
#define glGetFloatv(pname,params)                                                       _MGL_glFuncs.glGetFloatv(pname,params)
#define glGetIntegerv(pname,params)                                                     _MGL_glFuncs.glGetIntegerv(pname,params)
#define glGetLightfv(light,pname,params)                                                _MGL_glFuncs.glGetLightfv(light,pname,params)
#define glGetLightiv(light,pname,params)                                                _MGL_glFuncs.glGetLightiv(light,pname,params)
#define glGetMapdv(target,query,v)                                                      _MGL_glFuncs.glGetMapdv(target,query,v)
#define glGetMapfv(target,query,v)                                                      _MGL_glFuncs.glGetMapfv(target,query,v)
#define glGetMapiv(target,query,v)                                                      _MGL_glFuncs.glGetMapiv(target,query,v)
#define glGetMaterialfv(face,pname,params)                                              _MGL_glFuncs.glGetMaterialfv(face,pname,params)
#define glGetMaterialiv(face,pname,params)                                              _MGL_glFuncs.glGetMaterialiv(face,pname,params)
#define glGetPixelMapfv(map,values)                                                     _MGL_glFuncs.glGetPixelMapfv(map,values)
#define glGetPixelMapuiv(map,values)                                                    _MGL_glFuncs.glGetPixelMapuiv(map,values)
#define glGetPixelMapusv(map,values)                                                    _MGL_glFuncs.glGetPixelMapusv(map,values)
#define glGetPointerv(pname,params)                                           			_MGL_glFuncs.glGetPointerv(pname,params)
#define glGetPolygonStipple(mask)                                                      _MGL_glFuncs.glGetPolygonStipple(mask)
#define glGetString(name)                                                               _MGL_glFuncs.glGetString(name)
#define glGetTexEnvfv(target,pname,params)                                              _MGL_glFuncs.glGetTexEnvfv(target,pname,params)
#define glGetTexEnviv(target,pname,params)                                              _MGL_glFuncs.glGetTexEnviv(target,pname,params)
#define glGetTexGendv(coord,pname,params)                                               _MGL_glFuncs.glGetTexGendv(coord,pname,params)
#define glGetTexGenfv(coord,pname,params)                                               _MGL_glFuncs.glGetTexGenfv(coord,pname,params)
#define glGetTexGeniv(coord,pname,params)                                               _MGL_glFuncs.glGetTexGeniv(coord,pname,params)
#define glGetTexImage(target,level,format,type,pixels)                                  _MGL_glFuncs.glGetTexImage(target,level,format,type,pixels)
#define glGetTexLevelParameterfv(target,level,pname,params)                             _MGL_glFuncs.glGetTexLevelParameterfv(target,level,pname,params)
#define glGetTexLevelParameteriv(target,level,pname,params)                             _MGL_glFuncs.glGetTexLevelParameteriv(target,level,pname,params)
#define glGetTexParameterfv(target,pname,params)                                        _MGL_glFuncs.glGetTexParameterfv(target,pname,params)
#define glGetTexParameteriv(target,pname,params)                                        _MGL_glFuncs.glGetTexParameteriv(target,pname,params)
#define glHint(target,mode)                                                             _MGL_glFuncs.glHint(target,mode)
#define glIndexMask(mask)                                                               _MGL_glFuncs.glIndexMask(mask)
#define glIndexPointer(type,stride,pointer)                                             _MGL_glFuncs.glIndexPointer(type,stride,pointer)
#define glIndexd(c)                                                                     _MGL_glFuncs.glIndexd(c)
#define glIndexdv(c)                                                                    _MGL_glFuncs.glIndexdv(c)
#define glIndexf(c)                                                                     _MGL_glFuncs.glIndexf(c)
#define glIndexfv(c)                                                                    _MGL_glFuncs.glIndexfv(c)
#define glIndexi(c)                                                                     _MGL_glFuncs.glIndexi(c)
#define glIndexiv(c)                                                                    _MGL_glFuncs.glIndexiv(c)
#define glIndexs(c)                                                                     _MGL_glFuncs.glIndexs(c)
#define glIndexsv(c)                                                                    _MGL_glFuncs.glIndexsv(c)
#define glIndexub(c)                                                                    _MGL_glFuncs.glIndexub(c)
#define glIndexubv(c)                                                                   _MGL_glFuncs.glIndexubv(c)
#define glInitNames()                                                               _MGL_glFuncs.glInitNames()
#define glInterleavedArrays(format,stride,pointer)                                      _MGL_glFuncs.glInterleavedArrays(format,stride,pointer)
#define glIsEnabled(cap)                                                                _MGL_glFuncs.glIsEnabled(cap)
#define glIsList(list)                                                                  _MGL_glFuncs.glIsList(list)
#define glIsTexture(texture)                                                            _MGL_glFuncs.glIsTexture(texture)
#define glLightModelf(pname,param)                                                      _MGL_glFuncs.glLightModelf(pname,param)
#define glLightModelfv(pname,params)                                                    _MGL_glFuncs.glLightModelfv(pname,params)
#define glLightModeli(pname,param)                                                      _MGL_glFuncs.glLightModeli(pname,param)
#define glLightModeliv(pname,params)                                                    _MGL_glFuncs.glLightModeliv(pname,params)
#define glLightf(light,pname,param)                                                     _MGL_glFuncs.glLightf(light,pname,param)
#define glLightfv(light,pname,params)                                                   _MGL_glFuncs.glLightfv(light,pname,params)
#define glLighti(light,pname,param)                                                     _MGL_glFuncs.glLighti(light,pname,param)
#define glLightiv(light,pname,params)                                                   _MGL_glFuncs.glLightiv(light,pname,params)
#define glLineStipple(factor,pattern)                                                   _MGL_glFuncs.glLineStipple(factor,pattern)
#define glLineWidth(width)                                                              _MGL_glFuncs.glLineWidth(width)
#define glListBase(base)                                                                _MGL_glFuncs.glListBase(base)
#define glLoadIdentity()                                                            _MGL_glFuncs.glLoadIdentity()
#define glLoadMatrixd(m)                                                                _MGL_glFuncs.glLoadMatrixd(m)
#define glLoadMatrixf(m)                                                                _MGL_glFuncs.glLoadMatrixf(m)
#define glLoadName(name)                                                                _MGL_glFuncs.glLoadName(name)
#define glLogicOp(opcode)                                                               _MGL_glFuncs.glLogicOp(opcode)
#define glMap1d(target,u1,u2,stride,order,points)                                       _MGL_glFuncs.glMap1d(target,u1,u2,stride,order,points)
#define glMap1f(target,u1,u2,stride,order,points)                                       _MGL_glFuncs.glMap1f(target,u1,u2,stride,order,points)
#define glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)                _MGL_glFuncs.glMap2d(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)
#define glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)                _MGL_glFuncs.glMap2f(target,u1,u2,ustride,uorder,v1,v2,vstride,vorder,points)
#define glMapGrid1d(un,u1,u2)                                                           _MGL_glFuncs.glMapGrid1d(un,u1,u2)
#define glMapGrid1f(un,u1,u2)                                                           _MGL_glFuncs.glMapGrid1f(un,u1,u2)
#define glMapGrid2d(un,u1,u2,vn,v1,v2)                                                  _MGL_glFuncs.glMapGrid2d(un,u1,u2,vn,v1,v2)
#define glMapGrid2f(un,u1,u2,vn,v1,v2)                                                  _MGL_glFuncs.glMapGrid2f(un,u1,u2,vn,v1,v2)
#define glMaterialf(face,pname,param)                                                   _MGL_glFuncs.glMaterialf(face,pname,param)
#define glMaterialfv(face,pname,params)                                                 _MGL_glFuncs.glMaterialfv(face,pname,params)
#define glMateriali(face,pname,param)                                                   _MGL_glFuncs.glMateriali(face,pname,param)
#define glMaterialiv(face,pname,params)                                                 _MGL_glFuncs.glMaterialiv(face,pname,params)
#define glMatrixMode(mode)                                                              _MGL_glFuncs.glMatrixMode(mode)
#define glMultMatrixd(m)                                                                _MGL_glFuncs.glMultMatrixd(m)
#define glMultMatrixf(m)                                                                _MGL_glFuncs.glMultMatrixf(m)
#define glNewList(list,mode)                                                            _MGL_glFuncs.glNewList(list,mode)
#define glNormal3b(nx,ny,nz)                                                            _MGL_glFuncs.glNormal3b(nx,ny,nz)
#define glNormal3bv(v)                                                                  _MGL_glFuncs.glNormal3bv(v)
#define glNormal3d(nx,ny,nz)                                                            _MGL_glFuncs.glNormal3d(nx,ny,nz)
#define glNormal3dv(v)                                                                  _MGL_glFuncs.glNormal3dv(v)
#define glNormal3f(nx,ny,nz)                                                            _MGL_glFuncs.glNormal3f(nx,ny,nz)
#define glNormal3fv(v)                                                                  _MGL_glFuncs.glNormal3fv(v)
#define glNormal3i(nx,ny,nz)                                                            _MGL_glFuncs.glNormal3i(nx,ny,nz)
#define glNormal3iv(v)                                                                  _MGL_glFuncs.glNormal3iv(v)
#define glNormal3s(nx,ny,nz)                                                            _MGL_glFuncs.glNormal3s(nx,ny,nz)
#define glNormal3sv(v)                                                                  _MGL_glFuncs.glNormal3sv(v)
#define glNormalPointer(type,stride,pointer)                                            _MGL_glFuncs.glNormalPointer(type,stride,pointer)
#define glOrtho(left,right,bottom,top,zNear,zFar)                                       _MGL_glFuncs.glOrtho(left,right,bottom,top,zNear,zFar)
#define glPassThrough(token)                                                            _MGL_glFuncs.glPassThrough(token)
#define glPixelMapfv(map,mapsize,values)                                                _MGL_glFuncs.glPixelMapfv(map,mapsize,values)
#define glPixelMapuiv(map,mapsize,values)                                               _MGL_glFuncs.glPixelMapuiv(map,mapsize,values)
#define glPixelMapusv(map,mapsize,values)                                               _MGL_glFuncs.glPixelMapusv(map,mapsize,values)
#define glPixelStoref(pname,param)                                                      _MGL_glFuncs.glPixelStoref(pname,param)
#define glPixelStorei(pname,param)                                                      _MGL_glFuncs.glPixelStorei(pname,param)
#define glPixelTransferf(pname,param)                                                   _MGL_glFuncs.glPixelTransferf(pname,param)
#define glPixelTransferi(pname,param)                                                   _MGL_glFuncs.glPixelTransferi(pname,param)
#define glPixelZoom(xfactor,yfactor)                                                    _MGL_glFuncs.glPixelZoom(xfactor,yfactor)
#define glPointSize(size)                                                               _MGL_glFuncs.glPointSize(size)
#define glPolygonMode(face,mode)                                                        _MGL_glFuncs.glPolygonMode(face,mode)
#define glPolygonOffset(factor,units)                                                   _MGL_glFuncs.glPolygonOffset(factor,units)
#define glPolygonStipple(mask)                                                          _MGL_glFuncs.glPolygonStipple(mask)
#define glPopAttrib()                                                               _MGL_glFuncs.glPopAttrib()
#define glPopClientAttrib()                                                         _MGL_glFuncs.glPopClientAttrib()
#define glPopMatrix()                                                               _MGL_glFuncs.glPopMatrix()
#define glPopName()                                                                 _MGL_glFuncs.glPopName()
#define glPrioritizeTextures(n,textures,priorities)                    					_MGL_glFuncs.glPrioritizeTextures(n,textures,priorities)
#define glPushAttrib(mask)                                                              _MGL_glFuncs.glPushAttrib(mask)
#define glPushClientAttrib(mask)                                                        _MGL_glFuncs.glPushClientAttrib(mask)
#define glPushMatrix()                                                              _MGL_glFuncs.glPushMatrix()
#define glPushName(name)                                                                _MGL_glFuncs.glPushName(name)
#define glRasterPos2d(x,y)                                                              _MGL_glFuncs.glRasterPos2d(x,y)
#define glRasterPos2dv(v)                                                               _MGL_glFuncs.glRasterPos2dv(v)
#define glRasterPos2f(x,y)                                                              _MGL_glFuncs.glRasterPos2f(x,y)
#define glRasterPos2fv(v)                                                               _MGL_glFuncs.glRasterPos2fv(v)
#define glRasterPos2i(x,y)                                                              _MGL_glFuncs.glRasterPos2i(x,y)
#define glRasterPos2iv(v)                                                               _MGL_glFuncs.glRasterPos2iv(v)
#define glRasterPos2s(x,y)                                                              _MGL_glFuncs.glRasterPos2s(x,y)
#define glRasterPos2sv(v)                                                               _MGL_glFuncs.glRasterPos2sv(v)
#define glRasterPos3d(x,y,z)                                                            _MGL_glFuncs.glRasterPos3d(x,y,z)
#define glRasterPos3dv(v)                                                               _MGL_glFuncs.glRasterPos3dv(v)
#define glRasterPos3f(x,y,z)                                                            _MGL_glFuncs.glRasterPos3f(x,y,z)
#define glRasterPos3fv(v)                                                               _MGL_glFuncs.glRasterPos3fv(v)
#define glRasterPos3i(x,y,z)                                                            _MGL_glFuncs.glRasterPos3i(x,y,z)
#define glRasterPos3iv(v)                                                               _MGL_glFuncs.glRasterPos3iv(v)
#define glRasterPos3s(x,y,z)                                                            _MGL_glFuncs.glRasterPos3s(x,y,z)
#define glRasterPos3sv(v)                                                               _MGL_glFuncs.glRasterPos3sv(v)
#define glRasterPos4d(x,y,z,w)                                                          _MGL_glFuncs.glRasterPos4d(x,y,z,w)
#define glRasterPos4dv(v)                                                               _MGL_glFuncs.glRasterPos4dv(v)
#define glRasterPos4f(x,y,z,w)                                                          _MGL_glFuncs.glRasterPos4f(x,y,z,w)
#define glRasterPos4fv(v)                                                               _MGL_glFuncs.glRasterPos4fv(v)
#define glRasterPos4i(x,y,z,w)                                                          _MGL_glFuncs.glRasterPos4i(x,y,z,w)
#define glRasterPos4iv(v)                                                               _MGL_glFuncs.glRasterPos4iv(v)
#define glRasterPos4s(x,y,z,w)                                                          _MGL_glFuncs.glRasterPos4s(x,y,z,w)
#define glRasterPos4sv(v)                                                               _MGL_glFuncs.glRasterPos4sv(v)
#define glReadBuffer(mode)                                                              _MGL_glFuncs.glReadBuffer(mode)
#define glReadPixels(x,y,width,height,format,type,pixels)                               _MGL_glFuncs.glReadPixels(x,y,width,height,format,type,pixels)
#define glRectd(x1,y1,x2,y2)                                                            _MGL_glFuncs.glRectd(x1,y1,x2,y2)
#define glRectdv(v1,v2)                                                                 _MGL_glFuncs.glRectdv(v1,v2)
#define glRectf(x1,y1,x2,y2)                                                            _MGL_glFuncs.glRectf(x1,y1,x2,y2)
#define glRectfv(v1,v2)                                                                 _MGL_glFuncs.glRectfv(v1,v2)
#define glRecti(x1,y1,x2,y2)                                                            _MGL_glFuncs.glRecti(x1,y1,x2,y2)
#define glRectiv(v1,v2)                                                                 _MGL_glFuncs.glRectiv(v1,v2)
#define glRects(x1,y1,x2,y2)                                                            _MGL_glFuncs.glRects(x1,y1,x2,y2)
#define glRectsv(v1,v2)                                                                 _MGL_glFuncs.glRectsv(v1,v2)
#define glRenderMode(mode)                                                              _MGL_glFuncs.glRenderMode(mode)
#define glRotated(angle,x,y,z)                                                          _MGL_glFuncs.glRotated(angle,x,y,z)
#define glRotatef(angle,x,y,z)                                                          _MGL_glFuncs.glRotatef(angle,x,y,z)
#define glScaled(x,y,z)                                                                 _MGL_glFuncs.glScaled(x,y,z)
#define glScalef(x,y,z)                                                                 _MGL_glFuncs.glScalef(x,y,z)
#define glScissor(x,y,width,height)                                                     _MGL_glFuncs.glScissor(x,y,width,height)
#define glSelectBuffer(size,buffer)                                                     _MGL_glFuncs.glSelectBuffer(size,buffer)
#define glShadeModel(mode)                                                              _MGL_glFuncs.glShadeModel(mode)
#define glStencilFunc(func,ref,mask)                                                    _MGL_glFuncs.glStencilFunc(func,ref,mask)
#define glStencilMask(mask)                                                             _MGL_glFuncs.glStencilMask(mask)
#define glStencilOp(fail,zfail,zpass)                                                   _MGL_glFuncs.glStencilOp(fail,zfail,zpass)
#define glTexCoord1d(s)                                                                 _MGL_glFuncs.glTexCoord1d(s)
#define glTexCoord1dv(v)                                                                _MGL_glFuncs.glTexCoord1dv(v)
#define glTexCoord1f(s)                                                                 _MGL_glFuncs.glTexCoord1f(s)
#define glTexCoord1fv(v)                                                                _MGL_glFuncs.glTexCoord1fv(v)
#define glTexCoord1i(s)                                                                 _MGL_glFuncs.glTexCoord1i(s)
#define glTexCoord1iv(v)                                                                _MGL_glFuncs.glTexCoord1iv(v)
#define glTexCoord1s(s)                                                                 _MGL_glFuncs.glTexCoord1s(s)
#define glTexCoord1sv(v)                                                                _MGL_glFuncs.glTexCoord1sv(v)
#define glTexCoord2d(s,t)                                                               _MGL_glFuncs.glTexCoord2d(s,t)
#define glTexCoord2dv(v)                                                                _MGL_glFuncs.glTexCoord2dv(v)
#define glTexCoord2f(s,t)                                                               _MGL_glFuncs.glTexCoord2f(s,t)
#define glTexCoord2fv(v)                                                                _MGL_glFuncs.glTexCoord2fv(v)
#define glTexCoord2i(s,t)                                                               _MGL_glFuncs.glTexCoord2i(s,t)
#define glTexCoord2iv(v)                                                                _MGL_glFuncs.glTexCoord2iv(v)
#define glTexCoord2s(s,t)                                                               _MGL_glFuncs.glTexCoord2s(s,t)
#define glTexCoord2sv(v)                                                                _MGL_glFuncs.glTexCoord2sv(v)
#define glTexCoord3d(s,t,r)                                                             _MGL_glFuncs.glTexCoord3d(s,t,r)
#define glTexCoord3dv(v)                                                                _MGL_glFuncs.glTexCoord3dv(v)
#define glTexCoord3f(s,t,r)                                                             _MGL_glFuncs.glTexCoord3f(s,t,r)
#define glTexCoord3fv(v)                                                                _MGL_glFuncs.glTexCoord3fv(v)
#define glTexCoord3i(s,t,r)                                                             _MGL_glFuncs.glTexCoord3i(s,t,r)
#define glTexCoord3iv(v)                                                                _MGL_glFuncs.glTexCoord3iv(v)
#define glTexCoord3s(s,t,r)                                                             _MGL_glFuncs.glTexCoord3s(s,t,r)
#define glTexCoord3sv(v)                                                                _MGL_glFuncs.glTexCoord3sv(v)
#define glTexCoord4d(s,t,r,q)                                                           _MGL_glFuncs.glTexCoord4d(s,t,r,q)
#define glTexCoord4dv(v)                                                                _MGL_glFuncs.glTexCoord4dv(v)
#define glTexCoord4f(s,t,r,q)                                                           _MGL_glFuncs.glTexCoord4f(s,t,r,q)
#define glTexCoord4fv(v)                                                                _MGL_glFuncs.glTexCoord4fv(v)
#define glTexCoord4i(s,t,r,q)                                                           _MGL_glFuncs.glTexCoord4i(s,t,r,q)
#define glTexCoord4iv(v)                                                                _MGL_glFuncs.glTexCoord4iv(v)
#define glTexCoord4s(s,t,r,q)                                                           _MGL_glFuncs.glTexCoord4s(s,t,r,q)
#define glTexCoord4sv(v)                                                                _MGL_glFuncs.glTexCoord4sv(v)
#define glTexCoordPointer(size,type,stride,pointer)                                     _MGL_glFuncs.glTexCoordPointer(size,type,stride,pointer)
#define glTexEnvf(target,pname,param)                                                   _MGL_glFuncs.glTexEnvf(target,pname,param)
#define glTexEnvfv(target,pname,params)                                                 _MGL_glFuncs.glTexEnvfv(target,pname,params)
#define glTexEnvi(target,pname,param)                                                   _MGL_glFuncs.glTexEnvi(target,pname,param)
#define glTexEnviv(target,pname,params)                                                 _MGL_glFuncs.glTexEnviv(target,pname,params)
#define glTexGend(coord,pname,param)                                                    _MGL_glFuncs.glTexGend(coord,pname,param)
#define glTexGendv(coord,pname,params)                                                  _MGL_glFuncs.glTexGendv(coord,pname,params)
#define glTexGenf(coord,pname,param)                                                    _MGL_glFuncs.glTexGenf(coord,pname,param)
#define glTexGenfv(coord,pname,params)                                                  _MGL_glFuncs.glTexGenfv(coord,pname,params)
#define glTexGeni(coord,pname,param)                                                    _MGL_glFuncs.glTexGeni(coord,pname,param)
#define glTexGeniv(coord,pname,params)                                                  _MGL_glFuncs.glTexGeniv(coord,pname,params)
#define glTexImage1D(target,level,components,width,border,format,type,pixels)           _MGL_glFuncs.glTexImage1D(target,level,components,width,border,format,type,pixels)
#define glTexImage2D(target,level,components,width,height,border,format,type,pixels)    _MGL_glFuncs.glTexImage2D(target,level,components,width,height,border,format,type,pixels)
#define glTexParameterf(target,pname,param)                                             _MGL_glFuncs.glTexParameterf(target,pname,param)
#define glTexParameterfv(target,pname,params)                                           _MGL_glFuncs.glTexParameterfv(target,pname,params)
#define glTexParameteri(target,pname,param)                                             _MGL_glFuncs.glTexParameteri(target,pname,param)
#define glTexParameteriv(target,pname,params)                                           _MGL_glFuncs.glTexParameteriv(target,pname,params)
#define glTexSubImage1D(target,level,xoffset,width,format,type,pixels)                  _MGL_glFuncs.glTexSubImage1D(target,level,xoffset,width,format,type,pixels)
#define glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels)   _MGL_glFuncs.glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels)
#define glTranslated(x,y,z)                                                             _MGL_glFuncs.glTranslated(x,y,z)
#define glTranslatef(x,y,z)                                                             _MGL_glFuncs.glTranslatef(x,y,z)
#define glVertex2d(x,y)                                                                 _MGL_glFuncs.glVertex2d(x,y)
#define glVertex2dv(v)                                                                  _MGL_glFuncs.glVertex2dv(v)
#define glVertex2f(x,y)                                                                 _MGL_glFuncs.glVertex2f(x,y)
#define glVertex2fv(v)                                                                  _MGL_glFuncs.glVertex2fv(v)
#define glVertex2i(x,y)                                                                 _MGL_glFuncs.glVertex2i(x,y)
#define glVertex2iv(v)                                                                  _MGL_glFuncs.glVertex2iv(v)
#define glVertex2s(x,y)                                                                 _MGL_glFuncs.glVertex2s(x,y)
#define glVertex2sv(v)                                                                  _MGL_glFuncs.glVertex2sv(v)
#define glVertex3d(x,y,z)                                                               _MGL_glFuncs.glVertex3d(x,y,z)
#define glVertex3dv(v)                                                                  _MGL_glFuncs.glVertex3dv(v)
#define glVertex3f(x,y,z)                                                               _MGL_glFuncs.glVertex3f(x,y,z)
#define glVertex3fv(v)                                                                  _MGL_glFuncs.glVertex3fv(v)
#define glVertex3i(x,y,z)                                                               _MGL_glFuncs.glVertex3i(x,y,z)
#define glVertex3iv(v)                                                                  _MGL_glFuncs.glVertex3iv(v)
#define glVertex3s(x,y,z)                                                               _MGL_glFuncs.glVertex3s(x,y,z)
#define glVertex3sv(v)                                                                  _MGL_glFuncs.glVertex3sv(v)
#define glVertex4d(x,y,z,w)                                                             _MGL_glFuncs.glVertex4d(x,y,z,w)
#define glVertex4dv(v)                                                                  _MGL_glFuncs.glVertex4dv(v)
#define glVertex4f(x,y,z,w)                                                             _MGL_glFuncs.glVertex4f(x,y,z,w)
#define glVertex4fv(v)                                                                  _MGL_glFuncs.glVertex4fv(v)
#define glVertex4i(x,y,z,w)                                                             _MGL_glFuncs.glVertex4i(x,y,z,w)
#define glVertex4iv(v)                                                                  _MGL_glFuncs.glVertex4iv(v)
#define glVertex4s(x,y,z,w)                                                             _MGL_glFuncs.glVertex4s(x,y,z,w)
#define glVertex4sv(v)                                                                  _MGL_glFuncs.glVertex4sv(v)
#define glVertexPointer(size,type,stride,pointer)                                       _MGL_glFuncs.glVertexPointer(size,type,stride,pointer)
#define glViewport(x,y,width,height)                                                    _MGL_glFuncs.glViewport(x,y,width,height)

/* OpenGL 1.2 additional functions */
#define glDrawRangeElements(m,s,e,c,t,i)												_MGL_glFuncs.glDrawRangeElements(m,s,e,c,t,i)
#define glTexImage3D(t,l,i,w,h,d,b,f,t2,p)                                              _MGL_glFuncs.glTexImage3D(t,l,i,w,h,d,b,f,t2,p)
#define glTexSubImage3D(t,l,x,y,z,w,h,d,f,t2,p)                                         _MGL_glFuncs.glTexSubImage3D(t,l,x,y,z,w,h,d,f,t2,p)
#define glCopyTexSubImage3D(t,l,x,y,z,x2,y2,w,h)                                        _MGL_glFuncs.glCopyTexSubImage3D(t,l,x,y,z,x2,y2,w,h)
#endif

extern MGL_glFuncs  _VARAPI _MGL_glFuncs;

#ifdef __cplusplus
}
#endif

#endif /* __gl_mgl_h_ */
