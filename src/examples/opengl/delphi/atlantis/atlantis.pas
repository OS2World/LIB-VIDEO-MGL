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
unit  Atlantis;

interface

implementation

uses
  Windows,
  MGLGl,
  MGLTypes,
  MGLGlu,
  MGLGlut,
  Common,
  CmdLine,
  DemoList,
  Dolphin,
  Shark,
  Swim,
  Whale;

var
    moving: GLboolean;

procedure   InitFishs;
var
    i: int;
begin
    for i := 0 to NUM_SHARKS - 1 do
        with sharks[i] do
        begin
            x   := 70000.0 + Random(6000);
            y   := Random(6000);
            z   := Random(6000);
            psi := Random(360) - 180.0;
            v   := 1.0;
        end;

    with dolph do
    begin
        x       := 30000.0;
        y       := 0.0;
        z       := 6000.0;
        psi     := 90.0;
        theta   := 0.0;
        v       := 3.0;
    end;

    with momWhale do
    begin
        x       := 70000.0;
        y       := 0.0;
        z       := 0.0;
        psi     := 90.0;
        theta   := 0.0;
        v       := 3.0;
    end;

    with babyWhale do
    begin
        x       := 60000.0;
        y       := -2000.0;
        z       := -2000.0;
        psi     := 90.0;
        theta   := 0.0;
        v       := 3.0;
    end;
end;

procedure   Init;
const
    ambient             : array[0..3] of float = (0.1, 0.1, 0.1, 1.0);
    diffuse             : array[0..3] of float = (1.0, 1.0, 1.0, 1.0);
    position            : array[0..3] of float = (0.0, 1.0, 0.0, 0.0);
    mat_shininess       : array[0..0] of float = (90.0);
    mat_specular        : array[0..3] of float = (0.8, 0.8, 0.8, 1.0);
    mat_diffuse         : array[0..3] of float = (0.46, 0.66, 0.795, 1.0);
    mat_ambient         : array[0..3] of float = (0.0, 0.1, 0.2, 1.0);
    lmodel_ambient      : array[0..3] of float = (0.4, 0.4, 0.4, 1.0);
    lmodel_localviewer  : array[0..0] of float = (0.0);

    map1                : array[0..3] of GLfloat = (0.0, 0.0, 0.0, 0.0);
    map2                : array[0..3] of GLfloat = (0.0, 0.0, 0.0, 0.0);
begin
    glFrontFace(GL_CW);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, @ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, @position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, @lmodel_localviewer);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, @mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, @mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, @mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, @mat_ambient);

    InitFishs;

    glClearColor(0.0, 0.5, 0.9, 0.0);
end;

procedure   Reshape(width, height: int); cdecl;
begin
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity;
    gluPerspective(400.0, 2.0, 1.0, 2000000.0);
    glMatrixMode(GL_MODELVIEW);
end;

procedure   Animate; cdecl;
var
    i: int;
begin
    for i := 0 to NUM_SHARKS - 1 do
    begin
        SharkPilot(@sharks[i]);
        SharkMiss(i);
    end;

    WhalePilot(@dolph);
    dolph.phi := dolph.phi + 1;
    glutPostRedisplay;
    WhalePilot(@momWhale);
    momWhale.phi := momWhale.phi + 1;
    WhalePilot(@babyWhale);
    babyWhale.phi := babyWhale.phi + 1;
end;

procedure   Key(key: uchar; x, y: int); cdecl;
begin
    case Chr(key) of
        // Esc will quit
        #27 : glutExit;
        // space will advance frame
        ' ' : if not moving then Animate;
    end;
end;

procedure   Display; cdecl;
var
    i: int;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    for i := 0 to NUM_SHARKS - 1 do
    begin
        glPushMatrix;
        FishTransform(@sharks[i]);
        DrawShark(@sharks[i]);
        glPopMatrix;
    end;

    glPushMatrix;
    FishTransform(@dolph);
    DrawDolphin(@dolph);
    glPopMatrix;

    glPushMatrix;
    FishTransform(@momWhale);
    DrawWhale(@momWhale);
    glPopMatrix;

    glPushMatrix;
    FishTransform(@babyWhale);
    glScalef(0.45, 0.45, 0.3);
    DrawWhale(@babyWhale);
    glPopMatrix;

    glutSwapBuffers;
end;

procedure   Visible(state: int); cdecl;
begin
    if state = GLUT_VISIBLE then
    begin
        if moving then
            glutIdleFunc(Animate)
    end
    else
        if moving then
            glutIdleFunc(nil);
end;

procedure   MenuSelect(value: int); cdecl;
begin
    case value of
        1: begin
            moving := GL_TRUE;
            glutIdleFunc(Animate);
           end; 
        2: begin
            moving := GL_FALSE;
            glutIdleFunc(nil);
           end;
        3: glutExit;
    end;
end;

procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(500, 250);
    glutInitDisplayMode(GLUT_RGB or GLUT_DOUBLE or GLUT_DEPTH);
    glutCreateWindow('GLUT Atlantis Demo');
    Init;
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Key);
    moving := GL_TRUE;
    glutIdleFunc(Animate);
    glutVisibilityFunc(Visible);
    glutCreateMenu(MenuSelect);
    glutAddMenuEntry('Start motion', 1);
    glutAddMenuEntry('Stop motion', 2);
    glutAddMenuEntry('Quit', 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop;
end;

initialization
    RegisterDemo('Misc','Atlantis','*** No description yet',Demo);    
end.
