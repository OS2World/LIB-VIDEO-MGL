//
// "moth" by Robert Doyle, Naval Research Laboratory, Washington, DC.
// Scene objects are built into display lists in the 'myInit' function
// (look for three rows of I's). Objects are assembled and motion
// described in the 'display' function (look for three rows of $'s).
//
unit Moth;

interface

implementation

uses
    Classes,
    SysUtils,
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    CmdLine,
    DemoList;

type
    floatCell               = array[0..0] of GLfloat;
    floatRow                = array[0..3] of GLfloat;
    floatRow3               = array[0..2] of GLfloat;
    floatMat                = array[0..3] of floatRow;

const
    wall_color              : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    wall_ambient            : floatRow = ( 0.5, 0.5, 0.5, 1.0 );
    floor_color             : floatRow = ( 0.5, 1.0, 0.5, 0.5 );
    column_color            : floatRow = ( 1.0, 0.0, 0.0, 1.0 );
    column_ambient          : floatRow = ( 0.25, 0.0, 0.0, 1.0 );

    panel_color             : floatRow = ( 0.1, 0.1, 1.0, 1.0 );
    panel_ambient           : floatRow = ( 0.01, 0.01, 0.3, 1.0 );

    lamp_ambient            : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    lamp_diffuse            : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    lamp_specular           : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    lamp_post_diffuse       : floatRow = ( 0.8, 0.0, 0.0, 1.0 );
    lamp_post_specular      : floatRow = ( 0.8, 0.0, 0.0, 1.0 );
    lamp_post_ambient       : floatRow = ( 0.25, 0.0, 0.0, 1.0 );

    satellite_diffuse       : floatRow = ( 1.0, 0.69, 0.0, 1.0 );
    satellite_shiny         : floatCell = ( 128.0 );
    satellite_specular      : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    satellite_ambient       : floatRow = ( 0.37, 0.25, 0.0, 1.0 );

    cube_color              : floatRow = ( 1.0, 1.0, 0.0, 1.0 );
    cube_shiny              : floatCell = ( 99.0 );
    cube_specular           : floatRow = ( 0.9, 0.9, 0.9, 1.0 );
    cube_ambient            : floatRow = ( 0.1, 0.1, 0.1, 1.0 );

    shadow_ambient          : floatRow = ( 0.0, 0.0, 0.0, 1.0 );
    shadow_diffuse          : floatRow = ( 0.0, 0.0, 0.0, 0.3 );
    shadow_shiny            : floatCell = ( 0.0 );
    shadow_specular         : floatRow = ( 0.0, 0.0, 0.0, 1.0 );

    column                  = 3;
    ground_shadow_matrix    = 16;
    left_wall_shadow_matrix = 17;
    ground                  = 30;
    left_wall               = 40;
    right_wall              = 50;
    four_columns            = 7;
    two_columns             = 32;
    satellite1              = 301;
    satellite2              = 302;
    panel1                  = 303;
    panel2                  = 304;

    Tx                      = -0.01 * 5;
    Ty                      = -0.01 * 5;
    Tz                      = -0.02 * 5;
    mvt_zi                  = -0.7;
{
    mvt_x                   = 0.0;
    mvt_y                   = 0.0;
    mvt_z                   = -0.7;
}

var
    mvt_x                   : float = -15.0;
    mvt_y                   : float = -15.0;
    mvt_z                   : float = -30.7;

{
    Rx                      : float = 0.1;
    mvr_d                   : float = 0.0;
    mvr_x                   : float = 1.0;
    mvr_y                   : float = -1.0;
    mvr_z                   : float = -1.0;
}

    Rx                      : float = 0.1 * 5;
    mvr_d                   : float = 150.0;
    mvr_x                   : float = 1.0;
    mvr_y                   : float = -1.0;
    mvr_z                   : float = -1.0;

var
    cubeXform               : floatMat;
    column1Xform            : floatMat;
    column2Xform            : floatMat;
    column3Xform            : floatMat;
    four_columnsXform       : floatMat;

    // nest : array[0..0] of GLint;*/
    shadowMat_ground        : floatMat;
    shadowMat_left          : floatMat;
    shadowMat_back          : floatMat;
    shadowMat_column        : floatMat;
    shadowMat_right         : floatMat;

    shadowMat1_ground       : floatMat;
    shadowMat1_left         : floatMat;
    shadowMat1_back         : floatMat;
    shadowMat1_right        : floatMat;

    useRGB                  : bool  = true;
    useLighting             : bool  = true;
    useFog                  : bool  = false;
    useDB                   : bool  = true;
    useLogo                 : bool  = false;
    useQuads                : bool  = true;

    tick                    : int   = -1;
    moving                  : bool  = true;

    lmodel_ambient          : floatRow = ( 0.2, 0.2, 0.2, 1.0 );
    lightPos                : floatRow = ( 1.0, 2.5, 3.0, 1.0 );
    lightDir                : floatRow = ( -2.0, -4.0, -2.0, 1.0 );
    lightAmb                : floatRow = ( 0.2, 0.2, 0.2, 1.0 );
    lightDiff               : floatRow = ( 0.3, 0.3, 0.3, 1.0 );
    lightSpec               : floatRow = ( 0.4, 0.4, 0.4, 1.0 );

    light1Pos               : floatRow = ( 0.0, 1.6, -5.0, 1.0 );
    light1Amb               : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    light1Diff              : floatRow = ( 1.0, 1.0, 1.0, 1.0 );
    light1Spec              : floatRow = ( 1.0, 1.0, 1.0, 1.0 );

    leftPlane               : floatRow = ( 1.0, 0.0, 0.0, 4.88 );   // X = -4.88 
    rightPlane              : floatRow = ( -1.0, 0.0, 0.0, 4.88 );  // X = 4.98
    groundPlane             : floatRow = ( 0.0, 1.0, 0.0, 1.450 );  // Y = -1.480
    columnPlane             : floatRow = ( 0.0, 0.0, 1.0, 0.899 );  // Z = -0.899
    backPlane               : floatRow = ( 0.0, 0.0, 1.0, 8.98 );   // Z = -8.98

    column_clipPlaneL       : array[0..3] of GLdouble = ( 1.0, 0.0, 0.0, 2.6 );
                                                                    // X < -2.6
{
    column_clipPlaneR       : array[0..3] of GLdouble = ( -1.0, 0.0, 0.0, 2.4 );
                                                                    // X > 2.4 
}

    fogColor                : floatRow = ( 0.0, 0.0, 0.0, 0.0 );
    fogIndex                : floatCell = ( 0.0 );

const
    S   = 0.7071;
    NS  = 0.382683;
    NC  = 0.923880;

var
    // satellite body.
    oct_vertices            : array[0..7,0..2,0..3] of float =
        (
            (
                (0.0, 0.0, 0.0, 1.0),
                (0.0, 1.0, 0.0, 1.0),
                (-S, S, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (-S, S, 0.0, 1.0),
                (-1.0, 0.0, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (-1.0, 0.0, 0.0, 1.0),
                (-S, -S, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (-S, -S, 0.0, 1.0),
                (0.0, -1.0, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (0.0, -1.0, 0.0, 1.0),
                (S, -S, 0.0, 1.0)),
        
            (
        
                (0.0, 0.0, 0.0, 1.0),
                (S, -S, 0.0, 1.0),
                (1.0, 0.0, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (1.0, 0.0, 0.0, 1.0),
                (S, S, 0.0, 1.0)),

            (
                (0.0, 0.0, 0.0, 1.0),
                (S, S, 0.0, 1.0),
                (0.0, 1.0, 0.0, 1.0))
        );

    oct_side_vertices       : array[0..7,0..3,0..3] of float =
        (
            (
                (-S, S, 0.0, 1.0),
                (0.0, 1.0, 0.0, 1.0),
                (0.0, 1.0, -1.0, 1.0),
                (-S, S, -1.0, 1.0)),

            (
                (-1.0, 0.0, 0.0, 1.0),
                (-S, S, 0.0, 1.0),
                (-S, S, -1.0, 1.0),
                (-1.0, 0.0, -1.0, 1.0)),
        
            (
                (-S, -S, 0.0, 1.0),
                (-1.0, 0.0, 0.0, 1.0),
                (-1.0, 0.0, -1.0, 1.0),
                (-S, -S, -1.0, 1.0)),
        
            (
                (0.0, -1.0, 0.0, 1.0),
                (-S, -S, 0.0, 1.0),
                (-S, -S, -1.0, 1.0),
                (0.0, -1.0, -1.0, 1.0)),
        
            (
                (S, -S, 0.0, 1.0),
                (0.0, -1.0, 0.0, 1.0),
                (0.0, -1.0, -1.0, 1.0),
                (S, -S, -1.0, 1.0)),

            (
                (1.0, 0.0, 0.0, 1.0),
                (S, -S, 0.0, 1.0),
                (S, -S, -1.0, 1.0),
                (1.0, 0.0, -1.0, 1.0)),

            (
                (S, S, 0.0, 1.0),
                (1.0, 0.0, 0.0, 1.0),
                (1.0, 0.0, -1.0, 1.0),
                (S, S, -1.0, 1.0)),

            (
                (0.0, 1.0, 0.0, 1.0),
                (S, S, 0.0, 1.0),
                (S, S, -1.0, 1.0),
                (0.0, 1.0, -1.0, 1.0))
        );

    oct_side_normals        : array[0..7,0..2] of float =
        (
            (-NS, NC, 0.0),
            (-NC, NS, 0.0),
            (-NC, -NS, 0.0),
            (-NS, -NC, 0.0),
            (NS, -NC, 0.0),
            (NC, -NS, 0.0),
            (NC, NS, 0.0),
            (NS, NC, 0.0)
        );

    cube_vertexes           : array[0..5,0..3,0..3] of float =
        (
            (
                (-1.0, -1.0, -1.0, 1.0),
                (-1.0, -1.0, 1.0, 1.0),
                (-1.0, 1.0, 1.0, 1.0),
                (-1.0, 1.0, -1.0, 1.0)),

            (
                (1.0, 1.0, 1.0, 1.0),
                (1.0, -1.0, 1.0, 1.0),
                (1.0, -1.0, -1.0, 1.0),
                (1.0, 1.0, -1.0, 1.0)),

            (
                (-1.0, -1.0, -1.0, 1.0),
                (1.0, -1.0, -1.0, 1.0),
                (1.0, -1.0, 1.0, 1.0),
                (-1.0, -1.0, 1.0, 1.0)),

            (
                (1.0, 1.0, 1.0, 1.0),
                (1.0, 1.0, -1.0, 1.0),
                (-1.0, 1.0, -1.0, 1.0),
                (-1.0, 1.0, 1.0, 1.0)),

            (
                (-1.0, -1.0, -1.0, 1.0),
                (-1.0, 1.0, -1.0, 1.0),
                (1.0, 1.0, -1.0, 1.0),
                (1.0, -1.0, -1.0, 1.0)),

            (
                (1.0, 1.0, 1.0, 1.0),
                (-1.0, 1.0, 1.0, 1.0),
                (-1.0, -1.0, 1.0, 1.0),
                (1.0, -1.0, 1.0, 1.0))
        );

    cube_normals            : array[0..5,0..3] of float =
        (
            (-1.0, 0.0, 0.0, 0.0),
            (1.0, 0.0, 0.0, 0.0),
            (0.0, -1.0, 0.0, 0.0),
            (0.0, 1.0, 0.0, 0.0),
            (0.0, 0.0, -1.0, 0.0),
            (0.0, 0.0, 1.0, 0.0)
        );


{
procedure   usage;
begin
    Writeln;
    Writeln('usage: moth');
    Writeln;
    Writeln('   Open_gl demo.');
    Writeln;
    Writeln('   Options:");
    Writeln('   Press the right mouse button for very limited options.');
    Writeln;

    // to force exit from demo loop
    Abort;
end;
}

{-- Error checking -----------------------------------------------------------}

procedure   checkErrors;
var
    error   : GLenum;
    List    : TStringList;
begin
    error   := glGetError;
    if error = GL_NO_ERROR then
        Exit
    else
    begin
        List := TStringList.Create;
        try
            repeat
                List.Add(PChar(gluErrorString(error)));
                error := glGetError;
            until error = GL_NO_ERROR;
            raise Exception.Create('GL errors occured: ' + List.CommaText);
        finally
            List.Free;
        end;
    end;
end;

{=============================================================================}

{-- Draw cube ----------------------------------------------------------------}

procedure   drawCube(var color, ambient: floatRow);
var
    i   : int;
begin
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @ambient);

    for i := 0 to 5 do
    begin
        glNormal3fv(@cube_normals[i][0]);
        glBegin(GL_POLYGON);
        glVertex4fv(@cube_vertexes[i][0][0]);
        glVertex4fv(@cube_vertexes[i][1][0]);
        glVertex4fv(@cube_vertexes[i][2][0]);
        glVertex4fv(@cube_vertexes[i][3][0]);
        glEnd();
    end;
end;

{-- Draw octogon top ---------------------------------------------------------}

procedure   drawOct;
var
    i   : int;
begin
    for i := 0 to 7 do
    begin
        glNormal3f(0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex4fv(@oct_vertices[i][0][0]);
        glVertex4fv(@oct_vertices[i][1][0]);
        glVertex4fv(@oct_vertices[i][2][0]);
        glEnd();
    end;
end;

{-- Draw octogon sides -------------------------------------------------------}

procedure   drawOctSides;
var
    i   : int;
begin
    for i := 0 to 7 do
    begin
        glNormal3fv(@oct_side_normals[i][0]);
        glBegin(GL_POLYGON);
        glVertex4fv(@oct_side_vertices[i][0][0]);
        glVertex4fv(@oct_side_vertices[i][1][0]);
        glVertex4fv(@oct_side_vertices[i][2][0]);
        glVertex4fv(@oct_side_vertices[i][3][0]);
        glEnd();
    end;
end;

{-- Draw satellite body ------------------------------------------------------}

procedure   drawSatellite(var diffuse, ambient, specular: floatRow; var shiny: floatCell);
begin
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @shiny);

    glPushMatrix();
        glScalef(0.3, 0.3, 0.9);
    glPushMatrix();
        drawOctSides();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.0);
        drawOct();
    glPopMatrix();
    glPushMatrix();
        glRotatef(180, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 1.0);
        drawOct();
    glPopMatrix();
    glPopMatrix();
end;

{-- Draw solar panels --------------------------------------------------------}

procedure   drawPanels(var color, ambient: floatRow);
begin
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @ambient);

    glPushMatrix();
        glTranslatef(0.95, 0.0, -0.45);
        glRotatef(45.0, 1.0, 0.0, 0.0);
        glScalef(0.65, 0.20, 0.02);
        drawCube(color, ambient);
    glPopMatrix();

{
    glPushMatrix();
        glTranslatef(0.95, 0.0, -0.45);
        glTranslatef((1.3/3.0), 0.1, 0.01);
        glRotatef(45.0, 1.0, 0.0, 0.0);
        glScalef(0.65/3.2, 0.20/2.1, 0.08);
        drawCube(color, ambient);
    glPopMatrix();
}
    glPushMatrix();
        glTranslatef(-0.95, 0.0, -0.45);
        glRotatef(45.0, 1.0, 0.0, 0.0);
        glScalef(0.65, 0.20, 0.02);
        drawCube(color, ambient);
    glPopMatrix();
end;

{-- Draw floor ---------------------------------------------------------------}

procedure   drawFloor(var f_color, ambient: floatRow);
begin
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @f_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, @ambient);

{    glNormal3f (0.0, 0.0, 1.0); }

    glBegin (GL_QUADS);
    glVertex3f (-1.0, -1.0, 0.0);
    glVertex3f (1.0, -1.0, 0.0);
    glVertex3f (1.0, 1.0, 0.0);
    glVertex3f (-1.0, 1.0, 0.0);
    glEnd();
end;

{-- Draw ground --------------------------------------------------------------}

// Ground coordinates are in drawGround() below. Subdivision
// of triangles id done by subDivide().
procedure   subDivide(var u1, u2, u3: floatRow3; depth: int);
var
    u12, u23, u31   : floatRow3;
    i               : GLint;
begin
    if depth = 0 then
    begin
        glBegin (GL_POLYGON);
        glNormal3f (0.0, 0.0, 1.0); glVertex3fv(@u1);
        glNormal3f (0.0, 0.0, 1.0); glVertex3fv(@u2);
        glNormal3f (0.0, 0.0, 1.0); glVertex3fv(@u3);
        glEnd();
        Exit;
    end;

    for i := 0 to 2 do
    begin
        u12[i] := (u1[i] + u2[i]) / 2.0;
        u23[i] := (u2[i] + u3[i]) / 2.0;
        u31[i] := (u3[i] + u1[i]) / 2.0;
    end;

    subDivide(u1, u12, u31, depth - 1);
    subDivide(u2, u23, u12, depth - 1);
    subDivide(u3, u31, u23, depth - 1);
    subDivide(u12, u23, u31, depth - 1);
end;

procedure   drawGround;
const

    // Use two subdivided triangles for the unscaled 1X1 square.
    // Subdivide to this depth:

    maxdepth    = 2;

    // Coordinates of first triangle:

    u1          : floatRow3 = (-1.0, -1.0, 0.0);
    u2          : floatRow3 = (1.0, -1.0, 0.0);
    u3          : floatRow3 = (1.0, 1.0, 0.0);

    // Coordinates of second triangle:

    v1          : floatRow3 = ( -1.0, -1.0, 0.0 );
    v2          : floatRow3 = ( 1.0, 1.0, 0.0 );
    v3          : floatRow3 = ( -1.0, 1.0, 0.0 );
begin
    subDivide(u1, u2, u3, maxdepth);
    subDivide(v1, v2, v3, maxdepth);
end;

{-- Matrix for shadow. From Mark Kilgard's "scube" ---------------------------}

procedure   myShadowMatrix(var ground, light: floatRow; var shadowMat: floatMat);
var
    dot         : float;
//  shadowMat   : floatMat;
begin
    dot             := ground[0] * light[0] +
                       ground[1] * light[1] +
                       ground[2] * light[2] +
                       ground[3] * light[3];

    shadowMat[0][0] := dot - light[0] * ground[0];
    shadowMat[1][0] := 0.0 - light[0] * ground[1];
    shadowMat[2][0] := 0.0 - light[0] * ground[2];
    shadowMat[3][0] := 0.0 - light[0] * ground[3];

    shadowMat[0][1] := 0.0 - light[1] * ground[0];
    shadowMat[1][1] := dot - light[1] * ground[1];
    shadowMat[2][1] := 0.0 - light[1] * ground[2];
    shadowMat[3][1] := 0.0 - light[1] * ground[3];

    shadowMat[0][2] := 0.0 - light[2] * ground[0];
    shadowMat[1][2] := 0.0 - light[2] * ground[1];
    shadowMat[2][2] := dot - light[2] * ground[2];
    shadowMat[3][2] := 0.0 - light[2] * ground[3];

    shadowMat[0][3] := 0.0 - light[3] * ground[0];
    shadowMat[1][3] := 0.0 - light[3] * ground[1];
    shadowMat[2][3] := 0.0 - light[3] * ground[2];
    shadowMat[3][3] := dot - light[3] * ground[3];

//  glMultMatrixf(@shadowMat);
end;

procedure   idle; cdecl;
begin
    Inc(tick);
    if tick >= 60 then
        tick := 0;
    glutPostRedisplay();
end;

procedure   keyboard(ch: uchar; x, y: int); cdecl;
begin
    case chr(ch) of
        #27 : glutExit();
        ' ' : if not moving then
              begin
                idle();
                glutPostRedisplay();
              end;
    end;
end;

{-- Display ------------------------------------------------------------------}

procedure   Display; cdecl;
begin

    // Make sure the matrix stack is cleared at the end of this function.

    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Rotation and Translation of Entire Scene

    if (mvt_x < 0) and (mvt_y < 0) then
    begin
        glTranslatef(mvt_x, mvt_y, mvt_z);

        mvt_x := mvt_x - Tx;
        mvt_y := mvt_y - Ty;
        mvt_z := mvt_z - Tz;

        glRotatef(mvr_d, mvr_x, mvr_y, mvr_z);
        mvr_d := mvr_d - Rx;
    end
    else
        glTranslatef(0.0, 0.0, mvt_z);

    glPushMatrix();
        glLightfv(GL_LIGHT1, GL_POSITION, @light1Pos);
    glPopMatrix();

    // Draw Floor

    glPushMatrix();
        glCallList(ground);
    glPopMatrix();

    // Draw Lamp Post amd Lamp

    glPushMatrix();
        glCallList(21);
    glPopMatrix();

    glPushMatrix();
        glCallList(22);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCallList(501);
        glDisable(GL_BLEND);
    glPopMatrix();

    // Draw Left Wall

    glCallList(left_wall);

    // Draw Right Wall

    glCallList(right_wall);

    // Draw Columns

    // Place columns at front of scene.

    glCallList(four_columns);

    // Place columns at back of scene.

    glPushMatrix();
        glTranslatef(0.0, 0.0, -9.0);
        glCallList(four_columns);
    glPopMatrix();

    // Place columns at centers of left and right walls.

    glCallList(two_columns);

    // Draw Column Shadows 

{
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
  
    // shadows on floor

{
    glPushMatrix();
        glCallList(100);
    glPopMatrix();
}

    // shdows on left wall

{
    glPushMatrix();
        glCallList(101);
    glPopMatrix();
}

    // shdows on back wall 

{
    glPushMatrix();
        glCallList(102);
    glPopMatrix();
}

    // shdows on right wall

{
    glPushMatrix();
        glCallList(103);
    glPopMatrix();
}

{
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

    // CUBE

    glMaterialf(GL_FRONT, GL_SHININESS, 99.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @cube_specular);

    glPushMatrix();
        glTranslatef(0.0, 0.0, -5.0);
        glRotatef((360.0 / (30 * 2)) * tick, 0, 1, 0);
        glPushMatrix();
            glTranslatef(0.0, 0.2, 2.0);
{           glTranslatef(0.0, 0.2, 0.0); }
{           glScalef(0.3, 0.3, 0.3); }
            glRotatef((360.0 / (30 * 1)) * tick, 1, 0, 0);
            glRotatef((360.0 / (30 * 2)) * tick, 0, 1, 0);
            glRotatef((360.0 / (30 * 4)) * tick, 0, 0, 1);

            glGetFloatv(GL_MODELVIEW_MATRIX, @cubeXform);

{           drawSatellite(satellite_diffuse, satellite_ambient, satellite_specular, satellite_shiny); }
            glCallList(satellite1);
            glCallList(panel1);
{           drawPanels(panel_color, panel_ambient); }
        glPopMatrix();
    glPopMatrix();

    glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @shadow_specular);

    // CUBE SHADOWS 

{   glDepthMask(GL_FALSE);}
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        glMultMatrixf(@shadowMat1_ground);
        glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
        glTranslatef(-mvt_x, -mvt_y, -mvt_z);   // correct for modelview matrix
        glMultMatrixf(@cubeXform);


{       drawSatellite(shadow_diffuse, shadow_ambient, shadow_specular, shadow_shiny); } // draw ground shadow 
        glCallList(satellite2);
        glTranslatef(0.0, -0.040, 0.0);
        glCallList(panel2);
{       drawPanels(shadow_diffuse, shadow_ambient); }
    glPopMatrix();

    // Shadow left wall only if cube is in front of left wall.
    if((tick*6) >= 220) and ((tick*6) <= 320) then
    begin
        glPushMatrix();
            glMultMatrixf(@shadowMat1_left);
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z);   // correct for modelview matrix 
            glMultMatrixf(@cubeXform);
            drawSatellite(shadow_diffuse, shadow_ambient, shadow_specular, shadow_shiny); // draw left shadow 
            drawPanels(shadow_diffuse, shadow_ambient);
        glPopMatrix();
    end;

    // Shadow back wall only if cube is in front of back wall.
    if ((tick*6) >= 125) and ((tick*6) <= 330) then
    begin
        glPushMatrix();
            glMultMatrixf(@shadowMat1_back);
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z);   // correct for modelview matrix 
            glMultMatrixf(@cubeXform);
            drawSatellite(shadow_diffuse, shadow_ambient, shadow_specular, shadow_shiny); // draw back wall shadow 
            drawPanels(shadow_diffuse, shadow_ambient);
        glPopMatrix();
    end;

    // Shadow right wall only if cube is in front of right wall.
    if ((tick*6) >= 40) and ((tick*6) <= 145) then
    begin
        glPushMatrix();
            glMultMatrixf(@shadowMat1_right);
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z); // correct for modelview matrix 
            glMultMatrixf(@cubeXform);
            drawSatellite(shadow_diffuse, shadow_ambient, shadow_specular, shadow_shiny); // draw right wall shadow 
            drawPanels(shadow_diffuse, shadow_ambient);
        glPopMatrix();
    end;

{   glDepthMask(GL_TRUE); }
    glDisable(GL_BLEND);

    glutSwapBuffers();

    checkErrors();

    // Clear the matrix stack

    glPopMatrix();
end;

{-----------------------------------------------------------------------------}

procedure   menu_select(mode: int); cdecl;
begin
    case mode of
        1   : begin
                moving := true;
                glutIdleFunc(idle);
              end;
        2   : begin
                moving := false;
                glutIdleFunc(nil);
              end;
        5   : glutExit();
    end;
end;

procedure   visible(state: int); cdecl;
begin
    if state = GLUT_VISIBLE then
    begin
        if moving then
            glutIdleFunc(idle);
    end
    else
        if moving then
            glutIdleFunc(nil);
end;

{-- Initialize ---------------------------------------------------------------}

procedure   myInit;
const
    fog_color   : floatRow = ( 0.5, 0.5, 0.5, 1.0 );
    fog_start   : floatRow = ( 0.0, 0.0, 1.0, 20.0 );
begin
{
    glGetIntegerv(GL_MAX_CLIP_PLANES, nest);
    Writeln(Format('GL_MAX_CLIP_PLANES are %d', [nest[0]]);
}

    // Initialize Positional Light and Ambient Light

{   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient); }

{
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(GL_LIGHT0, GL_POSITION, @lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, @lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, @lightSpec);
    glEnable(GL_LIGHT0);
}

    // Initial light position is declared in the display function

    glLightfv(GL_LIGHT1, GL_AMBIENT, @light1Amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, @light1Diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, @light1Spec);
    glEnable(GL_LIGHT1);

    glEnable(GL_LIGHTING);

{   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.7); }
{   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.04); } // use 0.04 w/ 24 bit color
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.07); // try 0.07 w/ 24 bit color


    // Initialize Fog
{
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, @fog_color);
    glFogf(GL_FOG_DENSITY, 0.35);
    glFogfv(GL_FOG_START, @fog_start);
    glHint(GL_FOG_HINT, GL_FASTEST);
}

    // Shadow Matrices For Floor, Left Wall, Back Wall, and Right Wall

    // For light0

    myShadowMatrix(groundPlane, lightPos, shadowMat_ground);
    myShadowMatrix(leftPlane, lightPos, shadowMat_left);
    myShadowMatrix(columnPlane, lightPos, shadowMat_column);
    myShadowMatrix(backPlane, lightPos, shadowMat_back);
    myShadowMatrix(rightPlane, lightPos, shadowMat_right);

    // For light1

    myShadowMatrix(groundPlane, light1Pos, shadowMat1_ground);
    myShadowMatrix(leftPlane, light1Pos, shadowMat1_left);
    myShadowMatrix(backPlane, light1Pos, shadowMat1_back);
    myShadowMatrix(rightPlane, light1Pos, shadowMat1_right);

    // Make Satellite Body and Shadow

    glNewList(satellite1, GL_COMPILE);
        glPushMatrix();
            drawSatellite(satellite_diffuse, satellite_ambient, satellite_specular, satellite_shiny);
        glPopMatrix();
    glEndList();
    glNewList(satellite2, GL_COMPILE);
        glPushMatrix();
            drawSatellite(shadow_diffuse, shadow_ambient, shadow_specular, shadow_shiny);
        glPopMatrix();
    glEndList();

    // Make Solar Panels and Shadows

    glNewList(panel1, GL_COMPILE);
        glPushMatrix();
            drawPanels(panel_color, panel_ambient);
        glPopMatrix();
    glEndList();

    glNewList(panel2, GL_COMPILE);
        glPushMatrix();
            drawPanels(shadow_diffuse, shadow_ambient);
        glPopMatrix();
    glEndList();

    // Make Floor

    glNewList(ground, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, @floor_color);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @shadow_ambient);
                glTranslatef(0.0, -1.5, -5.0);
                glRotatef(-90.0, 1, 0, 0);
                glScalef(5.0, 5.0, 1.0);
                drawGround(); // draw ground 
            glPopAttrib();
        glPopMatrix();
    glEndList();

    // Make Lamp Post and Lamp

    glNewList(21, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @lamp_post_specular);
                glTranslatef(0.0, -0.1, -5.0);
                glScalef(0.07, 1.45, 0.07);
                drawCube(lamp_post_diffuse, lamp_post_ambient); // draw lamp post
            glPopAttrib();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.0, -1.45, -5.0);
            glScalef(0.3, 0.05, 0.3);
            drawCube(wall_color, cube_ambient); // draw lamp post base
        glPopMatrix();
    glEndList();

    glNewList(22, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @lamp_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, @lamp_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, @lamp_specular);
                glTranslatef(0.0, 1.6, -5.0);
                glutSolidSphere(0.3, 20, 20); // draw lamp
            glPopAttrib();
        glPopMatrix();
    glEndList();

    // Lamp post base shadow 

    glNewList(501, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @shadow_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, @shadow_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, @shadow_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, @shadow_shiny);
                glTranslatef(0.0, -1.49, -5.0);
                glRotatef(-90.0, 1.0, 0.0, 0.0);
                glScalef(0.7, 0.7, 1.0);
                drawOct();
            glPopAttrib();
        glPopMatrix();
    glEndList();

    // Make Left Wall

    glNewList(left_wall, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, @wall_color);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @wall_ambient);
                glTranslatef(0.0, -1.5, 0.0);
                glTranslatef(0.0, 1.2, 0.0);
                glTranslatef(0.0, 0.0, -5.0);
                glTranslatef(-5.0, 0.0, 0.0);
                glRotatef(90.0, 0, 1, 0);
                glScalef(4.5, 1.2, 1.0);
                glNormal3f (0.0, 0.0, 1.0);
                drawGround(); // draw left wall 
            glPopAttrib();
        glPopMatrix();
    glEndList();

    // Make Right Wall

    glNewList(right_wall, GL_COMPILE);
        glPushMatrix();
            glPushAttrib(GL_LIGHTING_BIT);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, @wall_color);
                glMaterialfv(GL_FRONT, GL_AMBIENT, @wall_ambient);
                glTranslatef(0.0, -1.5, 0.0);
                glTranslatef(0.0, 1.2, 0.0);

                glTranslatef(0.0, 0.0, -5.0);
                glTranslatef(5.0, 0.0, 0.0);
                glRotatef(270.0, 0, 1, 0);

                glScalef(4.5, 1.2, 1.0);
                glNormal3f (0.0, 0.0, 1.0);
                drawGround(); // draw right wall
            glPopAttrib();
        glPopMatrix();
    glEndList();

    // Build Columns

    glPushMatrix();
        glNewList(1, GL_COMPILE);
            glPushMatrix();
                glScalef(0.4, 1.4, 0.4);
                drawCube(column_color, column_ambient); // draw column1
            glPopMatrix();
        glEndList();

        glNewList(2, GL_COMPILE);
            glPushMatrix();
                glTranslatef(0.0, -1.45, 0.0);
                glScalef(0.5, 0.1, 0.5);
                drawCube(wall_color, cube_ambient); // draw base 
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0, 1.45, 0.0);
                glScalef(0.5, 0.1, 0.5);
                drawCube(wall_color, cube_ambient); // draw top 
            glPopMatrix();
        glEndList();
    glPopMatrix();

    glNewList(column, GL_COMPILE);
        glPushMatrix();
            glCallList(1);
            glCallList(2);
        glPopMatrix();
    glEndList();

    // Place columns at front of scene.

    glNewList(4, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-5.0, 0.0, -0.5);
            glCallList(column);
        glPopMatrix();
    glEndList();

    glNewList(5, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-1.75, 0.0, -0.5);
            glCallList(column);
        glPopMatrix();
    glEndList();

    glNewList(6, GL_COMPILE);
        glPushMatrix();
            glTranslatef(1.75, 0.0, -0.5);
            glCallList(column);
        glPopMatrix();
    glEndList();

    glNewList(17, GL_COMPILE);
        glPushMatrix();
            glTranslatef(5.0, 0.0, -0.5);
            glCallList(column);
        glPopMatrix();
    glEndList();

    // Get the modelview matrix once

    glPushMatrix();
        glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
        glTranslatef(-mvt_x, -mvt_y, -mvt_z);
        glGetFloatv(GL_MODELVIEW_MATRIX, @four_columnsXform);
    glPopMatrix();

    glNewList(four_columns, GL_COMPILE);
        glPushMatrix();
            glCallList(4);
            glCallList(5);
            glCallList(6);
            glCallList(17);
        glPopMatrix();
    glEndList();

    // Make two columns for sides of scene

    glNewList(two_columns, GL_COMPILE);
        glPushMatrix();
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glTranslatef(5.0, 0.0, -5.0);
            glPushMatrix();
                glTranslatef(0.0, 0.0, -0.3);
                glCallList(column);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0, 0.0, 10.3);
                glCallList(column);
            glPopMatrix();
        glPopMatrix();
    glEndList();

    // Make shadows

    glPushMatrix();
        glNewList(8, GL_COMPILE);
            glPushMatrix();
                glScalef(0.4, 1.4, 0.4);
                drawCube(shadow_diffuse, shadow_ambient); // draw column1
            glPopMatrix();
        glEndList();

        glNewList(9, GL_COMPILE);
            glPushMatrix();
                glTranslatef(0.0, -1.45, 0.0);
                glScalef(0.5, 0.1, 0.5);
                drawCube(shadow_diffuse, shadow_ambient); // draw base.
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0, 1.45, 0.0);
                glScalef(0.5, 0.1, 0.5);
                drawCube(shadow_diffuse, shadow_ambient); // draw top. 
            glPopMatrix();
        glEndList();
    glPopMatrix();

    glNewList(10, GL_COMPILE);
        glPushMatrix();
            glCallList(8);
            glCallList(9);
        glPopMatrix();
    glEndList();

    glNewList(11, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-5.0, 0.0, -0.5);
            glCallList(10);
        glPopMatrix();
    glEndList();

    glNewList(12, GL_COMPILE);
        glPushMatrix();
            glTranslatef(-1.75, 0.0, -0.5);
            glCallList(10);
        glPopMatrix();
    glEndList();

    glNewList(13, GL_COMPILE);
        glPushMatrix();
            glTranslatef(1.75, 0.0, -0.5 );
            glCallList(10);
        glPopMatrix();
    glEndList();

    glNewList(14, GL_COMPILE);
        glPushMatrix();
            glTranslatef(5.0, 0.0, -0.5 );
            glCallList(10);
        glPopMatrix();
    glEndList();

    glNewList(15, GL_COMPILE);
        glPushMatrix();
            glCallList(11);
            glCallList(12);
            glCallList(13);
            glCallList(14);
        glPopMatrix();
    glEndList();

    glNewList(100, GL_COMPILE);
        glPushMatrix();
            glMultMatrixf(@shadowMat_ground);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z); // correct for modelview matrix
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glMultMatrixf(@four_columnsXform);
            glCallList(15);
        glPopMatrix();
    glEndList();

    glNewList(101, GL_COMPILE);
        glPushMatrix();
            glMultMatrixf(@shadowMat_left);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z); // correct for modelview matrix 
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glMultMatrixf(@four_columnsXform);
            glCallList(15);
        glPopMatrix();
    glEndList();

    glNewList(102, GL_COMPILE);
        glPushMatrix();
            glMultMatrixf(@shadowMat_back);
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z);   // correct for modelview matrix 
            glMultMatrixf(@four_columnsXform);
            glCallList(15);
        glPopMatrix();
    glEndList();

    glNewList(103, GL_COMPILE);
        glPushMatrix();
            glMultMatrixf(@shadowMat_right);
            glRotatef(-mvr_d, mvr_x, mvr_y, mvr_z);
            glTranslatef(-mvt_x, -mvt_y, -mvt_z);   // correct for modelview matrix
            glMultMatrixf(@four_columnsXform);
            glCallList(15);
        glPopMatrix();
    glEndList();
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo;
var
    width, height, i: int;
begin;
    width   := 320;
    height  := 240;
    InitGLUT;
    glutInitWindowSize(width, height);

    // process commmand line args
    for i := 1 to ParamCount do
        if CompareText(ParamStr(i),'-db') = 0 then
            useDB := not useDB;
    { usage(); }

    // choose visual

    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGB or GLUT_DEPTH);
    glutCreateWindow('MOTH - by Bob Doyle');

    glutKeyboardFunc(keyboard);

    myInit(); // initialize objects in scene
    glutDisplayFunc(display);
    glutVisibilityFunc(visible);

    glutCreateMenu(menu_select);
    glutAddMenuEntry('Start motion', 1);
    glutAddMenuEntry('Stop motion', 2);
    glutAddMenuEntry('Quit', 5);
    glutAddMenuEntry('Drink Ed''s beer', 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-0.9, 0.9, -0.9, 0.9, 1.0, 35.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
{   glTranslatef(0.0, 0.0, mvt_zi); }

    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);        // double your fun
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    myInit();                   // initialize objects in scene

    glutMainLoop();
end;

initialization
    RegisterDemo('Demos',
                 'Moth',
                 '*** No description yet',
                 Demo);
end.
