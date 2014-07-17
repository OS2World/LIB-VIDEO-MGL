// Simple demo showing how to load a bitmap file from disk and
// display it on the screen.
unit Mouse;

interface

implementation

uses
    MGLSamp,
    MGLFx,
    SysUtils;

{-- MouseCursorDemo ----------------------------------------------------------}

// Display the mouse cursor on the screen, and change it to
// a number of different styles.

function    MouseCursorDemo(dc: PMGLDC): Boolean;
var
    cursor  : pcursor_t;
begin
    // First check to ensure that a mouse is actually installed in the
    // system. MS_init() will return 0 if no mouse is installed, or the
    // number of buttons if it is.
    if not MS_available then
        MGLError;

    // turn on the mouse
    MS_show;

    StatusLine('Press any key for Hour Glass cursor');
    WaitEvent;

    cursor := MGL_loadCursor('wait.cur');
    if cursor = nil then
        raise Exception.Create('Unable to load WAIT.CUR cursor file');
    try
        MS_setCursor(cursor);

        StatusLine('Press any key for IBEAM cursor');
        WaitEvent;

    finally
        MS_setCursor(MGL_DEF_CURSOR);
        MGL_unloadCursor(cursor);
    end;

    cursor := MGL_loadCursor('ibeam.cur');
    if cursor = nil then
        raise Exception.Create('Unable to load IBEAM.CUR cursor file');

    try
        MS_setCursor(cursor);
        WaitEvent;
    finally
        MS_setCursor(MGL_DEF_CURSOR);
        MGL_unloadCursor(cursor);
    end;

    if not Pause then
    begin
        Result := False;
        Exit;
    end;

    MS_hide;

    Result := True;
end;

{-- Demo ---------------------------------------------------------------------}

procedure   Demo(dc: PMGLDC);
begin
    // Display title message at the top of the window.
    MainWindow(dc,'Mouse Cursor Demonstration');

    MGL_setColorCI(MGL_DARKGRAY);
    MGL_setBackColor(MGL_getColor);
    MGL_clearViewport;
    MouseCursorDemo(dc);
end;

initialization
    RegisterDemo('Mouse','? Description should be corrected',Demo);
end.
