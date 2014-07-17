//
//
//  Description: Interactive 3D graphics, Assignment #1
//               Miniature Steam Engine Simulation.
//  Author:      Troy Robinette
//  Date:        29/9/95
//  Email:       troyr@yallara.cs.rmit.edu.au
//  Notes:       - Transparence doesn't quite work. The color of the
//                 underlying object doesn't show through.
//               - Also only the front side of the transparent objects are
//                transparent.
//
unit Steam;

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

{-- Dimensions of texture image ----------------------------------------------}

const
    IMAGE_WIDTH     = 64;
    IMAGE_HEIGHT    = 64;

{-- Step to be taken for each rotation ---------------------------------------}

    ANGLE_STEP      = 10;

{-- Magic numbers for relationship b/w cylinder head and crankshaft ----------}

    MAGNITUDE       = 120;
    PHASE           = 270.112;
    FREQ_DIV        = 58;
    ARC_LENGHT      = 2.7;
    ARC_RADIUS      = 0.15;

{-- Rotation angles ----------------------------------------------------------}

var
    view_h          : GLdouble  = 270;
    view_v          : GLdouble  = 0;
    head_angle      : GLdouble  = 0;
    crank_angle     : GLint     = 0;

{-- Crank rotation step ------------------------------------------------------}

    crank_step      : GLdouble  = 5;

{-- Toggles ------------------------------------------------------------------}

    shaded          : bool = True;
    anim            : bool = True;
    texture         : bool = False;
    transparent     : bool = False;
    light1          : bool = True;
    light2          : bool = False;

{-- Storage for the angle look up table and the texture map ------------------}

    head_look_up_table  : array[0..360] of GLdouble;
    image               : array[0..IMAGE_WIDTH-1,0..IMAGE_HEIGHT-1,0..2] of GLubyte;

{-- Indentifiers for each Display list ---------------------------------------}

    list_piston_shaded      : GLint = 1;
    list_piston_texture     : GLint = 2;
    list_flywheel_shaded    : GLint = 4;
    list_flywheel_texture   : GLint = 8;

{-- Variable used in the creaton of glu objects ------------------------------}

    obj                     : PGLUquadricObj;

{-- Draws a box by scaling a glut cube of size 1 -----------------------------}

// Also checks the shaded toggle to see which rendering style to use.
// NB Texture doesn't work correctly due to the cube being scaled.

procedure   myBox(x, y, z: GLdouble);
begin
    glPushMatrix();
        glScalef(x, y, z);
        if shaded then
            glutSolidCube(1)
        else
            glutWireCube(1);
    glPopMatrix();
end;

{-- Draws a cylinder using glu function --------------------------------------}

// drawing flat disc's at each end, to give the appearence of it being solid.

procedure   myCylinder(_object: PGLUquadricObj; outerRadius, innerRadius, lenght: GLdouble);
begin
    glPushMatrix();
        gluCylinder(_object, outerRadius, outerRadius, lenght, 20, 1);
        glPushMatrix();
            glRotatef(180, 0.0, 1.0, 0.0);
            gluDisk(_object, innerRadius, outerRadius, 20, 1);
        glPopMatrix();

        glTranslatef(0.0, 0.0, lenght);
        gluDisk(_object, innerRadius, outerRadius, 20, 1);
    glPopMatrix();
end;

{-- Draws a piston -----------------------------------------------------------}

procedure   draw_piston;
begin
    glPushMatrix();
        glColor4f(0.3, 0.6, 0.9, 1.0);

        glPushMatrix();
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 0.0, -0.07);
            myCylinder(obj, 0.125, 0.06, 0.12);
        glPopMatrix();

        glRotatef(-90, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 0.05);
        myCylinder(obj, 0.06, 0.0, 0.6);
        glTranslatef(0.0, 0.0, 0.6);
        myCylinder(obj, 0.2, 0.0, 0.5);
    glPopMatrix();
end;

{-- Draws the engine pole and the pivot pole for the cylinder head -----------}

procedure   draw_engine_pole;
begin
    glPushMatrix();
        glColor4f(0.9, 0.9, 0.9, 1.0);
        myBox(0.5, 3.0, 0.5);

        glColor3f(0.5, 0.1, 0.5);
        glRotatef(90, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.9, -0.4);
        myCylinder(obj, 0.1, 0.0, 2);
    glPopMatrix();
end;

{-- Draws the cylinder head at the appropreate angle -------------------------}

// doing the necesary translations for the rotation.

procedure   draw_cylinder_head;
begin
    glPushMatrix();
        glColor4f(0.5, 1.0, 0.5, 0.1);
        glRotatef(90, 1.0, 0.0, 0.0);
        glTranslatef(0, 0.0, 0.4);
        glRotatef(head_angle, 1, 0, 0);
        glTranslatef(0, 0.0, -0.4);
        myCylinder(obj, 0.23, 0.21, 1.6);
        glRotatef(180, 1.0, 0.0, 0.0);
        gluDisk(obj, 0, 0.23, 20, 1);
    glPopMatrix();
end;

{-- Draws the flywheel -------------------------------------------------------}

procedure   draw_flywheel;
begin
    glPushMatrix();
        glColor4f(0.5, 0.5, 1.0, 1.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        myCylinder(obj, 0.625, 0.08, 0.5);
    glPopMatrix();
end;

{-- Draws the crank bell -----------------------------------------------------}

// And the pivot pin for the piston. Also calls the appropreate display list
// of a piston doing the nesacary rotations before hand.

procedure   draw_crankbell;
begin
    glPushMatrix();
        glColor4f(1.0, 0.5, 0.5, 1.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        myCylinder(obj, 0.3, 0.08, 0.12);

        glColor4f(0.5, 0.1, 0.5, 1.0);
        glTranslatef(0.0, 0.2, 0.0);
        myCylinder(obj, 0.06, 0.0, 0.34);

        glTranslatef(0.0, 0.0, 0.22);
        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(crank_angle - head_angle, 1.0, 0.0, 0.0);
        if shaded then
            if texture then
                glCallList(list_piston_texture)
            else
                glCallList(list_piston_shaded)
        else
            draw_piston();
    glPopMatrix();
end;

{-- Draws the complete crank -------------------------------------------------}

// Piston also gets drawn through the crank bell function.

procedure   draw_crank;
begin
    glPushMatrix();
        glRotatef(crank_angle, 1.0, 0.0, 0.0);

        glPushMatrix();
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 0.0, -1.0);
            myCylinder(obj, 0.08, 0.0, 1.4);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.28, 0.0, 0.0);
            draw_crankbell();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.77, 0.0, 0.0);
            if shaded then
                if texture then
                    glCallList(list_flywheel_texture)
                else
                    glCallList(list_flywheel_shaded)
            else
                draw_flywheel();
        glPopMatrix();
    glPopMatrix();
end;

{-- Main display routine -----------------------------------------------------}

// Clears the drawing buffer and if transparency is set, displays the model
// twice, 1st time accepting those fragments with a ALPHA value of 1 only,
// then with DEPTH_BUFFER writing disabled for those with other values.

procedure   display; cdecl;
var
    pass    : int;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        if transparent then
        begin
            glEnable(GL_ALPHA_TEST);
            pass := 2;
        end
        else
        begin
            glDisable(GL_ALPHA_TEST);
            pass := 0;
        end;

        // Rotate the whole model
        glRotatef(view_h, 0, 1, 0);
        glRotatef(view_v, 1, 0, 0);

        repeat
            if pass = 2 then
            begin
                glAlphaFunc(GL_EQUAL, 1);
                glDepthMask(GL_TRUE);
                Dec(pass);
            end
            else if pass <> 0 then
            begin
                glAlphaFunc(GL_NOTEQUAL, 1);
                glDepthMask(GL_FALSE);
                Dec(pass);
            end;

            draw_engine_pole();

            glPushMatrix();
                glTranslatef(0.5, 1.4, 0.0);
                draw_cylinder_head();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0.0, -0.8, 0.0);
                draw_crank();
            glPopMatrix();
        until pass = 0;
        glDepthMask(GL_TRUE);
        glutSwapBuffers();
    glPopMatrix();
end;

{-- Animation ----------------------------------------------------------------}

// Called when the window is idle. When called increments the crank angle
// by ANGLE_STEP, updates the head angle and notifies the system that
// the screen needs to be updated.

procedure   animation; cdecl;
begin
    crank_angle := Trunc(crank_angle + crank_step);
    if crank_angle >= 360 then
        crank_angle := 0;
    head_angle := head_look_up_table[crank_angle];
    glutPostRedisplay();
end;

{-- Keyboard -----------------------------------------------------------------}

// Called when a key is pressed. Checks if it reconises the key and if so
// acts on it, updateing the screen.

procedure   keyboard(key: uchar; x, y: int); cdecl;
    procedure   ToggleTexture;
    begin
        if not texture then
        begin
            texture := True;
            glEnable(GL_TEXTURE_2D);
            gluQuadricTexture(obj, GL_TRUE);
        end
        else
        begin
            texture := False;
            glDisable(GL_TEXTURE_2D);
            gluQuadricTexture(obj, GL_FALSE);
        end;
    end;
begin
    case chr(key) of
        's' : begin
                if shaded = FALSE then
                begin
                    shaded := True;
                    glShadeModel(GL_SMOOTH);
                    glEnable(GL_LIGHTING);
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_COLOR_MATERIAL);
                    gluQuadricNormals(obj, GLU_SMOOTH);
                    gluQuadricDrawStyle(obj, GLU_FILL);
                end
                else
                begin
                    shaded := False;
                    glShadeModel(GL_FLAT);
                    glDisable(GL_LIGHTING);
                    glDisable(GL_DEPTH_TEST);
                    glDisable(GL_COLOR_MATERIAL);
                    gluQuadricNormals(obj, GLU_NONE);
                    gluQuadricDrawStyle(obj, GLU_LINE);
                    gluQuadricTexture(obj, GL_FALSE);
                end;
                if texture and not shaded then
                    ToggleTexture;
              end;
        't' : ToggleTexture;
        'o' : transparent := not transparent ;
        'a' : begin
                crank_angle := Trunc(crank_angle + crank_step);
                if (crank_angle >= 360) then
                    crank_angle := 0;
                head_angle := head_look_up_table[crank_angle];
              end;
        'z' : begin
                crank_angle := Trunc(crank_angle - crank_step);
                if crank_angle <= 0 then
                    crank_angle := 360;
                head_angle := head_look_up_table[crank_angle];
              end;
        '0' : if light1 then
              begin
                glDisable(GL_LIGHT0);
                light1 := False;
              end
              else
              begin
                glEnable(GL_LIGHT0);
                light1 := TRUE;
              end;
        '1' : if light2 then
              begin
                glDisable(GL_LIGHT1);
                light2 := False;
              end
              else
              begin
                glEnable(GL_LIGHT1);
                light2 := True;
              end;
        '4' : begin
                view_h := view_h - ANGLE_STEP;
                if view_h <= 0 then
                    view_h := 360;
              end;
        '6' : begin
                view_h := view_h + ANGLE_STEP;
                if view_h >= 360 then
                    view_h := 0;
              end;
        '8' : begin
                view_v := view_v + ANGLE_STEP;
                if view_v >= 360 then
                    view_v := 0;
              end;
        '2' : begin
                view_v := view_v - ANGLE_STEP;
                if view_v <= 0 then
                    view_v := 360;
              end;
        ' ' : if anim then
              begin
                glutIdleFunc(nil);
                anim := False;
              end
              else
              begin
                glutIdleFunc(animation);
                anim := True;
              end;
        '+' : begin
                crank_step := crank_step + 1;
                if crank_step > 45 then
                    crank_step := 45;
              end;
        '-' : begin
                crank_step := crank_step - 1;
                if crank_step <= 0 then
                    crank_step := 0;
              end;
    else
        Exit;
    end;
    glutPostRedisplay();
end;

procedure   special(key: int; x, y: int); cdecl;
begin
    case key of
        GLUT_KEY_LEFT   : begin
                            view_h := view_h - ANGLE_STEP;
                            if view_h <= 0 then
                                view_h := 360;
                          end;
        GLUT_KEY_RIGHT  : begin
                            view_h := view_h + ANGLE_STEP;
                            if view_h >= 360 then
                                view_h := 0;
                          end;
        GLUT_KEY_UP     : begin
                            view_v := view_v + ANGLE_STEP;
                            if view_v >= 360 then
                                view_v := 0;
                          end;
        GLUT_KEY_DOWN   : begin
                            view_v := view_v - ANGLE_STEP;
                            if view_v <= 0 then
                                view_v := 360;
                          end;
    else
        Exit;
    end;
    glutPostRedisplay();
end;

{-- Menu ---------------------------------------------------------------------}

// Called when a menu option has been selected. Translates the menu item
// identifier into a keystroke, then call's the keyboard function.

procedure   menu(val: int); cdecl;
const
    Keys    : array[1..8] of char =
        ('s',' ','t','o','0','1','+','-');
begin
    if val in [1..8] then
        keyboard(uchar(Keys[val]), 0, 0);
end;

{-- Initialises the menu of toggle -------------------------------------------}

procedure   create_menu;
begin
    glutCreateMenu(menu);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutAddMenuEntry('Shaded', 1);
    glutAddMenuEntry('Animation', 2);
    glutAddMenuEntry('Texture', 3);
    glutAddMenuEntry('Transparency', 4);
    glutAddMenuEntry('Right Light (0)', 5);
    glutAddMenuEntry('Left Light (1)', 6);
    glutAddMenuEntry('Speed UP', 7);
    glutAddMenuEntry('Slow Down', 8);
end;

{-- Makes a simple check pattern ---------------------------------------------}

// Copied from the redbook example "checker.c"

procedure   make_image;
var
    i, j, c : int;
begin
    for i := 0 to IMAGE_WIDTH - 1 do
        for j := 0 to IMAGE_HEIGHT - 1 do
        begin
            c := Ord(((i and $8) = 0) xor ((j and $8) = 0)) * 255;
            image[i][j][0] := c;
            image[i][j][1] := c;
            image[i][j][2] := c;
        end;
end;

{-- Makes the head look up table for all possible crank angle ----------------}

procedure   make_table;
var
    i   : GLint;
    k   : GLdouble;
begin
    k := 0.0;
    for i := 0 to 359 do
    begin
        head_look_up_table[i] :=
                MAGNITUDE * ArcTan(
                            (ARC_RADIUS * sin(PHASE - k / FREQ_DIV)) /
                            ((ARC_LENGHT - ARC_RADIUS * cos(PHASE - k / FREQ_DIV))));
        k := k + 1;
    end;
end;

{-- MyInit -------------------------------------------------------------------}

// Initialises texturing, lighting, display lists, and everything else
// associated with the model.

procedure   myinit;
const
    mat_specular    : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    mat_shininess   : array[0..0] of float = ( 50.0 );
    light_position1 : array[0..3] of float = ( 1.0, 1.0, 1.0, 0.0 );
    light_position2 : array[0..3] of float = ( -1.0, 1.0, 1.0, 0.0 );
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);

    obj := gluNewQuadric();
    make_table();
    make_image();

    // Set up Texturing
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, @image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Set up Lighting
    glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, @light_position1);
    glLightfv(GL_LIGHT1, GL_POSITION, @light_position2);

    // Initial render mode is with full shading and LIGHT 0 enabled.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    // Initialise display lists
    glNewList(list_piston_shaded, GL_COMPILE);
        draw_piston();
    glEndList();
    glNewList(list_flywheel_shaded, GL_COMPILE);
        draw_flywheel();
    glEndList();

    gluQuadricTexture(obj, GL_TRUE);
    glNewList(list_piston_texture, GL_COMPILE);
        draw_piston();
    glEndList();
    glNewList(list_flywheel_texture, GL_COMPILE);
        draw_flywheel();
    glEndList();
    gluQuadricTexture(obj, GL_FALSE);
end;

{-- MyReshape ----------------------------------------------------------------}

// Called when the model's window has been reshaped.

procedure   myReshape(w, h: int); cdecl;
begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, w / h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);  // viewing transform
    glScalef(1.5, 1.5, 1.5);
end;

{-- Demo ---------------------------------------------------------------------}

// An interactive model of a miniture steam engine.
// Sets system in Double Buffered mode and initialises all the call-back
// functions.

procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(400, 400);

    // Transperancy won't work properly without GLUT_ALPHA
    glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGBA or GLUT_DEPTH or GLUT_MULTISAMPLE);
    glutCreateWindow('Miniature Steam Engine by Troy Robinette');

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    create_menu();

    myinit();

    glutIdleFunc(animation);
    glutReshapeFunc(myReshape);
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Steam','Miniature Steam Engine Simulation',Demo);
end.
