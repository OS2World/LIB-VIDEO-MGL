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
unit Checker;

interface

implementation

uses
    MGLTypes,
    MGLGl,
    MGLGlut,
    MGLGlu,
    CmdLine,
    DemoList;

// Create checkerboard texture
const
    checkImageWidth     = 64;
    checkImageHeight    = 64;

var
    checkImage  : array[0..checkImageWidth-1,0..checkImageHeight-1,0..2] of GLubyte ;

procedure   MakeCheckImage;
var
    i, j, c: int;
begin
    for i := 0 to checkImageWidth - 1 do
        for j := 0 to checkImageHeight - 1 do
        begin
            c := Ord(((i and $8) = 0) xor ((j and $8) = 0)) * 255;
            checkImage[i,j,0] := GLubyte(c);
            checkImage[i,j,1] := GLubyte(c);
            checkImage[i,j,2] := GLubyte(c);
        end;
end;

procedure   MyInit;
begin
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    MakeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight,
                 0, GL_RGB, GL_UNSIGNED_BYTE, @checkImage[0][0][0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();
    glutSwapBuffers();
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*w/h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
end;

procedure   Demo;
begin
    InitGLUT;
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGB or GLUT_DEPTH);
    glutCreateWindow('Checker');
    MyInit();
    glutReshapeFunc (MyReshape);
    glutDisplayFunc(Display);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',
                 'Checker',
                 'Texture maps a checkerboard image onto two rectangles, ' +
                 'clamps the texture, if the texture coordinates fall outside ' +
                 '0.0 and 1.0',
                 Demo
                );
end.
