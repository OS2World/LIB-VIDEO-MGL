//
// Copyright (c) Mark J. Kilgard, 1995
//
// (c) Copyright 1993, Silicon Graphics, Inc.
// ALL RIGHTS RESERVED
// Permission to use, copy, modify, and distribute this software for
// any purpose and without fee is hereby granted, provided that the above
// copyright notice appear in all copies and that both the copyright notice
// and this permission notice appear in supporting documentation, and that
// the name of Silicon Graphics, Inc. not be used in advertising
// or publicity pertaining to distribution of the software without specific,
// written prior permission.
//
// THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
// AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
// INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
// GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
// SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
// KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
// LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
// THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
// POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
//
// US Government Users Restricted Rights
// Use, duplication, or disclosure by the Government is subject to
// restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
// (c)(1)(ii) of the Rights in Technical Data and Computer Software
// clause at DFARS 252.227-7013 and/or in similar or successor
// clauses in the FAR or the DOD or NASA FAR Supplement.
// Unpublished-- rights reserved under the copyright laws of the
// United States.  Contractor/manufacturer is Silicon Graphics,
// Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
//
// OpenGL(TM) is a trademark of Silicon Graphics, Inc.
//
unit Track;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGlut,
    MGLGl,
    MGLGlu,
    Math,
    Objects,
    SysUtils,
    CmdLine,
    DemoList,
    a,
    b,
    d,
    e,
    f,
    h,
    i,
    m,
    n,
    o,
    p,
    r,
    s,
    t,
    d_logo,
    d_lamp,
    d_logos,
    d_logol;

const
    X       = 0;
    Y       = 1;
    Z       = 2;

    DEG     = Pi/180.0;
    RAD     = 180.0/Pi;

var
    move_speed          : float ;           // Spline distance per second

    multisample         : bool = false;     // Antialias polygons?
    doublebuffer        : bool = true ;     // Doublebuffer?
    fullspeed           : bool = false;     // Full speed?


const
    SPEED_SLOW          = 0.2;              // Spline distances per second
    SPEED_MEDIUM        = 0.4;
    SPEED_FAST          = 0.7;
    SPEED_SUPER_FAST    = 1.0;

    O_NOMS              = 7;
    O_4MS               = 8;
    O_8MS               = 9;
    O_16MS              = 10;

const
    RGBA_SB_attributes      : int = GLUT_SINGLE or GLUT_RGBA or GLUT_DEPTH;

    RGBA_SB_ST_attributes   : int = GLUT_SINGLE or GLUT_RGBA or GLUT_DEPTH or GLUT_STEREO;

    RGBA_DB_attributes      : int = GLUT_DOUBLE or GLUT_RGBA or GLUT_DEPTH;

    RGBA_DB_ST_attributes   : int = GLUT_DOUBLE or GLUT_RGBA or GLUT_DEPTH or GLUT_STEREO;

const
    idmat   : array[0..3,0..3] of float =
        (
            ( 1.0, 0.0, 0.0, 0.0 ),
            ( 0.0, 1.0, 0.0, 0.0 ),
            ( 0.0, 0.0, 1.0, 0.0 ),
            ( 0.0, 0.0, 0.0, 1.0 )
        );

    light1_ambient              : array[0..3] of float = ( 0.0, 0.0, 0.0, 1.0 );
    light1_lcolor               : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    light1_position             : array[0..3] of float = ( 0.0, 1.0, 0.0, 0.0 );

    light2_ambient              : array[0..3] of float = (  0.0, 0.0, 0.0, 1.0 );
    light2_lcolor               : array[0..3] of float = (  0.3, 0.3, 0.5, 1.0 );
    light2_position             : array[0..3] of float = ( -1.0, 0.0, 0.0, 0.0 );

    light3_ambient              : array[0..3] of float = ( 0.2,  0.2, 0.2, 1.0 );
    light3_lcolor               : array[0..3] of float = ( 0.2,  0.2, 0.2, 1.0 );
    light3_position             : array[0..3] of float = ( 0.0, -1.0, 0.0, 0.0 );

    lmodel_LVW                  : array[0..0] of float = ( 0.0 );
    lmodel_ambient              : array[0..3] of float = ( 0.3, 0.3, 0.3, 1.0 );
    lmodel_TWO                  : array[0..0] of float = ( Ord(GL_TRUE) );

    mat_logo_ambient            : array[0..3] of float = ( 0.1, 0.1, 0.1, 1.0 );
    mat_logo_diffuse            : array[0..3] of float = ( 0.5, 0.4, 0.7, 1.0 );
    mat_logo_specular           : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    mat_logo_shininess          : array[0..0] of float = ( 30.0 );

    mat_holder_base_ambient     : array[0..3] of float = ( 0.0, 0.0, 0.0, 1.0 );
    mat_holder_base_diffuse     : array[0..3] of float = ( 0.6, 0.6, 0.6, 1.0 );
    mat_holder_base_specular    : array[0..3] of float = ( 0.8, 0.8, 0.8, 1.0 );
    mat_holder_base_shininess   : array[0..0] of float = ( 30.0 );

    mat_holder_rings_ambient    : array[0..3] of float = ( 0.0, 0.0, 0.0, 1.0 );
    mat_holder_rings_diffuse    : array[0..3] of float = ( 0.9, 0.8, 0.0, 1.0 );
    mat_holder_rings_specular   : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    mat_holder_rings_shininess  : array[0..0] of float = ( 30.0 );

    mat_hemisphere_ambient      : array[0..3] of float = ( 0.0, 0.0, 0.0, 1.0 );
    mat_hemisphere_diffuse      : array[0..3] of float = ( 1.0, 0.2, 0.2, 1.0 );
    mat_hemisphere_specular     : array[0..3] of float = ( 0.5, 0.5, 0.5, 1.0 );
    mat_hemisphere_shininess    : array[0..0] of float = ( 20.0 );

var
    stipple : array[0..32*32-1] of GLubyte;

type
    vector          = array[0..2] of float;
    vector4         = array[0..3] of float;
    parameter       = array[0..3] of vector;
    pparameter      = ^parameter;
    pvector         = ^vector;
    pvector4        = ^vector4;

procedure   initialize; forward;
procedure   resize_window(w, h: int); cdecl; forward;
procedure   build_table; forward;
function    calc_spline_params(ctl_pts: pvector; n: int): pparameter; forward;
procedure   calc_spline(var v: vector; params: pparameter; current_time: float); forward;
procedure   normalize(var v: vector); forward;
function    dot(var v1, v2: vector): float; forward;

var
    post_idle   : bool = false;

procedure   idle; cdecl; forward;
procedure   do_post_idle; cdecl; forward;
procedure   display; cdecl; forward;
procedure   mouse(b, s: int; x, y: int); cdecl; forward;
procedure   keyboard(c: uchar; x, y: int); cdecl; forward;
procedure   vis(v: int); cdecl; forward;

procedure   draw_table; forward;
procedure   draw_under_table; forward;

procedure   init_materials;
var
    x, y    : int;
begin
    // Stipple pattern
    for y := 0 to 31 do
        for x := 0 to 3 do
            if y mod 2 <> 0 then
                stipple[y * 4 + x] := $AA
            else
                stipple[y * 4 + x] := $55;

    glNewList(MAT_LOGO, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_logo_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_logo_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_logo_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_logo_shininess);
    glEndList();

    glNewList(MAT_HOLDER_BASE, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_holder_base_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_holder_base_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_holder_base_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_holder_base_shininess);
    glEndList();

    glNewList(MAT_HOLDER_RINGS, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_holder_rings_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_holder_rings_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_holder_rings_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_holder_rings_shininess);
    glEndList();

    glNewList(MAT_HEMISPHERE, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_hemisphere_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_hemisphere_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_hemisphere_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_hemisphere_shininess);
    glEndList();
end;

procedure   init_lights;
const
    ambient     : array[0..3] of float = ( 0.1, 0.1, 0.1, 1.0 );
    diffuse     : array[0..3] of float = ( 0.5, 1.0, 1.0, 1.0 );
    position    : array[0..3] of float = ( 90.0, 90.0, 150.0, 0.0 );
begin
    glLightfv(GL_LIGHT0, GL_AMBIENT, @ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, @position);

    glLightfv (GL_LIGHT1, GL_AMBIENT, @light1_ambient);
    glLightfv (GL_LIGHT1, GL_SPECULAR, @light1_lcolor);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, @light1_lcolor);
    glLightfv (GL_LIGHT1, GL_POSITION, @light1_position);

    glLightfv (GL_LIGHT2, GL_AMBIENT, @light2_ambient);
    glLightfv (GL_LIGHT2, GL_SPECULAR, @light2_lcolor);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, @light2_lcolor);
    glLightfv (GL_LIGHT2, GL_POSITION, @light2_position);

    glLightfv (GL_LIGHT3, GL_AMBIENT, @light3_ambient);
    glLightfv (GL_LIGHT3, GL_SPECULAR, @light3_lcolor);
    glLightfv (GL_LIGHT3, GL_DIFFUSE, @light3_lcolor);
    glLightfv (GL_LIGHT3, GL_POSITION, @light3_position);

    glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, @lmodel_LVW);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);
end;

var
    dev, val        : short;

    current_time    : float = 0.0;
    hold_time       : float = 0.0;  // Used when auto-running

    tmplight        : array[0..4] of float = (GL_POSITION, 0.0, 0.0, 0.0, 0.0);

    tv              : array[0..3,0..3] of GLfloat =
        (
            ( 1.0, 0.0, 0.0, 0.0 ),
            ( 0.0, 1.0, 0.0, -1.0 ),
            ( 0.0, 0.0, 1.0, 0.0 ),
            ( 0.0, 0.0, 0.0, 0.0 )
        );

const
    TABLERES            = 12;

var
    pcr, pcg, pcb, pca  : float;

    table_points        : array[0..TABLERES,0..TABLERES] of vector;
    tablecolors         : array[0..TABLERES,0..TABLERES] of int;

    paper_points        : array[0..3] of vector =
        (
            ( -0.8, 0.0, 0.4 ),
            ( -0.2, 0.0, -1.4 ),
            ( 1.0, 0.0, -1.0 ),
            ( 0.4, 0.0, 0.8 )
        );

const
    TIME            = 15 ;
    START_TIME      = 0.6;

const
    light_pos_ctl   : array[0..15] of vector =
        (
            ( 0.0, 1.8, 0.0 ),
            ( 0.0, 1.8, 0.0 ),
            ( 0.0, 1.6, 0.0 ),

            ( 0.0, 1.6, 0.0 ),
            ( 0.0, 1.6, 0.0 ),
            ( 0.0, 1.6, 0.0 ),
            ( 0.0, 1.4, 0.0 ),

            ( 0.0, 1.3, 0.0 ),
            ( -0.2, 1.5, 2.0 ),
            ( 0.8, 1.5, -0.4 ),
            ( -0.8, 1.5, -0.4 ),

            ( 0.8, 2.0, 1.0 ),
            ( 1.8, 5.0, -1.8 ),
            ( 8.0, 10.0, -4.0 ),
            ( 8.0, 10.0, -4.0 ),
            ( 8.0, 10.0, -4.0 )
        );

    logo_pos_ctl    : array[0..15] of vector =
        (
            ( 0.0, -0.5, 0.0 ),

            ( 0.0, -0.5, 0.0 ),
            ( 0.0, -0.5, 0.0 ),

            ( 0.0, -0.5, 0.0 ),
            ( 0.0, -0.5, 0.0 ),
            ( 0.0, -0.5, 0.0 ),
            ( 0.0, 0.0, 0.0 ),

            ( 0.0, 0.6, 0.0 ),
            ( 0.0, 0.75, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),

            ( 0.0, 0.5, 0.0 ),
            ( 0.0, 0.5, 0.0 ),
            ( 0.0, 0.5, 0.0 ),
            ( 0.0, 0.5, 0.0 ),
            ( 0.0, 0.5, 0.0 )
        );

    logo_rot_ctl    : array[0..14] of vector =
        (
            ( 0.0, 0.0, -18.4 ),

            ( 0.0, 0.0, -18.4 ),
            ( 0.0, 0.0, -18.4 ),

            ( 0.0, 0.0, -18.4 ),
            ( 0.0, 0.0, -18.4 ),
            ( 0.0, 0.0, -18.4 ),
            ( 0.0, 0.0, -18.4 ),
            ( 0.0, 0.0, -18.4 ),

        {   ( 90.0, 0.0, -90.0 ),
            ( 180.0, 180.0, 90.0 ), }
            ( 240.0, 360.0, 180.0 ),
            ( 90.0, 180.0, 90.0 ),

            ( 11.9, 0.0, -18.4 ),
            ( 11.9, 0.0, -18.4 ),
            ( 11.9, 0.0, -18.4 ),
            ( 11.9, 0.0, -18.4 ),
            ( 11.9, 0.0, -18.4 )
        );

    view_from_ctl   : array[0..15] of vector =
        (
            ( -1.0, 1.0, -4.0 ),

            ( -1.0, -3.0, -4.0 ),   // 0
            ( -3.0, 1.0, -3.0 ),    // 1

            ( -1.8, 2.0, 5.4 ),     // 2
            ( -0.4, 2.0, 1.2 ),     // 3
            ( -0.2, 1.5, 0.6 ),     // 4
            ( -0.2, 1.2, 0.6 ),     // 5

            ( -0.8, 1.0, 2.4 ),     // 6
            ( -1.0, 2.0, 3.0 ),     // 7
            ( 0.0, 4.0, 3.6 ),      // 8
            ( -0.8, 4.0, 1.2 ),     // 9

            ( -0.2, 3.0, 0.6 ),     // 10
            ( -0.1, 2.0, 0.3 ),     // 11
            ( -0.1, 2.0, 0.3 ),     // 12
            ( -0.1, 2.0, 0.3 ),     // 13
            ( -0.1, 2.0, 0.3 )      // 13
        );

    view_to_ctl     : array[0..15] of vector =
        (
            ( -1.0, 1.0, 0.0 ),

            ( -1.0, -3.0, 0.0 ),
            ( -1.0, 1.0, 0.0 ),

            ( 0.1, 0.0, -0.3 ),
            ( 0.1, 0.0, -0.3 ),
            ( 0.1, 0.0, -0.3 ),
            ( 0.0, 0.2, 0.0 ),

            ( 0.0, 0.6, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),

            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 ),
            ( 0.0, 0.8, 0.0 )
        );

var
    view_from, view_to,
    logo_pos, logo_rot  : vector;
    light_pos           : vector4;

var
    view_from_spline,
    view_to_spline,
    light_pos_spline,
    logo_pos_spline,
    logo_rot_spline     : pparameter;

var
    a3, a4              : double;

// procedure   ideas_usage;
// begin
//     Writeln('Usage: ideas [-a] [-m] [-d] -s[1-4}');
//     Writeln('Press ESC to quit, 1-4 to control speed, any other key');
//     Writeln('to pause');
// end;

var
    auto_run        : bool;         // If set, then automatically run forever
    new_speed       : float;        // Set new animation speed?
    timejerk        : bool;         // Set to indicate time jerked! (menu pulled down)
    paused          : bool = false; // Paused?
    right           : bool = false; // Draw right eye?
    resetclock      : bool ;        // Reset the clock?
    timeoffset      : float ;       // Used to compute timing

type
    TimeValue       = float;

var    
    start           : TimeValue;

procedure   idle; cdecl; 
begin
    if current_time > (TIME*1.0)-3.0 then
    begin
        if auto_run then
        begin
            hold_time := hold_time + current_time - (TIME - 3.001);
            if hold_time > 3.0 then // 3 second hold
            begin
                hold_time   := 0.0;
                resetclock  := true;
            end;
        end
        else
            if not resetclock then
                glutIdleFunc(nil);
        current_time := (TIME*1.0)-3.001;
    end
    else
        post_idle := true;
    glutPostRedisplay();
end;

procedure   mouse(b, s: int; x, y: int);
begin
    if (b = GLUT_LEFT_BUTTON) and (s = GLUT_DOWN) then
    begin
        resetclock  := true;
        paused      := false;
        glutIdleFunc(idle);
    end;
end;

procedure   keyboard(c: uchar; x, y: int);
begin
    case chr(c) of
        #27 : glutExit();
        '1' : new_speed := SPEED_SLOW;
        '2' : new_speed := SPEED_MEDIUM;
        '3' : new_speed := SPEED_FAST;
        '4' : new_speed := SPEED_SUPER_FAST;
    else
        if paused then
            timejerk := true;
        paused := not paused;
        if paused then
            glutIdleFunc(nil)
        else
            glutIdleFunc(idle);
    end;
end;

procedure   vis(v: int); cdecl;
begin
    if v = GLUT_VISIBLE then
    begin
        if not paused then
            glutIdleFunc(idle);
        do_post_idle();
    end
    else
        if not paused then
            glutIdleFunc(nil);
end;

procedure   display; cdecl;
var
    _x, _y, _z, c : float;
begin
    calc_spline(view_from, view_from_spline, current_time);
    calc_spline(view_to, view_to_spline, current_time);
    calc_spline(pvector(@light_pos)^, light_pos_spline, current_time);
    light_pos[3]    := 0.0;
    calc_spline(logo_pos, logo_pos_spline, current_time);
    calc_spline(logo_rot, logo_rot_spline, current_time);

    tmplight[1]     := light_pos[Trunc(X)] - logo_pos[Trunc(X)];
    tmplight[2]     := light_pos[Trunc(Y)] - logo_pos[Trunc(Y)];
    tmplight[3]     := light_pos[Trunc(Z)] - logo_pos[Trunc(Z)];

    glNewList(LIGHT_TMP, GL_COMPILE);
    glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmplight[0]);
    glEndList();

    tv[0][0]        := light_pos[Trunc(Y)];
    tv[1][1]        := light_pos[Trunc(Y)];
    tv[2][2]        := light_pos[Trunc(Y)];

    glColor3ub(0,  0,  0);
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    //
    // SHADOW
    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(view_from[Trunc(X)], view_from[Trunc(Y)], view_from[Trunc(Z)],
              view_to[Trunc(X)], view_to[Trunc(Y)], view_to[Trunc(Z)],
              0.0, 1.0, 0.0);

    if view_from[Trunc(Y)] > 0.0 then
        draw_table();

    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    if logo_pos[Trunc(Y)] < 0.0 then
    begin
        if logo_pos[Trunc(Y)] > -0.33 then
        begin
            // We're emerging from the table
            c   := 1.0 - (logo_pos[Trunc(Y)]) / -0.33;
            pca := pca / 4.0;
            glColor3ub(Trunc(128.0*(1.0-c)*0.5 + 255.0*pca*c),
                       Trunc(102.0*(1.0-c)*0.5 + 255.0*pca*c),
                       Trunc(179.0*(1.0-c)*0.5 + 200.0*pca*c));
        end
        else
            // Still under table
            glColor3ub(128 div 2,  102 div 2,  179 div 2);

        glPushMatrix();
        glScalef(0.04,  0.0,  0.04);
        glRotatef(0.1 * (-900), 1.0, 0.0, 0.0);
        glRotatef(0.1 * (Trunc(10.0*logo_rot[Trunc(Z)])), 0.0, 0.0, 1.0);
        glRotatef(0.1 * (Trunc(10.0*logo_rot[Trunc(Y)])), 0.0, 1.0, 0.0);
        glRotatef(0.1 * (Trunc(10.0*logo_rot[Trunc(X)])), 1.0, 0.0, 0.0);
        glRotatef(0.1 * (353), 1.0, 0.0, 0.0);
        glRotatef(0.1 * (450), 0.0, 1.0, 0.0);
        draw_logo_shadow();
        glPopMatrix();
    end;

    if logo_pos[Trunc(Y)] > 0.0 then
    begin
        glPushMatrix();
        if logo_pos[Trunc(Y)] < 0.33 then
        begin
            pca := pca / 4.0;
            c   := 1.0 - (logo_pos[Trunc(Y)])/0.33;
            glColor3ub(Trunc(255.0*pca*c),
                       Trunc(255.0*pca*c),
                       Trunc(200.0*pca*c));
        end
        else
            glColor3ub(0, 0, 0);

        glTranslatef(light_pos[Trunc(X)],  light_pos[Trunc(Y)],  light_pos[Trunc(Z)]);
        glMultMatrixf(@tv[0][0]);
        glTranslatef(-light_pos[Trunc(X)]+logo_pos[Trunc(X)],
                     -light_pos[Trunc(Y)]+logo_pos[Trunc(Y)],
                     -light_pos[Trunc(Z)]+logo_pos[Trunc(Z)]);
        glScalef(0.04,  0.04,  0.04);
        glRotatef (0.1 * (-900), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(Z)])), 0.0, 0.0, 1.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(Y)])), 0.0, 1.0, 0.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(X)])), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (353), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (450), 0.0, 1.0, 0.0);

        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(@stipple);
        draw_logo_shadow();
        glDisable(GL_POLYGON_STIPPLE);
        glPopMatrix();
    end;

    //
    // DONE SHADOW
    //
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(0.1*(450),  5.0/4.0,  0.5,  20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(@idmat[0][0]);

    gluLookAt(view_from[Trunc(X)],  view_from[Trunc(Y)],  view_from[Trunc(Z)],
              view_to[Trunc(X)],  view_to[Trunc(Y)],  view_to[Trunc(Z)],
              0.0, 1.0, 0.0);

    glCallList( MAT_HOLDER_RINGS);

    glPushMatrix();
    glTranslatef(light_pos[Trunc(X)],  light_pos[Trunc(Y)],  light_pos[Trunc(Z)]);
    glScalef(0.1,  0.1,  0.1);

    _x  := light_pos[Trunc(X)] - logo_pos[Trunc(X)];
    _y  := light_pos[Trunc(Y)] - logo_pos[Trunc(Y)];
    _z  := light_pos[Trunc(Z)] - logo_pos[Trunc(Z)];

    if _x <> 0.0 then
        a3 := -ArcTan2(_z, _x)*10.0*RAD
    else
        a3 := 0.0;

    a4 := -ArcTan2(sqrt(_x*_x + _z*_z), _y)*10.0*RAD;

    glRotatef (0.1 * Trunc(a3), 0.0, 1.0, 0.0);
    glRotatef (0.1 * Trunc(a4), 0.0, 0.0, 1.0);
    glRotatef (0.1 * (-900), 1.0, 0.0, 0.0);

    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glCallList(MAT_HEMISPHERE);
    glEnable(GL_NORMALIZE);
    draw_hemisphere();
    glDisable(GL_NORMALIZE);
    glPopMatrix();

    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3); 
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, @light_pos);
    
    if logo_pos[Trunc(Y)] > -0.33 then
    begin
        glCallList(MAT_LOGO);

        glPushMatrix();
        glTranslatef(logo_pos[Trunc(X)],  logo_pos[Trunc(Y)],  logo_pos[Trunc(Z)]);
        glScalef(0.04,  0.04,  0.04);
        glRotatef (0.1 * (-900), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(Z)])), 0.0, 0.0, 1.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(Y)])), 0.0, 1.0, 0.0);
        glRotatef (0.1 * (Trunc(10.0*logo_rot[Trunc(X)])), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (353), 1.0, 0.0, 0.0);
        glRotatef (0.1 * (450), 0.0, 1.0, 0.0);
        glEnable(GL_LIGHTING);
        draw_logo();
        glPopMatrix();
    end;

    if view_from[Trunc(Y)] < 0.0 then
        draw_under_table();

    glutSwapBuffers();

    if post_idle then
        do_post_idle();
end;

procedure   do_post_idle; cdecl;
var
    current     : TimeValue;
    timediff    : float;
begin
    // Time jerked -- adjust clock appropriately
    if timejerk then
    begin
        timejerk    := false;
        timeoffset  := current_time;
        start       := GetCurrentTime/1000;
    end;

    // Reset our timer
    if resetclock then
    begin
        resetclock  := false;
        paused      := false;
        timeoffset  := START_TIME;
        start       := GetCurrentTime/1000;
    end;

    // Compute new time
    current         := GetCurrentTime/1000;
    timediff        := current - start;

    if not paused then
        current_time := timediff * move_speed + timeoffset;

    // Adjust to new speed
    if new_speed <> move_speed then
    begin
        move_speed  := new_speed;
        timeoffset  := current_time;
        start       := GetCurrentTime/1000;
    end;
    post_idle := false;
end;

procedure   resize_window(w, h: int); cdecl;
begin
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0, 5.0/4.0, 0.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
end;

procedure   initialize; 
var
    attr: int;
begin
    if doublebuffer then
        attr := RGBA_DB_attributes
    else
        attr := RGBA_SB_attributes;

    glutInitDisplayMode(attr);
    glutInitWindowSize(300, 240);
    glutCreateWindow('Ideas');

    if multisample then
        glEnable(GL_POLYGON_SMOOTH);

    init_lights();
    init_materials();

    build_table();

    view_from_spline    := calc_spline_params(@view_from_ctl, TIME);
    view_to_spline      := calc_spline_params(@view_to_ctl, TIME);
    light_pos_spline    := calc_spline_params(@light_pos_ctl, TIME);
    logo_pos_spline     := calc_spline_params(@logo_pos_ctl, TIME);
    logo_rot_spline     := calc_spline_params(@logo_rot_ctl, TIME);

    glutReshapeFunc(resize_window);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutVisibilityFunc(vis);

    glMatrixMode(GL_MODELVIEW);
end;

procedure   build_table;
var
    i, j    : float;
begin
    j := 0.0;
    while j<=TABLERES*1.0 do
    begin
        i := 0.0;
        while i<=TABLERES*1.0 do
        begin
            table_points[Trunc(j)][Trunc(i)][Trunc(Z)] := (i-TABLERES*1.0/2.0)/2.0;
            table_points[Trunc(j)][Trunc(i)][Trunc(X)] := (j-TABLERES*1.0/2.0)/2.0;
            table_points[Trunc(j)][Trunc(i)][Trunc(Y)] := 0.0;
            i := i + 1.0;
        end;
        j := j + 1.0;
    end;
end;

procedure   draw_table;
var
    c       : float;
    i, j    : int;
    k, l    : int;
    ov      : vector;
    lv      : vector;
begin
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    ov[Trunc(X)]   := light_pos[Trunc(X)]-logo_pos[Trunc(X)];
    ov[Trunc(Y)]   := light_pos[Trunc(Y)]-logo_pos[Trunc(Y)];
    ov[Trunc(Z)]   := light_pos[Trunc(Z)]-logo_pos[Trunc(Z)];

    normalize(ov);

    for j := 0 to TABLERES do
    begin
        for i := 0 to TABLERES do
        begin
            lv[Trunc(X)] := light_pos[Trunc(X)] - table_points[j][i][Trunc(X)];
            lv[Trunc(Y)] := light_pos[Trunc(Y)] - table_points[j][i][Trunc(Y)];
            lv[Trunc(Z)] := light_pos[Trunc(Z)] - table_points[j][i][Trunc(Z)];
            normalize(lv);
            c := dot(lv, ov);
            if c < 0.0 then
                c := 0.0;
            c := c * c * c * lv[Trunc(Y)] * 255.0;
            // fade
            if (current_time>TIME-5.0) and (current_time<TIME-3.0) then
                c := c *(1.0 - (current_time-(TIME-5.0)) * 0.5);
            tablecolors[j][i] := Trunc(c);
        end;
    end;

    for l := 0 to TABLERES - 1 do
    begin
        glBegin(GL_TRIANGLE_STRIP);
        for k := 0 to TABLERES do
        begin
            glColor3ub(tablecolors[l][k],
                       tablecolors[l][k],
                       tablecolors[l][k]);
            glVertex3fv(@table_points[l][k]);

            glColor3ub(tablecolors[l+1][k],
                       tablecolors[l+1][k],
                       tablecolors[l+1][k]);
            glVertex3fv(@table_points[l+1][k]);
        end;
        glEnd();
    end;

    if (logo_pos[Trunc(Y)]>-0.33) and (logo_pos[Trunc(Y)]<0.33) then
        glEnable(GL_DEPTH_TEST);

    pca := 0.0;
    glBegin(GL_POLYGON);
    for i := 0 to 3 do
    begin
        lv[Trunc(X)]   := light_pos[Trunc(X)] - paper_points[i][Trunc(X)];
        lv[Trunc(Y)]   := light_pos[Trunc(Y)] - paper_points[i][Trunc(Y)];
        lv[Trunc(Z)]   := light_pos[Trunc(Z)] - paper_points[i][Trunc(Z)];
        normalize(lv);
        c       := dot(lv, ov);
        if c < 0.0 then
            c := 0.0;
        c       := c * c * c * lv[Trunc(Y)];
        // fade
        if (current_time>TIME-5.0) and (current_time<TIME-3.0) then
            c := c * (1.0 - (current_time-(TIME-5.0)) * 0.5);

        pcr := c * 255;
        pcg := c * 255;
        pcb := c * 200;
        pca := pca + c;
        glColor3ub(Trunc(pcr), Trunc(pcg), Trunc(pcb));
        glVertex3fv(@paper_points[i]);
    end;
    glEnd();

    glPushMatrix();
    glRotatef (0.1 * (-184), 0.0, 1.0, 0.0);
    glTranslatef(-0.3, 0.0, -0.8);
    glRotatef (0.1 * (-900), 1.0, 0.0, 0.0);
    glScalef(0.015, 0.015, 0.015);

    if current_time>TIME*1.0-5.0 then
    begin
        c := (current_time-(TIME*1.0-5.0))/2.0;
        glColor3ub(Trunc(c*255.0),  Trunc(c*255.0),  Trunc(c*255.0));
    end
    else
        glColor3ub(0,  0,  0);

    glDisable(GL_DEPTH_TEST);

    draw_i();
    glTranslatef(3.0,  0.0,  0.0);

    draw_d();
    glTranslatef(6.0,  0.0,  0.0);

    draw_e();
    glTranslatef(5.0,  0.0,  0.0);

    draw_a();
    glTranslatef(6.0,  0.0,  0.0);

    draw_s();
    glTranslatef(10.0,  0.0,  0.0);

    draw_i();
    glTranslatef(3.0,  0.0,  0.0);

    draw_n();
    glTranslatef(-31.0,  -13.0,  0.0);

    draw_m();
    glTranslatef(10.0,  0.0,  0.0);

    draw_o();
    glTranslatef(5.0,  0.0,  0.0);

    draw_t();
    glTranslatef(4.0,  0.0,  0.0);

    draw_i();
    glTranslatef(3.5,  0.0,  0.0);

    draw_o();
    glTranslatef(5.0,  0.0,  0.0);

    draw_n();

    glPopMatrix();
end;

procedure   draw_under_table;
var
    k, l    : int;
begin
    if False then
        glEnable(GL_DEPTH_TEST)
    else
        glDisable(GL_DEPTH_TEST);

    glColor3ub(0,  0,  0);

    for l := 0 to TABLERES - 1 do
    begin
        glBegin(GL_TRIANGLE_STRIP);
        for k := 0 to TABLERES do
        begin
            glVertex3fv(@table_points[l][k]);
            glVertex3fv(@table_points[l+1][k]);
        end;
        glEnd();
    end;

    if True then
        glEnable(GL_DEPTH_TEST)
    else
        glDisable(GL_DEPTH_TEST);
end;

procedure   calc_spline(var v: vector; params: pparameter; current_time: float);
var
    t   : float;
    i   : int;
    par : pparameter;
begin
    t   := current_time - Trunc(current_time);

    for i := 0 to 2 do
    begin
        par := params;
        Inc(par,Trunc(current_time));
        v[i] := par[3][i] +
                par[2][i] * t +
                par[1][i] * t * t +
                par[0][i] * t * t * t;
    end;
end;

function    calc_spline_params(ctl_pts: pvector; n: int): pparameter;
var
    i, j    : int;
    params  : pparameter;
    par     : pparameter;

    function    ctlpts(i: integer): pvector;
    begin
        Result := ctl_pts;
        Inc(Result,i);
    end;
begin
    if n < 4 then
        raise Exception.Create('calc_spline_params: not enough control points');

    GetMem(params, sizeof(parameter) * (n-3));

    for i := 0 to n-3-1 do
    begin
        for j := 0 to 2 do
        begin
            par := params;
            Inc(par,i);
            par[3][j] := ctlpts(i+1)[j];
            par[2][j] := ctlpts(i+2)[j] - ctlpts(i)[j];
            par[1][j] :=  2.0 * ctlpts(i)[j] +
                         -2.0 * ctlpts(i+1)[j] +
                          1.0 * ctlpts(i+2)[j] +
                         -1.0 * ctlpts(i+3)[j];
            par[0][j] := -1.0 * ctlpts(i)[j] +
                          1.0 * ctlpts(i+1)[j] +
                         -1.0 * ctlpts(i+2)[j] +
                          1.0 * ctlpts(i+3)[j];
        end;
    end;
    Result := params;
end;

procedure   normalize(var v: vector);
var
    r   : float;
begin
    r       := sqrt(v[Trunc(X)]*v[Trunc(X)] + v[Trunc(Y)]*v[Trunc(Y)] + v[Trunc(Z)]*v[Trunc(Z)]);

    v[Trunc(X)]    := v[Trunc(X)] / r;
    v[Trunc(Y)]    := v[Trunc(Y)] / r;
    v[Trunc(Z)]    := v[Trunc(Z)] / r;
end;

function    dot(var v1, v2: vector): float;
begin
    Result := v1[Trunc(X)]*v2[Trunc(X)]+v1[Trunc(Y)]*v2[Trunc(Y)]+v1[Trunc(Z)]*v2[Trunc(Z)];
end;

procedure   Demo;
var
    i   : int;
begin
    InitGLUT;

    auto_run    := false;   // Don't automatically run forever
    // .4 spline distance per second by default
    move_speed  := SPEED_MEDIUM;
    new_speed   := SPEED_MEDIUM;
    timeoffset  := START_TIME;

    for i := 1 to ParamCount do
        if Copy(ParamStr(i),1,1) = '-' then
            case Copy(ParamStr(i),2,1)[1] of
                'a' : auto_run := true;         // Keep running forever
                'm' : multisample := true;      // Multisample
                'd' : doublebuffer := false;    // Single buffer
                's' : case Copy(ParamStr(i),3,1)[1] of
                        '1' : begin
                                move_speed  := SPEED_SLOW ;
                                new_speed   := SPEED_SLOW ;
                              end;
                        '2' : begin
                                move_speed  := SPEED_MEDIUM ;
                                new_speed   := SPEED_MEDIUM ;
                              end;
                        '3' : begin
                                move_speed  := SPEED_FAST ;
                                new_speed   := SPEED_FAST ;
                              end;
                        '4' : begin
                                move_speed  := SPEED_SUPER_FAST ;
                                new_speed   := SPEED_SUPER_FAST ;
                              end;
                      end;
            end;

    initialize();

    current_time := timeoffset;
    resetclock   := true;
    timejerk     := false;
    glutMainLoop();
end;

initialization
    RegisterDemo('Misc','Ideas','Ideas in motion',Demo);
end.
