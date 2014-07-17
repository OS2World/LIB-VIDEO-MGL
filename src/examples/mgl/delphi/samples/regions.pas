// Demo showing how to use the MGL region functions to create
// simple regions
unit Regions;

interface

implementation

uses
    MGLFx,
    MGLTypes,
    MGLSamp;

{-- Constants ----------------------------------------------------------------}

const
    R_UNION         = 0;
    R_DIFFERENCE    = 1;
    R_INTERSECTION  = 2;

{-- Variables ----------------------------------------------------------------}

var
    maxx,maxy: int;

{-- RegionDemo ---------------------------------------------------------------}

// Displays two circles and shows there intersection,
// difference and union using region function from the MGL.

function    RegionDemo(dc: PMGLDC; arithmetic: int): Boolean;
var
    region1 : pregion_t;
    region2 : pregion_t;
    regionh : pregion_t;
    rect    : rect_t;
    radius  : int;
begin
    if maxx <= maxy then
        radius := maxx div 2
    else
        radius := maxy div 2;

    // Display title message at the top of the window.
    case arithmetic of
        R_INTERSECTION : MainWindow(dc,'Intersecting Region Demonstration');
        R_DIFFERENCE   : MainWindow(dc,'Difference Region Demonstration');
        R_UNION        : MainWindow(dc,'Union Region Demonstration');
    end;

    StatusLine('Press any key to continue...');

    // create first region
    with rect do
    begin
        left    := Trunc(maxx/2 - (radius*1.5)/2);
        top     := Trunc(radius/4-7);
        right   := rect.left+radius;
        bottom  := rect.top+radius-7;
    end;

    region1 := MGL_rgnSolidEllipse(rect);
    try
        if MGL_getBitsPerPixel(dc) <= 8 then
            MGL_setColor(1)
        else
            MGL_setColorRGB(0,0,255);
        MGL_drawRegion(0,0,region1);

        // create second region
        with rect do
        begin
            Inc(left,radius div 2);
            Inc(top,radius div 2);
            Inc(right,radius div 2);
            Inc(bottom,radius div 2);
        end;

        region2 := MGL_rgnSolidEllipse(rect);
        try
            if MGL_getBitsPerPixel(dc) <= 8 then
                MGL_setColor(2)
            else
                MGL_setColorRGB(0,255,0);

            MGL_drawRegion(0,0,region2);

            if MGL_getBitsPerPixel(dc) <= 8 then
                MGL_setColor(7)
            else
                MGL_setColorRGB(170,170,170);

            case arithmetic of
                R_INTERSECTION:
                begin
                    // determine if the two regions have intersected
                    regionh := MGL_sectRegion(region1,region2);
                    try
                        if not MGL_emptyRegion(regionh) then
                            MGL_drawRegion(0,0,regionh);
                    finally
                        MGL_freeRegion(regionh);
                    end;
                end;
                R_DIFFERENCE:
                    if MGL_diffRegion(region1,region2) then
                        MGL_drawRegion(0,0,region1);
                R_UNION:
                    if MGL_unionRegion(region1,region2) then
                        MGL_drawRegion(0,0,region1);
            end;
        finally
            MGL_freeRegion(region2);
        end;
    finally
        MGL_freeRegion(region1);
    end;

    Result := Pause;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    maxx := MGL_maxx;
    maxy := MGL_maxy;

    if not RegionDemo(dc,R_INTERSECTION) then
        Exit;
    if not RegionDemo(dc,R_DIFFERENCE) then
        Exit;
    if not RegionDemo(dc,R_UNION) then
        Exit;
end;

initialization
    RegisterDemo('Regions',
                 'Demo showing how to use the MGL region functions to create '+
                 'simple regions',
                 Demo);
end.
