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
unit Surface;

interface

implementation

uses
    MGLTypes,
    MGLGl,
    MGLGlut,
    MGLGlu,
    DemoList,
    CmdLine;

var
    ctlpoints   : array[0..3,0..3,0..2] of GLfloat;
    showPoints  : int = 0;

    theNurb     : PGLUnurbsObj;

// Initializes the control points of the surface to a small hill.
// The control points range from -3 to +3 in x, y, and z
procedure   InitSurface;
var
    u, v: int;
begin
    for u := 0 to 3 do
        for v := 0 to 3 do
        begin
            ctlpoints[u][v][0] := 2.0*(u - 1.5);
            ctlpoints[u][v][1] := 2.0*(v - 1.5);

            if ((u = 1) or (u = 2)) and ((v = 1) or (v = 2)) then
                ctlpoints[u][v][2] := 7.0
            else
                ctlpoints[u][v][2] := -3.0;
        end;
end;
            
// Initialize material property and depth buffer.
procedure   MyInit;
const
    mat_diffuse     : array[0..3] of GLfloat = ( 0.7, 0.7, 0.7, 1.0 );
    mat_specular    : array[0..3] of GLfloat = ( 1.0, 1.0, 1.0, 1.0 );
    mat_shininess   : array[0..0] of GLfloat = ( 100.0 );
begin    
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    InitSurface();

    theNurb := gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
end;

procedure   Display; cdecl;
const
    knots   : array[0..7] of GLfloat = (0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0);
var
    i, j    : int;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(330.0, 1.0, 0.0, 0.0);
    glScalef (0.25, 0.25, 0.25);

    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb,
        8, @knots,
        8, @knots,
        4 * 3,
        3,
        @ctlpoints[0][0][0],
        4, 4,
        GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);

    if showPoints <> 0 then
    begin
        glPointSize(5.0);
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        for i := 0 to 3 do
            for j := 0 to 3 do
                glVertex3f(ctlpoints[i][j][0], ctlpoints[i][j][1], ctlpoints[i][j][2]);
        glEnd();
        glEnable(GL_LIGHTING);
    end;

    glPopMatrix();
    glFlush();
end;

procedure   Reshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, w/h, 3.0, 8.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
end;

procedure   Menu(value: int); cdecl;
begin
    showPoints := value;
    glutPostRedisplay();
end;

// Main Loop
procedure   Demo;
begin
    InitGLUT;
    glutInitDisplayMode(GLUT_SINGLE or GLUT_RGB);
    glutCreateWindow('Surface');
    MyInit();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutCreateMenu(Menu);
    glutAddMenuEntry('Show control points', 1);
    glutAddMenuEntry('Hide control points', 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',   
                 'Surface',
                 'Draws a NURBS surface in the shape of a symmetrical hill.',
                 Demo
                );
end.
