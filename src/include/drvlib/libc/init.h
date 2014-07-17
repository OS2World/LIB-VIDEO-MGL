/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*					  Portions Copyright P.J. Plauger.
*
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Language:     ANSI C
* Environment:  Any
*
* Description:  Header file for the internal libc runtime library exports.
*
****************************************************************************/

#ifndef __DRVLIB_LIBC_INIT_H
#define __DRVLIB_LIBC_INIT_H

/* Define calling conventions for standard C calling conventions */

#if	defined(__GNUC__) || (defined(__UNIX__) && !defined(__WATCOMC__))
#define	_CDECL
#else
#define	_CDECL		__cdecl
#endif
#if	defined(__IBMC__) || defined(__IBMCPP__)
#define	_CDECLP		* _CDECL
#else
#define	_CDECLP		_CDECL *
#endif
#ifndef	__VARAPI
#ifdef	__WATCOMC__
#if (__WATCOMC__ >= 1050)
#define	_VARAPI		__cdecl
#else
#define	_VARAPI
#endif
#else
#define	_VARAPI
#endif
#endif

#ifndef	__WIN32_VXD__
#include <sys/types.h>
#endif

#if	defined(__BORLANDC__) || defined(__SC__) || defined(_MSC_VER)
typedef	long	off_t;
#endif

/* Structure for all imports to the standard C library */

typedef struct {
	long	dwSize;

	/* <stdlib.h> import functions */
	void    (_CDECLP abort)(void);
	int 	(_CDECLP atexit)(void (*)(void));
	void *  (_CDECLP calloc)(size_t _nelem, size_t _size);
	void    (_CDECLP exit)(int _status);
	void    (_CDECLP free)(void *_ptr);
	char *  (_CDECLP getenv)(const char *_name);
	void *  (_CDECLP malloc)(size_t _size);
	void *  (_CDECLP realloc)(void *_ptr, size_t _size);
	int     (_CDECLP system)(const char *_s);
	int     (_CDECLP putenv)(const char *_val);

	/* <libc/file.h> import functions */
	int     (_CDECLP open)(const char *_path, int _oflag, unsigned _mode);
	int   	(_CDECLP access)(const char *_path, int _amode);
	int     (_CDECLP close)(int _fildes);
	off_t   (_CDECLP lseek)(int _fildes, off_t _offset, int _whence);
	size_t 	(_CDECLP read)(int _fildes, void *_buf, size_t _nbyte);
	int     (_CDECLP unlink)(const char *_path);
	size_t 	(_CDECLP write)(int _fildes, const void *_buf, size_t _nbyte);
	int    	(_CDECLP isatty)(int _fildes);

	/* <stdio.h> import functions */
	int     (_CDECLP remove)(const char *_filename);
	int     (_CDECLP rename)(const char *_old, const char *_new);

	/* <signal.h> import functions */
	int 	(_CDECLP raise)(int);
	void *	(_CDECLP signal)(int, void *);

	/* <time.h> import functions */
	time_t 	(_CDECLP time)(time_t *_tod);

	/* <drvlib/attrib.h> import functions */
	void 	(_CDECLP _OS_setfileattr)(const char *filename,unsigned attrib);
	ulong 	(_CDECLP _OS_getcurrentdate)(void);

	/* Shared memory functions */
	void *	(_CDECLP __malloc_shared)(long size);
	int		(_CDECLP __map_shared)(void *ptr);
	void	(_CDECLP __free_shared)(void *ptr);
	} LIBC_imports;

/* Prototype for the InitLibC entry point in the DLL */

typedef int (_CDECLP InitLibC_t)(LIBC_imports *imports,long os_type);

extern LIBC_imports	_VARAPI ___imports;

#endif /* __DRVLIB_LIBC_INIT_H */
