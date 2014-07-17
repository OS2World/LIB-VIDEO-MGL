/****************************************************************************
*
*					SciTech OS Portability Manager Library
*
*  ========================================================================
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
*
* Language:		ANSI C
* Environment:	32-bit Windows VxD
*
* Description:  C library compatible I/O functions for use within a VxD.
*
****************************************************************************/

#ifndef	__VXDFILE_H
#define	__VXDFILE_H

/*---------------------- Macros and type definitions ----------------------*/

typedef	struct {
	int		handle;
	int		writemode;
	int		text;
	int		unputc;
	long	offset;
	long	filesize;
	char	buf[2048];
	char	*startp;
	char	*curp;
	char	*endp;
	} FILE;

/*---------------------------- Global variables ---------------------------*/

/* Your VxD must declare this external variable and set the value after
 * the OnInitComplete message is handled.
 */

extern ibool	initComplete;

/* This variable should be declared and set to NULL unless you have hooked
 * the Int 10h handler in your VxD.
 */

extern ushort	*disableTSRFlag;

/*------------------------- Function Prototypes ---------------------------*/

FILE *	fopen( const char *__filename, const char *__mode );
size_t	fread( void *__ptr, size_t __size, size_t __n, FILE *__fp );
size_t	fwrite( const void *__ptr, size_t __size, size_t __n, FILE *__fp );
int		fflush( FILE *__fp );
int		fseek( FILE *__fp, long int __offset, int __whence );
long	ftell( FILE *__fp );
int		feof( FILE *__fp );
int		fclose( FILE *__fp );
char *	fgets( char *__s, int __n, FILE *__fp );
int		fputs( const char *__s, FILE *__fp );

#endif  /* __VXDFILE_H */
