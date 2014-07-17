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
unit MipMap;

interface

implementation

uses
    MGLTypes,
    MGLGLu,
    MGLGlut,
    MGLGL,
    DemoList,
    CmdLine;
    
var
    mipmapImage32   : array[0..31,0..31,0..2] of GLubyte;
    mipmapImage16   : array[0..15,0..15,0..2] of GLubyte;
    mipmapImage8    : array[0..07,0..07,0..2] of GLubyte;
    mipmapImage4    : array[0..03,0..03,0..2] of GLubyte;
    mipmapImage2    : array[0..01,0..01,0..2] of GLubyte;
    mipmapImage1    : array[0..00,0..00,0..2] of GLubyte;

procedure   MakeImages;
var
    i, j: int;
begin
    for i := 0 to 31 do
        for j := 0 to 31 do
        begin
            mipmapImage32[i,j,0] := 255;
            mipmapImage32[i,j,1] := 255;
            mipmapImage32[i,j,2] := 0;
        end;

    for i := 0 to 15 do
        for j := 0 to 15 do
        begin
            mipmapImage16[i,j,0] := 255;
            mipmapImage16[i,j,1] := 0;
            mipmapImage16[i,j,2] := 255;
        end;

    for i := 0 to 7 do
        for j := 0 to 7 do
        begin
            mipmapImage8[i,j,0] := 255;
            mipmapImage8[i,j,1] := 0;
            mipmapImage8[i,j,2] := 0;
        end;

    for i := 0 to 3 do
        for j := 0 to 3 do
        begin
            mipmapImage4[i,j,0] := 0;
            mipmapImage4[i,j,1] := 255;
            mipmapImage4[i,j,2] := 0;
        end;

    for i := 0 to 1 do
        for j := 0 to 1 do
        begin
            mipmapImage2[i,j,0] := 0;
            mipmapImage2[i,j,1] := 0;
            mipmapImage2[i,j,2] := 255;
        end;

    mipmapImage1[0,0,0] := 255;
    mipmapImage1[0,0,1] := 255;
    mipmapImage1[0,0,2] := 255;
end;

procedure   MyInit;
begin
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_FLAT);

    glTranslatef(0.0, 0.0, -3.6);
    MakeImages();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage32[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 1, 3, 16, 16, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage16[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 2, 3, 8, 8, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage8[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 3, 3, 4, 4, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage4[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 4, 3, 2, 2, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage2[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 5, 3, 1, 1, 0,
         GL_RGB, GL_UNSIGNED_BYTE, @mipmapImage1[0][0][0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
    GL_NEAREST_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 8.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(8.0, 8.0); glVertex3f(2000.0, 1.0, -6000.0);
    glTexCoord2f(8.0, 0.0); glVertex3f(2000.0, -1.0, -6000.0);
    glEnd();
    glFlush();
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*w/h, 1.0, 30000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
end;

procedure   Demo;
begin
    InitGLUT;
    glutInitDisplayMode (GLUT_SINGLE or GLUT_RGB or GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutCreateWindow ('MipMap');
    MyInit();
    glutReshapeFunc (MyReshape);
    glutDisplayFunc(Display);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',   
                 'MipMap',
                 'Demonstrates using mipmaps for texture maps. '+
                 'To overtly show the effect of mipmaps, each mipmap reduction '+
                 'level has a solidly colored, contrasting texture image. '+
                 'Thus, the quadrilateral which is drawn is drawn with several '+
                 'different colors.',
                 Demo
                );
end.
