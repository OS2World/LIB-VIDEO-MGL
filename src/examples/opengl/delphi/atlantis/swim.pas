{=============================================================================}
{= (c) Copyright 1993, 1994, Silicon Graphics, Inc.                          =}
{= ALL RIGHTS RESERVED                                                       =}
{= Permission to use, copy, modify, and distribute this software for         =}
{= any purpose and without fee is hereby granted, provided that the above    =}
{= copyright notice appear in all copies and that both the copyright notice  =}
{= and this permission notice appear in supporting documentation, and that   =}
{= the name of Silicon Graphics, Inc. not be used in advertising             =}
{= or publicity pertaining to distribution of the software without specific, =}
{= written prior permission.                                                 =}
{=                                                                           =}
{= THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"         =}
{= AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,          =}
{= INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR          =}
{= FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON              =}
{= GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,           =}
{= SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY             =}
{= KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,            =}
{= LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF         =}
{= THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN            =}
{= ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON            =}
{= ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE         =}
{= POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.                          =}
{=                                                                           =}
{= US Government Users Restricted Rights                                     =}
{= Use, duplication, or disclosure by the Government is subject to           =}
{= restrictions set forth in FAR 52.227.19(c)(2) or subparagraph             =}
{= (c)(1)(ii) of the Rights in Technical Data and Computer Software          =}
{= clause at DFARS 252.227-7013 and/or in similar or successor               =}
{= clauses in the FAR or the DOD or NASA FAR Supplement.                     =}
{= Unpublished-- rights reserved under the copyright laws of the             =}
{= United States.  Contractor/manufacturer is Silicon Graphics,              =}
{= Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.             =}
{=                                                                           =}
{= OpenGL(TM) is a trademark of Silicon Graphics, Inc.                       =}
{=============================================================================}
unit Swim;

interface

uses
    MGLTypes,
    Common;

procedure   FishTransform(fish: pfishRec);
procedure   WhalePilot(fish: pfishRec);
procedure   SharkPilot(fish: pfishRec);
procedure   SharkMiss(i: int);

implementation

uses
    MGLGl,
    Math;

procedure   FishTransform(fish: pfishRec);
begin
    with fish^ do
    begin
        glTranslatef(y, z, -x);
        glRotatef(-psi, 0.0, 1.0, 0.0);
        glRotatef(theta, 1.0, 0.0, 0.0);
        glRotatef(-phi, 0.0, 0.0, 1.0);
    end;
end;

procedure   WhalePilot(fish: pfishRec);
begin
    with fish^ do
    begin
        phi     := -20.0;
        theta   := 0.0;
        psi     := psi - 0.5;
        x       := x + (WHALESPEED * v * cos(psi / RAD) * cos(theta / RAD));
        y       := y + (WHALESPEED * v * sin(psi / RAD) * cos(theta / RAD));
        z       := z + (WHALESPEED * v * sin(theta / RAD));
    end;
end;

var
    sign : int = 1;
    
procedure   SharkPilot(fish: pfishRec);
var
    X, Y, Z, tpsi, ttheta, thetal : float;
begin
    with fish^ do
    begin
        xt := 60000.0;
        yt := 0.0;
        zt := 0.0;
    end;

    X := fish.xt - fish.x;
    Y := fish.yt - fish.y;
    Z := fish.zt - fish.z;

    thetal := fish.theta;

    ttheta := RAD * ArcTan(Z / (sqrt(X * X + Y * Y)));

    if ttheta > fish.theta + 0.25 then
        fish.theta := fish.theta + 0.5
    else if ttheta < fish.theta - 0.25 then
        fish.theta := fish.theta - 0.5
    else if fish.theta > 90.0 then
        fish.theta := 90.0;

    if fish.theta < -90.0 then
        fish.theta := -90.0;

    fish.dtheta := fish.theta - thetal;

    tpsi := RAD * ArcTan2(Y, X);

    fish.attack := 0;

    if Abs(tpsi - fish.psi) < 10.0 then
        fish.attack := 1
    else if Abs(tpsi - fish.psi) < 45.0 then
    begin
        if fish.psi > tpsi then
        begin
            fish.psi := fish.psi - 0.5;
            if fish.psi < -180.0 then
                fish.psi := fish.psi + 360.0;
        end
        else if fish.psi < tpsi then
        begin
            fish.psi := fish.psi + 0.5;
            if fish.psi > 180.0 then
                fish.psi := fish.psi - 360.0;
        end
    end
    else
    begin
        if Random(100) > 98 then
            sign := 1 - sign;
        fish.psi := fish.psi + sign;
        if fish.psi > 180.0 then
            fish.psi := fish.psi - 360.0;
        if fish.psi < -180.0 then
            fish.psi := fish.psi + 360.0;
    end;

    if fish.attack <> 0 then
    begin
        if fish.v < 1.1 then
            fish.spurt := 1;
        if fish.spurt <> 0 then
            fish.v := fish.v + 0.2;
        if fish.v > 5.0 then
            fish.spurt := 0;
        if (fish.v > 1.0) and (fish.spurt = 0) then
            fish.v := fish.v - 0.2;
    end
    else
    begin
        if (Random(400) = 0) and (fish.spurt = 0) then
            fish.spurt := 1;
        if fish.spurt <> 0 then
            fish.v := fish.v + 0.05;
        if fish.v > 3.0 then
            fish.spurt := 0;
        if (fish.v > 1.0) and (fish.spurt = 0) then
            fish.v := fish.v - 0.05;
    end;

    fish.x := fish.x + (SHARKSPEED * fish.v * cos(fish.psi / RAD) * cos(fish.theta / RAD));
    fish.y := fish.y + SHARKSPEED * fish.v * sin(fish.psi / RAD) * cos(fish.theta / RAD);
    fish.z := fish.z + SHARKSPEED * fish.v * sin(fish.theta / RAD);
end;

procedure   SharkMiss(i: int);
var
    j               : int;
    avoid, thetal   : float;
    X, Y, Z, R      : float;
begin
    for j := 0 to NUM_SHARKS - 1 do
    begin
        if j <> i then
        begin
            X := sharks[j].x - sharks[i].x;
            Y := sharks[j].y - sharks[i].y;
            Z := sharks[j].z - sharks[i].z;

            R := sqrt(X * X + Y * Y + Z * Z);

            avoid   := 1.0;
            thetal  := sharks[i].theta;

            if R < SHARKSIZE then
                if Z > 0.0 then
                    sharks[i].theta := sharks[i].theta - avoid
                else
                    sharks[i].theta := sharks[i].theta + avoid;
                    
            sharks[i].dtheta := sharks[i].dtheta + (sharks[i].theta - thetal);
        end;
    end;
end;

end.
