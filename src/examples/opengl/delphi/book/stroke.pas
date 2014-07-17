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
unit Stroke;

interface

implementation

uses
    MGLTypes,
    MGLGl,
    MGLGlut,
    DemoList,
    CmdLine;

const
    _PT     = 1;
    _STROKE = 2;
    _END    = 3;

type
    PCP = ^CP;
    CP  = record
        x, y    : GLfloat;
        _type   : int;
    end;

const
    dataA   : array[0..7] of CP =
        (
            (x: 0; y:  0; _type:     _PT),
            (x: 0; y:  9; _type:     _PT),
            (x: 1; y: 10; _type:     _PT),
            (x: 4; y: 10; _type:     _PT),
            (x: 5; y:  9; _type:     _PT),
            (x: 5; y:  0; _type: _STROKE),
            (x: 0; y:  5; _type:     _PT),
            (x: 5; y:  5; _type:    _END)
        );

    dataE   : array[0..5] of CP =
        (
            (x: 5; y:  0; _type:     _PT),
            (x: 0; y:  0; _type:     _PT),
            (x: 0; y: 10; _type:     _PT),
            (x: 5; y: 10; _type: _STROKE),
            (x: 0; y:  5; _type:     _PT),
            (x: 4; y:  5; _type:    _END)
        );

    dataP   : array[0..6] of CP =
        (
            (x: 0; y:  0; _type:     _PT),
            (x: 0; y: 10; _type:     _PT),
            (x: 4; y: 10; _type:     _PT),
            (x: 5; y:  9; _type:     _PT),
            (x: 5; y:  6; _type:     _PT),
            (x: 4; y:  5; _type:     _PT),
            (x: 0; y:  5; _type:    _END)
        );

    dataR   : array[0..8] of CP =
        (
            (x: 0; y:  0; _type:     _PT),
            (x: 0; y: 10; _type:     _PT),
            (x: 4; y: 10; _type:     _PT),
            (x: 5; y:  9; _type:     _PT),
            (x: 5; y:  6; _type:     _PT),
            (x: 4; y:  5; _type:     _PT),
            (x: 0; y:  5; _type: _STROKE),
            (x: 3; y:  5; _type:     _PT),
            (x: 5; y:  0; _type:    _END)
        );

    dataS   : array[0..11] of CP =
        (
            (x: 0; y:  1; _type:     _PT),
            (x: 1; y:  0; _type:     _PT),
            (x: 4; y:  0; _type:     _PT),
            (x: 5; y:  1; _type:     _PT),
            (x: 5; y:  4; _type:     _PT),
            (x: 4; y:  5; _type:     _PT),
            (x: 1; y:  5; _type:     _PT),
            (x: 0; y:  6; _type:     _PT),
            (x: 0; y:  9; _type:     _PT),
            (x: 1; y: 10; _type:     _PT),
            (x: 4; y: 10; _type:     _PT),
            (x: 5; y:  9; _type:    _END)
        );

// drawLetter() interprets the instructions from the array
// for that letter and renders the letter with line segments.
procedure   DrawLetter(l: PCP);
begin
    glBegin(GL_LINE_STRIP);
    while True do
    begin
        case l._type of
            _PT     : glVertex2fv(@l.x);
            _STROKE : begin
                        glVertex2fv(@l.x);
                        glEnd();
                        glBegin(GL_LINE_STRIP);
                      end;
            _END    : begin
                        glVertex2fv(@l.x);
                        glEnd();
                        glTranslatef(8.0, 0.0, 0.0);
                        Exit;
                      end;
        end;
        Inc(l);
    end;
end;

// Create a display list for each of 6 characters
procedure   MyInit;
var
    base    : GLuint;
begin
    glShadeModel (GL_FLAT);

    base := glGenLists (128);
    glListBase(base);
    glNewList(base+Ord('A'), GL_COMPILE); drawLetter(@dataA); glEndList();
    glNewList(base+Ord('E'), GL_COMPILE); drawLetter(@dataE); glEndList();
    glNewList(base+Ord('P'), GL_COMPILE); drawLetter(@dataP); glEndList();
    glNewList(base+Ord('R'), GL_COMPILE); drawLetter(@dataR); glEndList();
    glNewList(base+Ord('S'), GL_COMPILE); drawLetter(@dataS); glEndList();
    glNewList(base+Ord(' '), GL_COMPILE); glTranslatef(8.0, 0.0, 0.0); glEndList();
end;

const
    Test1 = 'A SPARE SERAPE APPEARS AS';
    Test2 = 'APES PREPARE RARE PEPPERS';

procedure   PrintStrokedString(const S: string);
begin
    glCallLists(Length(S), GL_BYTE, PGLbyte(PChar(s)));
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 30.0, 0.0);
    printStrokedString(test1);
    glPopMatrix();
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 13.0, 0.0);
    printStrokedString(test2);
    glPopMatrix();
    glFlush();
end;

procedure   Reshape(w, h: GLsizei); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
end;

// Main Loop
// Open window with initial window size, title bar,
// RGBA display mode, and handle input events.
procedure   Demo;
begin
    InitGLUT;
    glutInitDisplayMode (GLUT_SINGLE or GLUT_RGB);
    glutInitWindowSize (440, 120);
    glutCreateWindow ('Stroke');
    MyInit ();
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',
                 'Stroke',
                 'Demonstrates some characters of a '+
                 'stroke (vector) font.  The characters are represented '+
                 'by display lists, which are given numbers which '+
                 'correspond to the ASCII values of the characters. '+
                 'Use of glCallLists() is demonstrated.',
                 Demo
                );
end.
