{=============================================================================}
{= Copyright (c) Brian Paul                                                  =}
{=============================================================================}
unit Bounce;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLFx,
    MGLGl,
    MGLGlut,
    CmdLine,
    DemoList;

function    COS(X: GLfloat): GLfloat;
begin
    Result := System.Cos(X*Pi/180.0);
end;

function    SIN(X: GLfloat): GLfloat;
begin
    Result := System.Sin(X*Pi/180.0);
end;

var
    Ball    : GLuint;
    Mode    : GLenum;

    Zrot    : GLfloat =   0.0;
    Zstep   : GLfloat =   4.0;
    Xpos    : GLfloat =   0.0;
    Ypos    : GLfloat =   1.0;
    Xvel    : GLfloat =   0.1;
    Yvel    : GLfloat =   0.0;
    Xmin    : GLfloat =  -4.0;
    Xmax    : GLfloat =   4.0;
    Ymin    : GLfloat =  -3.8;
    Ymax    : GLfloat =   4.0;
    G       : GLfloat = -0.05;

function    MakeBall: GLuint;
var
    list    : GLuint;
    a, b    : GLfloat;
    da      : GLfloat;
    db      : GLfloat;
    radius  : GLfloat;
    color   : GLuint;
    x, y, z : GLfloat;
begin
    da      := 18.0;
    db      := 18.0;
    radius  := 1.0;

    list    := glGenLists( 1 );

    glNewList(list, GL_COMPILE);

    color   := 0;
    a := -90.0;
    while a+da <= 90.0 do
    begin
        glBegin(GL_QUAD_STRIP);
        b := 0.0;
        while b <= 360.0 do
        begin

            if color <> 0 then
                glIndexf(MGL_RED)
            else
                glIndexf(MGL_WHITE);

            x       := COS(b) * COS(a);
            y       := SIN(b) * COS(a);
            z       := SIN(a);
            glVertex3f( x, y, z );

            x       := radius * COS(b) * COS(a+da);
            y       := radius * SIN(b) * COS(a+da);
            z       := radius * SIN(a+da);
            glVertex3f( x, y, z );

            color   := 1-color;
            b       := b + db;

        end;
        glEnd();
        a   := a + da;
    end;
    glEndList();
    Result := list;
end;

procedure   Reshape(width, height: int); cdecl;
begin
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -6.0, 6.0, -6.0, 6.0, -6.0, 6.0 );
    glMatrixMode(GL_MODELVIEW);
end;

procedure   Keyboard(key: uchar; x, y: int); cdecl;
begin
    case key of
        $1B: glutExit();
    end;
end;

procedure   Draw; cdecl;
var
    i : GLint;
begin
    glClear( GL_COLOR_BUFFER_BIT );
    glIndexf( MGL_CYAN );
    glBegin( GL_LINES );
    for i := -5 to 5 do
    begin
        glVertex2i( i, -5 );
        glVertex2i( i, 5 );
    end;

    for i := -5 to 5 do
    begin
        glVertex2i( -5,i );
        glVertex2i( 5,i );
    end;

    for i := -5 to 5 do
    begin
        glVertex2i( i, -5 );
        glVertex2f( i*1.15, -5.9 );
    end;

    glVertex2f( -5.3, -5.35 );
    glVertex2f( 5.3, -5.35 );
    glVertex2f( -5.75, -5.9 );
    glVertex2f( 5.75, -5.9 );
    glEnd();

    glPushMatrix();
    glTranslatef( Xpos, Ypos, 0.0 );
    glScalef( 2.0, 2.0, 2.0 );
    glRotatef( 8.0, 0.0, 0.0, 1.0 );
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    glRotatef( Zrot, 0.0, 0.0, 1.0 );

    glCallList( Ball );

    glPopMatrix();

    glFlush();
    glutSwapBuffers();
end;

var
    vel0 : float = -100.0;

procedure   Idle; cdecl;
begin
    Zrot := Zrot + Zstep;

    Xpos := Xpos + Xvel;
    if Xpos >= Xmax then
    begin
        Xpos    := Xmax;
        Xvel    := -Xvel;
        Zstep   := -Zstep;
    end;

    if Xpos <= Xmin then
    begin
        Xpos    := Xmin;
        Xvel    := -Xvel;
        Zstep   := -Zstep;
    end;

    Ypos := Ypos + Yvel;
    Yvel := Yvel + G;

    if Ypos < Ymin then
    begin
        Ypos := Ymin;
        if vel0 = -100.0 then
            vel0 := Abs(Yvel);
        Yvel := vel0;
    end;
    Draw;
end;

procedure   Demo;
begin
    InitGLUT;
    glutInitWindowSize(300, 300);
    glutInitDisplayMode(GLUT_INDEX or GLUT_DOUBLE);
    glutCreateWindow('Bounce');

    Ball := MakeBall();
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
    glDisable( GL_DITHER );
    glShadeModel( GL_FLAT );
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Bounce','Bouncing ball demo',Demo);
end.
