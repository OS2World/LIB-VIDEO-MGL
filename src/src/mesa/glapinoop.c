/* $Id: glapinoop.c,v 1.1 1999/11/11 01:22:26 brianp Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.3
 *
 * Copyright (C) 1999  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <stdio.h>
#include "glapi.h"


/*
 * No-op API functions.  These are called when there is no current context.
 * Do nothing except perhaps print a warning message when in debug mode.
 */

static GLboolean DebugMode = GL_TRUE;

#define WARNING(s)  if (DebugMode) fprintf(stderr, "Mesa User Error: calling %s without a current context\n", s);


static void NoOpAccum(GLenum op, GLfloat value)
{
   (void) op;
   (void) value;
   WARNING("glAccum");
}

static void NoOpAlphaFunc(GLenum func, GLclampf ref)
{
   (void) func;
   (void) ref;
   WARNING("glAlphaFunc");
}

static void NoOpBegin(GLenum mode)
{
   (void) mode;
   WARNING("glBegin");
}

static void NoOpBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
   (void) width;
   (void) height;
   (void) xorig;
   (void) yorig;
   (void) xmove;
   (void) ymove;
   (void) bitmap;
   WARNING("glBitmap");
}

static void NoOpBlendFunc(GLenum sfactor, GLenum dfactor)
{
   (void) sfactor;
   (void) dfactor;
   WARNING("glBlendFunc");
}

static void NoOpCallList(GLuint list)
{
   (void) list;
   WARNING("glCallList");
}

static void NoOpCallLists(GLsizei n, GLenum type, const GLvoid *lists)
{
   (void) n;
   (void) type;
   (void) lists;
   WARNING("glCallLists");
}

static void NoOpClear(GLbitfield mask)
{
   (void) mask;
   WARNING("glClear");
}

static void NoOpClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glClearAccum");
}

static void NoOpClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glClearColor");
}

static void NoOpClearDepth(GLclampd depth)
{
   (void) depth;
   WARNING("gl");
}

static void NoOpClearIndex(GLfloat c)
{
   (void) c;
   WARNING("glClearIndex");
}

static void NoOpClearStencil(GLint s)
{
   (void) s;
   WARNING("glStencil");
}

static void NoOpClipPlane(GLenum plane, const GLdouble *equation)
{
   (void) plane;
   (void) equation;
   WARNING("glClipPlane");
}

static void NoOpColor3b(GLbyte red, GLbyte green, GLbyte blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3b");
}

static void NoOpColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3d");
}

static void NoOpColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3f");
}

static void NoOpColor3i(GLint red, GLint green, GLint blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3i");
}

static void NoOpColor3s(GLshort red, GLshort green, GLshort blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3s");
}

static void NoOpColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3ub");
}

static void NoOpColor3ui(GLuint red, GLuint green, GLuint blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3ui");
}

static void NoOpColor3us(GLushort red, GLushort green, GLushort blue)
{
   (void) red;
   (void) green;
   (void) blue;
   WARNING("glColor3us");
}

static void NoOpColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4b");
}

static void NoOpColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4d");
}

static void NoOpColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4f");
}

static void NoOpColor4i(GLint red, GLint green, GLint blue, GLint alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4i");
}

static void NoOpColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4s");
}

static void NoOpColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4ub");
}

static void NoOpColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4ui");
}

static void NoOpColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColor4us");
}

static void NoOpColor3bv(const GLbyte *v)
{
   (void) v;
   WARNING("glColor3bv");
}

static void NoOpColor3dv(const GLdouble *v)
{
   (void) v;
   WARNING("glColor3dv");
}

static void NoOpColor3fv(const GLfloat *v)
{
   (void) v;
   WARNING("glColor3fv");
}

static void NoOpColor3iv(const GLint *v)
{
   (void) v;
   WARNING("glColor3iv");
}

static void NoOpColor3sv(const GLshort *v)
{
   (void) v;
   WARNING("glColor3sv");
}

static void NoOpColor3ubv(const GLubyte *v)
{
   (void) v;
   WARNING("glColor3ubv");
}

static void NoOpColor3uiv(const GLuint *v)
{
   (void) v;
   WARNING("glColor3uiv");
}

static void NoOpColor3usv(const GLushort *v)
{
   (void) v;
   WARNING("glColor3usv");
}

static void NoOpColor4bv(const GLbyte *v)
{
   (void) v;
   WARNING("glColor4bv");
}

static void NoOpColor4dv(const GLdouble *v)
{
   (void) v;
   WARNING("glColor4dv");
}

static void NoOpColor4fv(const GLfloat *v)
{
   (void) v;
   WARNING("glColor4fv");
}

static void NoOpColor4iv(const GLint *v)
{
   (void) v;
   WARNING("glColor4iv");
}

static void NoOpColor4sv(const GLshort *v)
{
   (void) v;
   WARNING("glColor4sv");
}

static void NoOpColor4ubv(const GLubyte *v)
{
   (void) v;
   WARNING("glColor4ubv");
}

static void NoOpColor4uiv(const GLuint *v)
{
   (void) v;
   WARNING("glColor4uiv");
}

static void NoOpColor4usv(const GLushort *v)
{
   (void) v;
   WARNING("glColor4usv");
}

static void NoOpColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
   (void) red;
   (void) green;
   (void) blue;
   (void) alpha;
   WARNING("glColorMask");
}

static void NoOpColorMaterial(GLenum face, GLenum mode)
{
   (void) face;
   (void) mode;
   WARNING("glColorMaterial");
}

static void NoOpCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   (void) type;
   WARNING("glCopyPixels");
}

static void NoOpCullFace(GLenum mode)
{
   (void) mode;
   WARNING("glCullFace");
}

static void NoOpDepthFunc(GLenum func)
{
   (void) func;
   WARNING("glDepthFunc");
}

static void NoOpDepthMask(GLboolean flag)
{
   (void) flag;
   WARNING("glDepthMask");
}

static void NoOpDepthRange(GLclampd nearVal, GLclampd farVal)
{
   (void) nearVal;
   (void) farVal;
   WARNING("glDepthRange");
}

static void NoOpDeleteLists(GLuint list, GLsizei range)
{
   (void) list;
   (void) range;
   WARNING("glDeleteLists");
}

static void NoOpDisable(GLenum cap)
{
   (void) cap;
   WARNING("glDisable");
}

static void NoOpDisableClientState(GLenum cap)
{
   (void) cap;
   WARNING("glDisableClientState");
}

static void NoOpDrawBuffer(GLenum mode)
{
   (void) mode;
   WARNING("glDrawBuffer");
}

static void NoOpDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) width;
   (void) height;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glDrawPixels");
}

static void NoOpEnable(GLenum mode)
{
   (void) mode;
   WARNING("glEnable");
}

static void NoOpEnd(void)
{
   WARNING("glEnd");
}

static void NoOpEndList(void)
{
   WARNING("glEndList");
}

static void NoOpEvalCoord1d(GLdouble u)
{
   (void) u;
   WARNING("glEvalCoord1d");
}

static void NoOpEvalCoord1f(GLfloat u)
{
   (void) u;
   WARNING("glEvalCoord1f");
}

static void NoOpEvalCoord1dv(const GLdouble *u)
{
   (void) u;
   WARNING("glEvalCoord1dv");
}

static void NoOpEvalCoord1fv(const GLfloat *u)
{
   (void) u;
   WARNING("glEvalCoord1fv");
}

static void NoOpEvalCoord2d(GLdouble u, GLdouble v)
{
   (void) u;
   (void) v;
   WARNING("glEvalCoord2d");
}

static void NoOpEvalCoord2f(GLfloat u, GLfloat v)
{
   (void) u;
   (void) v;
   WARNING("glEvalCoord2f");
}

static void NoOpEvalCoord2dv(const GLdouble *u)
{
   (void) u;
   WARNING("glEvalCoord2dv");
}

static void NoOpEvalCoord2fv(const GLfloat *u)
{
   (void) u;
   WARNING("glEvalCoord2fv");
}

static void NoOpEvalPoint1(GLint i)
{
   (void) i;
   WARNING("glEvalPoint1");
}

static void NoOpEvalPoint2(GLint i, GLint j)
{
   (void) i;
   (void) j;
   WARNING("glEvalPoint2");
}

static void NoOpEvalMesh1(GLenum mode, GLint i1, GLint i2)
{
   (void) mode;
   (void) i1;
   (void) i2;
   WARNING("glEvalMesh1");
}

static void NoOpEdgeFlag(GLboolean flag)
{
   (void) flag;
   WARNING("glEdgeFlag");
}

static void NoOpEdgeFlagv(const GLboolean *flag)
{
   (void) flag;
   WARNING("glEdgeFlagv");
}

static void NoOpEdgeFlagPointer(GLsizei stride, const GLvoid *ptr)
{
   (void) stride;
   (void) ptr;
   WARNING("glEdgeFlagPointer");
}

static void NoOpEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
   (void) mode;
   (void) i1;
   (void) i2;
   (void) j1;
   (void) j2;
   WARNING("glEvalMesh2");
}

static void NoOpFeedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer)
{
   (void) size;
   (void) type;
   (void) buffer;
   WARNING("glFeedbackBuffer");
}

static void NoOpFinish(void)
{
   WARNING("glFinish");
}

static void NoOpFlush(void)
{
   WARNING("glFlush");
}

static void NoOpFogf(GLenum pname, GLfloat param)
{
   (void) pname;
   (void) param;
   WARNING("glFogf");
}

static void NoOpFogi(GLenum pname, GLint param)
{
   (void) pname;
   (void) param;
   WARNING("glFogi");
}

static void NoOpFogfv(GLenum pname, const GLfloat *params)
{
   (void) pname;
   (void) params;
   WARNING("glFogfv");
}

static void NoOpFogiv(GLenum pname, const GLint *params)
{
   (void) pname;
   (void) params;
   WARNING("glFogiv");
}

static void NoOpFrontFace(GLenum mode)
{
   (void) mode;
   WARNING("glFrontFace");
}

static void NoOpFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearval, GLdouble farval)
{
   (void) left;
   (void) right;
   (void) bottom;
   (void) top;
   (void) nearval;
   (void) farval;
   WARNING("glFrustum");
}

GLuint NoOpGenLists(GLsizei range)
{
   (void) range;
   WARNING("glGenLists");
   return 0;
}

static void NoOpGetBooleanv(GLenum pname, GLboolean *params)
{
   (void) pname;
   (void) params;
   WARNING("glGetBooleanv");
}

static void NoOpGetClipPlane(GLenum plane, GLdouble *equation)
{
   (void) plane;
   (void) equation;
   WARNING("glGetClipPlane");
}

static void NoOpGetDoublev(GLenum pname, GLdouble *params)
{
   (void) pname;
   (void) params;
   WARNING("glGetDoublev");
}

static GLenum NoOpGetError(void)
{
   WARNING("glGetError");
   return (GLenum) 0;
}

static void NoOpGetFloatv(GLenum pname, GLfloat *params)
{
   (void) pname;
   (void) params;
   WARNING("glGetFloatv");
}

static void NoOpGetIntegerv(GLenum pname, GLint *params)
{
   (void) pname;
   (void) params;
   WARNING("glGetIntegerv");
}

static void NoOpGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
   (void) light;
   (void) pname;
   (void) params;
   WARNING("glGetLightfv");
}

static void NoOpGetLightiv(GLenum light, GLenum pname, GLint *params)
{
   (void) light;
   (void) pname;
   (void) params;
   WARNING("glGetLightiv");
}

static void NoOpGetMapdv(GLenum target, GLenum query, GLdouble *v)
{
   (void) target;
   (void) query;
   (void) v;
   WARNING("glGetMapdv");
}

static void NoOpGetMapfv(GLenum target, GLenum query, GLfloat *v)
{
   (void) target;
   (void) query;
   (void) v;
   WARNING("glGetMapfv");
}

static void NoOpGetMapiv(GLenum target, GLenum query, GLint *v)
{
   (void) target;
   (void) query;
   (void) v;
   WARNING("glGetMapiv");
}

static void NoOpGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
   (void) face;
   (void) pname;
   (void) params;
   WARNING("glGetMaterialfv");
}

static void NoOpGetMaterialiv(GLenum face, GLenum pname, GLint *params)
{
   (void) face;
   (void) pname;
   (void) params;
   WARNING("glGetMaterialiv");
}

static void NoOpGetPixelMapfv(GLenum map, GLfloat *values)
{
   (void) map;
   (void) values;
   WARNING("glGetPixelMapfv");
}

static void NoOpGetPixelMapuiv(GLenum map, GLuint *values)
{
   (void) map;
   (void) values;
   WARNING("glGetPixelMapuiv");
}

static void NoOpGetPixelMapusv(GLenum map, GLushort *values)
{
   (void) map;
   (void) values;
   WARNING("glGetPixelMapusv");
}

static void NoOpGetPolygonStipple(GLubyte *mask)
{
   (void) mask;
   WARNING("glGetPolygonStipple");
}

const GLubyte * NoOpGetString(GLenum name)
{
   (void) name;
   WARNING("glGetString");
   return NULL;
}

static void NoOpGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetTexEnvfv");
}

static void NoOpGetTexEnviv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetTexEnviv");
}

static void NoOpGetTexGeniv(GLenum coord, GLenum pname, GLint *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glGetTexGeniv");
}

static void NoOpGetTexGendv(GLenum coord, GLenum pname, GLdouble *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glGetTexGendv");
}

static void NoOpGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glGetTexGenfv");
}

static void NoOpGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glGetTexImage");
}

static void NoOpGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) level;
   (void) pname;
   (void) params;
   WARNING("glGetTexLevelParameterfv");
}

static void NoOpGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
{
   (void) target;
   (void) level;
   (void) pname;
   (void) params;
   WARNING("glGetTexLevelParameteriv");
}

static void NoOpGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetTexParameterfv");
}

static void NoOpGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetTexParameteriv");
}

static void NoOpHint(GLenum target, GLenum mode)
{
   (void) target;
   (void) mode;
   WARNING("glHint");
}

static void NoOpIndexd(GLdouble c)
{
   (void) c;
   WARNING("glIndexd");
}

static void NoOpIndexdv(const GLdouble *c)
{
   (void) c;
   WARNING("glIndexdv");
}

static void NoOpIndexf(GLfloat c)
{
   (void) c;
   WARNING("glIndexf");
}

static void NoOpIndexfv(const GLfloat *c)
{
   (void) c;
   WARNING("glIndexfv");
}

static void NoOpIndexi(GLint c)
{
   (void) c;
   WARNING("glIndexi");
}

static void NoOpIndexiv(const GLint *c)
{
   (void) c;
   WARNING("glIndexiv");
}

static void NoOpIndexs(GLshort c)
{
   (void) c;
   WARNING("glIndexs");
}

static void NoOpIndexsv(const GLshort *c)
{
   (void) c;
   WARNING("glIndexsv");
}

static void NoOpIndexMask(GLuint mask)
{
   (void) mask;
   WARNING("glIndexMask");
}

static void NoOpInitNames(void)
{
   WARNING("glInitNames");
}

static GLboolean NoOpIsList(GLuint list)
{
   (void) list;
   WARNING("glIsList");
   return GL_FALSE;
}

static GLboolean NoOpIsTexture(GLuint texture)
{
   (void) texture;
   WARNING("glIsTexture");
   return GL_FALSE;
}

static void NoOpLightf(GLenum light, GLenum pname, GLfloat param)
{
   (void) light;
   (void) pname;
   (void) param;
   WARNING("glLightf");
}

static void NoOpLighti(GLenum light, GLenum pname, GLint param)
{
   (void) light;
   (void) pname;
   (void) param;
   WARNING("glLighti");
}

static void NoOpLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
   (void) light;
   (void) pname;
   (void) params;
   WARNING("glLightfv");
}

static void NoOpLightiv(GLenum light, GLenum pname, const GLint *params)
{
   (void) light;
   (void) pname;
   (void) params;
   WARNING("glLightiv");
}

static void NoOpLightModelf(GLenum pname, GLfloat param)
{
   (void) pname;
   (void) param;
   WARNING("glLightModelf");
}

static void NoOpLightModeli(GLenum pname, GLint param)
{
   (void) pname;
   (void) param;
   WARNING("glLightModeli");
}

static void NoOpLightModelfv(GLenum pname, const GLfloat *params)
{
   (void) pname;
   (void) params;
   WARNING("glLightModelfv");
}

static void NoOpLightModeliv(GLenum pname, const GLint *params)
{
   (void) pname;
   (void) params;
   WARNING("glLightModeliv");
}

static void NoOpLineWidth(GLfloat width)
{
   (void) width;
   WARNING("glLineWidth");
}

static void NoOpLineStipple(GLint factor, GLushort pattern)
{
   (void) factor;
   (void) pattern;
   WARNING("glLineStipple");
}

static void NoOpListBase(GLuint base)
{
   (void) base;
   WARNING("glListBase");
}

static void NoOpLoadIdentity(void)
{
   WARNING("glLoadIdentity");
}

static void NoOpLoadMatrixd(const GLdouble *m)
{
   (void) m;
   WARNING("glLoadMatrixd");
}

static void NoOpLoadMatrixf(const GLfloat *m)
{
   (void) m;
   WARNING("glLoadMatrixf");
}

static void NoOpLoadName(GLuint name)
{
   (void) name;
   WARNING("glLoadName");
}

static void NoOpLogicOp(GLenum opcode)
{
   (void) opcode;
   WARNING("glLogicOp");
}

static void NoOpMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)
{
   (void) target;
   (void) u1;
   (void) u2;
   (void) stride;
   (void) order;
   (void) points;
   WARNING("glMap1d");
}

static void NoOpMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
   (void) target;
   (void) u1;
   (void) u2;
   (void) stride;
   (void) order;
   (void) points;
   WARNING("glMap1f");
}

static void NoOpMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)
{
   (void) target;
   (void) u1;
   (void) u2;
   (void) ustride;
   (void) uorder;
   (void) v1;
   (void) v2;
   (void) vstride;
   (void) vorder;
   (void) points;
   WARNING("glMap2d");
}

static void NoOpMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)
{
   (void) target;
   (void) u1;
   (void) u2;
   (void) ustride;
   (void) uorder;
   (void) v1;
   (void) v2;
   (void) vstride;
   (void) vorder;
   (void) points;
   WARNING("glMap2f");
}

static void NoOpMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
{
   (void) un;
   (void) u1;
   (void) u2;
   WARNING("glMapGrid1d");
}

static void NoOpMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
{
   (void) un;
   (void) u1;
   (void) u2;
   WARNING("glMapGrid1f");
}

static void NoOpMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
   (void) un;
   (void) u1;
   (void) u2;
   (void) vn;
   (void) v1;
   (void) v2;
   WARNING("glMapGrid2d");
}

static void NoOpMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
   (void) un;
   (void) u1;
   (void) u2;
   (void) vn;
   (void) v1;
   (void) v2;
   WARNING("glMapGrid2f");
}

static void NoOpMaterialf(GLenum face, GLenum pname, GLfloat param)
{
   (void) face;
   (void) pname;
   (void) param;
   WARNING("glMaterialf");
}

static void NoOpMateriali(GLenum face, GLenum pname, GLint param)
{
   (void) face;
   (void) pname;
   (void) param;
   WARNING("glMateriali");
}

static void NoOpMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
   (void) face;
   (void) pname;
   (void) params;
   WARNING("glMaterialfv");
}

static void NoOpMaterialiv(GLenum face, GLenum pname, const GLint *params)
{
   (void) face;
   (void) pname;
   (void) params;
   WARNING("glMaterialiv");
}

static void NoOpMatrixMode(GLenum mode)
{
   (void) mode;
   WARNING("glMatrixMode");
}

static void NoOpMultMatrixd(const GLdouble *m)
{
   (void) m;
   WARNING("glMultMatrixd");
}

static void NoOpMultMatrixf(const GLfloat *m)
{
   (void) m;
   WARNING("glMultMatrixf");
}

static void NoOpNewList(GLuint list, GLenum mode)
{
   (void) list;
   (void) mode;
   WARNING("glNewList");
}

static void NoOpNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
{
   (void) nx;
   (void) ny;
   (void) nz;
   WARNING("glNormal3b");
}

static void NoOpNormal3bv(const GLbyte *v)
{
   (void) v;
   WARNING("glNormal3bv");
}

static void NoOpNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
   (void) nx;
   (void) ny;
   (void) nz;
   WARNING("glNormal3d");
}

static void NoOpNormal3dv(const GLdouble *v)
{
   (void) v;
   WARNING("glNormal3dv");
}

static void NoOpNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
   (void) nx;
   (void) ny;
   (void) nz;
   WARNING("glNormal3f");
}

static void NoOpNormal3fv(const GLfloat *v)
{
   (void) v;
   WARNING("glNormal3fv");
}

static void NoOpNormal3i(GLint nx, GLint ny, GLint nz)
{
   (void) nx;
   (void) ny;
   (void) nz;
   WARNING("glNormal3i");
}

static void NoOpNormal3iv(const GLint *v)
{
   (void) v;
   WARNING("glNormal3iv");
}

static void NoOpNormal3s(GLshort nx, GLshort ny, GLshort nz)
{
   (void) nx;
   (void) ny;
   (void) nz;
   WARNING("glNormal3s");
}

static void NoOpNormal3sv(const GLshort *v)
{
   (void) v;
   WARNING("glNormal3sv");
}

static void NoOpOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearval, GLdouble farval)
{
   (void) left;
   (void) right;
   (void) bottom;
   (void) top;
   (void) nearval;
   (void) farval;
   WARNING("glOrtho");
}

static void NoOpPassThrough(GLfloat token)
{
   (void) token;
   WARNING("glPassThrough");
}

static void NoOpPixelMapfv(GLenum map, GLint mapsize, const GLfloat *values)
{
   (void) map;
   (void) mapsize;
   (void) values;
   WARNING("glPixelMapfv");
}

static void NoOpPixelMapuiv(GLenum map, GLint mapsize, const GLuint *values)
{
   (void) map;
   (void) mapsize;
   (void) values;
   WARNING("glPixelMapuiv");
}

static void NoOpPixelMapusv(GLenum map, GLint mapsize, const GLushort *values)
{
   (void) map;
   (void) mapsize;
   (void) values;
   WARNING("glPixelMapusv");
}

static void NoOpPixelStoref(GLenum pname, GLfloat param)
{
   (void) pname;
   (void) param;
   WARNING("glPixelStoref");
}

static void NoOpPixelStorei(GLenum pname, GLint param)
{
   (void) pname;
   (void) param;
   WARNING("glPixelStorei");
}

static void NoOpPixelTransferf(GLenum pname, GLfloat param)
{
   (void) pname;
   (void) param;
   WARNING("glPixelTransferf");
}

static void NoOpPixelTransferi(GLenum pname, GLint param)
{
   (void) pname;
   (void) param;
   WARNING("glPixelTransferi");
}

static void NoOpPixelZoom(GLfloat xfactor, GLfloat yfactor)
{
   (void) xfactor;
   (void) yfactor;
   WARNING("glPixelZoom");
}

static void NoOpPointSize(GLfloat size)
{
   (void) size;
   WARNING("glPointSize");
}

static void NoOpPolygonMode(GLenum face, GLenum mode)
{
   (void) face;
   (void) mode;
   WARNING("glPolygonMode");
}

static void NoOpPolygonOffset(GLfloat factor, GLfloat units)
{
   (void) factor;
   (void) units;
   WARNING("glPolygonOffset");
}

static void NoOpPolygonStipple(const GLubyte *pattern)
{
   (void) pattern;
   WARNING("glPolygonStipple");
}

static void NoOpPopAttrib(void)
{
   WARNING("glPopAttrib");
}

static void NoOpPopMatrix(void)
{
   WARNING("glPopMatrix");
}

static void NoOpPopName(void)
{
   WARNING("glPopName");
}

static void NoOpPushMatrix(void)
{
   WARNING("glPushMatrix");
}

static void NoOpRasterPos2d(GLdouble x, GLdouble y)
{
   (void) x;
   (void) y;
   WARNING("glRasterPos2d");
}

static void NoOpRasterPos2f(GLfloat x, GLfloat y)
{
   (void) x;
   (void) y;
   WARNING("glRasterPos2f");
}

static void NoOpRasterPos2i(GLint x, GLint y)
{
   (void) x;
   (void) y;
   WARNING("glRasterPos2i");
}

static void NoOpRasterPos2s(GLshort x, GLshort y)
{
   (void) x;
   (void) y;
   WARNING("glRasterPos2s");
}

static void NoOpRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRasterPos3d");
}

static void NoOpRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRasterPos3f");
}

static void NoOpRasterPos3i(GLint x, GLint y, GLint z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRasterPos3i");
}

static void NoOpRasterPos3s(GLshort x, GLshort y, GLshort z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRasterPos3s");
}

static void NoOpRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glRasterPos4d");
}

static void NoOpRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glRasterPos4f");
}

static void NoOpRasterPos4i(GLint x, GLint y, GLint z, GLint w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glRasterPos4i");
}

static void NoOpRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glRasterPos4s");
}

static void NoOpRasterPos2dv(const GLdouble *v)
{
   (void) v;
   WARNING("glRasterPos2dv");
}

static void NoOpRasterPos2fv(const GLfloat *v)
{
   (void) v;
   WARNING("glRasterPos2fv");
}

static void NoOpRasterPos2iv(const GLint *v)
{
   (void) v;
   WARNING("glRasterPos2iv");
}

static void NoOpRasterPos2sv(const GLshort *v)
{
   (void) v;
   WARNING("glRasterPos2sv");
}

static void NoOpRasterPos3dv(const GLdouble *v)
{
   (void) v;
   WARNING("glRasterPos3dv");
}

static void NoOpRasterPos3fv(const GLfloat *v)
{
   (void) v;
   WARNING("glRasterPos3fv");
}

static void NoOpRasterPos3iv(const GLint *v)
{
   (void) v;
   WARNING("glRasterPos3iv");
}

static void NoOpRasterPos3sv(const GLshort *v)
{
   (void) v;
   WARNING("glRasterPos3sv");
}

static void NoOpRasterPos4dv(const GLdouble *v)
{
   (void) v;
   WARNING("glRasterPos4dv");
}

static void NoOpRasterPos4fv(const GLfloat *v)
{
   (void) v;
   WARNING("glRasterPos4fv");
}

static void NoOpRasterPos4iv(const GLint *v)
{
   (void) v;
   WARNING("glRasterPos4iv");
}

static void NoOpRasterPos4sv(const GLshort *v)
{
   (void) v;
   WARNING("glRasterPos4sv");
}

static void NoOpReadBuffer(GLenum mode)
{ 
   (void) mode;
   WARNING("glReadBuffer");
}

static void NoOpReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glReadPixels");
}

static void NoOpRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
   (void) x1;
   (void) y1;
   (void) x2;
   (void) y2;
   WARNING("glRectd");
}

static void NoOpRectdv(const GLdouble *v1, const GLdouble *v2)
{
   (void) v1;
   (void) v2;
   WARNING("glRectdv");
}

static void NoOpRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
   (void) x1;
   (void) y1;
   (void) x2;
   (void) y2;
   WARNING("glRectf");
}

static void NoOpRectfv(const GLfloat *v1, const GLfloat *v2)
{
   (void) v1;
   (void) v2;
   WARNING("glRectfv");
}

static void NoOpRecti(GLint x1, GLint y1, GLint x2, GLint y2)
{
   (void) x1;
   (void) y1;
   (void) x2;
   (void) y2;
   WARNING("glRecti");
}

static void NoOpRectiv(const GLint *v1, const GLint *v2)
{
   (void) v1;
   (void) v2;
   WARNING("glRectiv");
}

static void NoOpRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
   (void) x1;
   (void) y1;
   (void) x2;
   (void) y2;
   WARNING("glRects");
}

static void NoOpRectsv(const GLshort *v1, const GLshort *v2)
{
   (void) v1;
   (void) v2;
   WARNING("glRectsv");
}

static void NoOpScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   WARNING("glScissor");
}

static GLboolean NoOpIsEnabled(GLenum cap)
{
   (void) cap;
   WARNING("glIsEnabled");
   return GL_FALSE;
}

static void NoOpPushAttrib(GLbitfield mask)
{
   (void) mask;
   WARNING("glPushAttrib");
}

static void NoOpPushName(GLuint name)
{
   (void) name;
   WARNING("glPushName");
}

static GLint NoOpRenderMode(GLenum mode)
{
   (void) mode;
   WARNING("glRenderMode");
   return 0;
}

static void NoOpRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
   (void) angle;
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRotated");
}

static void NoOpRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   (void) angle;
   (void) x;
   (void) y;
   (void) z;
   WARNING("glRotatef");
}

static void NoOpSelectBuffer(GLsizei size, GLuint *buffer)
{
   (void) size;
   (void) buffer;
   WARNING("glSelectBuffer");
}

static void NoOpScaled(GLdouble x, GLdouble y, GLdouble z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glScaled");
}

static void NoOpScalef(GLfloat x, GLfloat y, GLfloat z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glScalef");
}

static void NoOpShadeModel(GLenum mode)
{
   (void) mode;
   WARNING("glShadeModel");
}

static void NoOpStencilFunc(GLenum func, GLint ref, GLuint mask)
{
   (void) func;
   (void) ref;
   (void) mask;
   WARNING("glStencilFunc");
}

static void NoOpStencilMask(GLuint mask)
{
   (void) mask;
   WARNING("glStencilMask");
}

static void NoOpStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
   (void) fail;
   (void) zfail;
   (void) zpass;
   WARNING("glStencilOp");
}

static void NoOpTexCoord1d(GLdouble s)
{
   (void) s;
   WARNING("glTexCoord1d");
}

static void NoOpTexCoord1dv(const GLdouble *v)
{
   (void) v;
   WARNING("glTexCoord1dv");
}

static void NoOpTexCoord1f(GLfloat s)
{
   (void) s;
   WARNING("glTexCoord1f");
}

static void NoOpTexCoord1fv(const GLfloat *v)
{
   (void) v;
   WARNING("glTexCoord1fv");
}

static void NoOpTexCoord1i(GLint s)
{
   (void) s;
   WARNING("glTexCoord1i");
}

static void NoOpTexCoord1iv(const GLint *v)
{
   (void) v;
   WARNING("glTexCoord1iv");
}

static void NoOpTexCoord1s(GLshort s)
{
   (void) s;
   WARNING("glTexCoord1s");
}

static void NoOpTexCoord1sv(const GLshort *v)
{
   (void) v;
   WARNING("glTexCoord1sv");
}

static void NoOpTexCoord2d(GLdouble s, GLdouble t)
{
   (void) s;
   (void) t;
   WARNING("glTexCoord2d");
}

static void NoOpTexCoord2dv(const GLdouble *v)
{
   (void) v;
   WARNING("glTexCoord2dv");
}

static void NoOpTexCoord2f(GLfloat s, GLfloat t)
{
   (void) s;
   (void) t;
   WARNING("glTexCoord2f");
}

static void NoOpTexCoord2fv(const GLfloat *v)
{
   (void) v;
   WARNING("glTexCoord2fv");
}

static void NoOpTexCoord2s(GLshort s, GLshort t)
{
   (void) s;
   (void) t;
   WARNING("glTexCoord2s");
}

static void NoOpTexCoord2sv(const GLshort *v)
{
   (void) v;
   WARNING("glTexCoord2sv");
}

static void NoOpTexCoord2i(GLint s, GLint t)
{
   (void) s;
   (void) t;
   WARNING("glTexCoord2i");
}

static void NoOpTexCoord2iv(const GLint *v)
{
   (void) v;
   WARNING("glTexCoord2iv");
}

static void NoOpTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
{
   (void) s;
   (void) t;
   (void) r;
   WARNING("glTexCoord3d");
}

static void NoOpTexCoord3dv(const GLdouble *v)
{
   (void) v;
   WARNING("glTexCoord3dv");
}

static void NoOpTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
{
   (void) s;
   (void) t;
   (void) r;
   WARNING("glTexCoord3f");
}

static void NoOpTexCoord3fv(const GLfloat *v)
{
   (void) v;
   WARNING("glTexCoord3fv");
}

static void NoOpTexCoord3i(GLint s, GLint t, GLint r)
{
   (void) s;
   (void) t;
   (void) r;
   WARNING("glTexCoord3i");
}

static void NoOpTexCoord3iv(const GLint *v)
{
   (void) v;
   WARNING("glTexCoord3iv");
}

static void NoOpTexCoord3s(GLshort s, GLshort t, GLshort r)
{
   (void) s;
   (void) t;
   (void) r;
   WARNING("glTexCoord3s");
}

static void NoOpTexCoord3sv(const GLshort *v)
{
   (void) v;
   WARNING("glTexCoord3sv");
}

static void NoOpTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
   (void) s;
   (void) t;
   (void) r;
   WARNING("glTexCoord4d");
}

static void NoOpTexCoord4dv(const GLdouble *v)
{
   (void) v;
   WARNING("glTexCoord4dv");
}

static void NoOpTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glTexCoord4f");
}

static void NoOpTexCoord4fv(const GLfloat *v)
{
   (void) v;
   WARNING("glTexCoord4fv");
}

static void NoOpTexCoord4i(GLint s, GLint t, GLint r, GLint q)
{
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glTexCoord4i");
}

static void NoOpTexCoord4iv(const GLint *v)
{
   (void) v;
   WARNING("glTexCoord4iv");
}

static void NoOpTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
{
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glTexCoord4s");
}

static void NoOpTexCoord4sv(const GLshort *v)
{
   (void) v;
   WARNING("glTexCoord4sv");
}

static void NoOpTexGend(GLenum coord, GLenum pname, GLdouble param)
{
   (void) coord;
   (void) pname;
   (void) param;
   WARNING("glTexGend");
}

static void NoOpTexGenf(GLenum coord, GLenum pname, GLfloat param)
{
   (void) coord;
   (void) pname;
   (void) param;
   WARNING("glTexGenf");
}

static void NoOpTexGeni(GLenum coord, GLenum pname, GLint param)
{
   (void) coord;
   (void) pname;
   (void) param;
   WARNING("glTexGeni");
}

static void NoOpTexGendv(GLenum coord, GLenum pname, const GLdouble *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glTexGendv");
}

static void NoOpTexGeniv(GLenum coord, GLenum pname, const GLint *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glTexGeniv");
}

static void NoOpTexGenfv(GLenum coord, GLenum pname, const GLfloat *params)
{
   (void) coord;
   (void) pname;
   (void) params;
   WARNING("glTexGenfv");
}

static void NoOpTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexEnvf");
}

static void NoOpTexEnvi(GLenum target, GLenum pname, GLint param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexEnvi");
}

static void NoOpTexEnvfv(GLenum target, GLenum pname, const GLfloat *param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexEnvfv");
}

static void NoOpTexEnviv(GLenum target, GLenum pname, const GLint *param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexEnviv");
}

static void NoOpTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) internalformat;
   (void) width;
   (void) border;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexImage1D");
}

static void NoOpTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) internalformat;
   (void) width;
   (void) height;
   (void) border;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexImage2D");
}

static void NoOpTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexParameterf");
}

static void NoOpTexParameteri(GLenum target, GLenum pname, GLint param)
{
   (void) target;
   (void) pname;
   (void) param;
   WARNING("glTexParameteri");
}

static void NoOpTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glTexParameterfv");
}

static void NoOpTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glTexParameteriv");
}

static void NoOpTranslated(GLdouble x, GLdouble y, GLdouble z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glTranslated");
}

static void NoOpTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glTranslatef");
}

static void NoOpVertex2d(GLdouble x, GLdouble y)
{
   (void) x;
   (void) y;
   WARNING("glVertex2d");
}

static void NoOpVertex2dv(const GLdouble *v)
{
   (void) v;
   WARNING("glVertex2dv");
}

static void NoOpVertex2f(GLfloat x, GLfloat y)
{
   (void) x;
   (void) y;
   WARNING("glVertex2f");
}

static void NoOpVertex2fv(const GLfloat *v)
{
   (void) v;
   WARNING("glVertex2fv");
}

static void NoOpVertex2i(GLint x, GLint y)
{
   (void) x;
   (void) y;
   WARNING("glVertex2i");
}

static void NoOpVertex2iv(const GLint *v)
{
   (void) v;
   WARNING("glVertex2iv");
}

static void NoOpVertex2s(GLshort x, GLshort y)
{
   (void) x;
   (void) y;
   WARNING("glVertex2s");
}

static void NoOpVertex2sv(const GLshort *v)
{
   (void) v;
   WARNING("glVertex2sv");
}

static void NoOpVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glVertex3d");
}

static void NoOpVertex3dv(const GLdouble *v)
{
   (void) v;
   WARNING("glVertex3dv");
}

static void NoOpVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glVertex3f");
}

static void NoOpVertex3fv(const GLfloat *v)
{
   (void) v;
   WARNING("glVertex3fv");
}

static void NoOpVertex3i(GLint x, GLint y, GLint z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glVertex3i");
}

static void NoOpVertex3iv(const GLint *v)
{
   (void) v;
   WARNING("glVertex3iv");
}

static void NoOpVertex3s(GLshort x, GLshort y, GLshort z)
{
   (void) x;
   (void) y;
   (void) z;
   WARNING("glVertex3s");
}

static void NoOpVertex3sv(const GLshort *v)
{
   (void) v;
   WARNING("glVertex3sv");
}

static void NoOpVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glVertex4d");
}

static void NoOpVertex4dv(const GLdouble *v)
{
   (void) v;
   WARNING("glVertex4dv");
}

static void NoOpVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glVertex4f");
}

static void NoOpVertex4fv(const GLfloat *v)
{
   (void) v;
   WARNING("glVertex4fv");
}

static void NoOpVertex4i(GLint x, GLint y, GLint z, GLint w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glVertex4i");
}

static void NoOpVertex4iv(const GLint *v)
{
   (void) v;
   WARNING("glVertex4iv");
}

static void NoOpVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glVertex4s");
}

static void NoOpVertex4sv(const GLshort *v)
{
   (void) v;
   WARNING("glVertex4sv");
}

static void NoOpViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   WARNING("glViewport");
}


#ifdef _GLAPI_VERSION_1_1

static GLboolean NoOpAreTexturesResident(GLsizei n, const GLuint *textures, GLboolean *residences)
{
   (void) n;
   (void) textures;
   (void) residences;
   WARNING("glAreTexturesResident");
   return GL_FALSE;
}

static void NoOpArrayElement(GLint i)
{
   (void) i;
   WARNING("glArrayElement");
}

static void NoOpBindTexture(GLenum target, GLuint texture)
{
   (void) target;
   (void) texture;
   WARNING("glBindTexture");
}

static void NoOpColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
   (void) size;
   (void) type;
   (void) stride;
   (void) ptr;
   WARNING("glColorPointer");
}

static void NoOpCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
   (void) target;
   (void) level;
   (void) internalformat;
   (void) x;
   (void) y;
   (void) width;
   (void) border;
   WARNING("glCopyTexImage1D");
}

static void NoOpCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
   (void) target;
   (void) level;
   (void) internalformat;
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   (void) border;
   WARNING("glCopyTexImage2D");
}

static void NoOpCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) x;
   (void) y;
   (void) width;
   WARNING("glCopyTexSubImage1D");
}

static void NoOpCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) yoffset;
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   WARNING("glCopyTexSubImage2D");
}

static void NoOpDeleteTextures(GLsizei n, const GLuint *textures)
{
   (void) n;
   (void) textures;
   WARNING("glDeleteTextures");
}

static void NoOpDrawArrays(GLenum mode, GLint first, GLsizei count)
{
   (void) mode;
   (void) first;
   (void) count;
   WARNING("glDrawArrays");
}

static void NoOpDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
   (void) mode;
   (void) type;
   (void) indices;
   WARNING("glDrawElements");
}

static void NoOpEnableClientState(GLenum cap)
{
   (void) cap;
   WARNING("glEnableClientState");
}

static void NoOpGenTextures(GLsizei n, GLuint *textures)
{
   (void) n;
   (void) textures;
   WARNING("glGenTextures");
}

static void NoOpGetPointerv(GLenum pname, GLvoid **params)
{
   (void) pname;
   (void) params;
   WARNING("glGetPointerv");
}

static void NoOpIndexub(GLubyte c)
{
   (void) c;
   WARNING("glIndexub");
}

static void NoOpIndexubv(const GLubyte *c)
{
   (void) c;
   WARNING("glIndexubv");
}

static void NoOpIndexPointer(GLenum type, GLsizei stride, const GLvoid *ptr)
{
   (void) type;
   (void) stride;
   (void) ptr;
   WARNING("glIndexPointer");
}

static void NoOpInterleavedArrays(GLenum format, GLsizei stride, const GLvoid *pointer)
{
   (void) format;
   (void) stride;
   (void) pointer;
   WARNING("glInterleavedArrays");
}

static void NoOpNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr)
{
   (void) type;
   (void) stride;
   (void) ptr;
   WARNING("glNormalPointer");
}

static void NoOpPopClientAttrib(void)
{
   WARNING("glPopClientAttrib");
}

static void NoOpPrioritizeTextures(GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
   (void) n;
   (void) textures;
   (void) priorities;
   WARNING("glPrioritizeTextures");
}

static void NoOpPushClientAttrib(GLbitfield mask)
{
   (void) mask;
   WARNING("glPushClientAttrib");
}

static void NoOpTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
   (void) size;
   (void) type;
   (void) stride;
   (void) ptr;
   WARNING("glTexCoordPointer");
}

static void NoOpTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) width;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexSubImage1D");
}

static void NoOpTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) yoffset;
   (void) width;
   (void) height;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexSubImage2D");
}

static void NoOpVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
   (void) size;
   (void) type;
   (void) stride;
   (void) ptr;
   WARNING("glVertexPointer");
}

#endif


#ifdef _GLAPI_VERSION_1_2

static void NoOpCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) yoffset;
   (void) zoffset;
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   WARNING("glCopyTexSubImage3D");
}

static void NoOpDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices)
{
   (void) mode;
   (void) start;
   (void) end;
   (void) count;
   (void) type;
   (void) indices;
   WARNING("glDrawRangeElements");
}

static void NoOpTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) internalformat;
   (void) width;
   (void) height;
   (void) depth;
   (void) border;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexImage3D");
}

static void NoOpTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels)
{
   (void) target;
   (void) level;
   (void) xoffset;
   (void) yoffset;
   (void) zoffset;
   (void) width;
   (void) height;
   (void) depth;
   (void) depth;
   (void) format;
   (void) type;
   (void) pixels;
   WARNING("glTexSubImage3D");
}



#ifdef _GLAPI_ARB_imaging

static void NoOpBlendColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
   (void) r;
   (void) g;
   (void) b;
   (void) a;
   WARNING("glBlendColor");
}

static void NoOpBlendEquation(GLenum eq)
{
   (void) eq;
   WARNING("glBlendEquation");
}

static void NoOpColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data)
{
   (void) target;
   (void) start;
   (void) count;
   (void) format;
   (void) type;
   (void) data;
   WARNING("glColorSubTable");
}

static void NoOpColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table)
{
   (void) target;
   (void) internalformat;
   (void) width;
   (void) format;
   (void) type;
   (void) table;
   WARNING("glColorTable");
}

static void NoOpColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glColorTableParameterfv");
}

static void NoOpColorTableParameteriv(GLenum target, GLenum pname, const GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glColorTableParameteriv");
}


static void NoOpConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image)
{
   (void) target;
   (void) internalformat;
   (void) width;
   (void) format;
   (void) type;
   (void) image;
   WARNING("glConvolutionFilter1D");
}

static void NoOpConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image)
{
   (void) target;
   (void) internalformat;
   (void) width;
   (void) height;
   (void) format;
   (void) type;
   (void) image;
   WARNING("glConvolutionFilter2D");
}

static void NoOpConvolutionParameterf(GLenum target, GLenum pname, GLfloat params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glConvolutionParameterf");
}

static void NoOpConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glConvolutionParameterfv");
}

static void NoOpConvolutionParameteri(GLenum target, GLenum pname, GLint params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glConvolutionParameteri");
}

static void NoOpConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glConvolutionParameteriv");
}

static void NoOpCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)
{
   (void) target;
   (void) start;
   (void) x;
   (void) y;
   (void) width;
   WARNING("glCopyColorSubTable");
}

static void NoOpCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
   (void) target;
   (void) internalformat;
   (void) x;
   (void) y;
   (void) width;
   WARNING("glCopyColorTable");
}

static void NoOpCopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)
{
   (void) target;
   (void) internalformat;
   (void) x;
   (void) y;
   (void) width;
   WARNING("glCopyConvolutionFilter1D");
}

static void NoOpCopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)
{
   (void) target;
   (void) internalformat;
   (void) x;
   (void) y;
   (void) width;
   (void) height;
   WARNING("glCopyConvolutionFilter2D");
}

static void NoOpGetColorTable(GLenum target, GLenum format, GLenum type, GLvoid *table)
{
   (void) target;
   (void) format;
   (void) type;
   (void) table;
   WARNING("glGetColorTable");
}

static void NoOpGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetColorTableParameterfv");
}

static void NoOpGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetColorTableParameteriv");
}

static void NoOpGetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid *image)
{
   (void) target;
   (void) format;
   (void) type;
   (void) image;
   WARNING("glGetConvolutionFilter");
}

static void NoOpGetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetConvolutionParameterfv");
}

static void NoOpGetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetConvolutionParameteriv");
}

static void NoOpGetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values)
{
   (void) target;
   (void) reset;
   (void) format;
   (void) types;
   (void) values;
   WARNING("glGetMinmax");
}

static void NoOpGetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values)
{
   (void) target;
   (void) reset;
   (void) format;
   (void) type;
   (void) values;
   WARNING("glGetHistogram");
}

static void NoOpGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetHistogramParameterfv");
}

static void NoOpGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetHistogramParameteriv");
}

static void NoOpGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetMinmaxParameterfv");
}

static void NoOpGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetMinmaxParameteriv");
}

static void NoOpGetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span)
{
   (void) target;
   (void) format;
   (void) type;
   (void) row;
   (void) column;
   (void) span;
   WARNING("glGetSeperableFilter");
}

static void NoOpHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)
{
   (void) target;
   (void) width;
   (void) internalformat;
   (void) sink;
   WARNING("glHistogram");
}

static void NoOpMinmax(GLenum target, GLenum internalformat, GLboolean sink)
{
   (void) target;
   (void) internalformat;
   (void) sink;
   WARNING("glMinmax");
}

static void NoOpResetHistogram(GLenum target)
{
   (void) target;
   WARNING("glResetHistogram");
}

static void NoOpResetMinmax(GLenum target)
{
   (void) target;
   WARNING("glResetMinmax");
}

static void NoOpSeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column)
{
   (void) target;
   (void) internalformat;
   (void) width;
   (void) height;
   (void) format;
   (void) type;
   (void) row;
   (void) column;
   WARNING("glSeparableFilter2D");
}

#endif  /*GL_ARB_imaging*/
#endif


/***
 *** Extension functions
 ***/


#ifdef _GLAPI_EXT_polygon_offset
static void NoOpPolygonOffsetEXT(GLfloat factor, GLfloat bias)
{
   (void) factor;
   (void) bias;
   WARNING("glPolygonOffsetEXT");
}
#endif  /* GL_EXT_polygon_offset */


#ifdef _GLAPI_EXT_blend_minmax
static void NoOpBlendEquationEXT(GLenum mode)
{
   (void) mode;
   WARNING("glBlendEuqationEXT");
}
#endif


#ifdef _GLAPI_EXT_blend_color
static void NoOpBlendColorEXT(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
   (void) r;
   (void) g;
   (void) b;
   (void) a;
   WARNING("glBlendColor");
}
#endif


#ifdef _GLAPI_EXT_color_table

static void NoOpColorTableEXT(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table)
{
   (void) target;
   (void) internalformat;
   (void) width;
   (void) format;
   (void) type;
   (void) table;
   WARNING("glColorTableEXT");
}

static void NoOpColorSubTableEXT(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data)
{
   (void) target;
   (void) start;
   (void) count;
   (void) format;
   (void) type;
   (void) data;
   WARNING("glColorSubTableEXT");
}

static void NoOpGetColorTableEXT(GLenum target, GLenum format, GLenum type, GLvoid *table)
{
   (void) target;
   (void) format;
   (void) type;
   (void) table;
   WARNING("glGetColorTableEXT");
}

static void NoOpGetColorTableParameterfvEXT(GLenum target, GLenum pname, GLfloat *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetColorTableParameterfvEXT");
}

static void NoOpGetColorTableParameterivEXT(GLenum target, GLenum pname, GLint *params)
{
   (void) target;
   (void) pname;
   (void) params;
   WARNING("glGetColorTableParameterivEXT");
}

#endif  /* GL_EXT_color_table */



#ifdef _GLAPI_EXT_compiled_vertex_array

static void NoOpLockArraysEXT(GLint first, GLsizei count)
{
   (void) first;
   (void) count;
   WARNING("glLockArraysEXT");
}

static void NoOpUnlockArraysEXT(void)
{
   WARNING("glUnlockArraysEXT");
}

#endif  /* GL_EXT_compiled_vertex_array */



#ifdef _GLAPI_EXT_point_parameters

static void NoOpPointParameterfEXT(GLenum target, GLfloat param)
{
   (void) target;
   (void) param;
   WARNING("glPointParameterfEXT");
}

static void NoOpPointParameterfvEXT(GLenum target, const GLfloat *param)
{
   (void) target;
   (void) param;
   WARNING("glPointParameterfvEXT");
}

#endif  /* GL_EXT_point_parameters */



#ifdef _GLAPI_ARB_multitexture

static void NoOpActiveTextureARB(GLenum texture)
{
   (void) texture;
   WARNING("glActiveTextureARB");
}

static void NoOpClientActiveTextureARB(GLenum texture)
{
   (void) texture;
   WARNING("glClientActiveTextureARB");
}

static void NoOpMultiTexCoord1dARB(GLenum target, GLdouble s)
{
   (void) target;
   (void) s;
   WARNING("glMultiTexCoord1dARB");
}

static void NoOpMultiTexCoord1dvARB(GLenum target, const GLdouble *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord1dvARB");
}

static void NoOpMultiTexCoord1fARB(GLenum target, GLfloat s)
{
   (void) target;
   (void) s;
   WARNING("glMultiTexCoord1fARB");
}

static void NoOpMultiTexCoord1fvARB(GLenum target, const GLfloat *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord1fvARB");
}

static void NoOpMultiTexCoord1iARB(GLenum target, GLint s)
{
   (void) target;
   (void) s;
   WARNING("glMultiTexCoord1iARB");
}

static void NoOpMultiTexCoord1ivARB(GLenum target, const GLint *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord1ivARB");
}

static void NoOpMultiTexCoord1sARB(GLenum target, GLshort s)
{
   (void) target;
   (void) s;
   WARNING("glMultiTexCoord1sARB");
}

static void NoOpMultiTexCoord1svARB(GLenum target, const GLshort *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord1svARB");
}

static void NoOpMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t)
{
   (void) target;
   (void) s;
   (void) t;
   WARNING("glMultiTexCoord2dARB");
}

static void NoOpMultiTexCoord2dvARB(GLenum target, const GLdouble *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord2dvARB");
}

static void NoOpMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
   (void) target;
   (void) s;
   (void) t;
   WARNING("glMultiTexCoord2fARB");
}

static void NoOpMultiTexCoord2fvARB(GLenum target, const GLfloat *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord2fvARB");
}

static void NoOpMultiTexCoord2iARB(GLenum target, GLint s, GLint t)
{
   (void) target;
   (void) s;
   (void) t;
   WARNING("glMultiTexCoord2iARB");
}

static void NoOpMultiTexCoord2ivARB(GLenum target, const GLint *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord2ivARB");
}

static void NoOpMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t)
{
   (void) target;
   (void) s;
   (void) t;
   WARNING("glMultiTexCoord2sARB");
}

static void NoOpMultiTexCoord2svARB(GLenum target, const GLshort *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord2svARB");
}

static void NoOpMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   WARNING("glMultiTexCoord3dARB");
}

static void NoOpMultiTexCoord3dvARB(GLenum target, const GLdouble *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord3dvARB");
}

static void NoOpMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   WARNING("glMultiTexCoord3fARB");
}

static void NoOpMultiTexCoord3fvARB(GLenum target, const GLfloat *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord3fvARB");
}

static void NoOpMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   WARNING("glMultiTexCoord3iARB");
}

static void NoOpMultiTexCoord3ivARB(GLenum target, const GLint *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord3ivARB");
}

static void NoOpMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   WARNING("glMultiTexCoord3sARB");
}

static void NoOpMultiTexCoord3svARB(GLenum target, const GLshort *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord3svARB");
}

static void NoOpMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glMultiTexCoord4dARB");
}

static void NoOpMultiTexCoord4dvARB(GLenum target, const GLdouble *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord4dvARB");
}

static void NoOpMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glMultiTexCoord4fARB");
}

static void NoOpMultiTexCoord4fvARB(GLenum target, const GLfloat *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord4fvARB");
}

static void NoOpMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glMultiTexCoord4iARB");
}

static void NoOpMultiTexCoord4ivARB(GLenum target, const GLint *v)
{
   (void) target;
   (void) v;
   WARNING("glMultiTexCoord4ivARB");
}

static void NoOpMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
   (void) target;
   (void) s;
   (void) t;
   (void) r;
   (void) q;
   WARNING("glMultiTexCoord4sARB");
}

static void NoOpMultiTexCoord4svARB(GLenum target, const GLshort *v)
{
   (void) v;
   WARNING("glMultiTexCoord4svARB");
}

#endif  /* GL_ARB_multitexture */



#ifdef _GLAPI_INGR_blend_func_separate
static void NoOpBlendFuncSeparateINGR(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
   (void) sfactorRGB;
   (void) dfactorRGB;
   (void) sfactorAlpha;
   (void) dfactorAlpha;
   WARNING("glBlendFuncSeparateINGR");
}
#endif  /* GL_INGR_blend_func_separate */


#ifdef _GLAPI_MESA_window_pos
static void NoOpWindowPos4fMESA(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
   (void) x;
   (void) y;
   (void) z;
   (void) w;
   WARNING("glWindowPos[234][ifdv][v]MESA");
}
#endif  /* GL_MESA_window_pos */


#ifdef _GLAPI_MESA_resize_buffers
static void NoOpResizeBuffersMESA(void)
{
   WARNING("glResizeBuffersMESA");
}
#endif  /* GL_MESA_resize_buffers */



struct _glapi_table __glapi_noop_table = {
   NoOpAccum,
   NoOpAlphaFunc,
   NoOpBegin,
   NoOpBitmap,
   NoOpBlendFunc,
   NoOpCallList,
   NoOpCallLists,
   NoOpClear,
   NoOpClearAccum,
   NoOpClearColor,
   NoOpClearDepth,
   NoOpClearIndex,
   NoOpClearStencil,
   NoOpClipPlane,
   NoOpColor3b,
   NoOpColor3bv,
   NoOpColor3d,
   NoOpColor3dv,
   NoOpColor3f,
   NoOpColor3fv,
   NoOpColor3i,
   NoOpColor3iv,
   NoOpColor3s,
   NoOpColor3sv,
   NoOpColor3ub,
   NoOpColor3ubv,
   NoOpColor3ui,
   NoOpColor3uiv,
   NoOpColor3us,
   NoOpColor3usv,
   NoOpColor4b,
   NoOpColor4bv,
   NoOpColor4d,
   NoOpColor4dv,
   NoOpColor4f,
   NoOpColor4fv,
   NoOpColor4i,
   NoOpColor4iv,
   NoOpColor4s,
   NoOpColor4sv,
   NoOpColor4ub,
   NoOpColor4ubv,
   NoOpColor4ui,
   NoOpColor4uiv,
   NoOpColor4us,
   NoOpColor4usv,
   NoOpColorMask,
   NoOpColorMaterial,
   NoOpCopyPixels,
   NoOpCullFace,
   NoOpDeleteLists,
   NoOpDepthFunc,
   NoOpDepthMask,
   NoOpDepthRange,
   NoOpDisable,
   NoOpDrawBuffer,
   NoOpDrawPixels,
   NoOpEdgeFlag,
   NoOpEdgeFlagv,
   NoOpEnable,
   NoOpEnd,
   NoOpEndList,
   NoOpEvalCoord1d,
   NoOpEvalCoord1dv,
   NoOpEvalCoord1f,
   NoOpEvalCoord1fv,
   NoOpEvalCoord2d,
   NoOpEvalCoord2dv,
   NoOpEvalCoord2f,
   NoOpEvalCoord2fv,
   NoOpEvalMesh1,
   NoOpEvalMesh2,
   NoOpEvalPoint1,
   NoOpEvalPoint2,
   NoOpFeedbackBuffer,
   NoOpFinish,
   NoOpFlush,
   NoOpFogf,
   NoOpFogfv,
   NoOpFogi,
   NoOpFogiv,
   NoOpFrontFace,
   NoOpFrustum,
   NoOpGenLists,
   NoOpGetBooleanv,
   NoOpGetClipPlane,
   NoOpGetDoublev,
   NoOpGetError,
   NoOpGetFloatv,
   NoOpGetIntegerv,
   NoOpGetLightfv,
   NoOpGetLightiv,
   NoOpGetMapdv,
   NoOpGetMapfv,
   NoOpGetMapiv,
   NoOpGetMaterialfv,
   NoOpGetMaterialiv,
   NoOpGetPixelMapfv,
   NoOpGetPixelMapuiv,
   NoOpGetPixelMapusv,
   NoOpGetPolygonStipple,
   NoOpGetString,
   NoOpGetTexEnvfv,
   NoOpGetTexEnviv,
   NoOpGetTexGendv,
   NoOpGetTexGenfv,
   NoOpGetTexGeniv,
   NoOpGetTexImage,
   NoOpGetTexLevelParameterfv,
   NoOpGetTexLevelParameteriv,
   NoOpGetTexParameterfv,
   NoOpGetTexParameteriv,
   NoOpHint,
   NoOpIndexMask,
   NoOpIndexd,
   NoOpIndexdv,
   NoOpIndexf,
   NoOpIndexfv,
   NoOpIndexi,
   NoOpIndexiv,
   NoOpIndexs,
   NoOpIndexsv,
   NoOpInitNames,
   NoOpIsEnabled,
   NoOpIsList,
   NoOpLightModelf,
   NoOpLightModelfv,
   NoOpLightModeli,
   NoOpLightModeliv,
   NoOpLightf,
   NoOpLightfv,
   NoOpLighti,
   NoOpLightiv,
   NoOpLineStipple,
   NoOpLineWidth,
   NoOpListBase,
   NoOpLoadIdentity,
   NoOpLoadMatrixd,
   NoOpLoadMatrixf,
   NoOpLoadName,
   NoOpLogicOp,
   NoOpMap1d,
   NoOpMap1f,
   NoOpMap2d,
   NoOpMap2f,
   NoOpMapGrid1d,
   NoOpMapGrid1f,
   NoOpMapGrid2d,
   NoOpMapGrid2f,
   NoOpMaterialf,
   NoOpMaterialfv,
   NoOpMateriali,
   NoOpMaterialiv,
   NoOpMatrixMode,
   NoOpMultMatrixd,
   NoOpMultMatrixf,
   NoOpNewList,
   NoOpNormal3b,
   NoOpNormal3bv,
   NoOpNormal3d,
   NoOpNormal3dv,
   NoOpNormal3f,
   NoOpNormal3fv,
   NoOpNormal3i,
   NoOpNormal3iv,
   NoOpNormal3s,
   NoOpNormal3sv,
   NoOpOrtho,
   NoOpPassThrough,
   NoOpPixelMapfv,
   NoOpPixelMapuiv,
   NoOpPixelMapusv,
   NoOpPixelStoref,
   NoOpPixelStorei,
   NoOpPixelTransferf,
   NoOpPixelTransferi,
   NoOpPixelZoom,
   NoOpPointSize,
   NoOpPolygonMode,
   NoOpPolygonOffset,
   NoOpPolygonStipple,
   NoOpPopAttrib,
   NoOpPopMatrix,
   NoOpPopName,
   NoOpPushAttrib,
   NoOpPushMatrix,
   NoOpPushName,
   NoOpRasterPos2d,
   NoOpRasterPos2dv,
   NoOpRasterPos2f,
   NoOpRasterPos2fv,
   NoOpRasterPos2i,
   NoOpRasterPos2iv,
   NoOpRasterPos2s,
   NoOpRasterPos2sv,
   NoOpRasterPos3d,
   NoOpRasterPos3dv,
   NoOpRasterPos3f,
   NoOpRasterPos3fv,
   NoOpRasterPos3i,
   NoOpRasterPos3iv,
   NoOpRasterPos3s,
   NoOpRasterPos3sv,
   NoOpRasterPos4d,
   NoOpRasterPos4dv,
   NoOpRasterPos4f,
   NoOpRasterPos4fv,
   NoOpRasterPos4i,
   NoOpRasterPos4iv,
   NoOpRasterPos4s,
   NoOpRasterPos4sv,
   NoOpReadBuffer,
   NoOpReadPixels,
   NoOpRectd,
   NoOpRectdv,
   NoOpRectf,
   NoOpRectfv,
   NoOpRecti,
   NoOpRectiv,
   NoOpRects,
   NoOpRectsv,
   NoOpRenderMode,
   NoOpRotated,
   NoOpRotatef,
   NoOpScaled,
   NoOpScalef,
   NoOpScissor,
   NoOpSelectBuffer,
   NoOpShadeModel,
   NoOpStencilFunc,
   NoOpStencilMask,
   NoOpStencilOp,
   NoOpTexCoord1d,
   NoOpTexCoord1dv,
   NoOpTexCoord1f,
   NoOpTexCoord1fv,
   NoOpTexCoord1i,
   NoOpTexCoord1iv,
   NoOpTexCoord1s,
   NoOpTexCoord1sv,
   NoOpTexCoord2d,
   NoOpTexCoord2dv,
   NoOpTexCoord2f,
   NoOpTexCoord2fv,
   NoOpTexCoord2i,
   NoOpTexCoord2iv,
   NoOpTexCoord2s,
   NoOpTexCoord2sv,
   NoOpTexCoord3d,
   NoOpTexCoord3dv,
   NoOpTexCoord3f,
   NoOpTexCoord3fv,
   NoOpTexCoord3i,
   NoOpTexCoord3iv,
   NoOpTexCoord3s,
   NoOpTexCoord3sv,
   NoOpTexCoord4d,
   NoOpTexCoord4dv,
   NoOpTexCoord4f,
   NoOpTexCoord4fv,
   NoOpTexCoord4i,
   NoOpTexCoord4iv,
   NoOpTexCoord4s,
   NoOpTexCoord4sv,
   NoOpTexEnvf,
   NoOpTexEnvfv,
   NoOpTexEnvi,
   NoOpTexEnviv,
   NoOpTexGend,
   NoOpTexGendv,
   NoOpTexGenf,
   NoOpTexGenfv,
   NoOpTexGeni,
   NoOpTexGeniv,
   NoOpTexImage1D,
   NoOpTexImage2D,
   NoOpTexParameterf,
   NoOpTexParameterfv,
   NoOpTexParameteri,
   NoOpTexParameteriv,
   NoOpTranslated,
   NoOpTranslatef,
   NoOpVertex2d,
   NoOpVertex2dv,
   NoOpVertex2f,
   NoOpVertex2fv,
   NoOpVertex2i,
   NoOpVertex2iv,
   NoOpVertex2s,
   NoOpVertex2sv,
   NoOpVertex3d,
   NoOpVertex3dv,
   NoOpVertex3f,
   NoOpVertex3fv,
   NoOpVertex3i,
   NoOpVertex3iv,
   NoOpVertex3s,
   NoOpVertex3sv,
   NoOpVertex4d,
   NoOpVertex4dv,
   NoOpVertex4f,
   NoOpVertex4fv,
   NoOpVertex4i,
   NoOpVertex4iv,
   NoOpVertex4s,
   NoOpVertex4sv,
   NoOpViewport,

#ifdef _GLAPI_VERSION_1_1
   NoOpAreTexturesResident,
   NoOpArrayElement,
   NoOpBindTexture,
   NoOpColorPointer,
   NoOpCopyTexImage1D,
   NoOpCopyTexImage2D,
   NoOpCopyTexSubImage1D,
   NoOpCopyTexSubImage2D,
   NoOpDeleteTextures,
   NoOpDisableClientState,
   NoOpDrawArrays,
   NoOpDrawElements,
   NoOpEdgeFlagPointer,
   NoOpEnableClientState,
   NoOpGenTextures,
   NoOpGetPointerv,
   NoOpIndexPointer,
   NoOpIndexub,
   NoOpIndexubv,
   NoOpInterleavedArrays,
   NoOpIsTexture,
   NoOpNormalPointer,
   NoOpPopClientAttrib,
   NoOpPrioritizeTextures,
   NoOpPushClientAttrib,
   NoOpTexCoordPointer,
   NoOpTexSubImage1D,
   NoOpTexSubImage2D,
   NoOpVertexPointer,
#endif

#ifdef _GLAPI_VERSION_1_2
   NoOpCopyTexSubImage3D,
   NoOpDrawRangeElements,
   NoOpTexImage3D,
   NoOpTexSubImage3D,

#ifdef _GLAPI_ARB_imaging
   NoOpBlendColor,
   NoOpBlendEquation,
   NoOpColorSubTable,
   NoOpColorTable,
   NoOpColorTableParameterfv,
   NoOpColorTableParameteriv,
   NoOpConvolutionFilter1D,
   NoOpConvolutionFilter2D,
   NoOpConvolutionParameterf,
   NoOpConvolutionParameterfv,
   NoOpConvolutionParameteri,
   NoOpConvolutionParameteriv,
   NoOpCopyColorSubTable,
   NoOpCopyColorTable,
   NoOpCopyConvolutionFilter1D,
   NoOpCopyConvolutionFilter2D,
   NoOpGetColorTable,
   NoOpGetColorTableParameterfv,
   NoOpGetColorTableParameteriv,
   NoOpGetConvolutionFilter,
   NoOpGetConvolutionParameterfv,
   NoOpGetConvolutionParameteriv,
   NoOpGetHistogram,
   NoOpGetHistogramParameterfv,
   NoOpGetHistogramParameteriv,
   NoOpGetMinmax,
   NoOpGetMinmaxParameterfv,
   NoOpGetMinmaxParameteriv,
   NoOpGetSeparableFilter,
   NoOpHistogram,
   NoOpMinmax,
   NoOpResetHistogram,
   NoOpResetMinmax,
   NoOpSeparableFilter2D,
#endif
#endif


   /*
    * Extensions
    */

#ifdef _GLAPI_EXT_color_table
   NoOpColorTableEXT,
   NoOpColorSubTableEXT,
   NoOpGetColorTableEXT,
   NoOpGetColorTableParameterfvEXT,
   NoOpGetColorTableParameterivEXT,
#endif

#ifdef _GLAPI_EXT_compiled_vertex_array
   NoOpLockArraysEXT,
   NoOpUnlockArraysEXT,
#endif

#ifdef _GLAPI_EXT_point_parameters
   NoOpPointParameterfEXT,
   NoOpPointParameterfvEXT,
#endif

#ifdef _GLAPI_EXT_polygon_offset
   NoOpPolygonOffsetEXT,
#endif

#ifdef _GLAPI_EXT_blend_minmax
   NoOpBlendEquationEXT,
#endif

#ifdef _GLAPI_EXT_blend_color
   NoOpBlendColorEXT,
#endif

#ifdef _GLAPI_ARB_multitexture
   NoOpActiveTextureARB,
   NoOpClientActiveTextureARB,
   NoOpMultiTexCoord1dARB,
   NoOpMultiTexCoord1dvARB,
   NoOpMultiTexCoord1fARB,
   NoOpMultiTexCoord1fvARB,
   NoOpMultiTexCoord1iARB,
   NoOpMultiTexCoord1ivARB,
   NoOpMultiTexCoord1sARB,
   NoOpMultiTexCoord1svARB,
   NoOpMultiTexCoord2dARB,
   NoOpMultiTexCoord2dvARB,
   NoOpMultiTexCoord2fARB,
   NoOpMultiTexCoord2fvARB,
   NoOpMultiTexCoord2iARB,
   NoOpMultiTexCoord2ivARB,
   NoOpMultiTexCoord2sARB,
   NoOpMultiTexCoord2svARB,
   NoOpMultiTexCoord3dARB,
   NoOpMultiTexCoord3dvARB,
   NoOpMultiTexCoord3fARB,
   NoOpMultiTexCoord3fvARB,
   NoOpMultiTexCoord3iARB,
   NoOpMultiTexCoord3ivARB,
   NoOpMultiTexCoord3sARB,
   NoOpMultiTexCoord3svARB,
   NoOpMultiTexCoord4dARB,
   NoOpMultiTexCoord4dvARB,
   NoOpMultiTexCoord4fARB,
   NoOpMultiTexCoord4fvARB,
   NoOpMultiTexCoord4iARB,
   NoOpMultiTexCoord4ivARB,
   NoOpMultiTexCoord4sARB,
   NoOpMultiTexCoord4svARB,
#endif

#ifdef _GLAPI_INGR_blend_func_separate
   NoOpBlendFuncSeparateINGR,
#endif

#ifdef _GLAPI_MESA_window_pos
   NoOpWindowPos4fMESA,
#endif

#ifdef _GLAPI_MESA_resize_buffers
   NoOpResizeBuffersMESA
#endif
};

