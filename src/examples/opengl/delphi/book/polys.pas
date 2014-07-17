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
unit Polys;

interface

implementation

uses
    MGLGl,
    MGLGlut,
    DemoList,
    CmdLine;

procedure   Display; cdecl;
const
    fly : array[0..127] of GLubyte =
        (
            $00, $00, $00, $00, $00, $00, $00, $00,
            $03, $80, $01, $C0, $06, $C0, $03, $60, $04, $60, $06, $20,
            $04, $30, $0C, $20, $04, $18, $18, $20, $04, $0C, $30, $20,
            $04, $06, $60, $20, $44, $03, $C0, $22, $44, $01, $80, $22,
            $44, $01, $80, $22, $44, $01, $80, $22, $44, $01, $80, $22,
            $44, $01, $80, $22, $44, $01, $80, $22, $66, $01, $80, $66,
            $33, $01, $80, $CC, $19, $81, $81, $98, $0C, $C1, $83, $30,
            $07, $e1, $87, $e0, $03, $3f, $fc, $c0, $03, $31, $8c, $c0,
            $03, $33, $cc, $c0, $06, $64, $26, $60, $0c, $cc, $33, $30,
            $18, $cc, $33, $18, $10, $c4, $23, $08, $10, $63, $C6, $08,
            $10, $30, $0c, $08, $10, $18, $18, $08, $10, $00, $00, $08
        );

    halftone : array[0..127] of GLubyte =
        (
            $AA, $AA, $AA, $AA, $55, $55, $55, $55, $AA, $AA, $AA, $AA,
            $55, $55, $55, $55, $AA, $AA, $AA, $AA, $55, $55, $55, $55,
            $AA, $AA, $AA, $AA, $55, $55, $55, $55, $AA, $AA, $AA, $AA,
            $55, $55, $55, $55, $AA, $AA, $AA, $AA, $55, $55, $55, $55,
            $AA, $AA, $AA, $AA, $55, $55, $55, $55, $AA, $AA, $AA, $AA,
            $55, $55, $55, $55, $AA, $AA, $AA, $AA, $55, $55, $55, $55,
            $AA, $AA, $AA, $AA, $55, $55, $55, $55, $AA, $AA, $AA, $AA,
            $55, $55, $55, $55, $AA, $AA, $AA, $AA, $55, $55, $55, $55,
            $AA, $AA, $AA, $AA, $55, $55, $55, $55, $AA, $AA, $AA, $AA,
            $55, $55, $55, $55, $AA, $AA, $AA, $AA, $55, $55, $55, $55,
            $AA, $AA, $AA, $AA, $55, $55, $55, $55
        );
begin
    glClear (GL_COLOR_BUFFER_BIT);

    // draw all polygons in white
    glColor3f (1.0, 1.0, 1.0);

    // draw one solid, unstippled rectangle,        
    // then two stippled rectangles     
    glRectf (25.0, 25.0, 125.0, 125.0);
    glEnable (GL_POLYGON_STIPPLE);
    glPolygonStipple (@fly);
    glRectf (125.0, 25.0, 225.0, 125.0);
    glPolygonStipple (@halftone);
    glRectf (225.0, 25.0, 325.0, 125.0);
    glDisable (GL_POLYGON_STIPPLE);

    glFlush ();
end;

procedure   MyInit;
begin 
    // clear background to black
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
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
    glutInitWindowSize (350, 150);
    glutCreateWindow ('Polys');
    MyInit ();
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
end;

initialization
    RegisterDemo(
                 'Book',
                 'Polys',
                 'Demonstrates polygon stippling',
                 Demo
                );
end.
