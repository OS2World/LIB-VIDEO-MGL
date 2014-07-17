//
// Homework 4, Part 1: perspective, hierarchical coords, moving eye pos.
//
// Do a slow zoom on a bunch of rings (ala Superman III?)
//
// Philip Winston - 2/21/95
// pwinston@hmc.edu
// http://www.cs.hmc.edu/people/pwinston
//

unit Rings;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    CmdLine,
    DemoList;

const
    MENU_STARTOVER      = 0;
    MENU_ZOOM_OUT       = 1;
    MENU_STOP_RINGS     = 2;
    MENU_STOP_FADE      = 3;
    MENU_START_RINGS    = 4;
    MENU_START_FADE     = 5;
    MENU_QUIT           = 6;

    NOTALLOWED          = 0;
    CONE                = 1;
    TORUS               = 2;
    INNERMAT            = 3;
    OUTTERMAT           = 4;

    STEPS               = 30;

var
    Fade                : bool = true; // Start moving out 

    Axis                : float = 0;
    AxisInc             : float = (2.0 * Pi / STEPS);

    InnerRad, OutterRad,
    Tilt, Trans,
    TransCone, Dist     : GLfloat;

// mainly computes the translation amount as a function of the tilt angle
// and torus radii
procedure   myInit;
var
    sinoftilt   : float;
begin
    InnerRad    := 0.70;
    OutterRad   := 5.0;
    Tilt        := 15;
    Dist        := 800;

    sinoftilt   := sin(Tilt * Pi*2/360);

    Trans       := (2*OutterRad + InnerRad) * sinoftilt + InnerRad +
                   ((1 - sinoftilt) * InnerRad) - (InnerRad * 1/10);

    TransCone   := Trans + (OutterRad * sinoftilt + InnerRad);
end;

// I used code from the book's accnot.c as a starting point for lighting.
// I have one positional light in center, then one directional 
procedure   myglInit;
const
    light0_position : array[0..3] of float = ( 1.0, 0.2, 1.0, 0.0 );
    light1_position : array[0..3] of float = ( 0.0, 0.0, 0.0, 1.0 );
    light1_diffuse  : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    light1_specular : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    lm_ambient      : array[0..3] of float = ( 0.2, 0.2, 0.2, 1.0 );
begin
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, @light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, @light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, @light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, @light1_specular);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lm_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glFlush();
end;

procedure   myreshape(w, h: GLsizei); cdecl;
begin
    glViewport(0,0,w,h);
    glFlush();
end;

// setup display lists to change material for inner/outter rings and
// to draw a single torus or cone */
procedure   MakeDisplayLists;
const
    cone_diffuse    : array[0..3] of float = ( 0.0, 0.7, 0.7, 1.0 );
    mat1_ambient    : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    mat2_ambient    : array[0..3] of float = ( 0.0, 0.0, 0.0, 0.0 );
    torus1_diffuse  : array[0..3] of float = ( 0.7, 0.7, 0.0, 1.0 );
    torus2_diffuse  : array[0..3] of float = ( 0.3, 0.0, 0.0, 1.0 );
    mat1_specular   : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    mat2_specular   : array[0..3] of float = ( 0.5, 0.5, 0.5, 1.0 );
begin
    glNewList(INNERMAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_SPECULAR, @mat1_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, @torus1_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, @mat1_ambient);
    glEndList();

    glNewList(OUTTERMAT, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_SPECULAR, @mat2_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, @torus2_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, @mat2_ambient);
    glEndList();

    glNewList(CONE, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, @cone_diffuse);
        glPushMatrix();
            glTranslatef(0, -TransCone, 0);
            glRotatef(90, 1, 0, 0);
            glutSolidCone(OutterRad, 10, 8, 8);
        glPopMatrix();
    glEndList();

    glNewList(TORUS, GL_COMPILE);
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glutSolidTorus(InnerRad, OutterRad, 15, 25);
        glPopMatrix();
    glEndList();
end;

// Draw three rings, rotated and translate so they look cool
procedure   DrawRings(axis: float);
var
    x, y    : GLfloat;
begin
    x := sin(axis);
    y := cos(axis);

    glPushMatrix();
        glTranslatef(0, Trans, 0);
        glRotatef(Tilt, x, 0, y);
        glCallList(TORUS);
    glPopMatrix();

    glPushMatrix();
        glRotatef(-Tilt, x, 0, y);
        glCallList(TORUS);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -Trans, 0);
        glRotatef(Tilt, x, 0, y);
        glCallList(TORUS);
    glPopMatrix();
end;

// Draw the inner thing, then glScale and draw 3 huge rings
procedure   mydisplay; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 10, 1000);
    gluLookAt(0, 0, Dist, 0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);

    glCallList(INNERMAT);
    DrawRings(Axis);
    glCallList(CONE);

    glCallList(OUTTERMAT);
    glPushMatrix();
        glScalef(10, 10, 10);
        DrawRings(Axis/3);
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
end;

// rotate the axis and adjust position if nec.
procedure   myidle; cdecl;
begin
    Axis := Axis + AxisInc;

    if (Dist < 15) and Fade then        // start slow
        Dist := Dist + 0.1
    else if (Dist < 800) and Fade then  // don't go back too far
        Dist := Dist * 1.005;
    mydisplay();
end;

// nothing fancy
procedure   handlemenu(value: int); cdecl;
begin
    case value of
        MENU_STARTOVER  : begin
                            Dist    := 10; Axis := 0; Fade := True ;
                            AxisInc := (2.0 * Pi / STEPS);
                            glutChangeToMenuEntry(3, 'Stop rings', MENU_STOP_RINGS);
                            glutChangeToMenuEntry(4, 'Stop fade', MENU_STOP_FADE);
                          end;
        MENU_ZOOM_OUT   : Dist := 800;
        MENU_STOP_RINGS : begin
                            AxisInc := 0;
                            glutChangeToMenuEntry(3, 'Start rings', MENU_START_RINGS);
                          end;
        MENU_START_RINGS: begin
                            AxisInc := (2.0 * Pi / STEPS);
                            glutChangeToMenuEntry(3, 'Stop rings', MENU_STOP_RINGS);
                          end;
        MENU_STOP_FADE  : begin
                            Fade    := false;
                            glutChangeToMenuEntry(4, 'Start fade', MENU_START_FADE);
                          end;
        MENU_START_FADE : begin
                            Fade    := true;
                            glutChangeToMenuEntry(4, 'Stop fade', MENU_STOP_FADE);
                          end;
        MENU_QUIT       : glutExit();
    end;
end;

procedure   MenuInit;
begin
    glutCreateMenu(handlemenu);
    glutAddMenuEntry('Start Over', MENU_STARTOVER);
    glutAddMenuEntry('Zoom Out', MENU_ZOOM_OUT);
    glutAddMenuEntry('Stop rings', MENU_STOP_RINGS);
    glutAddMenuEntry('Stop fade', MENU_STOP_FADE);
    glutAddMenuEntry('Quit', MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
end;

procedure   vis(visible: int); cdecl;
begin
    if visible = GLUT_VISIBLE then
        glutIdleFunc(myidle)
    else
        glutIdleFunc(nil);
end;

procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGB or GLUT_DEPTH);
    glutCreateWindow('rings');

    myInit();
    myglInit();

    MakeDisplayLists();
    MenuInit();

    glutReshapeFunc(myreshape);
    glutDisplayFunc(mydisplay);
    glutVisibilityFunc(vis);

    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Rings','*** No description yet',Demo);
end.
