(****************************************************************************
*
*                       MegaGraph Graphics Library
*
*               Copyright (C) 1991-1997 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*       This library is free software; you can use it and/or
*       modify it under the terms of the SciTech MGL Software License.
*
*       This library is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY; without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*       SciTech MGL Software License for more details.
*  ======================================================================
*
* Filename:     $Workfile:   gears.c  $
* Version:      $Revision:   1.10  $
*
* Language:     ANSI C
* Environment:  MSDOS/Win32 full screen and windowed
*
* Description:  Simple Game Framework OpenGL sample program. This is a
*               port of Brian Paul's Gears demo from Mesa to the MGL
*               Game Framework.
*
* $Date:   01 Oct 1997 05:51:00  $ $Author:   KendallB  $
*
****************************************************************************)

program Gears;

uses
    Windows,
    SysUtils,
    MGLTypes,
    MGLFx,
    MGLGm,
    MGLGl,
    ZTimer;

// Note that we need to include the Game Framework dialog box resources
// in here, so that we can properly display the mode browsing dialog box.
{$R GMDLG.RES}

var
    view_rotx       : GLfloat = 20.0;
    view_roty       : GLfloat = 30.0;
    view_rotz       : GLfloat = 0.0;
    gear1,
    gear2,
    gear3           : GLint;
    angle           : GLfloat = 0.0;
    gm              : PGMDC;
    font8x8         : pfont_t = nil;
    lastCount       : ulong;
    frameCount      : int;
    fpsRate         : int;
    driverOpt       : GM_driverOptions;
    waitForRetrace  : int = MGL_waitVRT;
    startWindowed   : bool = true;
    info            : GM_modeInfo;

//
// Draw a gear wheel.  You'll probably want to call this function when
// building a display list since we do a lot of trig here.
//
// Input:  inner_radius - radius of hole at center
//         outer_radius - radius at center of teeth
//         width - width of gear
//         teeth - number of teeth
//         tooth_depth - depth of tooth
//
procedure   Gear(inner_radius, outer_radius, width: GLfloat; teeth: GLint; tooth_depth: GLfloat);
var
    i           : GLint;
    r0, r1, r2  : GLfloat;
    angle, da   : GLfloat;
    u, v, len   : GLfloat;
begin
    r0 := inner_radius;
    r1 := outer_radius - tooth_depth/2.0;
    r2 := outer_radius + tooth_depth/2.0;

    da := 2.0*Pi/teeth/4.0;
    glShadeModel( GL_FLAT );
    glNormal3f( 0.0, 0.0, 1.0 );

    // draw front face
    glBegin( GL_QUAD_STRIP );
    for i := 0 to teeth do
    begin
        angle := i * 2.0*Pi/teeth;
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    end;
    glEnd;

    // draw front sides of teeth
    glBegin( GL_QUADS );
    da := 2.0*Pi/teeth/4.0;
    for i := 0 to teeth - 1 do
    begin
        angle := i*2.0*Pi/teeth;

        glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    end;
    glEnd;

    glNormal3f( 0.0, 0.0, -1.0 );

    // draw back face
    glBegin( GL_QUAD_STRIP );
    for i := 0 to teeth do
    begin
        angle := i*2.0*Pi/teeth;
        glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    end;
    glEnd;

    // draw back sides of teeth
    glBegin( GL_QUADS );
    da := 2.0*Pi/teeth/4.0;
    for i := 0 to teeth - 1 do
    begin
        angle := i*2.0*Pi/teeth;

        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
        glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
    end;
    glEnd;

    // draw outward faces of teeth
    glBegin( GL_QUAD_STRIP );
    for i := 0 to teeth - 1 do
    begin
        angle := i*2.0*Pi/teeth;

        glVertex3f( r1*cos(angle),      r1*sin(angle),       width*0.5 );
        glVertex3f( r1*cos(angle),      r1*sin(angle),      -width*0.5 );
        u   := r2*cos(angle+da) - r1*cos(angle);
        v   := r2*sin(angle+da) - r1*sin(angle);
        len := sqrt( u*u + v*v );
        u   := u/len;
        v   := v/len;
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),    width*0.5 );
        glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),   -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
        glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
        u   := r1*cos(angle+3*da) - r2*cos(angle+2*da);
        v   := r1*sin(angle+3*da) - r2*sin(angle+2*da);
        glNormal3f( v, -u, 0.0 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
        glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
        glNormal3f( cos(angle), sin(angle), 0.0 );
    end;

    glVertex3f( r1*cos(0), r1*sin(0), width*0.5 );
    glVertex3f( r1*cos(0), r1*sin(0), -width*0.5 );

    glEnd;

    glShadeModel( GL_SMOOTH );

    // draw inside radius cylinder
    glBegin( GL_QUAD_STRIP );
    for i := 0 to teeth do
    begin
        angle := i*2.0*Pi/teeth;
        glNormal3f( -cos(angle), -sin(angle), 0.0 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    end;
    glEnd;
end;

procedure   DrawFrame; cdecl;
var
    newCount   : ulong;
begin
    glClear( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );

    glPushMatrix;
    glRotatef( view_rotx, 1.0, 0.0, 0.0 );
    glRotatef( view_roty, 0.0, 1.0, 0.0 );
    glRotatef( view_rotz, 0.0, 0.0, 1.0 );

    glPushMatrix;
    glTranslatef( -3.0, -2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, 1.0 );
    glCallList(gear1);
    glPopMatrix;

    glPushMatrix;
    glTranslatef( 3.1, -2.0, 0.0 );
    glRotatef( -2.0*angle-9.0, 0.0, 0.0, 1.0 );
    glCallList(gear2);
    glPopMatrix;

    glPushMatrix;
    glTranslatef( -3.1, 4.2, 0.0 );
    glRotatef( -2.0*angle-25.0, 0.0, 0.0, 1.0 );
    glCallList(gear3);
    glPopMatrix;

    glPopMatrix;

    // Update the frames per second count if we have gone past at least
    // a quarter of a second since the last update.
    newCount := LZTimerLap();
    Inc(frameCount);
    if (newCount - lastCount) > 100000 then
    begin
	  fpsRate := (10000000 div (newCount - lastCount)) * frameCount;
	  lastCount := newCount;
	  frameCount := 0;
    end;
    MGL_setColorRGB($FF,$FF,$FF);
    MGL_setTextJustify(MGL_RIGHT_TEXT,MGL_BOTTOM_TEXT);
    if MGL_surfaceAccessType(gm^.dc) = MGL_LINEAR_ACCESS then
       MGL_drawStrXY(MGL_maxx(),MGL_maxy(),PChar(Format('%3d.%d fps', [fpsRate div 10, fpsRate mod 10])));

    GM_swapBuffers(waitForRetrace);
end;

procedure   MGLError;
begin
    raise Exception.CreateFmt('MGL error %s',[MGL_errorMsg(MGL_result)]);
end;

// Sets up the correct viewing parameters after the size
// of our rendering context has changed. This occurs whenever
// as mode change has happened. We also create our display
// lists in here, as these need to be re-built anytime that
// we change resolutions.
procedure   Init;
const
    pos     : array[0..3] of GLfloat = (5.0, 5.0, 10.0, 0.0);
    red     : array[0..3] of GLfloat = (0.8, 0.1,  0.0, 1.0);
    green   : array[0..3] of GLfloat = (0.0, 0.8,  0.2, 1.0);
    blue    : array[0..3] of GLfloat = (0.2, 0.2,  1.0, 1.0);
var
    width   : GLint;
    height  : GLint;
    h       : GLfloat;
begin
    width   := MGL_sizex(gm^.dispdc)+1;
    height  := MGL_sizey(gm^.dispdc)+1;
    h       := height/width;

    // Start OpenGL rendering. Note that we just use some simple flags to
    // do this, however you can call the MGL_glChooseVisual/MGL_glSetVisual
    // functions on the gm->dispdc device context if you need more control
    // over the type of OpenGL visual created.
    if not GM_startOpenGL(MGL_GL_RGB or MGL_GL_DEPTH or MGL_GL_DOUBLE) then
       MGLError;

    // Initialize the viewing transforms
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity;
    glFrustum( -1.0, 1.0, -h, h, 5.0, 60.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity;
    glTranslatef( 0.0, 0.0, -40.0 );
    glClear( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );

    // Initialize default state information
    glLightfv( GL_LIGHT0, GL_POSITION, @pos );
    glEnable( GL_CULL_FACE );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_DEPTH_TEST );

    // make the gears
    gear1 := glGenLists(1);
    glNewList(gear1, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @red );
    Gear( 1.0, 4.0, 1.0, 20, 0.7 );
    glEndList;

    gear2 := glGenLists(1);
    glNewList(gear2, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @green );
    Gear( 0.5, 2.0, 2.0, 10, 0.7 );
    glEndList;

    gear3 := glGenLists(1);
    glNewList(gear3, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @blue );
    Gear( 1.3, 2.0, 0.5, 10, 0.7 );
    glEndList;

    glEnable( GL_NORMALIZE );

    if font8x8 = nil then
       font8x8 := MGL_loadFont('pc8x8.fnt');
    MGL_useFont(font8x8);
end;

// Handles keyboard events and passes them to ProcessInput.
procedure   KeyDown(evt: pevent_t); cdecl;
begin
    case ushort(evt^.message) of
        kbLeft  : view_roty := view_roty + 5.0;
        kbRight : view_roty := view_roty - 5.0;
        kbUp    : view_rotx := view_rotx + 5.0;
        kbDown  : view_rotx := view_rotx - 5.0;
    else
        case EVT_asciiCode(evt^.message) of
            Ord('z'): view_rotz := view_rotz + 5.0;
            Ord('Z'): view_rotz := view_rotz - 5.0;
            $1B     : GM_exit;
            Ord('v'): if not MGL_isWindowedDC(gm^.dispdc) then
                      begin
                           if waitForRetrace = MGL_waitVRT then
                               waitForRetrace := MGL_dontWait
                           else
                               waitForRetrace := MGL_waitVRT;
                      end;
        end;
    end;
end;

// Update all object positions for the next frame
procedure   GameLogic; cdecl;
begin
    angle := angle + 2.0;
end;

// Main callback to switch between fullscreen and windowed graphics modes. This
// function is called by the game framework when the user requests to switch to
// fullscreen modes either by typing 'Alt-Enter' or by clicking on the maximum
// button when in windowed modes.
procedure SwitchModes(var mode: GM_modeInfo; windowed: bool); cdecl;
begin
     Init;
end;

// This function is where you would put your own code to handle
// switching to and from fullscreen modes, such as pausing
// CD-Audio and other stuff. The main game loop will continue
// to run as we return MGL_NO_SUSPEND_APP (but no drawFrame will
// not be called, only gameLogic), and you can optionally return
// MGL_NO_DEACTIVATE to forcibly ignore the switch.
function    suspendApp(dc: PMGLDC; flags: int): int; cdecl;
begin
    if flags = MGL_DEACTIVATE then
    begin
        // We are switching back to GDI mode, so put code in here to disable
        // stuff when switched away from fullscreen mode. Note that this
        // function may get once to determine if the switch should occur,
        // and again when the switch actually happens.
    end
    else if flags = MGL_REACTIVATE then
    begin
        // We are now back in fullscreen mode, so put code in here to renable
        // stuff for fullscreen modes.
    end;
    Result := MGL_NO_SUSPEND_APP;
end;

begin
    {Fill in default driver options}
    driverOpt.useWinDirect := true;
    driverOpt.useDirectDraw := true;
    driverOpt.useVGA := true;
    driverOpt.useVGAX := true;
    driverOpt.useVBE := true;
    driverOpt.useLinear := true;
    driverOpt.useVBEAF := true;
    driverOpt.useFullscreenDIB := true;
    driverOpt.useHWOpenGL := true;
    driverOpt.openGLType := MGL_GL_AUTO;
    driverOpt.modeFlags := GM_MODE_ALLBPP;

    {Parse command line}
    if ParamCount > 0 then
    begin
        if CompareText(ParamStr(1),'-nowindirect') = 0 then
            driverOpt.useWinDirect := false
        else if CompareText(ParamStr(1),'-novbe') = 0 then
            driverOpt.useVBE := false
        else if CompareText(ParamStr(1),'-novbeaf') = 0 then
            driverOpt.useVBEAF := false
        else if CompareText(ParamStr(1),'-nodirectdraw') = 0 then
            driverOpt.useDirectDraw := false
        else if CompareText(ParamStr(1),'-nohwopengl') = 0 then
            driverOpt.useHWOpenGL := false
        else if CompareText(ParamStr(1),'-usemesa') = 0 then
            driverOpt.openGLType := MGL_GL_MESA
        else if CompareText(ParamStr(1),'-usesgi') = 0 then
            driverOpt.openGLType := MGL_GL_SGI
    end;

    {Initialize Game Framework}
    GM_setDriverOptions(driverOpt);
	 ZTimerInit;
	 LZTimerOn;
	 GM_initSysPalNoStatic(true);
    gm := GM_init('Game Framework OpenGL Demo');
    if gm = nil then
        MGLError;
    GM_setGameLogicFunc(GameLogic);
    GM_setDrawFunc(DrawFrame);
    GM_setKeyDownFunc(KeyDown);
    GM_setKeyRepeatFunc(KeyDown);
    GM_setModeSwitchFunc(SwitchModes);
    GM_setSuspendAppCallback(suspendApp);

    while GM_chooseMode(info,startWindowed) do
    begin
        if not GM_setMode(info,startWindowed,2,true) then
           MGLError;
        Init;
        GM_mainLoop;
    end;
end.

