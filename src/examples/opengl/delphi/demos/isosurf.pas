{=============================================================================}
{= Copyright (c) Brian Paul                                                  =}
{=============================================================================}
unit IsoSurf;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlut,
    SysUtils,
    CmdLine,
    DemoList;

var
    speed_test          : GLboolean = GL_FALSE;
    use_vertex_arrays   : GLboolean = GL_FALSE;

    doubleBuffer        : GLboolean = GL_TRUE;

    smooth              : GLboolean = GL_TRUE;
    lighting            : GLboolean = GL_TRUE;

const
    MAXVERTS = 10000;

var
    verts               : array[0..MAXVERTS-1,0..2] of GLfloat;
    norms               : array[0..MAXVERTS-1,0..2] of GLfloat;
    numverts            : GLint;

    xrot, yrot          : GLfloat;

procedure   ReadSurface(const Filename: string);
var
    F: TextFile;
begin
    AssignFile(F,FileName);
    Reset(F);
    try
        numverts := 0;
        while not Eof(F) and (numverts < MAXVERTS) do
        begin
            Read(F,verts[numverts][0],verts[numverts][1],verts[numverts][2],
                     norms[numverts][0],norms[numverts][1],norms[numverts][2]);
            Inc(numverts);
        end;
        { TODO: Why Dec? }
        Dec(numverts);
    finally
        CloseFile(F);
    end;
end;

procedure   DrawSurface;
var
    i: GLuint;
begin
{$IFDEF GL_EXT_vertex_array}
    if use_vertex_arrays then
        glDrawArraysEXT( GL_TRIANGLE_STRIP, 0, numverts )
    else
    begin
{$ENDIF}
        glBegin( GL_TRIANGLE_STRIP );
        for i := 0 to numverts - 1 do
        begin
            glNormal3fv( @norms[i] );
            glVertex3fv( @verts[i] );
        end;
        glEnd();
{$IFDEF GL_EXT_vertex_array}
    end;
{$ENDIF}
end;    

procedure   Draw1;
begin
    glClear( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    glRotatef( yrot, 0.0, 1.0, 0.0 );
    glRotatef( xrot, 1.0, 0.0, 0.0 );

    DrawSurface();

    glPopMatrix();

    glFlush();
    if doubleBuffer then
        glutSwapBuffers();
end;

procedure   Draw; cdecl;
begin
    if speed_test then
    begin
        xrot := 0.0;
        while xrot<=360.0 do
        begin
            draw1();
            xrot := xrot + 10.0;
        end;
        glutExit();
    end
    else
        draw1();
end;

procedure   InitMaterials;
const
    ambient             : array[0..3] of float = ( 0.1, 0.1, 0.1, 1.0 );
    diffuse             : array[0..3] of float = ( 0.5, 1.0, 1.0, 1.0 );
    position0           : array[0..3] of float = ( 0.0, 0.0, 20.0, 0.0 );
    position1           : array[0..3] of float = ( 0.0, 0.0, -20.0, 0.0 );
    front_mat_shininess : array[0..0] of float = ( 60.0 );
    front_mat_specular  : array[0..3] of float = ( 0.2, 0.2, 0.2, 1.0 );
    front_mat_diffuse   : array[0..3] of float = ( 0.5, 0.28, 0.38, 1.0 );
    {
    back_mat_shininess  : array[0..0] of float = ( 60.0 );
    back_mat_specular   : array[0..3] of float = ( 0.5, 0.5, 0.2, 1.0 );
    back_mat_diffuse    : array[0..3] of float = ( 1.0, 1.0, 0.2, 1.0 );
    }
    lmodel_ambient      : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    lmodel_twoside      : array[0..0] of float = ( Ord(GL_FALSE) );
begin    
    glLightfv(GL_LIGHT0, GL_AMBIENT, @ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, @position0);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, @ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, @diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, @position1);
    glEnable(GL_LIGHT1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, @lmodel_twoside);
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, @front_mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, @front_mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, @front_mat_diffuse);
end;

procedure   Init;
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    InitMaterials();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5, 25 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, -6.0 );

{$IFDEF GL_EXT_vertex_array}
    if use_vertex_arrays then
    begin
        glVertexPointerEXT( 3, GL_FLOAT, 0, numverts, @verts );
        glNormalPointerEXT( GL_FLOAT, 0, numverts, @norms );
        glEnable( GL_VERTEX_ARRAY_EXT );
        glEnable( GL_NORMAL_ARRAY_EXT );
    end;
{$ENDIF}
end;

procedure   Reshape(width, height: int); cdecl;
begin
    glViewport(0, 0, width, height);
end;

procedure   Keyboard(key: uchar; x, y: int); cdecl;
begin
    case Chr(key) of
        's'     : begin
                    smooth := not smooth;
                    if smooth then
                        glShadeModel(GL_SMOOTH)
                    else
                        glShadeModel(GL_FLAT);
                  end;
        'l'     : begin
                    lighting := not lighting;
                    if lighting then
                        glEnable(GL_LIGHTING)
                    else
                        glDisable(GL_LIGHTING);
                  end;
        #$1B    : glutExit();
    else
        Exit;
    end;
    glutPostRedisplay();
end;

procedure   Special(key: int; x, y: int); cdecl;
begin
    case key of
        GLUT_KEY_LEFT   : yrot := yrot - 15.0;
        GLUT_KEY_RIGHT  : yrot := yrot + 15.0;
        GLUT_KEY_UP     : xrot := xrot + 15.0;
        GLUT_KEY_DOWN   : xrot := xrot - 15.0;
    else
        Exit;
    end;
    glutPostRedisplay();
end;

procedure   Args;
var
    i: GLint;
begin
    for i := 1 to ParamCount do
    begin
        if CompareText(ParamStr(i),'-sb') = 0 then
            doubleBuffer    := GL_FALSE
        else if CompareText(ParamStr(i),'-db') = 0 then
            doubleBuffer    := GL_TRUE
        else if CompareText(ParamStr(i),'-speed') = 0 then
        begin
            speed_test      := GL_TRUE;
            doubleBuffer    := GL_TRUE;
        end
        else if CompareText(ParamStr(i),'-va') = 0 then
            use_vertex_arrays := GL_TRUE;
    end;
end;

procedure   Demo;
var
    _type       : GLenum;
    extensions  : PChar;
begin
    if not FileExists('data\isosurf.dat') then
    begin
        MessageBox(0,
                   'Can''t find DATA\ISOSURF.DAT. ' +
                   'It should be present in the corresponding C example program',
                   'Error',
                   MB_ICONSTOP or MB_OK);
        Abort;
    end;

    ReadSurface( 'data\isosurf.dat' );
    
    Args;

    glutInitWindowSize(400, 400);
    InitGLUT;

    _type   := GLUT_DEPTH;
    _type   := _type or GLUT_RGB;
    if doubleBuffer then
        _type := _type or GLUT_DOUBLE
    else
        _type := _type or GLUT_SINGLE;

    glutInitDisplayMode(_type);
    glutCreateWindow('Isosurface');

    // Make sure server supports the vertex array extension
    extensions  := PChar(glGetString( GL_EXTENSIONS ));
    if Pos('GL_EXT_vertex_array', extensions) = 0 then
        use_vertex_arrays := GL_FALSE;

    Init();
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(Special);
    glutReshapeFunc(Reshape);
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos',
                 'IsoSurf',
                 'Display an isosurface of 3-D wind speed volume. Use arrow keys to '+
                 'rotate, S toggles smooth shading, L toggles lighting',
                 Demo);
end.
