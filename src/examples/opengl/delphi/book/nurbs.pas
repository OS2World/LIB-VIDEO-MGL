{=============================================================================}
{= Copyright (c) Mark J. Kilgard, 1994.                                      =}
{=============================================================================}

{=============================================================================}
{= (c) Copyright 1993, 1994, Silicon Graphics, Inc.                          =}
{= ALL RIGHTS RESERVED                                                       =}
{= Permission to use, copy, modify, and distribute this software for         =}
{= any purpose and without fee is hereby granted, provided that the above    =}
{= copyright notice appear in all copies and that both the copyright notice  =}
{= and this permission notice appear in supporting documentation, and that   =}
{= the name of Silicon Graphics, Inc. not be used in advertising             =}
{= or publicity pertaining to distribution of the software without specific, =}
{= written prior permission.                                                 =}
{=                                                                           =}
{= THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"         =}
{= AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,          =}
{= INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR          =}
{= FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON              =}
{= GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,           =}
{= SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY             =}
{= KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,            =}
{= LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF         =}
{= THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN            =}
{= ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON            =}
{= ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE         =}
{= POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.                          =}
{=                                                                           =}
{= US Government Users Restricted Rights                                     =}
{= Use, duplication, or disclosure by the Government is subject to           =}
{= restrictions set forth in FAR 52.227.19(c)(2) or subparagraph             =}
{= (c)(1)(ii) of the Rights in Technical Data and Computer Software          =}
{= clause at DFARS 252.227-7013 and/or in similar or successor               =}
{= clauses in the FAR or the DOD or NASA FAR Supplement.                     =}
{= Unpublished-- rights reserved under the copyright laws of the             =}
{= United States.  Contractor/manufacturer is Silicon Graphics,              =}
{= Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.             =}
{=                                                                           =}
{= OpenGL(TM) is a trademark of Silicon Graphics, Inc.                       =}
{=============================================================================}
unit Nurbs;

interface

implementation

uses
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    DemoList,
    CmdLine;
    
const
    S_NUMPOINTS = 13;
    S_ORDER     = 3;
    S_NUMKNOTS  = (S_NUMPOINTS + S_ORDER);
    T_NUMPOINTS = 3;
    T_ORDER     = 3;
    T_NUMKNOTS  = (T_NUMPOINTS + T_ORDER);
    SQRT2       = 1.41421356237309504880; // Sqrt(2)

// initialized local data 

const
    sknots      : array[0..S_NUMKNOTS-1] of GLfloat =
        (-1.0, -1.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0,
          4.0,  5.0,  6.0, 7.0, 8.0, 9.0, 9.0, 9.0);

    tknots      : array[0..T_NUMKNOTS-1] of GLfloat =
        (1.0, 1.0, 1.0, 2.0, 2.0, 2.0);

    ctlpoints   : array[0..S_NUMPOINTS-1,0..T_NUMPOINTS-1,0..3] of GLfloat =
        (
            ((4.0,2.0,2.0,1.0),
             (4.0,1.6,2.5,1.0),
             (4.0,2.0,3.0,1.0)),
            ((5.0,4.0,2.0,1.0),
             (5.0,4.0,2.5,1.0),
             (5.0,4.0,3.0,1.0)),
            ((6.0,5.0,2.0,1.0),
             (6.0,5.0,2.5,1.0),
             (6.0,5.0,3.0,1.0)),
            ((SQRT2*6.0,SQRT2*6.0,SQRT2*2.0,SQRT2),
             (SQRT2*6.0,SQRT2*6.0,SQRT2*2.5,SQRT2),
             (SQRT2*6.0,SQRT2*6.0,SQRT2*3.0,SQRT2)),
            ((5.2,6.7,2.0,1.0),
             (5.2,6.7,2.5,1.0),
             (5.2,6.7,3.0,1.0)),
            ((SQRT2*4.0,SQRT2*6.0,SQRT2*2.0,SQRT2),
             (SQRT2*4.0,SQRT2*6.0,SQRT2*2.5,SQRT2),
             (SQRT2*4.0,SQRT2*6.0,SQRT2*3.0,SQRT2)),
            ((4.0,5.2,2.0,1.0),
             (4.0,4.6,2.5,1.0),
             (4.0,5.2,3.0,1.0)),
            ((SQRT2*4.0,SQRT2*6.0,SQRT2*2.0,SQRT2),
             (SQRT2*4.0,SQRT2*6.0,SQRT2*2.5,SQRT2),
             (SQRT2*4.0,SQRT2*6.0,SQRT2*3.0,SQRT2)),
            ((2.8,6.7,2.0,1.0),
             (2.8,6.7,2.5,1.0),
             (2.8,6.7,3.0,1.0)),
            ((SQRT2*2.0,SQRT2*6.0,SQRT2*2.0,SQRT2),
             (SQRT2*2.0,SQRT2*6.0,SQRT2*2.5,SQRT2),
             (SQRT2*2.0,SQRT2*6.0,SQRT2*3.0,SQRT2)),
            ((2.0,5.0,2.0,1.0),
             (2.0,5.0,2.5,1.0),
             (2.0,5.0,3.0,1.0)),
            ((3.0,4.0,2.0,1.0),
             (3.0,4.0,2.5,1.0),
             (3.0,4.0,3.0,1.0)),
            ((4.0,2.0,2.0,1.0),
             (4.0,1.6,2.5,1.0),
             (4.0,2.0,3.0,1.0))
        );

var
    theNurb : PGLUnurbsObj;

// Initialize material property, light source, lighting model,
// and depth buffer.
procedure   MyInit;
const
    mat_ambient     : array[0..3] of GLfloat = ( 1.0, 1.0, 1.0, 1.0 );
    mat_diffuse     : array[0..3] of GLfloat = ( 1.0, 0.2, 1.0, 1.0 );
    mat_specular    : array[0..3] of GLfloat = ( 1.0, 1.0, 1.0, 1.0 );
    mat_shininess   : array[0..0] of GLfloat = ( 50.0 );

    light0_position : array[0..3] of GLfloat = ( 1.0, 0.1, 1.0, 0.0 );
    light1_position : array[0..3] of GLfloat = ( -1.0, 0.1, 1.0, 0.0 );

    lmodel_ambient  : array[0..3] of GLfloat = ( 0.3, 0.3, 0.3, 1.0 );
begin
    glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, @light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, @light1_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);

    theNurb := gluNewNurbsRenderer();

    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef (4., 4.5, 2.5);
    glRotatef (220.0, 1.0, 0.0, 0.0);
    glRotatef (115.0, 0.0, 1.0, 0.0);
    glTranslatef (-4., -4.5, -2.5);

    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 
        S_NUMKNOTS, @sknots,
        T_NUMKNOTS, @tknots,
        4 * T_NUMPOINTS,
        4,
        @ctlpoints[0][0][0], 
        S_ORDER, T_ORDER,
        GL_MAP2_VERTEX_4);
    gluEndSurface(theNurb);

    glPopMatrix();
    glFlush();
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.5, 0.5, 0.8, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(7.0,4.5,4.0, 4.5,4.5,2.0, 6.0,-3.0,2.0);
end;

// Main Loop
// Open window with initial window size, title bar,
// RGBA display mode, and handle input events.
procedure   Demo;
begin
    InitGLUT;
    glutInitDisplayMode (GLUT_SINGLE or GLUT_RGB or GLUT_DEPTH);
    glutCreateWindow ('Nurbs');
    MyInit();
    glutReshapeFunc (MyReshape);
    glutDisplayFunc(Display);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',
                 'Nurbs',
                 'Shows a NURBS (Non-uniform rational B-splines) '+
                 'surface, shaped like a heart.',
                 Demo
                );
end.
