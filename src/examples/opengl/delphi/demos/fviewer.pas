{=============================================================================}
{=                                                                           =}
{= fractviewer.c [from agviewer.c  (version 1.0)]                            =}
{=                                                                           =}
{= AGV: a glut viewer. Routines for viewing a 3d scene w/ glut               =}
{=                                                                           =}
{= See agv_example.c and agviewer.h comments within for more info.           =}
{=                                                                           =}
{= The two view movement modes are POLAR and FLYING.  Both move the eye, NOT =}
{= THE OBJECT.  You can never be upside down or twisted (roll) in either     =}
{= mode.                                                                     =}
{=                                                                           =}
{= A nice addition would be an examiner type trackball mode where you are    =}
{= moving the object and so could see it from any angle.  Also less          =}
{= restricted flying and polar modes (fly upside down, do rolls, etc.).      =}
{=                                                                           =}
{= Controls for Polar are just left and middle buttons -- for flying it's    =}
{= those plus 0-9 number keys and +/- for speed adjustment.                  =}
{=                                                                           =}
{= See agv_example.c and agviewer.c for more info.  Probably want to make    =}
{= a copy of these and then edit for each program.  This isn't meant to be   =}
{= a library, just something to graft onto your own programs.                =}
{=                                                                           =}
{= I welcome any feedback or improved versions!                              =}
{=                                                                           =}
{= Philip Winston - 4/11/95                                                  =}
{= pwinston@hmc.edu                                                          =}
{= http://www.cs.hmc.edu/people/pwinston                                     =}
{=                                                                           =}
{=============================================================================}
unit FViewer;

interface

uses
    MGLTypes,
    MGLGl,
    Windows;

//
// Call agvInit() with glut's current window set to the window in
// which you want to run the viewer. Right after creating it is fine.  It
// will remember that window for possible later use (see below) and
// registers mouse, motion, and keyboard handlers for that window (see below).
//
// allowidle is 1 or 0 depnding on whether you will let AGV install
// and uninstall an idle function.  0 means you will not let it (because
// you will be having your own idle function). In this case it is your
// responsibility to put a statement like:
//
//     if (agvMoving)
//       agvMove();
//
// at the end of your idle function, to let AGV update the viewpoint if it
// is moving.
//
// If allowidle is 1 it means AGV will install its own idle which
// will update the viewpoint as needed and send glutPostRedisplay() to the
// window which was current when agvInit() was called.
//
// agvSetIdleAllow changes this value so you can let AGV install its idle
// when your idle isn't installed.
//
procedure   agvInit(allowidle: bool);
procedure   agvSetAllowIdle(allowidle: bool);

//
// Set which movement mode you are in.
//
const
    FLYING  = 0;
    POLAR   = 1;

procedure   agvSwitchMoveMode(move: int); cdecl;

//
// agvViewTransform basically does the appropriate gluLookAt() for the
// current position.  So call it in your display on the projection matrix
//
procedure   agvViewTransform;

//
// agvMoving will be set by AGV according to whether it needs you to call
// agvMove() at the end of your idle function.  You only need these if
// you aren't allowing AGV to do its own idle.
// (Don't change the value of agvMoving)
//
var
    agvMoving: int;
    
procedure   agvMove; cdecl;

//
// These are the routines AGV registers to deal with mouse and keyboard input.
// Keyboard input only matters in flying mode, and then only to set speed.
// Mouse input only uses left two buttons in both modes.
// These are all registered with agvInit(), but you could register
// something else which called these, or reregister these as needed
//
procedure   agvHandleButton(button: int; state: int; x, y: int); cdecl;
procedure   agvHandleMotion(x, y: int); cdecl;
procedure   agvHandleKeys(key: uchar; x, y: int); cdecl;

//
// Just an extra routine which makes an x-y-z axes (about 10x10x10)
// which is nice for aligning things and debugging.  Pass it an available
// displaylist number.
//
procedure   agvMakeAxesList(displaylist: int);

type
    floatVec = array[0..2] of float;

procedure   normalize(var v: floatVec); forward;
procedure   ncrossprod(var v1, v2, cp: floatVec); forward;

implementation

uses
    SysUtils,
    MGLGlu,
    MGLGlut;

{== Settings =================================================================}

{-- Initial polar movement settings ------------------------------------------}

const
    INIT_POLAR_AZ   =  0.0 ;
    INIT_POLAR_EL   = 30.0 ;
    INIT_DIST       =  4.0 ;
    INIT_AZ_SPIN    =  0.5 ;
    INIT_EL_SPIN    =  0.0 ;

{-- Initial flying movement settings -----------------------------------------}

    INIT_EX         =   0.0 ;
    INIT_EY         =  -2.0 ;
    INIT_EZ         =  -2.0 ;
    INIT_MOVE       =  0.01 ;
    MINMOVE         = 0.001 ;

{-- Start in this mode -------------------------------------------------------}

    INIT_MODE       = POLAR ;

{-- Controls -----------------------------------------------------------------}

{-- Map 0-9 to an EyeMove value when number key is hit in FLYING mode --------}

function    SPEEDFUNCTION(x: int): float;
begin
    Result := ((x)*(x)*0.001)
end;

{-- Multiply EyeMove by (1+-MOVEFRACTION) when +/- hit in FLYING mode --------}

const
    MOVEFRACTION    = 0.25 ;

{-- What to multiply number of pixels mouse moved by to get rotation amount --}

    EL_SENS         = 0.5 ;
    AZ_SENS         = 0.5 ;

{-- What to multiply number of pixels mouse moved by for movement amount -----}

    DIST_SENS       = 0.01 ;
    E_SENS          = 0.01 ;

{-- Minimum spin to allow in polar (lower forced to zero) --------------------}

    MIN_AZSPIN      = 0.1 ;
    MIN_ELSPIN      = 0.1 ;

{-- Factors used in computing dAz and dEl (which determine AzSpin, ElSpin) ---}

    SLOW_DAZ        = 0.90 ;
    SLOW_DEL        = 0.90 ;
    PREV_DAZ        = 0.80 ;
    PREV_DEL        = 0.80 ;
    CUR_DAZ         = 0.20 ;
    CUR_DEL         = 0.20 ;

{== Globals ==================================================================}    

var
    MoveMode    : int     = INIT_MODE;      // FLYING or POLAR mode?

    Ex          : GLfloat = INIT_EX;    // flying parameters
    Ey          : GLfloat = INIT_EY;
    Ez          : GLfloat = INIT_EZ;
    EyeMove     : GLfloat = INIT_MOVE;

    EyeDist     : GLfloat = INIT_DIST;  // polar params
    AzSpin      : GLfloat = INIT_AZ_SPIN;
    ElSpin      : GLfloat = INIT_EL_SPIN;

    EyeAz       : GLfloat = INIT_POLAR_AZ; // used by both
    EyeEl       : GLfloat = INIT_POLAR_EL;

    downx,
    downy,        // for tracking mouse position
    lastx,
    lasty       : int;

    downb       : int = -1; // and button status 
                        
    downDist,
    downEl,
    downAz      : GLfloat; // for saving state of things
    downEx,
    downEy,
    downEz      : GLfloat; // when button is pressed
    downEyeMove : GLfloat;

    dAz,
    dEl,
    lastAz,
    lastEl      : GLfloat; // to calculate spinning w/ polar motion

    AdjustingAzEl   : int = 0;

    AllowIdle       : bool;
    RedisplayWindow : int;

    // If AllowIdle is 1 it means AGV will install its own idle which
    // will update the viewpoint as needed and send glutPostRedisplay() to the
    // window RedisplayWindow which was set in agvInit().  AllowIdle of 0
    // means AGV won't install an idle funciton, and something like
    // "if (agvMoving) agvMove()" should exist at the end of the running
    // idle function.
    //

function    Max(a,b: float): float;
begin
    if a > b then
        Result := a
    else
        Result := b;
end;

function    SignNZ(v: float): float;
begin
    if v > 0 then
        Result := 1
    else
        Result := -1;
end;

function    TORAD(x: float): float;
begin
    Result  := (Pi/180.0)*x;
end;

function    TODEG(x: float): float;
begin
    Result  := (180.0/Pi)*x;
end;

{== Prototypes ===============================================================}

//
// these are functions meant for internal use only
// the other prototypes are in agviewer.h
//

procedure   PolarLookFrom(dist, elevation, azimuth: GLfloat); forward;
procedure   FlyLookFrom(x, y, z, az, el: GLfloat); forward;
function    ConstrainEl: int; forward;
procedure   MoveOn(v: int); forward;
procedure   SetMove(newmove: float); forward;

{-- agvInit ------------------------------------------------------------------}

procedure   agvInit(allowidle: bool);
begin
    glutMouseFunc(agvHandleButton);
    glutMotionFunc(agvHandleMotion);
    glutKeyboardFunc(agvHandleKeys);
    RedisplayWindow := glutGetWindow();
    agvSetAllowIdle(allowidle);
end;

{-- Viewpoint stuff ----------------------------------------------------------}

//
// viewing transformation modified from page 90 of red book
//
procedure   PolarLookFrom(dist, elevation, azimuth: GLfloat);
begin
    glTranslatef(0, 0, -dist);
    glRotatef(elevation, 1, 0, 0);
    glRotatef(azimuth, 0, 1, 0);
end;

//
// I took the idea of tracking eye position in absolute
// coords and direction looking in Polar form from denis
//
procedure   FlyLookFrom(x, y, z, az, el: GLfloat);
var
    lookat, perp, up: floatVec;
begin
    lookat[0] := sin(TORAD(az))*cos(TORAD(el));
    lookat[1] := sin(TORAD(el));
    lookat[2] := -cos(TORAD(az))*cos(TORAD(el));
    normalize(lookat);
    perp[0] := lookat[2];
    perp[1] := 0;
    perp[2] := -lookat[0];
    normalize(perp);
    ncrossprod(lookat, perp, up);
    gluLookAt(x, y, z,
            x+lookat[0], y+lookat[1], z+lookat[2],
            up[0], up[1], up[2]);
end;

//
// Call viewing transformation based on movement mode
//
procedure   agvViewTransform;
begin
    case MoveMode of
        FLYING  : FlyLookFrom(Ex, Ey, Ez, EyeAz, EyeEl);
        POLAR   : PolarLookFrom(EyeDist, EyeEl, EyeAz);
    end;
end;

//
// keep them vertical; I think this makes a lot of things easier,
// but maybe it wouldn't be too hard to adapt things to let you go
// upside down
//
function    ConstrainEl: int;
begin
    Result := 1;
    if EyeEl <= -90 then
        EyeEl := -89.99
    else if EyeEl >= 90 then
        EyeEl := 89.99
    else
    begin
        Result := 0;
        Exit;
    end;
end;

//
// Idle Function - moves eyeposition
//
procedure   agvMove; cdecl;
begin
    case MoveMode of
        FLYING  : begin
                    Ex := Ex + EyeMove*sin(TORAD(EyeAz))*cos(TORAD(EyeEl));
                    Ey := Ey + EyeMove*sin(TORAD(EyeEl));
                    Ez := Ez - EyeMove*cos(TORAD(EyeAz))*cos(TORAD(EyeEl));
                  end;
        POLAR   : begin
                    EyeEl := EyeEl + ElSpin;
                    EyeAz := EyeAz + AzSpin;
                    if ConstrainEl() <> 0 then // weird spin thing to make things look
                        ElSpin := -ElSpin // look better when you are kept from going
                    else
                        // upside down while spinning - Isn't great
                        if Abs(ElSpin) > Abs(AzSpin) then
                            AzSpin := Abs(ElSpin) * SignNZ(AzSpin)
                  end;
    end;

    if AdjustingAzEl <> 0 then
    begin
        dAz := dAz * SLOW_DAZ;
        dEl := dEl * SLOW_DEL;
    end;

    if (AllowIdle) then
    begin
        glutSetWindow(RedisplayWindow);
        glutPostRedisplay();
    end;
end;

//
// Don't install agvMove as idle unless we will be updating the view
// and we've been given a RedisplayWindow
//
procedure   MoveOn(v: int);
begin
    if (v <> 0) and
       ((MoveMode = FLYING) and (EyeMove <> 0)) or
       ((MoveMode = POLAR) and ((AzSpin <> 0) or (ElSpin <> 0) or (AdjustingAzEl <> 0))) then
    begin
        agvMoving := 1;
        if AllowIdle then
            glutIdleFunc(agvMove);
    end
    else
    begin
        agvMoving := 0;
        if AllowIdle then
            glutIdleFunc(nil);
    end;
end;

//
// set new redisplay window.  If <= 0 it means we are not to install
// an idle function and will rely on whoever does install one to
// put statement like "if (agvMoving) agvMove();" at end of it
//
procedure   agvSetAllowIdle(allowidle: bool);
begin
    FViewer.AllowIdle := allowidle;
    if allowidle then
        MoveOn(1);
end;

//
// when moving to flying we stay in the same spot, moving to polar we
// reset since we have to be looking at the origin (though a pivot from
// current position to look at origin might be cooler)
//
procedure   agvSwitchMoveMode(move: int); cdecl;
begin
    case move of
        FLYING  : begin
                    if MoveMode = FLYING then
                        Exit;
                    Ex      := -EyeDist*sin(TORAD(EyeAz))*cos(TORAD(EyeEl));
                    Ey      :=  EyeDist*sin(TORAD(EyeEl));
                    Ez      :=  EyeDist*(cos(TORAD(EyeAz))*cos(TORAD(EyeEl)));
                    EyeAz   :=  EyeAz;
                    EyeEl   := -EyeEl;
                    EyeMove := INIT_MOVE;
                  end;
        POLAR   : begin
                    EyeDist := INIT_DIST;
                    EyeAz   := INIT_POLAR_AZ;
                    EyeEl   := INIT_POLAR_EL;
                    AzSpin  := INIT_AZ_SPIN;
                    ElSpin  := INIT_EL_SPIN;
                  end;
    end;
    MoveMode := move;
    MoveOn(1);
    glutPostRedisplay();
end;

{-- Mouse handling -----------------------------------------------------------}

procedure   agvHandleButton(button: int; state: int; x, y: int); cdecl;
begin
    if (state = GLUT_DOWN) and (downb = -1) then
    begin
        downx := x;
        downy := y;
        lastx := downx;
        lasty := downy;
        downb := button;

        case button of
            GLUT_LEFT_BUTTON    : begin
                                    downEl          := EyeEl;
                                    lastEl          := downEl;
                                    downAz          := EyeAz;
                                    lastAz          := downAz;
                                    AzSpin          := 0;
                                    ElSpin          := 0;
                                    dAz             := 0;
                                    dEl             := 0;
                                    AdjustingAzEl   := 1;
                                    MoveOn(1);
                                  end;
            GLUT_MIDDLE_BUTTON  : begin
                                    downDist        := EyeDist;
                                    downEx          := Ex;
                                    downEy          := Ey;
                                    downEz          := Ez;
                                    downEyeMove     := EyeMove;
                                    EyeMove         := 0;
                                  end;
        end;
    end
    else if (state = GLUT_UP) and (button = downb) then
    begin
        downb := -1;

        case button of
            GLUT_LEFT_BUTTON    : begin
                                    if MoveMode <> FLYING then
                                    begin
                                        AzSpin := -dAz;
                                        if (AzSpin < MIN_AZSPIN) and (AzSpin > -MIN_AZSPIN) then
                                            AzSpin := 0;
                                        ElSpin := -dEl;
                                        if (ElSpin < MIN_ELSPIN) and (ElSpin > -MIN_ELSPIN) then
                                            ElSpin := 0;
                                    end;
                                    AdjustingAzEl := 0;
                                    MoveOn(1);
                                  end;  
            GLUT_MIDDLE_BUTTON  : EyeMove := downEyeMove;
        end;
    end;
end;

//
// change EyeEl and EyeAz and position when mouse is moved w/ button down
//
procedure   agvHandleMotion(x, y: int); cdecl;
var
    deltax, deltay  : int;
    delta           : int;
begin
    deltax := x - downx;
    deltay := y - downy;

    case downb of
        GLUT_LEFT_BUTTON    : begin
                                if MoveMode = FLYING then
                                    delta := -deltay
                                else
                                    delta := deltay;
                                EyeEl   := downEl + EL_SENS * delta;
                                ConstrainEl();
                                EyeAz   := downAz + AZ_SENS * deltax;
                                dAz     := PREV_DAZ*dAz + CUR_DAZ*(lastAz - EyeAz);
                                dEl     := PREV_DEL*dEl + CUR_DEL*(lastEl - EyeEl);
                                lastAz  := EyeAz;
                                lastEl  := EyeEl;
                              end;
        GLUT_MIDDLE_BUTTON  : begin
                                EyeDist := downDist + DIST_SENS*deltay;
                                Ex      := downEx - E_SENS*deltay*sin(TORAD(EyeAz))*cos(TORAD(EyeEl));
                                Ey      := downEy - E_SENS*deltay*sin(TORAD(EyeEl));
                                Ez      := downEz + E_SENS*deltay*cos(TORAD(EyeAz))*cos(TORAD(EyeEl));
                              end;
    end;
    glutPostRedisplay();
end;

{-- Keyboard handling --------------------------------------------------------}

//
// set EyeMove (current speed) for FLYING mode
//
procedure   SetMove(newmove: float);
begin
    if newmove > MINMOVE then
    begin
        EyeMove := newmove;
        MoveOn(1);
    end
    else
    begin
        EyeMove := 0;
        MoveOn(0);
    end;
end;

//
// 0->9 set speed, +/- adjust current speed  -- in FLYING mode
//
procedure   agvHandleKeys(key: uchar; x, y: int); cdecl;
begin
    if MoveMode <> FLYING then
        Exit;

    if Chr(key) in ['0'..'9'] then
        SetMove(SPEEDFUNCTION(key-Ord('0')))
    else
        case Chr(key) of
            '+' : if EyeMove = 0 then
                    SetMove(MINMOVE)
                  else
                  begin
                    EyeMove := EyeMove * (1 + MOVEFRACTION);
                    SetMove(EyeMove);
                  end;
            '-' : begin
                    EyeMove := EyeMove * (1 - MOVEFRACTION);
                    SetMove(EyeMove);
                  end;
        end;
end;

{-- Vector stuff -------------------------------------------------------------}

// normalizes v
procedure   normalize(var v: floatVec);
var
    d: GLfloat;
begin
    d := sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if d = 0 then
        raise Exception.Create('Zero length vector in normalize')
    else
    begin
        v[0] := v[0] / d;
        v[1] := v[1] / d;
        v[2] := v[2] / d;
    end;
end;

// calculates a normalized crossproduct to v1, v2
procedure   ncrossprod(var v1, v2, cp: floatVec);
begin
    cp[0] := v1[1]*v2[2] - v1[2]*v2[1];
    cp[1] := v1[2]*v2[0] - v1[0]*v2[2];
    cp[2] := v1[0]*v2[1] - v1[1]*v2[0];
    normalize(cp);
end;

{-- Axes ---------------------------------------------------------------------}

// draw axes -- was helpful to debug/design things
procedure   agvMakeAxesList(displaylist: int);
const
    axes_ambuse : array[0..3] of GLfloat = ( 0.5, 0.0, 0.0, 1.0 );
var
    i, j    : int;

    function    Check(Cond: Boolean): int;
    begin
        if Cond then
            Result := 1
        else
            Result := 0;
    end;
begin
    glNewList(displaylist, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMatrixMode(GL_MODELVIEW);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @axes_ambuse);
    glBegin(GL_LINES);
        glVertex3f(15, 0, 0); glVertex3f(-15, 0, 0);
        glVertex3f(0, 15, 0); glVertex3f(0, -15, 0);
        glVertex3f(0, 0, 15); glVertex3f(0, 0, -15);
    glEnd();
    for i := 0 to 2 do
    begin
        glPushMatrix();
        glTranslatef(-10*Check(i=0), -10*Check(i=1), -10*Check(i=2));
        for j := 0 to 20 do
        begin
            glutSolidCube(0.1);
            glTranslatef(Check(i=0), Check(i=1), Check(i=2));
        end;
        glPopMatrix();
    end;
    glPopAttrib();
    glEndList();
end;
end.
