//
// Copyright (c) 1991, 1992, 1993 Silicon Graphics, Inc.
//
// Permission to use, copy, modify, distribute, and sell this software and
// its documentation for any purpose is hereby granted without fee, provided
// that (i) the above copyright notices and this permission notice appear in
// all copies of the software and related documentation, and (ii) the name of
// Silicon Graphics may not be used in any advertising or
// publicity relating to the software without the specific, prior written
// permission of Silicon Graphics.
//
// THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF
// ANY KIND,
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR
// ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
// LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
// OF THIS SOFTWARE.
//
//
//
// Pentium@133Mhz+Win95+Mesa+Monster 3D:
//
//                driver v0.13
//                28.1 frames/sec (Screen Refresh 75Hz)
//
//
//
// Pentium@133Mhz+Linux+Mesa+X11 (XFree 3.2) 8bit (Diamond Stealth 64 2MB VRAM):
//
//
//
// Indy 4600SC@133MHz+SGI OpenGL 24bit:
//
unit Wave;

interface

implementation

uses
    Windows,
    MGLTypes,
    MGLGl,
    MGLGlu,
    MGLGlut,
    CmdLine,
    SysUtils,
    DemoList;

var
    rgb,
    doubleBuffer,
    directRender    : GLboolean ;

type
    indexesArr      = array[0..2] of GLint ;
    pindexesArr     = ^indexesArr;
    colorArr        = array[0..2] of GLfloat ;
    pcolorArr       = ^colorArr;
    coordArr        = array[0..2] of GLfloat ;
    pcoordArr       = ^coordArr;

var
    colorIndexes1   : indexesArr;
    colorIndexes2   : indexesArr;
    clearMask       : GLenum = GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT;

    smooth          : GLboolean = GL_TRUE;
    lighting        : GLboolean = GL_TRUE;
    depth           : GLboolean = GL_TRUE;
    stepMode        : GLboolean = GL_FALSE;
    spinMode        : GLboolean = GL_FALSE;
    contouring      : GLint = 0;

    widthX, widthY  : GLint;
    checkerSize     : GLint;
    height          : float;

    frames          : GLint;
    curFrame        : GLint = 0;
    nextFrame       : GLint = 0;

type
    pfacet  = ^facet;
    facet   = record
        color   : colorArr;
        normal  : coordArr;
    end;

    pcoord  = ^coord;
    coord   = record
        vertex  : coordArr;
        normal  : coordArr;
    end;

    mesh    = record
        widthX, widthY  : GLint;
        numFacets       : GLint;
        numCoords       : GLint;
        frames          : GLint;
        coords          : pcoord;
        facets          : pfacet;
    end;

var
    theMesh : mesh;

const
    contourTexture1 : array[0..15] of GLubyte =
        (
            255, 255, 255, 255,
            255, 255, 255, 255,
            255, 255, 255, 255,
            127, 127, 127, 127
        );

    contourTexture2 : array[0..15] of GLubyte =
        (
            255, 255, 255, 255,
            255, 127, 127, 127,
            255, 127, 127, 127,
            255, 127, 127, 127
        );

function    GETCOORD(frame, x, y: int): pcoord;
begin
    Result := theMesh.coords;
    Inc(Result,frame*theMesh.numCoords+x+y*(theMesh.widthX+1));
end;

function    GETFACET(frame, x, y: int): pfacet;
begin
    Result := theMesh.facets;
    Inc(Result,frame*theMesh.numFacets+x+y*theMesh.widthX);
end;

type
    pfloat = ^float;

var
    told    : ulong = 0;
    count   : ulong = 0;

procedure   Animate; cdecl;
var
    coord       : pcoord;
    facet       : pfacet;
    lastColor   : pfloat;
    thisColor   : pfloat;
    i, j        : GLint;
    framesec    : float;
    t, et       : ulong;
begin
    if (count mod 100) = 0 then
    begin
        framesec    := 0.0;
        t           := GetCurrentTime;
        et          := t-told;

        if et>0 then
        begin
            framesec    := 100.0/(et/1000);
            told        := t;
            // fprintf(stderr,"Frames/sec %.2f \n",framesec);
            // fflush(stderr);
        end;
    end;
    Inc(count);

    glClear(clearMask);

    if (nextFrame <> 0) or not stepMode then
        Inc(curFrame);

    if curFrame >= theMesh.frames then
        curFrame := 0;

    if ((nextFrame <> 0) or not stepMode) and spinMode then
        glRotatef(5.0, 0.0, 0.0, 1.0);

    nextFrame := 0;

    for i := 0 to theMesh.widthX - 1 do
    begin
        glBegin(GL_QUAD_STRIP);
        lastColor := nil;
        for j := 0 to theMesh.widthY - 1 do
        begin
            facet := GETFACET(curFrame, i, j);
            if not smooth and lighting then
                glNormal3fv(@facet.normal);

            if lighting then
                if rgb then
                begin
                    thisColor := @facet.color;
                    glColor3fv(@facet.color);
                end
                else
                begin
                    thisColor := @facet.color;
                    glMaterialfv(GL_FRONT_AND_BACK, GL_COLOR_INDEXES,
                                 @facet.color);
                end
            else
                if rgb then
                begin
                    thisColor := @facet.color;
                    glColor3fv(@facet.color);
                end
                else
                begin
                    thisColor := @facet.color;
                    glIndexf(facet.color[1]);
                end;

            if (lastColor = nil) or ((thisColor^ <> lastColor^) and smooth) then
            begin
                if lastColor <> nil then
                begin
                    glEnd();
                    glBegin(GL_QUAD_STRIP);
                end;
                coord := GETCOORD(curFrame, i, j);
                if smooth and lighting then
                    glNormal3fv(@coord.normal);
                glVertex3fv(@coord.vertex);

                coord := GETCOORD(curFrame, i+1, j);
                if smooth and lighting then
                    glNormal3fv(@coord.normal);
                glVertex3fv(@coord.vertex);
            end;

            coord := GETCOORD(curFrame, i, j+1);
            if smooth and lighting then
                glNormal3fv(@coord.normal);
            glVertex3fv(@coord.vertex);

            coord := GETCOORD(curFrame, i+1, j+1);
            if smooth and lighting then
                glNormal3fv(@coord.normal);
            glVertex3fv(@coord.vertex);

            lastColor := thisColor;
        end;
        glEnd();
    end;

    glFlush();
    if doubleBuffer then
        glutSwapBuffers();
end;

procedure   SetColorMap;
const
    green       : colorArr = ( 0.2, 1.0, 0.2 );
    red         : colorArr = ( 1.0, 0.2, 0.2 );
var    
    color       : pcolorArr;
    percent     : float;
    indexes     : pindexesArr;
    entries,
    i, j        : GLint;
begin
    entries     := 256;

    colorIndexes1[0] := 1;
    colorIndexes1[1] := 1 + Trunc((entries - 1) * 0.3);
    colorIndexes1[2] := Trunc((entries - 1) * 0.5);
    colorIndexes2[0] := 1 + Trunc((entries - 1) * 0.5);
    colorIndexes2[1] := 1 + Trunc((entries - 1) * 0.8);
    colorIndexes2[2] := entries - 1;

    for i := 0 to 1 do
    begin
        if i = 0 then
        begin
            color   := @green;
            indexes := @colorIndexes1;
        end
        else
        begin
            color   := @red;
            indexes := @colorIndexes2;
        end;

        for j := indexes[0] to indexes[1] - 1 do
        begin
            percent := 0.2 + 0.8 * (j - indexes[0]) / (indexes[1] - indexes[0]);
            glutSetColor(j, percent*color[0], percent*color[1],
                            percent*color[2]);
        end;

        for j := indexes[1] to indexes[2] do
        begin
            percent := (j - indexes[1]) / (indexes[2] - indexes[1]);
            glutSetColor(j, percent*(1-color[0])+color[0],
                            percent*(1-color[1])+color[1],
                            percent*(1-color[2])+color[2]);
        end;
    end;
end;

procedure   InitMesh;
var
    coord       : pcoord;
    facet       : pfacet;
    dp1, dp2    : coordArr;
    pt1,
    pt2,
    pt3         : pcoordArr;
    angle,
    d, x, y     : float;
    numFacets,
    numCoords,
    frameNum,
    i, j        : GLint;
begin
    theMesh.widthX      := widthX;
    theMesh.widthY      := widthY;
    theMesh.frames      := frames;

    numFacets           := widthX * widthY;
    numCoords           := (widthX + 1) * (widthY + 1);

    theMesh.numCoords   := numCoords;
    theMesh.numFacets   := numFacets;

    GetMem(theMesh.coords,frames*numCoords*sizeof(Wave.coord));
    GetMem(theMesh.facets,frames*numFacets*sizeof(Wave.facet));

    for frameNum := 0 to frames - 1 do
    begin
        for i := 0 to widthX do
        begin
            x := i / widthX;
            for j := 0 to widthY do
            begin
                y := j / widthY;

                d := sqrt(x*x+y*y);
                if d = 0.0 then
                    d := 0.0001;

                angle := 2 * Pi * d + (2 * Pi / frames * frameNum);

                coord := GETCOORD(frameNum, i, j);

                coord.vertex[0] := x - 0.5;
                coord.vertex[1] := y - 0.5;
                coord.vertex[2] := (height - height * d) * cos(angle);

                coord.normal[0] := -(height / d) * x * ((1 - d) * 2 * Pi *
                                    sin(angle) + cos(angle));
                coord.normal[1] := -(height / d) * y * ((1 - d) * 2 * Pi *
                                    sin(angle) + cos(angle));
                coord.normal[2] := -1;

                d := 1.0 / sqrt(coord.normal[0]*coord.normal[0]+
                                coord.normal[1]*coord.normal[1]+1);

                coord.normal[0] := coord.normal[0] * d;
                coord.normal[1] := coord.normal[1] * d;
                coord.normal[2] := coord.normal[2] * d;
            end;
        end;

        for i := 0 to widthX - 1 do
        begin
            for j := 0 to widthY - 1 do
            begin
                facet := GETFACET(frameNum, i, j);
                if ((((i div checkerSize) mod 2) xor (j div checkerSize) mod 2)) <> 0 then
                begin
                    if rgb then
                    begin
                        facet.color[0] := 1.0;
                        facet.color[1] := 0.2;
                        facet.color[2] := 0.2;
                    end
                    else
                    begin
                        facet.color[0] := colorIndexes1[0];
                        facet.color[1] := colorIndexes1[1];
                        facet.color[2] := colorIndexes1[2];
                    end;
                end
                else
                begin
                    if rgb then
                    begin
                        facet.color[0] := 0.2;
                        facet.color[1] := 1.0;
                        facet.color[2] := 0.2;
                    end
                    else
                    begin
                        facet.color[0] := colorIndexes2[0];
                        facet.color[1] := colorIndexes2[1];
                        facet.color[2] := colorIndexes2[2];
                    end;
                end;

                pt1    := @GETCOORD(frameNum, i, j).vertex;
                pt2    := @GETCOORD(frameNum, i, j+1).vertex;
                pt3    := @GETCOORD(frameNum, i+1, j+1).vertex;

                dp1[0] := pt2[0] - pt1[0];
                dp1[1] := pt2[1] - pt1[1];
                dp1[2] := pt2[2] - pt1[2];

                dp2[0] := pt3[0] - pt2[0];
                dp2[1] := pt3[1] - pt2[1];
                dp2[2] := pt3[2] - pt2[2];

                facet.normal[0] := dp1[1] * dp2[2] - dp1[2] * dp2[1];
                facet.normal[1] := dp1[2] * dp2[0] - dp1[0] * dp2[2];
                facet.normal[2] := dp1[0] * dp2[1] - dp1[1] * dp2[0];

                d := 1.0 / sqrt(facet.normal[0]*facet.normal[0]+
                                facet.normal[1]*facet.normal[1]+
                                facet.normal[2]*facet.normal[2]);

                facet.normal[0] := facet.normal[0] * d;
                facet.normal[1] := facet.normal[0] * d;
                facet.normal[2] := facet.normal[0] * d;
            end;
        end;
    end;
end;

procedure   InitMaterials;
const
    ambient             : array[0..3] of float = ( 0.1, 0.1, 0.1, 1.0 );
    diffuse             : array[0..3] of float = ( 0.5, 1.0, 1.0, 1.0 );
    position            : array[0..3] of float = ( 90.0, 90.0, 150.0, 0.0 );
    front_mat_shininess : array[0..0] of float = ( 60.0 );
    front_mat_specular  : array[0..3] of float = ( 0.2, 0.2, 0.2, 1.0 );
    front_mat_diffuse   : array[0..3] of float = ( 0.5, 0.28, 0.38, 1.0 );
    back_mat_shininess  : array[0..0] of float = ( 60.0 );
    back_mat_specular   : array[0..3] of float = ( 0.5, 0.5, 0.2, 1.0 );
    back_mat_diffuse    : array[0..3] of float = ( 1.0, 1.0, 0.2, 1.0 );
    lmodel_ambient      : array[0..3] of float = ( 1.0, 1.0, 1.0, 1.0 );
    lmodel_twoside      : array[0..0] of float = ( Ord(GL_TRUE) );
begin
    glMatrixMode(GL_PROJECTION);
    gluPerspective(90.0, 1.0, 0.5, 10.0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, @ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, @diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, @position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, @lmodel_twoside);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT, GL_SHININESS, @front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, @front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, @front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, @back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, @back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, @back_mat_diffuse);
    if rgb then
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    if rgb then
        glEnable(GL_COLOR_MATERIAL)
    else
        SetColorMap();
end;

procedure   InitTexture;
begin
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
end;

procedure   Init;
begin
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel(GL_FLAT);

    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);

    InitMaterials();
    InitTexture();
    InitMesh();

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0, 0.4, -1.8);
    glScalef(2.0, 2.0, 2.0);
    glRotatef(-35.0, 1.0, 0.0, 0.0);
    glRotatef(35.0, 0.0, 0.0, 1.0);
end;

procedure   Reshape(width, height: int); cdecl;
begin
    glViewport(0, 0, width, height);
end;

procedure   Keyboard(key: uchar; x, y: int); cdecl;
const
    map : array[0..3] of GLfloat = ( 0, 0, 20, 0 );
begin
    case chr(key) of
        'c' : begin
                Inc(contouring);
                if contouring = 1 then
                begin
                    glTexImage2D(GL_TEXTURE_2D, 0, 3, 4, 4, 0, GL_LUMINANCE,
                                 GL_UNSIGNED_BYTE, @contourTexture1);
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, @map);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, @map);
                    glEnable(GL_TEXTURE_2D);
                    glEnable(GL_TEXTURE_GEN_S);
                    glEnable(GL_TEXTURE_GEN_T);
                end
                else if contouring = 2 then
                begin
                    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
                    glPushMatrix();
                        glMatrixMode(GL_MODELVIEW);
                        glLoadIdentity();
                        glTexGenfv(GL_S, GL_EYE_PLANE, @map);
                        glTexGenfv(GL_T, GL_EYE_PLANE, @map);
                    glPopMatrix();
                end
                else
                begin
                    contouring := 0;
                    glDisable(GL_TEXTURE_GEN_S);
                    glDisable(GL_TEXTURE_GEN_T);
                    glDisable(GL_TEXTURE_2D);
                end;
              end;
        's' : begin
                smooth := not smooth;
                if smooth then
                    glShadeModel(GL_SMOOTH)
                else
                    glShadeModel(GL_FLAT);
              end;
        'l' : begin
                lighting := not lighting;
                if lighting then
                begin
                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);
                    if rgb then
                        glEnable(GL_COLOR_MATERIAL);
                end
                else
                begin
                    glDisable(GL_LIGHTING);
                    glDisable(GL_LIGHT0);
                    if rgb then
                        glDisable(GL_COLOR_MATERIAL);
                end;
              end;
        'd' : begin
                depth := not depth;
                if depth then
                begin
                    glEnable(GL_DEPTH_TEST);
                    clearMask := clearMask or GL_DEPTH_BUFFER_BIT;
                end
                else
                begin
                    glDisable(GL_DEPTH_TEST);
                    clearMask := clearMask and not GL_DEPTH_BUFFER_BIT;
                end;
              end;
        ' ' : begin
                stepMode := not stepMode;
                if stepMode then
                begin
                    glutIdleFunc(nil);
                    glutDisplayFunc(Animate);
                end
                else
                begin
                    glutIdleFunc(Animate);
                    glutDisplayFunc(nil);
                end;
              end;
        'n' : if stepMode then
                nextFrame := 1;
        'a' : spinMode := not spinMode;
        #$1B: glutExit();
    else
        Exit;
    end;
end;

procedure   Args;
var
    i: GLint;
begin
    rgb             := GL_TRUE;
    doubleBuffer    := GL_TRUE;
    directRender    := GL_TRUE;
    frames          := 10;
    widthX          := 22;
    widthY          := 22;
    checkerSize     := 2;
    height          := 0.2;

    for i := 1 to ParamCount do
    begin
        if CompareText(ParamStr(i),'-ci') = 0 then
            rgb             := GL_FALSE
        else if CompareText(ParamStr(i),'-rgb') = 0 then
            rgb             := GL_TRUE
        else if CompareText(ParamStr(i),'-sb') = 0 then
            doubleBuffer    := GL_FALSE
        else if CompareText(ParamStr(i),'-db') = 0 then
            doubleBuffer    := GL_TRUE
        else if CompareText(ParamStr(i),'-dr') = 0 then
            directRender    := GL_TRUE
        else if CompareText(ParamStr(i),'-ir') = 0 then
            directRender    := GL_FALSE
        else if CompareText(ParamStr(i),'-grid') = 0 then
        begin
            if (i+2 <= ParamCount) and (ParamStr(i+1)[1] <> '-') and (ParamStr(i+2)[1] <> '-') then
            begin
                widthX := StrToInt(ParamStr(i+1));
                widthY := StrToInt(ParamStr(i+2));
            end
        end
        else if CompareText(ParamStr(i),'-size') = 0 then
        begin
            if (i+1 <= ParamCount) and (ParamStr(i+1)[1] <> '-') then
                checkerSize := StrToInt(ParamStr(i+1));
        end
        else if CompareText(ParamStr(i),'-wave') = 0 then
        begin
            if (i+1 <= ParamCount) and (ParamStr(i+1)[1] <> '-') then
                height := StrToInt(ParamStr(i+1));
        end
        else if CompareText(ParamStr(i),'-frames') = 0 then
        begin
            if (i+1 <= ParamCount) and (ParamStr(i+1)[1] <> '-') then
                frames := StrToInt(ParamStr(i+1));
        end
    end;
end;

procedure   Demo;
var
    _type   : GLenum;
begin
    Args;
    InitGLUT;
    glutInitWindowSize(640,480);

    _type := GLUT_DEPTH;
    if rgb then
        _type := _type or GLUT_RGB
    else
        _type := _type or GLUT_INDEX;

    if doubleBuffer then
        _type := _type or GLUT_DOUBLE
    else
        _type := _type or GLUT_SINGLE;

    glutInitDisplayMode(_type);
    glutCreateWindow('Wave Demo');

    Init();
    glutDisplayFunc(Animate);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(Reshape);
    glutIdleFunc(Animate);
    glutMainLoop();
end;

initialization
    RegisterDemo('Demos','Wave','*** No description yet',Demo);
end.
