#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <stdio.h>

#include <gl/gl.h>

HDC   hDC;
HGLRC hGLRC;
int   bpp;

BOOL  WhichGLInit( void );
BOOL  WhichGLCreateGL( void );
void  WhichGLShutdown( void );

int __cdecl main( void )
{
    if ( WhichGLInit() )
    {
        const char *vendor   = glGetString( GL_VENDOR );
        const char *renderer = glGetString( GL_RENDERER );
        const char *version  = glGetString( GL_VERSION );

        printf( "WhichGL *  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
        printf( "Renderer:        %s\n", renderer );
        printf( "Version:         %s\n", version );

        WhichGLShutdown();
    }
    return 1;
}

BOOL WhichGLInit( void )
{
    bpp = GetDeviceCaps( hDC , BITSPIXEL );
    hDC = GetDC( 0 );

    return WhichGLCreateGL();
}

BOOL WhichGLCreateGL( void )
{    
    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof( PIXELFORMATDESCRIPTOR ),
            1,
            PFD_DRAW_TO_WINDOW |	PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            16,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16,                 // 16-bit depth buffer
            0,                  // no stencil buffer
            0,                  // no aux buffers
            PFD_MAIN_PLANE,			/* main layer */
            0,	
            0, 0, 0
    };
    int  selected_pf;
    
    if ( ( selected_pf = ChoosePixelFormat( hDC, &pfd ) ) == 0 )
    {
        MessageBox( 0, "Failed to find acceptable pixel format", "WhichGL Error", MB_ICONERROR | MB_OK );
        return FALSE;
    }
    
    if ( !SetPixelFormat( hDC, selected_pf, &pfd) )
    {
        MessageBox( 0, "Failed to SetPixelFormat", "WhichGL Error", MB_ICONERROR | MB_OK );
        return FALSE;
    }

    if ( ( hGLRC = wglCreateContext( hDC ) ) == 0 )
    {
        MessageBox( 0, "Failed to create context", "WhichGL Error", MB_ICONERROR | MB_OK );
        return FALSE;
    }
    if ( !wglMakeCurrent( hDC, hGLRC ) )
    {
        wglDeleteContext( hGLRC );
        MessageBox( 0, "Failed to make context current", "WhichGL Error", MB_ICONERROR | MB_OK );
        return FALSE;
    }

    return TRUE;
}

void WhichGLShutdown( void )
{
    if ( hGLRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hGLRC );
    }
   ReleaseDC( 0, hDC );
}

