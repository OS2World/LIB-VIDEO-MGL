{=============================================================================}
{=                                                                           =}
{= Homework 6, Part 2: fractal mountains and fractal trees                   =}
{= (Pretty Late)                                                             =}
{=                                                                           =}
{= Draws fractal mountains and trees -- and an island of mountains in water  =}
{= (I tried having trees on the island but it didn't work too well.)         =}
{=                                                                           =}
{= Two viewer modes: polar and flying (both restrained to y>0 for up vector).=}
{= Keyboard 0->9 and +/- control speed when flying.                          =}
{=                                                                           =}
{= Only keyboard commands are 0-9 and +/- for speed in flying mode.          =}
{=                                                                           =}
{= Fog would make the island look much better, but I couldn't get it to work =}
{= correctly.  Would line up on -z axis not from eye.                        =}
{=                                                                           =}
{= Philip Winston - 3/4/95                                                   =}
{= pwinston@hmc.edu                                                          =}
{= http://www.cs.hmc.edu/people/pwinston                                     =}
{=                                                                           =}
{=============================================================================}
unit Fractals;

interface

implementation

uses
    FViewer,
    MGLGl,
    MGLGlu,
    MGLGlut,
    MGLTypes,
    CmdLine,
    DemoList,
    Windows,
    SysUtils;

//
const
    FLT_MAX     = 1e+38;
    // To avoid Floating point errors
    ULONG_MAX   = MaxLongInt/2;
//    

{ TODO: May be it is not correct definition? }
    
function    drand48: float;
begin
    Result      := Random;
end;

procedure   srand48(x: int);
begin
    RandSeed    := x;
end;

const
    NOTALLOWED      = 0;
    MOUNTAIN        = 1;
    TREE            = 2;
    ISLAND          = 3;
    BIGMTN          = 4;
    STEM            = 5;
    LEAF            = 6;
    MOUNTAIN_MAT    = 7;
    WATER_MAT       = 8;
    LEAF_MAT        = 9;
    TREE_MAT        = 10;
    STEMANDLEAVES   = 11;
    AXES            = 12;

const
    MAXLEVEL        = 8;

var
    Rebuild         : int = 1;        // Rebuild display list in next display? 
    Fract           : int = TREE;     // What fractal are we building
    Level           : int = 4;        // levels of recursion for fractals

    DrawAxes        : int = 0;       

{-- Vector junk --------------------------------------------------------------}

// print vertex to stderr 
function    FormatVertex(var v: floatVec): string;
begin
    Result := Format('(%f, %f, %f)', [v[0], v[1], v[2]]);
end;

// calculates normal to the triangle designated by v1, v2, v3 
procedure   triagnormal(var v1, v2, v3, norm: floatVec);
var
    vec1, vec2: floatVec;
begin
    vec1[0] := v3[0] - v1[0];  vec2[0] := v2[0] - v1[0];
    vec1[1] := v3[1] - v1[1];  vec2[1] := v2[1] - v1[1];
    vec1[2] := v3[2] - v1[2];  vec2[2] := v2[2] - v1[2];

    ncrossprod(vec2, vec1, norm);
end;

function    xzlength(var v1, v2: floatVec): float;
begin
    Result := sqrt((v1[0] - v2[0])*(v1[0] - v2[0]) +
                   (v1[2] - v2[2])*(v1[2] - v2[2]));
end;

function    xzslope(var v1, v2: floatVec): float;
begin
    if v1[0] <> v2[0] then
        Result := (v1[2] - v2[2]) / (v1[0] - v2[0])
    else
        Result := FLT_MAX;
end;

{-- Mountain stuff -----------------------------------------------------------}

var
    // Array of what to multiply random number by for a given level to get midpoint
    // displacement
    DispFactor  : array[0..MAXLEVEL-1] of GLfloat;

    // Array of what to add to random number before multiplying it by DispFactor
    DispBias    : array[0..MAXLEVEL-1] of GLfloat;

const
    NUMRANDS    = 191;

var    
    // hash table of random numbers so we can raise the same midpoints by the same amount 
    RandTable   : array[0..NUMRANDS-1] of GLfloat;

    // The following are for permitting an edge of a moutain to be
    // pegged so it won't be displaced up or down.  This makes it
    // easier to setup scenes and makes a single moutain look better 

    Verts       : array[0..2] of floatVec;  // Vertices of outside edges of mountain
    Slopes      : floatVec;                 // Slopes between these outside edges 
    Pegged      : floatVec;                 // Is this edge pegged or not 

//
// Comes up with a new table of random numbers [0,1)
//
procedure   InitRandTable(seed: unsigned);
var
    i: int;
begin
    srand48(seed);
    for i := 0 to NUMRANDS - 1 do
        RandTable[i] := drand48() - 0.5;
end;

// calculate midpoint and displace it if required
procedure   Midpoint(var mid, v1, v2: floatVec; edge, level: int);
var
    hash: unsigned;
begin
    mid[0] := (v1[0] + v2[0]) / 2;
    mid[1] := (v1[1] + v2[1]) / 2;
    mid[2] := (v1[2] + v2[2]) / 2;
    if (Pegged[edge] <> 0) or (Abs(xzslope(Verts[edge], mid)- Slopes[edge]) > 0.00001) then
    begin
        srand48(Trunc((v1[0]+v2[0])*23344));
        hash    := Trunc(drand48() * 7334334);
        srand48(Trunc((v2[2]+v1[2])*43433));
        hash    := unsigned(Trunc(drand48() * 634344 + hash) mod NUMRANDS);
        mid[1]  := mid[1] + ((RandTable[hash] + DispBias[level]) * DispFactor[level]);
    end;
end;

//
// Recursive moutain drawing routine -- from lecture with addition of
// allowing an edge to be pegged.  This function requires the above
// globals to be set, as well as the Level global for fractal level
//
procedure   FMR(var v1, v2, v3: floatVec; level: int);
var
    norm        : floatVec;
    m1, m2, m3  : floatVec;
begin
    if level = Fractals.Level then
    begin
        triagnormal(v1, v2, v3, norm);
        glNormal3fv(@norm);
        glVertex3fv(@v1);
        glVertex3fv(@v2);
        glVertex3fv(@v3);
    end
    else
    begin
        Midpoint(m1, v1, v2, 0, level);
        Midpoint(m2, v2, v3, 1, level);
        Midpoint(m3, v3, v1, 2, level);

        FMR(v1, m1, m3, level + 1);
        FMR(m1, v2, m2, level + 1);
        FMR(m3, m2, v3, level + 1);
        FMR(m1, m2, m3, level + 1);
    end;
end;

type
    intVec = array[0..2] of int;
    
//
// sets up lookup tables and calls recursive mountain function
//
procedure   FractalMountain(var v1, v2, v3: floatVec; var pegged: intVec);
var
    lengths     : array[0..MAXLEVEL-1] of GLfloat;
const
    fraction    : array[0..7] of GLfloat = ( 0.3, 0.3, 0.4, 0.2, 0.3, 0.2, 0.4, 0.4 );
    bias        : array[0..7] of GLfloat = ( 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 );
var
    i           : int;
    avglen      : float;
    value       : GLfloat;
begin
    { TODO: possibly error here? }
    avglen := (xzlength(v1, v2) + xzlength(v2, v3) + xzlength(v3, v1) / 3);

    for i := 0 to 2 do
    begin
        Verts[0][i] := v1[i];       // set mountain vertex globals
        Verts[1][i] := v2[i];
        Verts[2][i] := v3[i];
        Pegged[i]   := pegged[i];
    end;

    Slopes[0]   := xzslope(Verts[0], Verts[1]);   // set edge slope globals
    Slopes[1]   := xzslope(Verts[1], Verts[2]);
    Slopes[2]   := xzslope(Verts[2], Verts[0]);

    lengths[0]  := avglen;
    for i := 1 to Level - 1 do
        lengths[i] := lengths[i-1]/2;   // compute edge length for each level

    for i := 0 to Level - 1 do          // DispFactor and DispBias arrays
    begin
        if i <= 7 then
            value := fraction[i]
        else
            value := fraction[7];
        DispFactor[i]   := lengths[i] * value;
        if i <= 7 then
            value := bias[i]
        else
            value := bias[7];
        DispBias[i]     := value;
    end;

    glBegin(GL_TRIANGLES);
        FMR(v1, v2, v3, 0); // issues no GL but vertex calls
    glEnd();
end;

//
// draw a mountain and build the display list
//
procedure   CreateMountain;
const
    v1      : floatVec  = ( 0, 0, -1 );
    v2      : floatVec  = ( -1, 0, 1 );
    v3      : floatVec  = ( 1, 0, 1 );
    pegged  : intVec    = ( 1, 1, 1 );
begin
    glNewList(MOUNTAIN, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
        glCallList(MOUNTAIN_MAT);
        FractalMountain(v1, v2, v3, pegged);
    glPopAttrib();
    glEndList();
end;

//
// new random numbers to make a different moutain
//
procedure   NewMountain;
begin
    InitRandTable(Trunc(Time));
end;

{-- Tree ---------------------------------------------------------------------}

var
    TreeSeed    : ulong;    // for srand48 - remember so we can build "same tree"
                            // at a different level 

//
// recursive tree drawing thing, fleshed out from class notes pseudocode
//
procedure   FractalTree(level: int);
var
    savedseed: ulong;   // need to save seeds while building tree too
begin
    if level = Fractals.Level then
    begin
        glPushMatrix();
            glRotatef(drand48()*180, 0, 1, 0);
            glCallList(STEMANDLEAVES);
        glPopMatrix();
    end
    else
    begin
        glCallList(STEM);
        glPushMatrix();

            glRotatef(drand48()*180, 0, 1, 0);
            glTranslatef(0, 1, 0);
            glScalef(0.7, 0.7, 0.7);

            savedseed := Trunc(drand48()*ULONG_MAX);    // recurse on a 3-way branching
            glPushMatrix();
                glRotatef(110 + drand48()*40, 0, 1, 0);
                glRotatef(30 + drand48()*20, 0, 0, 1);
                FractalTree(level + 1);
            glPopMatrix();

            srand48(savedseed);
            savedseed := Trunc(drand48()*ULONG_MAX);
            glPushMatrix();
                glRotatef(-130 + drand48()*40, 0, 1, 0);
                glRotatef(30 + drand48()*20, 0, 0, 1);
                FractalTree(level + 1);
            glPopMatrix();

            srand48(savedseed);
            glPushMatrix();
                glRotatef(-20 + drand48()*40, 0, 1, 0);
                glRotatef(30 + drand48()*20, 0, 0, 1);
                FractalTree(level + 1);
            glPopMatrix();

        glPopMatrix();
    end;
end;

//
// Create display lists for a leaf, a set of leaves, and a stem
//
procedure   CreateTreeLists;
var
    cylquad : PGLUquadricObj;
    i       : int;
begin
    cylquad := gluNewQuadric();

    glNewList(STEM, GL_COMPILE);
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        gluCylinder(cylquad, 0.1, 0.08, 1, 10, 2 );
    glPopMatrix();
    glEndList();

    glNewList(LEAF, GL_COMPILE);    // I think this was jeff allen's leaf idea
    glBegin(GL_TRIANGLES);
        glNormal3f(-0.1, 0, 0.25);  // not normalized
        glVertex3f(0, 0, 0);
        glVertex3f(0.25, 0.25, 0.1);
        glVertex3f(0, 0.5, 0);

        glNormal3f(0.1, 0, 0.25);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0.5, 0);
        glVertex3f(-0.25, 0.25, 0.1);
    glEnd();
    glEndList();

    glNewList(STEMANDLEAVES, GL_COMPILE);
    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
        glCallList(STEM);
        glCallList(LEAF_MAT);
        for i := 0 to 2 do
        begin
            glTranslatef(0, 0.333, 0);
            glRotatef(90, 0, 1, 0);
            glPushMatrix();
                glRotatef(0, 0, 1, 0);
                glRotatef(50, 1, 0, 0);
                glCallList(LEAF);
            glPopMatrix();
            glPushMatrix();
                glRotatef(180, 0, 1, 0);
                glRotatef(60, 1, 0, 0);
                glCallList(LEAF);
            glPopMatrix();
        end;
    glPopAttrib();
    glPopMatrix();
    glEndList();
end;

//
// draw and build display list for tree
//
procedure   CreateTree;
begin
    srand48(TreeSeed);

    glNewList(TREE, GL_COMPILE);
        glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);
            glCallList(TREE_MAT);
            glTranslatef(0, -1, 0);
            FractalTree(0);
        glPopAttrib();
        glPopMatrix();
    glEndList();
end;

//
// new seed for a new tree (groan)
//
procedure   NewTree;
begin
    TreeSeed := Trunc(Time);
end;

{-- Fractal planet -----------------------------------------------------------}

procedure   CreateIsland;
begin
    CreateMountain();
    glNewList(ISLAND, GL_COMPILE);
    glPushAttrib(GL_LIGHTING_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glCallList(WATER_MAT);

        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(1000, 0.01, 1000);
            glVertex3f(1000, 0.01, -1000);
            glVertex3f(-1000, 0.01, -1000);
            glVertex3f(-1000, 0.01, 1000);
        glEnd();

        glPushMatrix();
            glTranslatef(0, -0.1, 0);
            glCallList(MOUNTAIN);
        glPopMatrix();

        glPushMatrix();
            glRotatef(135, 0, 1, 0);
            glTranslatef(0.2, -0.15, -0.4);
            glCallList(MOUNTAIN);
        glPopMatrix();

        glPushMatrix();
            glRotatef(-60, 0, 1, 0);
            glTranslatef(0.7, -0.07, 0.5);
            glCallList(MOUNTAIN);
        glPopMatrix();

        glPushMatrix();
            glRotatef(-175, 0, 1, 0);
            glTranslatef(-0.7, -0.05, -0.5);
            glCallList(MOUNTAIN);
        glPopMatrix();

        glPushMatrix();
            glRotatef(165, 0, 1, 0);
            glTranslatef(-0.9, -0.12, 0.0);
            glCallList(MOUNTAIN);
        glPopMatrix();

    glPopMatrix();
    glPopAttrib();
    glEndList();
end;

procedure   NewFractals;
begin
    NewMountain();
    NewTree();
end;

procedure   Create(fract: int);
begin
    case fract of
        MOUNTAIN    : CreateMountain();
        TREE        : CreateTree();
        ISLAND      : CreateIsland();
    end;
end;

{-- OpenGL -------------------------------------------------------------------}

procedure   SetupMaterials;
const
    mtn_ambuse      : array[0..3] of GLfloat = ( 0.426, 0.256, 0.108, 1.0 );
    mtn_specular    : array[0..3] of GLfloat = ( 0.394, 0.272, 0.167, 1.0 );
    mtn_shininess   : array[0..0] of GLfloat = ( 10 );

    water_ambuse    : array[0..3] of GLfloat = ( 0.0, 0.1, 0.5, 1.0 );
    water_specular  : array[0..3] of GLfloat = ( 0.0, 0.1, 0.5, 1.0 );
    water_shininess : array[0..0] of GLfloat = ( 10 );

    tree_ambuse     : array[0..3] of GLfloat = ( 0.4, 0.25, 0.1, 1.0 );
    tree_specular   : array[0..3] of GLfloat = ( 0.0, 0.0, 0.0, 1.0 );
    tree_shininess  : array[0..0] of GLfloat = ( 0 );

    leaf_ambuse     : array[0..3] of GLfloat = ( 0.0, 0.8, 0.0, 1.0 );
    leaf_specular   : array[0..3] of GLfloat = ( 0.0, 0.8, 0.0, 1.0 );
    leaf_shininess  : array[0..0] of GLfloat = ( 10 );
begin
    glNewList(MOUNTAIN_MAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @mtn_ambuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, @mtn_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, @mtn_shininess);
    glEndList();

    glNewList(WATER_MAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @water_ambuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, @water_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, @water_shininess);
    glEndList();

    glNewList(TREE_MAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, @tree_ambuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, @tree_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, @tree_shininess);
    glEndList();

    glNewList(LEAF_MAT, GL_COMPILE);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, @leaf_ambuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, @leaf_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, @leaf_shininess);
    glEndList();
end;

procedure   MyGLInit;
const
    light_ambient   : array[0..3] of GLfloat = ( 0.0, 0.0, 0.0, 1.0 );
    light_diffuse   : array[0..3] of GLfloat = ( 1.0, 1.0, 1.0, 1.0 );
    light_specular  : array[0..3] of GLfloat = ( 1.0, 1.0, 1.0, 1.0 );
    light_position  : array[0..3] of GLfloat = ( 0.0, 0.3, 0.3, 0.0 );

    lmodel_ambient  : array[0..3] of GLfloat = ( 0.4, 0.4, 0.4, 1.0 );
begin
    glLightfv(GL_LIGHT0, GL_AMBIENT, @light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, @light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, @light_position);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
{$IFDEF NEVERDEFINED}
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
{$ENDIF}

    glShadeModel(GL_SMOOTH);
{$IFDEF NEVERDEFINED}
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
{$ENDIF}

    SetupMaterials();
    CreateTreeLists();

    glFlush();
end;

{-- GLUT stuff ---------------------------------------------------------------}

procedure   Reshape(w, h: GLsizei); cdecl;
begin
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, w/h, 0.01, 100);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glFlush();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPushMatrix(); // clear of last viewing xform, leaving perspective

    agvViewTransform();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if Rebuild <> 0 then
    begin
        Create(Fract);
        Rebuild := 0;
    end;

    glCallList(Fract);

    if DrawAxes <> 0 then
        glCallList(AXES);

    glutSwapBuffers();
    glFlush();
end;

procedure   Visible(v: int); cdecl;
begin
    if v = GLUT_VISIBLE then
        agvSetAllowIdle(True)
    else
    begin
        glutIdleFunc(nil);
        agvSetAllowIdle(False);
    end;
end;

procedure   MenuUse(v: int); cdecl;
begin
    if v = GLUT_MENU_NOT_IN_USE then
        agvSetAllowIdle(True)
    else
    begin
        glutIdleFunc(nil);
        agvSetAllowIdle(False);
    end;
end;

{-- Menu setup and handling --------------------------------------------------}

const
    MENU_QUIT   = 0;
    MENU_RAND   = 1;
    MENU_MOVE   = 2;
    MENU_AXES   = 3;

procedure   SetLevel(value: int); cdecl;
begin
    Level   := value;
    Rebuild := 1;
    glutPostRedisplay();
end;

procedure   ChooseFract(value: int); cdecl;
begin
    Fract   := value;
    Rebuild := 1;
    glutPostRedisplay();
end;

procedure   HandleMenu(value: int); cdecl;
begin
    case value of
        MENU_QUIT   : glutExit();
        MENU_RAND   : begin
                        NewFractals();
                        Rebuild := 1;
                        glutPostRedisplay();
                      end;
        MENU_AXES   : begin
                        DrawAxes := not DrawAxes;
                        glutPostRedisplay();
                      end;
    end;
end;

procedure   MenuInit;
var
    submenu3, submenu2, submenu1 : int;
begin
    submenu1 := glutCreateMenu(setlevel);
    glutAddMenuEntry('0', 0);  glutAddMenuEntry('1', 1);
    glutAddMenuEntry('2', 2);  glutAddMenuEntry('3', 3);
    glutAddMenuEntry('4', 4);  glutAddMenuEntry('5', 5);
    glutAddMenuEntry('6', 6);  glutAddMenuEntry('7', 7);
    glutAddMenuEntry('8', 8);

    { TODO: May be '9'? }

    submenu2 := glutCreateMenu(choosefract);
    glutAddMenuEntry('Moutain', MOUNTAIN);
    glutAddMenuEntry('Tree', TREE);
    glutAddMenuEntry('Island', ISLAND);

    submenu3 := glutCreateMenu(agvSwitchMoveMode);
    glutAddMenuEntry('Flying', FLYING);
    glutAddMenuEntry('Polar', POLAR);

    glutCreateMenu(handlemenu);
    glutAddSubMenu('Level', submenu1);
    glutAddSubMenu('Fractal', submenu2);
    glutAddSubMenu('Movement', submenu3);
    glutAddMenuEntry('New Fractal', MENU_RAND);
    glutAddMenuEntry('Toggle Axes', MENU_AXES);
    glutAddMenuEntry('Quit', MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
end;

{-- Main ---------------------------------------------------------------------}

procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGBA or GLUT_DEPTH or GLUT_MULTISAMPLE);
    { TODO: What ? }
    glutCreateWindow('Fractal Planet?');

    agvInit(True); // True cause we don't have our own idle 

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutVisibilityFunc(visible);
    glutMenuStateFunc(menuuse);

    NewFractals();
    agvMakeAxesList(AXES);
    myGLInit();
    MenuInit();

    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Fractals','*** No description yet', Demo);
end.
