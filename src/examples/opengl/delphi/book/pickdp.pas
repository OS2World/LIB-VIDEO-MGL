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
unit PickDp;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    CmdLine,
    DemoList;

procedure   MyInit;
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDepthRange(0.0, 1.0);  // The default z mapping
end;

// The three rectangles are drawn.  In selection mode,
// each rectangle is given the same name.  Note that
// each rectangle is drawn with a different z value.
procedure   DrawRects(mode: GLenum);
begin
    if mode = GL_SELECT then
        glLoadName(1);
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3i(2, 0, 0);
    glVertex3i(2, 6, 0);
    glVertex3i(6, 6, 0);
    glVertex3i(6, 0, 0);
    glEnd();
    if mode = GL_SELECT then
        glLoadName(2);
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3i(3, 2, -1);
    glVertex3i(3, 8, -1);
    glVertex3i(8, 8, -1);
    glVertex3i(8, 2, -1);
    glEnd();
    if mode = GL_SELECT then
        glLoadName(3);
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3i(0, 2, -2);
    glVertex3i(0, 7, -2);
    glVertex3i(5, 7, -2);
    glVertex3i(5, 2, -2);
    glEnd();
end;

// processHits() prints out the contents of the
// selection array.
procedure   ProcessHits(hits: GLint; buffer: PGLuint);
var
    i, j    : uint;
    names   : GLuint;
    ptr     : PGLuint;
begin
    // printf("hits = %d\n", hits);
    ptr := buffer;
    for i := 0 to hits - 1 do
    begin
        names := ptr^;
        // printf(" number of names for hit = %d\n", names);
        Inc(ptr);
        // printf("  z1 is %g;", (float) *ptr/0xffffffff);
        Inc(ptr);
        // printf(" z2 is %g\n", (float) *ptr/0xffffffff);
        Inc(ptr);
        // printf("   the name is ");
        for j := 0 to names - 1 do
        begin
            // printf("%d ", *ptr);
            Inc(ptr);
        end;
        // printf("\n");
    end;
end;

// PickRects() sets up selection mode, name stack,
// and projection matrix for picking.  Then the objects
// are drawn.

const
    BUFSIZE = 512;

procedure   PickRects(button: int; state: int; x, y: int); cdecl;
var
    selectBuf   : array[0..BUFSIZE-1] of GLuint; 
    hits        : GLint;
    viewport    : array[0..3] of GLint;
begin
    if (button <> GLUT_LEFT_BUTTON) or (state <> GLUT_DOWN) then
        Exit;

    glGetIntegerv(GL_VIEWPORT, @viewport);

    glSelectBuffer(BUFSIZE, @selectBuf);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(-1);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // create 5x5 pixel picking region near cursor location
    gluPickMatrix(x, viewport[3] - y, 5.0, 5.0, @viewport);
    glOrtho(0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
    drawRects(GL_SELECT);
    glPopMatrix();
    glFlush();

    hits := glRenderMode(GL_RENDER);
    processHits(hits, @selectBuf);
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    drawRects(GL_RENDER);
    glutSwapBuffers();
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
end;

// Main Loop
// Open window with initial window size, title bar,
// RGBA display mode, depth buffer, and handle input events.
procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(200, 200);
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGB or GLUT_DEPTH);
    glutCreateWindow('PickDepth');
    MyInit();
    glutMouseFunc(PickRects);
    glutReshapeFunc(MyReshape);
    glutDisplayFunc(Display);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',
                 'PickDepth',
                 'Picking is demonstrated in this program. In '+
                 'rendering mode, three overlapping rectangles are '+
                 'drawn.  When the left mouse button is pressed, '+
                 'selection mode is entered with the picking matrix. '+
                 'Rectangles which are drawn under the cursor position '+
                 'are "picked."  Pay special attention to the depth '+
                 'value range, which is returned.',
                 Demo
                );
end.
