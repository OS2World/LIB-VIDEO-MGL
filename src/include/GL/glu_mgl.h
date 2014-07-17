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

#ifndef __glu_mgl_h_
#define __glu_mgl_h_

#ifdef __cplusplus
extern "C" {
#endif

/* Structure containing all public OpenGL functions supported by the MGL */

#pragma pack(1)
typedef struct {
	long	dwSize;
	void 	(APIENTRYP gluBeginCurve)(GLUnurbs *nobj);
	void 	(APIENTRYP gluBeginPolygon)(GLUtesselator *tess);
	void 	(APIENTRYP gluBeginSurface)(GLUnurbs *nobj);
	void 	(APIENTRYP gluBeginTrim)(GLUnurbs *nobj);
	int 	(APIENTRYP gluBuild1DMipmaps)(GLenum target,GLint components,GLint width,GLenum format,GLenum type,const void *data);
	int 	(APIENTRYP gluBuild2DMipmaps)(GLenum target,GLint components,GLint width,GLint height,GLenum format,GLenum type,const void *data);
	void 	(APIENTRYP gluCylinder)(GLUquadric *qobj,GLdouble baseRadius,GLdouble topRadius,GLdouble height,GLint slices,GLint stacks);
	void 	(APIENTRYP gluDeleteNurbsRenderer)(GLUnurbs *nobj);
	void 	(APIENTRYP gluDeleteQuadric)(GLUquadric *state);
	void 	(APIENTRYP gluDeleteTess)(GLUtesselator *tess);
	void 	(APIENTRYP gluDisk)(GLUquadric *qobj,GLdouble innerRadius,GLdouble outerRadius,GLint slices,GLint loops);
	void 	(APIENTRYP gluEndCurve)(GLUnurbs *nobj);
	void 	(APIENTRYP gluEndPolygon)(GLUtesselator *tess);
	void 	(APIENTRYP gluEndSurface)(GLUnurbs *nobj);
	void 	(APIENTRYP gluEndTrim)(GLUnurbs *nobj);
	const GLubyte* 	(APIENTRYP gluErrorString)(GLenum errCode);
	void 	(APIENTRYP gluGetNurbsProperty)(GLUnurbs *nobj,GLenum property,GLfloat *value);
	const GLubyte* 	(APIENTRYP gluGetString)(GLenum name);
	void 	(APIENTRYP gluGetTessProperty)(GLUtesselator *tess,GLenum which,GLdouble *value);
	void 	(APIENTRYP gluLoadSamplingMatrices)(GLUnurbs *nobj,const GLfloat modelMatrix[16],const GLfloat projMatrix[16],const GLint viewport[4]);
	void 	(APIENTRYP gluLookAt)(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz);
	GLUnurbs* (APIENTRYP gluNewNurbsRenderer)(void);
	GLUquadric* (APIENTRYP gluNewQuadric)(void);
	GLUtesselator* (APIENTRYP gluNewTess)(void );
	void 	(APIENTRYP gluNextContour)(GLUtesselator *tess,GLenum type);
	void 	(APIENTRYP gluNurbsCallback)(GLUnurbs *nobj,GLenum which,void (CALLBACKP fn)());
	void 	(APIENTRYP gluNurbsCurve)(GLUnurbs *nobj,GLint nknots,GLfloat *knot,GLint stride,GLfloat *ctlarray,GLint order,GLenum type);
	void 	(APIENTRYP gluNurbsProperty)(GLUnurbs *nobj,GLenum property,GLfloat value);
	void 	(APIENTRYP gluNurbsSurface)(GLUnurbs *nobj,GLint sknot_count,float *sknot,GLint tknot_count,GLfloat *tknot,GLint s_stride,GLint t_stride,GLfloat *ctlarray,GLint sorder,GLint torder,GLenum type);
	void	(APIENTRYP gluOrtho2D)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top);
	void 	(APIENTRYP gluPartialDisk)(GLUquadric *qobj,GLdouble innerRadius,GLdouble outerRadius,GLint slices,GLint loops,GLdouble startAngle,GLdouble sweepAngle);
	void 	(APIENTRYP gluPerspective)(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);
	void 	(APIENTRYP gluPickMatrix)(GLdouble x,GLdouble y,GLdouble width,GLdouble height,const GLint viewport[4]);
	int 	(APIENTRYP gluProject)(GLdouble objx,GLdouble objy,GLdouble objz,const GLdouble modelMatrix[16],const GLdouble projMatrix[16],const GLint viewport[4],GLdouble *winx,GLdouble *winy,GLdouble *winz);
	void 	(APIENTRYP gluPwlCurve)(GLUnurbs *nobj,GLint count,GLfloat *array,GLint stride,GLenum type);
	void 	(APIENTRYP gluQuadricCallback)(GLUquadric *qobj,GLenum which,void (CALLBACKP fn)());
	void 	(APIENTRYP gluQuadricDrawStyle)(GLUquadric *quadObject,GLenum drawStyle);
	void 	(APIENTRYP gluQuadricNormals)(GLUquadric *quadObject,GLenum normals);
	void 	(APIENTRYP gluQuadricOrientation)(GLUquadric *quadObject,GLenum orientation);
	void 	(APIENTRYP gluQuadricTexture)(GLUquadric *quadObject,GLboolean textureCoords);
	int 	(APIENTRYP gluScaleImage)(GLenum format,GLint widthin,GLint heightin,GLenum typein,const void *datain,GLint widthout,GLint heightout,GLenum typeout,void *dataout);
	void 	(APIENTRYP gluSphere)(GLUquadric *qobj,GLdouble radius,GLint slices,GLint stacks);
	void 	(APIENTRYP gluTessBeginContour)(GLUtesselator *tess);
	void 	(APIENTRYP gluTessBeginPolygon)(GLUtesselator *tess,void *polygon_data);
	void 	(APIENTRYP gluTessCallback)(GLUtesselator *tess,GLenum which,void (CALLBACKP fn)());
	void 	(APIENTRYP gluTessEndContour)(GLUtesselator *tess);
	void 	(APIENTRYP gluTessEndPolygon)(GLUtesselator *tess);
	void 	(APIENTRYP gluTessNormal)(GLUtesselator *tess,GLdouble x,GLdouble y,GLdouble z);
	void 	(APIENTRYP gluTessProperty)(GLUtesselator *tess,GLenum which,GLdouble value);
	void 	(APIENTRYP gluTessVertex)(GLUtesselator *tess,GLdouble coords[3],void *data);
	int 	(APIENTRYP gluUnProject)(GLdouble winx,GLdouble winy,GLdouble winz,const GLdouble modelMatrix[16],const GLdouble projMatrix[16],const GLint viewport[4],GLdouble *objx,GLdouble *objy,GLdouble *objz);
	} MGL_gluFuncs;
#pragma pack()

#if	!defined(NO_GLU_MACROS)
#define gluErrorString(errCode)                                                                                 _MGL_gluFuncs.gluErrorString(errCode)
#define gluGetString(name)                                                                                      _MGL_gluFuncs.gluGetString(name)
#define gluOrtho2D(left,right,bottom,top)                                                                       _MGL_gluFuncs.gluOrtho2D(left,right,bottom,top)
#define gluPerspective(fovy,aspect,zNear,zFar)                                                                  _MGL_gluFuncs.gluPerspective(fovy,aspect,zNear,zFar)
#define gluPickMatrix(x,y,width,height,viewport)                                                                _MGL_gluFuncs.gluPickMatrix(x,y,width,height,viewport)
#define gluLookAt(eyex,eyey,eyez,centerx,centery,centerz,upx,upy,upz)                                           _MGL_gluFuncs.gluLookAt(eyex,eyey,eyez,centerx,centery,centerz,upx,upy,upz)
#define gluProject(objx,objy,objz,modelMatrix,projMatrix,viewport,winx,winy,winz)                               _MGL_gluFuncs.gluProject(objx,objy,objz,modelMatrix,projMatrix,viewport,winx,winy,winz)
#define gluUnProject(winx,winy,winz,modelMatrix,projMatrix,viewport,objx,objy,objz)                             _MGL_gluFuncs.gluUnProject(winx,winy,winz,modelMatrix,projMatrix,viewport,objx,objy,objz)
#define gluScaleImage(format,widthin,heightin,typein,datain,widthout,heightout,typeout,dataout)                 _MGL_gluFuncs.gluScaleImage(format,widthin,heightin,typein,datain,widthout,heightout,typeout,dataout)
#define gluBuild1DMipmaps(target,components,width,format,type,data)                                             _MGL_gluFuncs.gluBuild1DMipmaps(target,components,width,format,type,data)
#define gluBuild2DMipmaps(target,components,width,height,format,type,data)                                      _MGL_gluFuncs.gluBuild2DMipmaps(target,components,width,height,format,type,data)
#define gluNewQuadric()                                                                                         _MGL_gluFuncs.gluNewQuadric()
#define gluDeleteQuadric(state)                                                                                 _MGL_gluFuncs.gluDeleteQuadric(state)
#define gluQuadricNormals(quadObject,normals)                                                                   _MGL_gluFuncs.gluQuadricNormals(quadObject,normals)
#define gluQuadricTexture(quadObject,textureCoords)                                                             _MGL_gluFuncs.gluQuadricTexture(quadObject,textureCoords)
#define gluQuadricOrientation(quadObject,orientation)                                                           _MGL_gluFuncs.gluQuadricOrientation(quadObject,orientation)
#define gluQuadricDrawStyle(quadObject,drawStyle)                                                               _MGL_gluFuncs.gluQuadricDrawStyle(quadObject,drawStyle)
#define gluCylinder(qobj,baseRadius,topRadius,height,slices,stacks)                                             _MGL_gluFuncs.gluCylinder(qobj,baseRadius,topRadius,height,slices,stacks)
#define gluDisk(qobj,innerRadius,outerRadius,slices,loops)                                                      _MGL_gluFuncs.gluDisk(qobj,innerRadius,outerRadius,slices,loops)
#define gluPartialDisk(qobj,innerRadius,outerRadius,slices,loops,startAngle,sweepAngle)                         _MGL_gluFuncs.gluPartialDisk(qobj,innerRadius,outerRadius,slices,loops,startAngle,sweepAngle)
#define gluSphere(qobj,radius,slices,stacks)                                                                    _MGL_gluFuncs.gluSphere(qobj,radius,slices,stacks)
#define gluQuadricCallback(qobj,which,fn)                                                                       _MGL_gluFuncs.gluQuadricCallback(qobj,which,fn)
#define gluNewTess()                                                                                            _MGL_gluFuncs.gluNewTess()
#define gluDeleteTess(tess)                                                                                     _MGL_gluFuncs.gluDeleteTess(tess)
#define gluTessBeginPolygon(tess,polygon_data)                                                                  _MGL_gluFuncs.gluTessBeginPolygon(tess,polygon_data)
#define gluTessBeginContour(tess)                                                                               _MGL_gluFuncs.gluTessBeginContour(tess)
#define gluTessVertex(tess,coords,data)                                                                         _MGL_gluFuncs.gluTessVertex(tess,coords,data)
#define gluTessEndContour(tess)                                                                                 _MGL_gluFuncs.gluTessEndContour(tess)
#define gluTessEndPolygon(tess)                                                                                 _MGL_gluFuncs.gluTessEndPolygon(tess)
#define gluTessProperty(tess,which,value)                                                                       _MGL_gluFuncs.gluTessProperty(tess,which,value)
#define gluTessNormal(tess,x,y,z)                                                                               _MGL_gluFuncs.gluTessNormal(tess,x,y,z)
#define gluTessCallback(tess,which,fn)                                                                          _MGL_gluFuncs.gluTessCallback(tess,which,fn)
#define gluGetTessProperty(tess,which,value)                                                                    _MGL_gluFuncs.gluGetTessProperty(tess,which,value)
#define gluNewNurbsRenderer()                                                                                   _MGL_gluFuncs.gluNewNurbsRenderer()
#define gluDeleteNurbsRenderer(nobj)                                                                            _MGL_gluFuncs.gluDeleteNurbsRenderer(nobj)
#define gluBeginSurface(nobj)                                                                                   _MGL_gluFuncs.gluBeginSurface(nobj)
#define gluBeginCurve(nobj)                                                                                     _MGL_gluFuncs.gluBeginCurve(nobj)
#define gluEndCurve(nobj)                                                                                       _MGL_gluFuncs.gluEndCurve(nobj)
#define gluEndSurface(nobj)                                                                                     _MGL_gluFuncs.gluEndSurface(nobj)
#define gluBeginTrim(nobj)                                                                                      _MGL_gluFuncs.gluBeginTrim(nobj)
#define gluEndTrim(nobj)                                                                                        _MGL_gluFuncs.gluEndTrim(nobj)
#define gluPwlCurve(nobj,count,array,stride,type)                                                               _MGL_gluFuncs.gluPwlCurve(nobj,count,array,stride,type)
#define gluNurbsCurve(nobj,nknots,knot,stride,ctlarray,order,type)                                              _MGL_gluFuncs.gluNurbsCurve(nobj,nknots,knot,stride,ctlarray,order,type)
#define gluNurbsSurface(nobj,sknot_count,sknot,tknot_count,tknot,s_stride,t_stride,ctlarray,sorder,torder,type) _MGL_gluFuncs.gluNurbsSurface(nobj,sknot_count,sknot,tknot_count,tknot,s_stride,t_stride,ctlarray,sorder,torder,type)
#define gluLoadSamplingMatrices(nobj,modelMatrix,projMatrix,viewport)                                           _MGL_gluFuncs.gluLoadSamplingMatrices(nobj,modelMatrix,projMatrix,viewport)
#define gluNurbsProperty(nobj,property,value)                                                                   _MGL_gluFuncs.gluNurbsProperty(nobj,property,value)
#define gluGetNurbsProperty(nobj,property,value)                                                                _MGL_gluFuncs.gluGetNurbsProperty(nobj,property,value)
#define gluNurbsCallback(nobj,which,fn)                                                                         _MGL_gluFuncs.gluNurbsCallback(nobj,which,fn)
#define gluBeginPolygon(tess)                                                                                   _MGL_gluFuncs.gluBeginPolygon(tess)
#define gluNextContour(tess,type)                                                                               _MGL_gluFuncs.gluNextContour(tess,type)
#define gluEndPolygon(tess)                                                                                     _MGL_gluFuncs.gluEndPolygon(tess)
#endif

extern MGL_gluFuncs	_VARAPI _MGL_gluFuncs;

#ifdef __cplusplus
}
#endif

#endif /* __glu_mgl_h_ */
