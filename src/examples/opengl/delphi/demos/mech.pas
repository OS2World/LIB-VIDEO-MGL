//
// program  : glutmech V1.1
// author   : Simon Parkinson-Bates.
// E-mail   : sapb@yallara.cs.rmit.edu.au
// Copyright Simon Parkinson-Bates.
// "source if freely avaliable to anyone to copy as long as they
//  acknowledge me in their work."
//
// Funtional features
// ------------------
// * online menu system avaliable by pressing left mouse button
// * online cascading help system avaliable, providing information on
//  the several  key strokes and what they do.
// * animation sequence coded which makes the mech walk through an
//  environment.  Shadows will soon be added to make it look
//  more realistic.
// * menu control to view mech in wireframe or sold mode.
// * various key strokes avaliable to control idependently the mechs
//  many joints.
// * various key strokes avaliable to view mech and environment from
//  different angles
// * various key strokes avaliable to alter positioning of the single
//  light source.
//
//
// Program features
// ----------------
// * uses double buffering
// * uses display lists
// * uses glut to manage windows, callbacks, and online menu.
// * uses glpolygonfill() to maintain colors in wireframe and solid
//  mode.
//
unit Mech;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    Math,
    CmdLine,
    DemoList;

// start of compilation conditions

{$DEFINE SPHERE}
{$DEFINE COLOR}
{$DEFINE LIGHT}
{$DEFINE TORSO}
{$DEFINE HIP}
{$DEFINE SHOULDER}
{$DEFINE UPPER_ARM}
{$DEFINE LOWER_ARM}
{$DEFINE ROCKET_POD}
{$DEFINE UPPER_LEG}
{$DEFINE LOWER_LEG}
{$DEFINE NO_NORM}
{$DEFINE ANIMATION}
{$DEFINE DRAW_MECH}
{$DEFINE DRAW_ENVIRO}
{$DEFINE MOVE_LIGHT}

// end of compilation conditions 

// start of display list definitions

const
    SOLID_MECH_TORSO            = 1  ;
    SOLID_MECH_HIP              = 2  ;
    SOLID_MECH_SHOULDER         = 3  ;
    SOLID_MECH_UPPER_ARM        = 4  ;
    SOLID_MECH_FOREARM          = 5  ;
    SOLID_MECH_UPPER_LEG        = 6  ;
    SOLID_MECH_FOOT             = 7  ;
    SOLID_MECH_ROCKET           = 8  ;
    SOLID_MECH_VULCAN           = 9  ;
    SOLID_ENVIRO                = 10 ;

// end of display list definitions 

// start of motion rate variables

const
    ANKLE_RATE                  = 3  ;
    HEEL_RATE                   = 3  ;
    ROTATE_RATE                 = 10 ;
    TILT_RATE                   = 10 ;
    ELBOW_RATE                  = 2  ;
    SHOULDER_RATE               = 5  ;
    LAT_RATE                    = 5  ;
    CANNON_RATE                 = 40 ;
    UPPER_LEG_RATE              = 3  ;
    UPPER_LEG_RATE_GROIN        = 10 ;
    LIGHT_TURN_RATE             = 10 ;
    VIEW_TURN_RATE              = 10 ;
    
// end of motion rate variables

// start of motion  variables

var
    leg         : bool  = false;

    shoulder1   : int   = 0;
    shoulder2   : int   = 0;
    shoulder3   : int   = 0;
    shoulder4   : int   = 0;
    lat1        : int   = 20;
    lat2        : int   = 20;
    elbow1      : int   = 0;
    elbow2      : int   = 0;
    pivot       : int   = 0;
    tilt        : int   = 10;
    ankle1      : int   = 0;
    ankle2      : int   = 0;
    heel1       : int   = 0;
    heel2       : int   = 0;
    hip11       : int   = 0;
    hip12       : int   = 10;
    hip21       : int   = 0;
    hip22       : int   = 10;
    fire        : int   = 0;
    solid_part  : bool  = True;
    anim        : int   = 0;
    turn        : int   = 0;
    turn1       : int   = 0;
    lightturn   : int   = 0;
    lightturn1  : int   = 0;

    elevation   : float = 0.0;
    distance    : float = 0.0;
    frame       : float = 3.0;

    // foot1v[] = {} foot2v[] = {}

// end of motion variables

// start of material definitions
{$IFDEF LIGHT}
const
    mat_specular    : array[0..3] of GLfloat = ( 0.628281, 0.555802, 0.366065, 1.0 );
    mat_ambient     : array[0..3] of GLfloat = ( 0.24725, 0.1995, 0.0745, 1.0 );
    mat_diffuse     : array[0..3] of GLfloat = ( 0.75164, 0.60648, 0.22648, 1.0 );
    mat_shininess   : array[0..0] of GLfloat = ( 128.0 * 0.4 );

    mat_specular2   : array[0..2] of GLfloat = ( 0.508273, 0.508273, 0.508373 );
    mat_ambient2    : array[0..2] of GLfloat = ( 0.19225, 0.19225, 0.19225 );
    mat_diffuse2    : array[0..2] of GLfloat = ( 0.50754, 0.50754, 0.50754 );
    mat_shininess2  : array[0..0] of GLfloat = ( 128.0 * 0.6 );

    mat_specular3   : array[0..2] of GLfloat = ( 0.296648, 0.296648, 0.296648 );
    mat_ambient3    : array[0..2] of GLfloat = ( 0.25, 0.20725, 0.20725 );
    mat_diffuse3    : array[0..2] of GLfloat = ( 1, 0.829, 0.829 );
    mat_shininess3  : array[0..0] of GLfloat = ( 128.0 * 0.088 );

    mat_specular4   : array[0..2] of GLfloat = ( 0.633, 0.727811, 0.633 );
    mat_ambient4    : array[0..2] of GLfloat = ( 0.0215, 0.1745, 0.0215 );
    mat_diffuse4    : array[0..2] of GLfloat = ( 0.07568, 0.61424, 0.07568 );
    mat_shininess4  : array[0..0] of GLfloat = ( 128 * 0.6 );

    mat_specular5   : array[0..2] of GLfloat = ( 0.60, 0.60, 0.50 );
    mat_ambient5    : array[0..2] of GLfloat = ( 0.0, 0.0, 0.0 );
    mat_diffuse5    : array[0..2] of GLfloat = ( 0.5, 0.5, 0.0 );
    mat_shininess5  : array[0..0] of GLfloat = ( 128.0 * 0.25 );
{$ENDIF}    
// end of material definitions 

// start of the body motion functions
procedure   Add(var X: int; Val: int);
begin
    X := (X + Val) mod 360;
end;

procedure   Sub(var X: int; Val: int);
begin
    X := (X - Val) mod 360;
end;

procedure   Heel1Add;
begin
    Add(heel1,HEEL_RATE);
end;

procedure   Heel1Subtract;
begin
    Sub(heel1,HEEL_RATE);
end;

procedure   Heel2Add;
begin
    Add(heel2,HEEL_RATE);
end;

procedure   Heel2Subtract;
begin
    Sub(heel2,HEEL_RATE);
end;

procedure   Ankle1Add;
begin
    Add(ankle1,ANKLE_RATE);
end;

procedure   Ankle1Subtract;
begin
    Sub(ankle1,ANKLE_RATE);
end;

procedure   Ankle2Add;
begin
    Add(ankle2,ANKLE_RATE);
end;

procedure   Ankle2Subtract;
begin
    Sub(ankle2,ANKLE_RATE);
end;

procedure   RotateAdd;
begin
    Add(pivot,ROTATE_RATE);
end;

procedure   RotateSubtract;
begin
    Sub(pivot,ROTATE_RATE);
end;

procedure   MechTiltAdd;
begin
    Add(tilt,TILT_RATE);
end;

procedure   MechTiltSubtract;
begin
    Sub(tilt,TILT_RATE);
end;

procedure   Elbow1Add;
begin
    Add(elbow1,ELBOW_RATE);
end;

procedure   Elbow1Subtract;
begin
    Sub(elbow1,ELBOW_RATE);
end;

procedure   Elbow2Add;
begin
    Add(elbow2,ELBOW_RATE);
end;

procedure   Elbow2Subtract;
begin
    Sub(elbow2,ELBOW_RATE);
end;

procedure   Shoulder1Add;
begin
    Add(shoulder1,SHOULDER_RATE);
end;

procedure   Shoulder1Subtract;
begin
    Sub(shoulder1,SHOULDER_RATE);
end;

procedure   Shoulder2Add;
begin
    Add(shoulder2,SHOULDER_RATE);
end;

procedure   Shoulder2Subtract;
begin
    Sub(shoulder2,SHOULDER_RATE);
end;

procedure   Shoulder3Add;
begin
    Add(shoulder3,SHOULDER_RATE);
end;

procedure   Shoulder3Subtract;
begin
    Sub(shoulder3,SHOULDER_RATE);
end;

procedure   Shoulder4Add;
begin
    Add(shoulder4,SHOULDER_RATE);
end;

procedure   Shoulder4Subtract;
begin
    Sub(shoulder4,SHOULDER_RATE);
end;

procedure   Lat1Raise;
begin
    Add(lat1,LAT_RATE);
end;

procedure   Lat1Lower;
begin
    Sub(lat1,LAT_RATE);
end;

procedure   Lat2Raise;
begin
    Add(lat2,LAT_RATE);
end;

procedure   Lat2Lower;
begin
    Sub(lat2,LAT_RATE);
end;

procedure   FireCannon;
begin
    Add(fire,CANNON_RATE);
end;

procedure   RaiseLeg1Forward;
begin
    Add(hip11,UPPER_LEG_RATE);
end;

procedure   LowerLeg1Backwards;
begin
    Sub(hip11,UPPER_LEG_RATE);
end;

procedure   RaiseLeg1Outwards;
begin
    Add(hip12,UPPER_LEG_RATE_GROIN);
end;

procedure   LowerLeg1Inwards;
begin
    Sub(hip12,UPPER_LEG_RATE_GROIN);
end;

procedure   RaiseLeg2Forward;
begin
    Add(hip21,UPPER_LEG_RATE);
end;

procedure   LowerLeg2Backwards;
begin
    Sub(hip21,UPPER_LEG_RATE);
end;

procedure   RaiseLeg2Outwards;
begin
    Add(hip22,UPPER_LEG_RATE_GROIN);
end;

procedure   LowerLeg2Inwards;
begin
    Sub(hip22,UPPER_LEG_RATE_GROIN);
end;

// end of body motion functions 

// start of light source position functions
procedure   TurnRight;
begin
    Sub(turn,VIEW_TURN_RATE);
end;

procedure   TurnLeft;
begin
    Add(turn,VIEW_TURN_RATE);
end;

procedure   TurnForwards;
begin
    Sub(turn1,VIEW_TURN_RATE);
end;

procedure   TurnBackwards;
begin
    Add(turn1,VIEW_TURN_RATE);
end;

procedure   LightTurnRight;
begin
    Add(lightturn,LIGHT_TURN_RATE);
end;

procedure   LightTurnLeft;
begin
    Sub(lightturn,LIGHT_TURN_RATE);
end;

procedure   LightForwards;
begin
    Add(lightturn1,LIGHT_TURN_RATE);
end;

procedure   LightBackwards;
begin
    Sub(lightturn1,LIGHT_TURN_RATE);
end;

// end of light source position functions 

// start of geometric shape functions
procedure   Box(width, height, depth: float; solid: bool);
var
    i       : int;
    j       : bool;
    x, y, z : float;
begin
    x   := width / 2.0;
    y   := height / 2.0;
    z   := depth / 2.0;
    j   := false;

    for i := 0 to 3 do
    begin
        glRotatef(90.0, 0.0, 0.0, 1.0);
        if j then
        begin
            if not solid then
                glBegin(GL_LINE_LOOP)
            else
                glBegin(GL_QUADS);
            glNormal3f(-1.0, 0.0, 0.0);
            glVertex3f(-x, y, z);
            glVertex3f(-x, -y, z);
            glVertex3f(-x, -y, -z);
            glVertex3f(-x, y, -z);
            glEnd();
            if solid then
                glBegin(GL_TRIANGLES);
            glNormal3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, z);
            glVertex3f(-x, y, z);
            glVertex3f(-x, -y, z);
            glNormal3f(0.0, 0.0, -1.0);
            glVertex3f(0.0, 0.0, -z);
            glVertex3f(-x, -y, -z);
            glVertex3f(-x, y, -z);
            glEnd();
            j := false;
        end
        else
        begin
            if not solid then
                glBegin(GL_LINE_LOOP)
            else
                glBegin(GL_QUADS);
            glNormal3f(-1.0, 0.0, 0.0);
            glVertex3f(-y, x, z);
            glVertex3f(-y, -x, z);
            glVertex3f(-y, -x, -z);
            glVertex3f(-y, x, -z);
            glEnd();
            if solid then
                glBegin(GL_TRIANGLES);
            glNormal3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, z);
            glVertex3f(-y, x, z);
            glVertex3f(-y, -x, z);
            glNormal3f(0.0, 0.0, -1.0);
            glVertex3f(0.0, 0.0, -z);
            glVertex3f(-y, -x, -z);
            glVertex3f(-y, x, -z);
            glEnd();
            j := true;
        end;
    end;
end;

procedure   Octagon(side, height: float; solid: bool);
var
    j           : int;
    x, y, z, c  : float;
begin
    x   := sin(Pi/4) * side;
    y   := side / 2.0;
    z   := height / 2.0;

    c   := x + y;
    for j := 0 to 7 do
    begin
        glTranslatef(-c, 0.0, 0.0);
        if not solid then
            glBegin(GL_LINE_LOOP)
        else
            glBegin(GL_QUADS);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(0.0, -y, z);
        glVertex3f(0.0, y, z);
        glVertex3f(0.0, y, -z);
        glVertex3f(0.0, -y, -z);
        glEnd();
        glTranslatef(c, 0.0, 0.0);
        if solid then
        begin
            glBegin(GL_TRIANGLES);
            glNormal3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, z);
            glVertex3f(-c, -y, z);
            glVertex3f(-c, y, z);
            glNormal3f(0.0, 0.0, -1.0);
            glVertex3f(0.0, 0.0, -z);
            glVertex3f(-c, y, -z);
            glVertex3f(-c, -y, -z);
            glEnd();
        end;
        glRotatef(45.0, 0.0, 0.0, 1.0);
    end;
end;

type
    floatVec    = array[0..2] of float;

// end of geometric shape functions
{$IFDEF NORM}
procedure   Normalize(var v: floatVec);
var
    d: GLfloat;
begin
    d := sqrt(v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);

    if d = 0.0 then
        raise Exception.Create('Zero length vector');

    v[1] := v[1] / d;
    v[2] := v[2] / d;
    v[3] := v[3] / d;
end;

procedure   NormXprod(var v1, v2, v, _out: floatVec);
var
    i, j    : GLint;
    length  : GLfloat;
begin
    _out[0] := v1[1] * v2[2] - v1[2] * v2[1];
    _out[1] := v1[2] * v2[0] - v1[0] * v2[2];
    _out[2] := v1[0] * v2[1] - v1[1] * v2[0];
    Normalize(_out);
end;
{$ENDIF}

procedure   SetMaterial(spec, amb, diff, shin: PGLfloat);
begin
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shin);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
end;

procedure   MechTorso(solid: bool);
begin
    glNewList(SOLID_MECH_TORSO, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    Box(1.0, 1.0, 3.0, solid);
    glTranslatef(0.75, 0.0, 0.0);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
    glColor3f(0.5, 0.5, 0.5);
    Box(0.5, 0.6, 2.0, solid);
    glTranslatef(-1.5, 0.0, 0.0);
    Box(0.5, 0.6, 2.0, solid);
    glTranslatef(0.75, 0.0, 0.0);
    glEndList();
end;

procedure   MechHip(solid: bool);
var
    i   : int;
    hip : array[0..1] of PGLUquadricObj;
begin
    glNewList(SOLID_MECH_HIP, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    Octagon(0.7, 0.5, solid);
{$IFDEF SPHERE}
    for i := 0 to 1 do
    begin
        if i <> 0 then
            glScalef(-1.0, 1.0, 1.0);
        glTranslatef(1.0, 0.0, 0.0);
        hip[i] := gluNewQuadric();
{$IFDEF LIGHT}
        SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
        glColor3f(0.5, 0.5, 0.5);
        if not solid then
            gluQuadricDrawStyle(hip[i], GLU_LINE);
        gluSphere(hip[0], 0.2, 16, 16);
        glTranslatef(-1.0, 0.0, 0.0);
    end;
    glScalef(-1.0, 1.0, 1.0);
{$ENDIF}
    glEndList();
end;

procedure   Shoulder(solid: bool);
var
    deltoid : PGLUquadricObj;
begin
    deltoid := gluNewQuadric();
    glNewList(SOLID_MECH_SHOULDER, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    Box(1.0, 0.5, 0.5, solid);
    glTranslatef(0.9, 0.0, 0.0);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}    
    glColor3f(0.5, 0.5, 0.5);
{$IFDEF SPHERE}
    if not solid then
        gluQuadricDrawStyle(deltoid, GLU_LINE);
    gluSphere(deltoid, 0.6, 16, 16);
{$ENDIF}    
    glTranslatef(-0.9, 0.0, 0.0);
    glEndList();
end;

procedure   UpperArm(solid: bool);
var
    upper   : PGLUquadricObj;
    joint   : array[0..1] of PGLUquadricObj;
    joint1  : array[0..1] of PGLUquadricObj;
    i       : int;
begin
    upper   := gluNewQuadric();

    glNewList(SOLID_MECH_UPPER_ARM, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    Box(1.0, 2.0, 1.0, solid);
    glTranslatef(0.0, -0.95, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}    
    glColor3f(0.5, 0.5, 0.5);
    if not solid then
        gluQuadricDrawStyle(upper, GLU_LINE);
    gluCylinder(upper, 0.4, 0.4, 1.5, 16, 10);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}    
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(-0.4, -1.85, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    for i := 0 to 1 do
    begin
        joint[i] := gluNewQuadric();
        if not solid then
            gluQuadricDrawStyle(joint[i], GLU_LINE);
        if i <> 0 then
            gluCylinder(joint[i], 0.5, 0.5, 0.8, 16, 10)
        else
            gluCylinder(joint[i], 0.2, 0.2, 0.8, 16, 10);
    end;
    for i := 0 to 1 do
    begin
        if i <> 0 then
            glScalef(-1.0, 1.0, 1.0);
        joint1[i] := gluNewQuadric();
        if not solid then
            gluQuadricDrawStyle(joint1[i], GLU_LINE);
        if i <> 0 then
            glTranslatef(0.0, 0.0, 0.8);
        gluDisk(joint1[i], 0.2, 0.5, 16, 10);
        if i <> 0 then
            glTranslatef(0.0, 0.0, -0.8);
    end;
    glScalef(-1.0, 1.0, 1.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glTranslatef(0.4, 2.9, 0.0);
    glEndList();
end;

procedure   VulcanGun(solid: bool);
var
    i               : int;
    Barrel          : array[0..4] of PGLUquadricObj;
    BarrelFace      : array[0..4] of PGLUquadricObj;
    Barrel2         : array[0..4] of PGLUquadricObj;
    Barrel3         : array[0..4] of PGLUquadricObj;
    BarrelFace2     : array[0..4] of PGLUquadricObj;
    Mount           : PGLUquadricObj ;
    Mount_face      : PGLUquadricObj ;
begin
    Mount           := gluNewQuadric();
    Mount_face      := gluNewQuadric();

    glNewList(SOLID_MECH_VULCAN, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
    glColor3f(0.5, 0.5, 0.5);

    if not solid then
    begin
        gluQuadricDrawStyle(Mount, GLU_LINE);
        gluQuadricDrawStyle(Mount_face, GLU_LINE);
    end;

    gluCylinder(Mount, 0.5, 0.5, 0.5, 16, 10);
    glTranslatef(0.0, 0.0, 0.5);
    gluDisk(Mount_face, 0.0, 0.5, 16, 10);

    for i := 0 to 4 do
    begin
        Barrel[i]       := gluNewQuadric();
        BarrelFace[i]   := gluNewQuadric();
        BarrelFace2[i]  := gluNewQuadric();
        Barrel2[i]      := gluNewQuadric();
        Barrel3[i]      := gluNewQuadric();
        glRotatef(72.0, 0.0, 0.0, 1.0);
        glTranslatef(0.0, 0.3, 0.0);
        if not solid then
        begin
            gluQuadricDrawStyle(Barrel[i], GLU_LINE);
            gluQuadricDrawStyle(BarrelFace[i], GLU_LINE);
            gluQuadricDrawStyle(BarrelFace2[i], GLU_LINE);
            gluQuadricDrawStyle(Barrel2[i], GLU_LINE);
            gluQuadricDrawStyle(Barrel3[i], GLU_LINE);
        end;
        gluCylinder(Barrel[i], 0.15, 0.15, 2.0, 16, 10);
        gluCylinder(Barrel3[i], 0.06, 0.06, 2.0, 16, 10);
        glTranslatef(0.0, 0.0, 2.0);
        gluDisk(BarrelFace[i], 0.1, 0.15, 16, 10);
        gluCylinder(Barrel2[i], 0.1, 0.1, 0.1, 16, 5);
        glTranslatef(0.0, 0.0, 0.1);
        gluDisk(BarrelFace2[i], 0.06, 0.1, 16, 5);
        glTranslatef(0.0, -0.3, -2.1);
    end;
    glEndList();
end;

procedure   ForeArm(solid: bool);
var
    i: int;
begin
    glNewList(SOLID_MECH_FOREARM, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}    
    glColor3f(1.0, 1.0, 0.0);
    for i := 0 to 4 do
    begin
        glTranslatef(0.0, -0.1, -0.15);
        Box(0.6, 0.8, 0.2, solid);
        glTranslatef(0.0, 0.1, -0.15);
        Box(0.4, 0.6, 0.1, solid);
    end;
    glTranslatef(0.0, 0.0, 2.45);
    Box(1.0, 1.0, 2.0, solid);
    glTranslatef(0.0, 0.0, -1.0);
    glEndList();
end;

procedure   UpperLeg(solid: bool);
var
    i           : int;
    Hamstring   : PGLUquadricObj;
    Knee        : PGLUquadricObj;
    joint       : array[0..1] of PGLUquadricObj;
begin
    Hamstring   := gluNewQuadric();
    Knee        := gluNewQuadric();

    glNewList(SOLID_MECH_UPPER_LEG, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}    
    glColor3f(1.0, 1.0, 0.0);
    if not solid then
    begin
        gluQuadricDrawStyle(Hamstring, GLU_LINE);
        gluQuadricDrawStyle(Knee, GLU_LINE);
    end;
    glTranslatef(0.0, -1.0, 0.0);
    Box(0.4, 1.0, 0.7, solid);
    glTranslatef(0.0, -0.65, 0.0);
    for i := 0 to 4 do
    begin
        Box(1.2, 0.3, 1.2, solid);
        glTranslatef(0.0, -0.2, 0.0);
        Box(1.0, 0.1, 1.0, solid);
        glTranslatef(0.0, -0.2, 0.0);
    end;
    glTranslatef(0.0, -0.15, -0.4);
    Box(2.0, 0.5, 2.0, solid);
    glTranslatef(0.0, -0.3, -0.2);
    glRotatef(90.0, 1.0, 0.0, 0.0);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}    
    glColor3f(0.5, 0.5, 0.5);
    gluCylinder(Hamstring, 0.6, 0.6, 3.0, 16, 10);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -1.5, 1.0);
    Box(1.5, 3.0, 0.5, solid);
    glTranslatef(0.0, -1.75, -0.8);
    Box(2.0, 0.5, 2.0, solid);
    glTranslatef(0.0, -0.9, -0.85);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
    glColor3f(0.5, 0.5, 0.5);
    gluCylinder(Knee, 0.8, 0.8, 1.8, 16, 10);
    for i := 0 to 1 do
    begin
        if i <> 0 then
            glScalef(-1.0, 1.0, 1.0);
        joint[i] := gluNewQuadric();
        if not solid then
            gluQuadricDrawStyle(joint[i], GLU_LINE);
        if i <> 0 then
            glTranslatef(0.0, 0.0, 1.8);
        gluDisk(joint[i], 0.0, 0.8, 16, 10);
        if i <> 0 then
            glTranslatef(0.0, 0.0, -1.8);
    end;
    glScalef(-1.0, 1.0, 1.0);
    glEndList();
end;

procedure   Foot(solid: bool);
begin
    glNewList(SOLID_MECH_FOOT, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
    glColor3f(0.5, 0.5, 0.5);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    Octagon(1.5, 0.6, solid);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glEndList();
end;

procedure   LowerLeg(solid: bool);
var
    k, l        : int;
    ankle       : PGLUquadricObj;
    ankle_face  : array[0..1] of PGLUquadricObj;
    j           : int;
begin
    ankle := gluNewQuadric();
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}    
    glColor3f(1.0, 1.0, 0.0);
    for k := 0 to 1 do
    begin
        for l := 0 to 1 do
        begin
            glPushMatrix();
            glTranslatef(k, 0.0, l);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
            glColor3f(1.0, 1.0, 0.0);
            Box(1.0, 0.5, 1.0, solid);
            glTranslatef(0.0, -0.45, 0.0);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
            glColor3f(0.5, 0.5, 0.5);
{$IFDEF SPHERE}
            if not solid then
                glutWireSphere(0.2, 16, 10)
            else
                glutSolidSphere(0.2, 16, 10);
            {
              joints[i] = gluNewQuadric();
              if not solid then
                gluQuadricDrawStyle(joints[i], GLU_LINE);
              gluSphere(joints[i],0.2, 16, 16);
              Inc(i);
            }
{$ENDIF}
            if leg then
                glRotatef(heel1, 1.0, 0.0, 0.0)
            else
                glRotatef(heel2, 1.0, 0.0, 0.0);
            // glTranslatef(0.0, -0.2, 0.0);
            glTranslatef(0.0, -1.7, 0.0);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}            
            glColor3f(1.0, 1.0, 0.0);
            Box(0.25, 3.0, 0.25, solid);
            glTranslatef(0.0, -1.7, 0.0);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
            glColor3f(0.5, 0.5, 0.5);
{$IFDEF SPHERE}
            if not solid then
                glutWireSphere(0.2, 16, 10)
            else
                glutSolidSphere(0.2, 16, 10);
            {
                joints[i] = gluNewQuadric();
                if not solid then
                    gluQuadricDrawStyle(joints[i], GLU_LINE);
                gluSphere(joints[i], 0.2, 16, 16);
                Inc(i);
            }
{$ENDIF}
            if leg then
                glRotatef(- heel1, 1.0, 0.0, 0.0)
            else
                glRotatef(- heel2, 1.0, 0.0, 0.0);
            glTranslatef(0.0, -0.45, 0.0);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
            glColor3f(1.0, 1.0, 0.0);
            Box(1.0, 0.5, 1.0, solid);
            if (k = 0) and (l = 0) then
            begin
                glTranslatef(-0.4, -0.8, 0.5);
                if leg then
                    glRotatef(ankle1, 1.0, 0.0, 0.0)
                else
                    glRotatef(ankle2, 1.0, 0.0, 0.0);
                glRotatef(90.0, 0.0, 1.0, 0.0);
                if not solid then
                    gluQuadricDrawStyle(ankle, GLU_LINE);
                gluCylinder(ankle, 0.8, 0.8, 1.8, 16, 10);
                for j := 0 to 1 do
                begin
                    ankle_face[j] := gluNewQuadric();
                    if not solid then
                        gluQuadricDrawStyle(ankle_face[j], GLU_LINE);
                    if j <> 0 then
                    begin
                        glScalef(-1.0, 1.0, 1.0);
                        glTranslatef(0.0, 0.0, 1.8);
                    end;
                    gluDisk(ankle_face[j], 0.0, 0.8, 16, 10);
                    if j <> 0 then
                        glTranslatef(0.0, 0.0, -1.8);
                end;
                glScalef(-1.0, 1.0, 1.0);
                glRotatef(-90.0, 0.0, 1.0, 0.0);
                glTranslatef(0.95, -0.8, 0.0);
                glCallList(SOLID_MECH_FOOT);
            end;
            glPopMatrix();
        end;
    end;
end;

procedure   RocketPod(solid: bool);
var
    i, j, k : int;
    rocket  : array[0..5] of PGLUquadricObj;
    rocket1 : array[0..5] of PGLUquadricObj;
begin
    k := 0;

    glNewList(SOLID_MECH_ROCKET, GL_COMPILE);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}
    glColor3f(0.5, 0.5, 0.5);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);
    Box(2.0, 0.5, 3.0, solid);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glTranslatef(0.5, 0.0, 0.0);
    Box(1.2, 0.5, 3.0, solid);
    glTranslatef(2.1, 0.0, 0.0);
    glRotatef(-90.0, 0.0, 0.0, 1.0);
{$IFDEF LIGHT}
    SetMaterial(@mat_specular, @mat_ambient, @mat_diffuse, @mat_shininess);
{$ENDIF}
    glColor3f(1.0, 1.0, 0.0);
    Box(2.0, 3.0, 4.0, solid);
    glTranslatef(-0.5, -1.0, 1.3);
    for i := 0 to 1 do
        for j := 0 to 2 do
        begin
            rocket[k]   := gluNewQuadric();
            rocket1[k]  := gluNewQuadric();
            if not solid then
            begin
                gluQuadricDrawStyle(rocket[k], GLU_LINE);
                gluQuadricDrawStyle(rocket1[k], GLU_LINE);
            end;
            glTranslatef(i, j, 0.6);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular3, @mat_ambient3, @mat_diffuse3, @mat_shininess3);
{$ENDIF}
            glColor3f(1.0, 1.0, 1.0);
            gluCylinder(rocket[k], 0.4, 0.4, 0.3, 16, 10);
            glTranslatef(0.0, 0.0, 0.3);
{$IFDEF LIGHT}
            SetMaterial(@mat_specular4, @mat_ambient4, @mat_diffuse4, @mat_shininess4);
{$ENDIF}
            glColor3f(0.0, 1.0, 0.0);
            gluCylinder(rocket1[k], 0.4, 0.0, 0.5, 16, 10);
            Inc(k);
            glTranslatef(-i, -j, -0.9);
        end;
    glEndList();
end;

procedure   Enviro(solid: bool);
var
    i, j : int;
begin
    glNewList(SOLID_ENVIRO, GL_COMPILE);
    SetMaterial(@mat_specular4, @mat_ambient4, @mat_diffuse4, @mat_shininess4);
    glColor3f(0.0, 1.0, 0.0);
    Box(20.0, 0.5, 30.0, solid);
    SetMaterial(@mat_specular4, @mat_ambient3, @mat_diffuse2, @mat_shininess);
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(0.0, 0.0, -10.0);
    for j := 0 to 5 do
    begin
        for i := 0 to 1 do
        begin
            if i <> 0 then
                glScalef(-1.0, 1.0, 1.0);
            glTranslatef(10.0, 4.0, 0.0);
            Box(4.0, 8.0, 2.0, solid);
            glTranslatef(0.0, -1.0, -3.0);
            Box(4.0, 6.0, 2.0, solid);
            glTranslatef(-10.0, -3.0, 3.0);
        end;
        glScalef(-1.0, 1.0, 1.0);
        glTranslatef(0.0, 0.0, 5.0);
    end;
    glEndList();
end;

procedure   Toggle;
begin
    solid_part := not solid_part;
end;

procedure   Disable;
begin
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
end;

procedure   Lighting;
const
    position    : array[0..3] of GLfloat = ( 0.0, 0.0, 2.0, 1.0 );
begin
{$IFDEF MOVE_LIGHT}
    glRotatef(lightturn1, 1.0, 0.0, 0.0);
    glRotatef(lightturn, 0.0, 1.0, 0.0);
    glRotatef(0.0, 1.0, 0.0, 0.0);
{$ENDIF}    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_FLAT);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLightfv(GL_LIGHT0, GL_POSITION, @position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0);

    glTranslatef(0.0, 0.0, 2.0);
    glDisable(GL_LIGHTING);
    Box(0.1, 0.1, 0.1, false);
    glEnable(GL_LIGHTING);
end;

procedure   DrawMech;
var
    i, j: int;
begin
    glScalef(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(0.0, -0.75, 0.0);
    glRotatef(tilt, 1.0, 0.0, 0.0);

    glRotatef(90.0, 1.0, 0.0, 0.0);
{$IFDEF HIP}
    glCallList(SOLID_MECH_HIP);
{$ENDIF}
    glRotatef(-90.0, 1.0, 0.0, 0.0);

    glTranslatef(0.0, 0.75, 0.0);
    glPushMatrix();
    glRotatef(pivot, 0.0, 1.0, 0.0);
    glPushMatrix();
{$IFDEF TORSO}
    glCallList(SOLID_MECH_TORSO);
{$ENDIF}
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.0);
{$IFDEF ROCKET_POD}
    glCallList(SOLID_MECH_ROCKET);
{$ENDIF}    
    glPopMatrix();
    for i := 0 to 1 do
    begin
        glPushMatrix();
        if i <> 0 then
            glScalef(-1.0, 1.0, 1.0);
        glTranslatef(1.5, 0.0, 0.0);
{$IFDEF SHOULDER}
        glCallList(SOLID_MECH_SHOULDER);
{$ENDIF}
        glTranslatef(0.9, 0.0, 0.0);
        if i <> 0 then
        begin
            glRotatef(lat1, 0.0, 0.0, 1.0);
            glRotatef(shoulder1, 1.0, 0.0, 0.0);
            glRotatef(shoulder3, 0.0, 1.0, 0.0);
        end
        else
        begin
            glRotatef(lat2, 0.0, 0.0, 1.0);
            glRotatef(shoulder2, 1.0, 0.0, 0.0);
            glRotatef(shoulder4, 0.0, 1.0, 0.0);
        end;
        glTranslatef(0.0, -1.4, 0.0);
{$IFDEF UPPER_ARM}
        glCallList(SOLID_MECH_UPPER_ARM);
{$ENDIF}
        glTranslatef(0.0, -2.9, 0.0);
        if i <> 0 then
            glRotatef(elbow1, 1.0, 0.0, 0.0)
        else
            glRotatef(elbow2, 1.0, 0.0, 0.0);
        glTranslatef(0.0, -0.9, -0.2);
{$IFDEF LOWER_ARM}
        glCallList(SOLID_MECH_FOREARM);
        glPushMatrix();
        glTranslatef(0.0, 0.0, 2.0);
        glRotatef(fire, 0.0, 0.0, 1.0);
        glCallList(SOLID_MECH_VULCAN);
        glPopMatrix();
{$ENDIF}
        glPopMatrix();
    end;
    glPopMatrix();
    glPopMatrix();
    for j := 0 to 1 do
    begin
        glPushMatrix();
        if j <> 0 then
        begin
            glScalef(-0.5, 0.5, 0.5);
            leg := True;
        end
        else
        begin
            glScalef(0.5, 0.5, 0.5);
            leg := False;
        end;
        glTranslatef(2.0, -1.5, 0.0);
        if j <> 0 then
        begin
            glRotatef(hip11, 1.0, 0.0, 0.0);
            glRotatef(hip12, 0.0, 0.0, 1.0);
        end
        else
        begin
            glRotatef(hip21, 1.0, 0.0, 0.0);
            glRotatef(hip22, 0.0, 0.0, 1.0);
        end;
        glTranslatef(0.0, 0.3, 0.0);
{$IFDEF UPPER_LEG}
        glPushMatrix();
        glCallList(SOLID_MECH_UPPER_LEG);
        glPopMatrix();
{$ENDIF}
        glTranslatef(0.0, -8.3, -0.4);
        if j <> 0 then
            glRotatef(- hip12, 0.0, 0.0, 1.0)
        else
            glRotatef(- hip22, 0.0, 0.0, 1.0);
        glTranslatef(-0.5, -0.85, -0.5);
{$IFDEF LOWER_LEG}
        LowerLeg(true);
{$ENDIF}        
        glPopMatrix();
    end;
end;

procedure   Display; cdecl;
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glRotatef(turn, 0.0, 1.0, 0.0);
    glRotatef(turn1, 1.0, 0.0, 0.0);
{$IFDEF LIGHT}
    if solid_part then
    begin
        glPushMatrix();
        lighting();
        glPopMatrix();
    end
    else
        disable();
{$ENDIF}
{$IFDEF DRAW_MECH}
    glPushMatrix();
    glTranslatef(0.0, elevation, 0.0);
    DrawMech();
    glPopMatrix();
{$ENDIF}    
{$IFDEF DRAW_ENVIRO}
    glPushMatrix();
    if distance >= 20.136 then
        distance := 0.0;
    glTranslatef(0.0, -5.0, -distance);
    glCallList(SOLID_ENVIRO);
    glTranslatef(0.0, 0.0, 10.0);
    glCallList(SOLID_ENVIRO);
    glPopMatrix();
{$ENDIF}    
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
end;

procedure   MyInit;
var
    i: bool;
begin
    i := true;
{$IFDEF LIGHT}
    SetMaterial(@mat_specular2, @mat_ambient2, @mat_diffuse2, @mat_shininess2);
{$ENDIF}    
    glEnable(GL_DEPTH_TEST);
    MechTorso(i);
    MechHip(i);
    Shoulder(i);
    RocketPod(i);
    UpperArm(i);
    ForeArm(i);
    UpperLeg(i);
    Foot(i);
    VulcanGun(i);
    Enviro(i);
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, w / h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 1.2, -5.5); // viewing transform
end;

{$IFDEF ANIMATION}
var
    step    : int = 0;

procedure   animation_walk;
var
    angle   : float;
begin
    if (step = 0) or (step = 2) then
    begin
        // for(frame=3.0; frame<=21.0; frame=frame+3.0){
        if (frame >= 0.0) and (frame <= 21.0) then
        begin
            if frame = 0.0 then
                frame := 3.0;
            angle := (180 / Pi) * (ArcCos(((cos((Pi / 180) * frame) * 2.043) + 1.1625) / 3.2059));
            if frame > 0 then
                elevation := -(3.2055 - (cos((Pi / 180) * angle) * 3.2055))
            else
                elevation := 0.0;
            if step = 0 then
            begin
                hip11       := Trunc(-(frame * 1.7));
                if 1.7 * frame > 15 then
                  heel1 := Trunc(frame * 1.7);
                heel2       := 0;
                ankle1      := Trunc(frame * 1.7);
                if (frame > 0) then
                  hip21 := Trunc(angle)
                else
                  hip21 := 0;
                ankle2      := -hip21;
                shoulder1   := Trunc(frame * 1.5);
                shoulder2   := Trunc(-frame * 1.5);
                elbow1      := Trunc(frame);
                elbow2      := Trunc(-frame);
            end
            else
            begin
                hip21       := Trunc(-(frame * 1.7));
                if 1.7 * frame > 15 then
                  heel2 := Trunc(frame * 1.7);
                heel1       := 0;
                ankle2      := Trunc(frame * 1.7);
                if frame > 0 then
                  hip11 := Trunc(angle)
                else
                  hip11 := 0;
                ankle1      := -hip11;
                shoulder1   := Trunc(-frame * 1.5);
                shoulder2   := Trunc(frame * 1.5);
                elbow1      := Trunc(-frame);
                elbow2      := Trunc(frame);
            end;
            if frame = 21 then
                Inc(step);
            if frame < 21 then
                frame := frame + 3.0;
        end;
    end;

    if (step = 1) or (step = 3) then
    begin
        // for(x=21.0; x>=0.0; x=x-3.0){
        if (frame <= 21.0) and (frame >= 0.0) then
        begin
            angle := (180 / Pi) * (ArcCos(((cos((Pi / 180) * frame) * 2.043) + 1.1625) / 3.2029));
            if frame > 0 then
                elevation := -(3.2055 - (cos((PI / 180) * angle) * 3.2055))
            else
                elevation := 0.0;
            if step = 1 then
            begin
                hip11       := Trunc(-frame);
                elbow2      := hip11;
                heel1       := Trunc(frame);
                elbow1      := heel1;
                heel2       := 15;
                ankle1      := Trunc(frame);
                if frame > 0 then
                    hip21 := Trunc(angle)
                else
                    hip21 := 0;
                ankle2      := -hip21;
                shoulder1   := Trunc(1.5 * frame);
                shoulder2   := Trunc(-frame * 1.5);
            end
            else
            begin
                hip21       := Trunc(-frame);
                elbow1      := hip21;
                heel2       := Trunc(frame);
                elbow2      := heel2;
                heel1       := 15;
                ankle2      := Trunc(frame);
                if frame > 0 then
                    hip11 := Trunc(angle)
                else
                    hip11 := 0;
                ankle1      := -hip11;
                shoulder1   := Trunc(-frame * 1.5);
                shoulder2   := Trunc(frame * 1.5);
            end;

            if frame = 0.0 then
                Inc(step);
            if frame > 0 then
                frame := frame - 3.0;
        end;
    end;

    if step = 4 then
        step := 0;
        
    distance := distance + 0.1678;
    glutPostRedisplay();
end;

procedure   Animation; cdecl;
begin
    animation_walk();
end;
{$ENDIF}

procedure   Keyboard(key: uchar; x, y: int); cdecl;
begin
    case Chr(key) of
        // start arm control functions
        'q' : shoulder2Subtract();
        'a' : shoulder2Add();
        'w' : shoulder1Subtract();
        's' : shoulder1Add();
        '2' : shoulder3Add();
        '1' : shoulder4Add();
        '4' : shoulder3Subtract();
        '3' : shoulder4Subtract();
        'z' : lat2Raise();
        'Z' : lat2Lower();
        'x' : lat1Raise();
        'X' : lat1Lower();
        'A' : elbow2Add();
        'Q' : elbow2Subtract();
        'S' : elbow1Add();
        'W' : elbow1Subtract();

        // end of arm control functions

        // start of torso control functions
        'd' : RotateAdd();
        'g' : RotateSubtract();
        'r' : MechTiltAdd();
        'f' : MechTiltSubtract();
        // end of torso control functions

        // start of leg control functions
        'h' : RaiseLeg2Forward();
        'y' : LowerLeg2Backwards();
        'Y' : RaiseLeg2Outwards();
        'H' : LowerLeg2Inwards();
        'j' : RaiseLeg1Forward();
        'u' : LowerLeg1Backwards();
        'U' : RaiseLeg1Outwards();
        'J' : LowerLeg1Inwards();
        'N' : Heel2Add();
        'n' : Heel2Subtract();
        'M' : Heel1Add();
        'm' : Heel1Subtract();
        'k' : Ankle2Add();
        'K' : Ankle2Subtract();
        'l' : Ankle1Add();
        'L' : Ankle1Subtract();
        // end of leg control functions

        // start of light source position functions
        'p' : LightTurnRight();
        'i' : LightTurnLeft();
        'o' : LightForwards();
        '9' : LightBackwards();
        // end of light source position functions
    else
        Exit;
    end;
    glutPostRedisplay();
end;

procedure   Special(key: int; x, y: int); cdecl;
begin
    case key of
        // start of view position functions
        GLUT_KEY_RIGHT      : TurnRight();
        GLUT_KEY_LEFT       : TurnLeft();
        GLUT_KEY_DOWN       : TurnForwards();
        GLUT_KEY_UP         : TurnBackwards();
        // end of view postions functions
        // start of miseclleneous functions
        GLUT_KEY_PAGE_UP    : FireCannon();
        // end of miscelleneous functions
    else
        Exit;
    end;
    glutPostRedisplay();
end;

procedure   MenuSelect(mode: int); cdecl;
begin
    case mode of
{$IFDEF ANIMATION}
        1   : ; //    glutIdleFunc(animation);
{$ENDIF}
        2   : ; //    glutIdleFunc(NULL);
        3   : begin
                Toggle();
                glutPostRedisplay();
              end;
        4   : glutExit;
    end;
end;

procedure   NullSelect(mode: int); cdecl;
begin
    ;
end;

procedure   glutMenu;
var
    glut_menu   : array[0..12] of int;
begin
    glut_menu[5] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('forward       : q,w', 0);
    glutAddMenuEntry('backwards     : a,s', 0);
    glutAddMenuEntry('outwards      : z,x', 0);
    glutAddMenuEntry('inwards       : Z,X', 0);

    glut_menu[6] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('upwards       : Q,W', 0);
    glutAddMenuEntry('downwards     : A,S', 0);
    glutAddMenuEntry('outwards      : 1,2', 0);
    glutAddMenuEntry('inwards       : 3,4', 0);

    glut_menu[1] := glutCreateMenu(NullSelect);
    glutAddMenuEntry(' : Page_up', 0);

    glut_menu[8] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('forward       : y,u', 0);
    glutAddMenuEntry('backwards     : h.j', 0);
    glutAddMenuEntry('outwards      : Y,U', 0);
    glutAddMenuEntry('inwards       : H,J', 0);

    glut_menu[9] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('forward       : n,m', 0);
    glutAddMenuEntry('backwards     : N,M', 0);

    glut_menu[9] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('forward       : n,m', 0);
    glutAddMenuEntry('backwards     : N,M', 0);

    glut_menu[10] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('toes up       : K,L', 0);
    glutAddMenuEntry('toes down     : k,l', 0);

    glut_menu[11] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('right         : right arrow', 0);
    glutAddMenuEntry('left          : left arrow', 0);
    glutAddMenuEntry('down          : up arrow', 0);
    glutAddMenuEntry('up            : down arrow', 0);

    glut_menu[12] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('right         : p', 0);
    glutAddMenuEntry('left          : i', 0);
    glutAddMenuEntry('up            : 9', 0);
    glutAddMenuEntry('down          : o', 0);

    glut_menu[4] := glutCreateMenu(nil);
    glutAddSubMenu('at the shoulders? ', glut_menu[5]);
    glutAddSubMenu('at the elbows?', glut_menu[6]);

    glut_menu[7] := glutCreateMenu(nil);
    glutAddSubMenu('at the hip? ', glut_menu[8]);
    glutAddSubMenu('at the knees?', glut_menu[9]);
    glutAddSubMenu('at the ankles? ', glut_menu[10]);

    glut_menu[2] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('turn left    : d', 0);
    glutAddMenuEntry('turn right    : g', 0);

    glut_menu[3] := glutCreateMenu(NullSelect);
    glutAddMenuEntry('tilt backwards : f', 0);
    glutAddMenuEntry('tilt forwards  : r', 0);

    glut_menu[0] := glutCreateMenu(nil);
    glutAddSubMenu('move the arms.. ', glut_menu[4]);
    glutAddSubMenu('fire the vulcan guns?', glut_menu[1]);
    glutAddSubMenu('move the legs.. ', glut_menu[7]);
    glutAddSubMenu('move the torso?', glut_menu[2]);
    glutAddSubMenu('move the hip?', glut_menu[3]);
    glutAddSubMenu('rotate the scene..', glut_menu[11]);
{$IFDEF MOVE_LIGHT}
    glutAddSubMenu('rotate the light source..', glut_menu[12]);
{$ENDIF}    

    glutCreateMenu(MenuSelect);
{$IFDEF ANIMATION}
    glutAddMenuEntry('Start Walk', 1);
    glutAddMenuEntry('Stop Walk', 2);
{$ENDIF}
    glutAddMenuEntry('Toggle Wireframe', 3);
    glutAddSubMenu('How do I ..', glut_menu[0]);
    glutAddMenuEntry('Quit', 4);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
end;

procedure   Demo;
begin
    // start of glut windowing and control functions
    InitGLUT;
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGBA or GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow('glutmech: Vulcan Gunner');
    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(animation);
    glutMenu();
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos',
                 'Mech',
                 '*** No description yet',
                 Demo);
end.
