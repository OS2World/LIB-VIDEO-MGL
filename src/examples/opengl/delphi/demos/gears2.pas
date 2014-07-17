unit Gears2;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlut,
    CmdLine,
    DemoList;

const
    ccolors : array[0..11] of byte =
        (
            $ff, $00, $00,
            $00, $ff, $00,
            $00, $00, $ff,
            $99, $99, $99
        );

var
    cptr    : int = 0;
    wd, ht  : int;

    d_near  : double = 1.0;
    d_far   : double = 2000;
    poo     : int = 0;

type
    PProfile        = ^Profile;
    Profile         = record
        rad, wid    : float;
    end;

    PProfileData    = ^ProfileData;
    ProfileData     = array[0..100] of Profile;

procedure   flat_face(ir, _or, wd: float); forward;
procedure   draw_inside(w1, w2, rad: float); forward;
procedure   draw_outside(w1, w2, rad: float); forward;
procedure   tooth_side(nt: int; ir, _or, tp, tip, wd: float); forward;

var
    circle_subdiv   : int;

//
// nt - number of teeth
// wd - width of gear at teeth
// ir - inside radius absolute scale
// or - radius at outside of wheel (tip of tooth) ratio of ir
// sa - angle of slant of teeth on gear (1 = slant of one whole tooth)
// tp - ratio of tooth in slice of circle (0..1] (1 = teeth are touching at base)
// tip - ratio of tip of tooth (0..tp] (cant be wider that base of tooth)
// ns - number of elements in wheel width profile
// ip - list of float pairs {start radius, width, ...} (width is ratio to wd)
//
procedure   gear(nt: int; wd, ir, _or, sa, tp, tip: float; ns: int; ip: PProfileData);
var
    prev    : float;
    k, t    : int;
begin
    { TODO: May be init prev and t somehow? }
    
    // gear lying on xy plane, z for width. all normals calulated (normalized)

    // estimat # times to divide circle
    if nt <= 0 then
        circle_subdiv := 64
    else
    begin
        // lowest multiple of number of teeth
        circle_subdiv := nt;
        while circle_subdiv < 64 do
            circle_subdiv := circle_subdiv + nt;
    end;

    // --- draw wheel face ---

    // draw horzontal, vertical faces for each section. if first
    // section radius not zero, use wd for 0.. first if ns == 0
    // use wd for whole face. last width used to edge.

    if ns <= 0 then
        flat_face(0.0, ir, wd)
    else
    begin
        // draw first flat_face, then continue in loop
        if ip[0].rad > 0.0 then
        begin
            flat_face(0.0, ip[0].rad * ir, wd);
            prev    := wd;
            t       := 0;
        end
        else
        begin
            flat_face(0.0, ip[1].rad * ir, ip[0].wid * wd);
            prev    := ip[0].wid;
            t       := 1;
        end;
    end;

    for k := t to ns - 1 do
    begin
        if prev < ip[k].wid then
            draw_inside(prev * wd, ip[k].wid * wd, ip[k].rad * ir)
        else
            draw_outside(prev * wd, ip[k].wid * wd, ip[k].rad * ir);

        prev := ip[k].wid;

        // - draw to edge of wheel, add final face if needed -
        if k = ns - 1 then
        begin
            flat_face(ip[k].rad * ir, ir, ip[k].wid * wd);

            // now draw side to match tooth rim
            if ip[k].wid < 1.0 then
                draw_inside(ip[k].wid * wd, wd, ir)
            else
                draw_outside(ip[k].wid * wd, wd, ir);
        end
        else
            flat_face(ip[k].rad * ir, ip[k + 1].rad * ir, ip[k].wid * wd);
    end;

    // --- tooth side faces ---
    tooth_side(nt, ir, _or, tp, tip, wd);

    // --- tooth hill surface ---
end;

procedure   tooth_side(nt: int; ir, _or, tp, tip, wd: float);
var
    i       : float;
    _end    : float;
    x       : array[0..5] of float;
    y       : array[0..5] of float;
    s       : array[0..2] of float;
    c       : array[0..2] of float;
    a, b, n : float;
    _c      : float;
begin
    _end    := 2.0 * Pi / nt;
    _or     := _or * ir;   // or is really a ratio of ir

    i := 0;
    while i < 2.0 * Pi - _end / 4.0 do
    begin
        c[0] := cos(i);
        s[0] := sin(i);
        c[1] := cos(i + _end * (0.5 - tip / 2));
        s[1] := sin(i + _end * (0.5 - tip / 2));
        c[2] := cos(i + _end * (0.5 + tp / 2));
        s[2] := sin(i + _end * (0.5 + tp / 2));

        x[0] := ir * c[0];
        y[0] := ir * s[0];
        x[5] := ir * cos(i + _end);
        y[5] := ir * sin(i + _end);
        // -- treat veritices 1,4 special to match strait edge of face
        x[1] := x[0] + (x[5] - x[0]) * (0.5 - tp / 2);
        y[1] := y[0] + (y[5] - y[0]) * (0.5 - tp / 2);
        x[4] := x[0] + (x[5] - x[0]) * (0.5 + tp / 2);
        y[4] := y[0] + (y[5] - y[0]) * (0.5 + tp / 2);
        x[2] := _or * cos(i + _end * (0.5 - tip / 2));
        y[2] := _or * sin(i + _end * (0.5 - tip / 2));
        x[3] := _or * cos(i + _end * (0.5 + tip / 2));
        y[3] := _or * sin(i + _end * (0.5 + tip / 2));

        // draw face trapezoids as 2 tmesh
        glNormal3f(0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[2], y[2], wd / 2);
        glVertex3f(x[1], y[1], wd / 2);
        glVertex3f(x[3], y[3], wd / 2);
        glVertex3f(x[4], y[4], wd / 2);
        glEnd();

        glNormal3f(0.0, 0.0, -1.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[2], y[2], -wd / 2);
        glVertex3f(x[1], y[1], -wd / 2);
        glVertex3f(x[3], y[3], -wd / 2);
        glVertex3f(x[4], y[4], -wd / 2);
        glEnd();

        // draw inside rim pieces
        glNormal3f(c[0], s[0], 0.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[0], y[0], -wd / 2);
        glVertex3f(x[1], y[1], -wd / 2);
        glVertex3f(x[0], y[0], wd / 2);
        glVertex3f(x[1], y[1], wd / 2);
        glEnd();

        // draw up hill side

        // calculate normal of face
        a   := x[2] - x[1];
        b   := y[2] - y[1];
        n   := 1.0 / sqrt(a * a + b * b);
        a   := a * n;
        b   := b * n;
        glNormal3f(b, -a, 0.0);

        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[1], y[1], -wd / 2);
        glVertex3f(x[2], y[2], -wd / 2);
        glVertex3f(x[1], y[1], wd / 2);
        glVertex3f(x[2], y[2], wd / 2);
        glEnd();

        // draw top of hill 
        glNormal3f(c[1], s[1], 0.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[2], y[2], -wd / 2);
        glVertex3f(x[3], y[3], -wd / 2);
        glVertex3f(x[2], y[2], wd / 2);
        glVertex3f(x[3], y[3], wd / 2);
        glEnd();

        // draw down hill side

        // calculate normal of face
        a   := x[4] - x[3];
        b   := y[4] - y[3];
        _c  := 1.0 / sqrt(a * a + b * b);
        a   := a * _c;
        b   := b * _c;
        glNormal3f(b, -a, 0.0);

        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[3], y[3], -wd / 2);
        glVertex3f(x[4], y[4], -wd / 2);
        glVertex3f(x[3], y[3], wd / 2);
        glVertex3f(x[4], y[4], wd / 2);
        glEnd();

        // inside rim part

        glNormal3f(c[2], s[2], 0.0);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x[4], y[4], -wd / 2);
        glVertex3f(x[5], y[5], -wd / 2);
        glVertex3f(x[4], y[4], wd / 2);
        glVertex3f(x[5], y[5], wd / 2);
        glEnd();

        i := i + _end;
    end;
end;

procedure   flat_face(ir, _or, wd: float);
var
    i   : int;
    w   : float;
begin
    // draw each face (top & bottom )
    if poo <> 0 then
        ; // printf("Face  : %f..%f wid=%f\n", ir, or, wd); 
    if wd = 0.0 then
        Exit;
    w := wd / 2;
    while w > -wd do
    begin
        if w > 0.0 then
            glNormal3f(0.0, 0.0, 1.0)
        else
            glNormal3f(0.0, 0.0, -1.0);

        if ir = 0.0 then
        begin
            // draw as t-fan
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0, 0.0, w);    // center
            glVertex3f(_or, 0.0, w);
            for i := 1 to circle_subdiv - 1 do
                glVertex3f(cos(2.0 * Pi * i / circle_subdiv) * _or,
                           sin(2.0 * Pi * i / circle_subdiv) * _or,
                           w);
            glVertex3f(_or, 0.0, w);
            glEnd();
        end
        else
        begin
            // draw as tmesh
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(_or, 0.0, w);
            glVertex3f(ir, 0.0, w);
            for i := 1 to circle_subdiv - 1 do
            begin
                glVertex3f(cos(2.0 * Pi * i / circle_subdiv) * _or,
                           sin(2.0 * Pi * i / circle_subdiv) * _or,
                           w);
                glVertex3f(cos(2.0 * Pi * i / circle_subdiv) * ir,
                           sin(2.0 * Pi * i / circle_subdiv) * ir,
                           w);
            end;
            glVertex3f(_or, 0.0, w);
            glVertex3f(ir, 0.0, w);
            glEnd();
        end;
        w := w - wd;
    end;
end;

procedure   draw_inside(w1, w2, rad: float);
var
    i, j    : int;
    c, s    : float;
begin
    if poo <> 0 then
        ; // printf("Inside: wid=%f..%f rad=%f\n", w1, w2, rad);
    if w1 = w2 then
        Exit;

    w1 := w1 / 2;
    w2 := w2 / 2;

    for j := 0 to 1 do
    begin
        if j = 1 then
        begin
            w1 := -w1;
            w2 := -w2;
        end;
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(rad, 0.0, w1);
        glVertex3f(rad, 0.0, w2);
        for i := 1 to circle_subdiv - 1 do
        begin
            c := cos(2.0 * Pi * i / circle_subdiv);
            s := sin(2.0 * Pi * i / circle_subdiv);
            glNormal3f(-c, -s, 0.0);
            glVertex3f(c * rad, s * rad, w1);
            glVertex3f(c * rad, s * rad, w2);
        end;
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(rad, 0.0, w1);
        glVertex3f(rad, 0.0, w2);
        glEnd();
    end;
end;

procedure   draw_outside(w1, w2, rad: float);
var
    i, j    : int;
    c, s    : float;
begin
    if poo <> 0 then
        ; // printf("Outsid: wid=%f..%f rad=%f\n", w1, w2, rad);
    if w1 = w2 then
        Exit;

    w1 := w1 / 2;
    w2 := w2 / 2;
    for j := 0 to 1 do
    begin
        if j = 1 then
        begin
            w1 := -w1;
            w2 := -w2;
        end;
        glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(rad, 0.0, w1);
        glVertex3f(rad, 0.0, w2);
        for i := 1 to circle_subdiv - 1 do
        begin
            c := cos(2.0 * Pi * i / circle_subdiv);
            s := sin(2.0 * Pi * i / circle_subdiv);
            glNormal3f(c, s, 0.0);
            glVertex3f(c * rad, s * rad, w1);
            glVertex3f(c * rad, s * rad, w2);
        end;
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(rad, 0.0, w1);
        glVertex3f(rad, 0.0, w2);
        glEnd();
    end;
end;

const
    gear_profile    : array[0..5] of Profile =
        (
            (rad: 0.000; wid: 0.0),
            (rad: 0.300; wid: 7.0),
            (rad: 0.340; wid: 0.4),
            (rad: 0.550; wid: 0.64),
            (rad: 0.600; wid: 0.4),
            (rad: 0.950; wid: 1.0)
        );

var
    a1 : float = 27.0;
    a2 : float = 67.0;
    a3 : float = 47.0;
    a4 : float = 87.0;
    i1 : float = 1.2;
    i2 : float = 3.1;
    i3 : float = 2.3;
    i4 : float = 1.1;

procedure   OneFrame; cdecl;
    function    FixAdd(v1, v2: float): float;
    begin
        Result := v1 + v2;
        if Result > 360.0 then Result := Result - 360.0;
        { TODO: may be + here? }
        if Result < 0.0 then Result := Result - 360.0;
    end;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(0.0, 0.0, -4.0);
    glRotatef(a3, 1.0, 1.0, 1.0);
    glRotatef(a4, 0.0, 0.0, -1.0);
    glTranslatef(0.14, 0.2, 0.0);
    gear(76, 0.4, 2.0, 1.1, 0.0, 0.4, 0.04, High(gear_profile)+1, @gear_profile);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1, 0.2, -3.8);
    glRotatef(a2, -4.0, 2.0, -1.0);
    glRotatef(a1, 1.0, -3.0, 1.0);
    glTranslatef(0.0, -0.2, 0.0);
    gear(36, 0.4, 2.0, 1.1, 0.0, 0.7, 0.2, High(gear_profile)+1, @gear_profile);
    glPopMatrix();

    a1 := FixAdd(a1,i1);
    a2 := FixAdd(a2,i2);
    a3 := FixAdd(a3,i3);
    a4 := FixAdd(a4,i4);

    glutSwapBuffers();
end;

procedure   Display; cdecl;
begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
end;

procedure   MyReshape(w, h: int); cdecl;
begin
    wd := w;
    ht := h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, d_near, d_far);
    {
        use perspective instead:

        if w <= h then
            glOrtho( 0.0, 1.0,
                     0.0, 1.0 * h / w,
                    -16.0, 4.0)
        else
            glOrtho( 0.0, 1.0 * w / h,
                     0.0, 1.0,
                    -16.0, 4.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
end;

procedure   Visibility(status: int); cdecl;
begin
    if status = GLUT_VISIBLE then
        glutIdleFunc(OneFrame)
    else
        glutIdleFunc(nil);
end;

procedure   MyInit;
var
    f : array[0..19] of float;
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);
    myReshape(640, 480);
    // glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);

    glLightf(GL_LIGHT0, GL_SHININESS, 1.0);
    f[0] := 1.3;
    f[1] := 1.3;
    f[2] := -3.3;
    f[3] := 1.0;
    glLightfv(GL_LIGHT0, GL_POSITION, @f);
    f[0] := 0.8;
    f[1] := 1.0;
    f[2] := 0.83;
    f[3] := 1.0;
    glLightfv(GL_LIGHT0, GL_SPECULAR, @f);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @f);
    glEnable(GL_LIGHT0);

    glLightf(GL_LIGHT1, GL_SHININESS, 1.0);
    f[0] := -2.3;
    f[1] := 0.3;
    f[2] := -7.3;
    f[3] := 1.0;
    glLightfv(GL_LIGHT1, GL_POSITION, @f);
    f[0] := 1.0;
    f[1] := 0.8;
    f[2] := 0.93;
    f[3] := 1.0;
    glLightfv(GL_LIGHT1, GL_SPECULAR, @f);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, @f);
    glEnable(GL_LIGHT1);

    // gear material
    f[0] := 0.1;
    f[1] := 0.15;
    f[2] := 0.2;
    f[3] := 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, @f);

    f[0] := 0.9;
    f[1] := 0.3;
    f[2] := 0.3;
    f[3] := 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, @f);

    f[0] := 0.4;
    f[1] := 0.9;
    f[2] := 0.6;
    f[3] := 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, @f);

    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 4);
end;

procedure   Keys(c: uchar; x, y: int); cdecl;
begin
    if c = $1B then
        glutExit;
end;

procedure   Demo;
var
    mode: int;
begin
    mode := GLUT_DOUBLE;

    InitGLUT;

    glutInitDisplayMode(mode or GLUT_RGB or GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow('Gears2');

    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutVisibilityFunc(visibility);
    glutPostRedisplay();
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Gears2','*** No description yet',Demo);
end.
