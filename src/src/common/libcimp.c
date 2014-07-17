/****************************************************************************
*
*                		SciTech MGL Graphics Library
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
* Environment:  Any
*
* Description:	Module to implement a the OS specific side of the Binary
*				Portable DLL C runtime library. The functions in here
*				are imported into the Binary Portable DLL's to implement
*				OS specific services.
*
****************************************************************************/

#include "pmapi.h"
#ifdef	__WIN32_VXD__
#include "drvlib/peloader.h"
#include "drvlib/attrib.h"
#include "drvlib/libc/init.h"
#define	__BUILDING_PE_LOADER__
#include "drvlib/libc/file.h"
#include "vxdfile.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#if	defined(__GNUC__) || defined(__UNIX__)
#include <unistd.h>
#include <sys/stat.h>
#else
#include <io.h>
#endif
#include "drvlib/attrib.h"
#include "drvlib/libc/init.h"
#define	__BUILDING_PE_LOADER__
#include "drvlib/libc/file.h"
#if	defined(__WINDOWS__) || defined(TNT) || defined(__RTTARGET__)
#define	WIN32_LEAN_AND_MEAN
#define	STRICT
#include <windows.h>
#endif
#ifdef	__MSDOS__
#include <dos.h>
#endif
#ifdef	__OS2__
#define INCL_DOS
#define INCL_SUB
#include <os2.h>
#endif
#endif

/* No text or binary modes for Unix */

#ifndef	O_BINARY
#define	O_BINARY	0
#define	O_TEXT		0
#endif

/*--------------------------- Global variables ----------------------------*/

#ifdef	__WIN32_VXD__
#define	MAX_FILES	16
static FILE *openHandles[MAX_FILES] = {NULL};
#endif

/* <stdlib.h> stub functions */
void    _CDECL stub_abort(void);
int		_CDECL stub_atexit(void (*)(void));
void *  _CDECL stub_calloc(size_t _nelem, size_t _size);
void    _CDECL stub_exit(int _status);
void    _CDECL stub_free(void *_ptr);
char *  _CDECL stub_getenv(const char *_name);
void *  _CDECL stub_malloc(size_t _size);
void *  _CDECL stub_realloc(void *_ptr, size_t _size);
int     _CDECL stub_system(const char *_s);
int     _CDECL stub_putenv(const char *_val);

/* <libc/file.h> stub functions */
int     _CDECL stub_open(const char *_path, int _oflag, unsigned _mode);
int   	_CDECL stub_access(const char *_path, int _amode);
int     _CDECL stub_close(int _fildes);
off_t   _CDECL stub_lseek(int _fildes, off_t _offset, int _whence);
size_t 	_CDECL stub_read(int _fildes, void *_buf, size_t _nbyte);
int     _CDECL stub_unlink(const char *_path);
size_t 	_CDECL stub_write(int _fildes, const void *_buf, size_t _nbyte);
int    	_CDECL stub_isatty(int _fildes);

/* <stdio.h> stub functions */
int     _CDECL stub_remove(const char *_filename);
int     _CDECL stub_rename(const char *_old, const char *_new);

/* <time.h> stub functions */
time_t 	_CDECL stub_time(time_t *_tod);

/* <signal.h> stub functions */
int 	_CDECL stub_raise(int);
void *	_CDECL stub_signal(int, void *);

LIBC_imports	_VARAPI ___imports = {
	sizeof(LIBC_imports),

	/* <stdlib.h> exports */
	stub_abort,
	stub_atexit,
	stub_calloc,
	stub_exit,
	stub_free,
	stub_getenv,
	stub_malloc,
	stub_realloc,
	stub_system,
	stub_putenv,

	/* <libc/file.h> exports */
	stub_open,
	stub_access,
	stub_close,
	stub_lseek,
	stub_read,
	stub_unlink,
	stub_write,
	stub_isatty,

	/* <stdio.h> exports */
	stub_remove,
	stub_rename,

	/* <signal.h> functions */
	stub_raise,
	stub_signal,

	/* <time.h> exports */
	stub_time,

	/* <drvlib/attrib.h> exports */
	_OS_setfileattr,
	_OS_getcurrentdate,

	/* Shared memory functions */
	PM_mallocShared,
	PM_mapShared,
	PM_freeShared,
	};

/*---------------------- Stub function implementation ---------------------*/

/* <stdlib.h> stub functions */
void _CDECL stub_abort(void)
{
#ifndef	__WIN32_VXD__
	abort();
#endif
}

int _CDECL stub_atexit(void (*func)(void))
{
#ifndef	__WIN32_VXD__
	return atexit((void(*)(void))func);
#else
	return -1;
#endif
}

void * _CDECL stub_calloc(size_t _nelem, size_t _size)
{ return calloc(_nelem,_size); }

void _CDECL stub_exit(int _status)
{
#ifndef	__WIN32_VXD__
	exit(_status);
#endif
}

void _CDECL stub_free(void *_ptr)
{ free(_ptr); }

char * _CDECL stub_getenv(const char *_name)
{
#ifdef	__WIN32_VXD__
	return NULL;
#else
	return getenv(_name);
#endif
}

void * _CDECL stub_malloc(size_t _size)
{ return malloc(_size); }

void * _CDECL stub_realloc(void *_ptr, size_t _size)
{ return realloc(_ptr,_size); }

int _CDECL stub_system(const char *_s)
{
#if defined(__WINDOWS__) || defined(__WIN32_VXD__) || defined(__SMX32__) || defined(__RTTARGET__)
	(void)_s;
	return -1;
#else
	return system(_s);
#endif
}

int _CDECL stub_putenv(const char *_val)
{
#ifdef	__WIN32_VXD__
	return -1;
#else
	return putenv(_val);
#endif
}

time_t _CDECL stub_time(time_t *_tod)
{
#ifdef	__WIN32_VXD__
	return 0;
#else
	return time(_tod);
#endif
}

#if		defined(__MSDOS__)

#if	defined(TNT) && defined(_MSC_VER)

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{ SetFileAttributes((LPSTR)filename, (DWORD)attrib); }

#else

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{ _dos_setfileattr(filename,attrib); }

#endif

#elif	defined(__WIN32_VXD__)

#define	USE_LOCAL_FILEIO
#define	USE_LOCAL_GETDATE

/* <libc/file.h> stub functions */
int _CDECL stub_open(const char *_path, int _oflag, unsigned _mode)
{
	char	mode[10];
	int		i;

	/* Find an empty file handle to use */
	for (i = 3; i < MAX_FILES; i++) {
		if (!openHandles[i])
			break;
		}
	if (openHandles[i])
		return -1;

	/* Find the open flags to use */
	if (_oflag & ___O_TRUNC)
		strcpy(mode,"w");
	else if (_oflag & ___O_CREAT)
		strcpy(mode,"a");
	else
		strcpy(mode,"r");
	if (_oflag & ___O_BINARY)
		strcat(mode,"b");
	if (_oflag & ___O_TEXT)
		strcat(mode,"t");

	/* Open the file and store the file handle */
	if ((openHandles[i] = fopen(_path,mode)) == NULL)
		return -1;
	return i;
}

int _CDECL stub_access(const char *_path, int _amode)
{ return -1; }

int _CDECL stub_close(int _fildes)
{
	if (_fildes >= 3 && openHandles[_fildes]) {
		fclose(openHandles[_fildes]);
		openHandles[_fildes] = NULL;
		}
	return 0;
}

off_t _CDECL stub_lseek(int _fildes, off_t _offset, int _whence)
{
	if (_fildes >= 3) {
		fseek(openHandles[_fildes],_offset,_whence);
		return ftell(openHandles[_fildes]);
		}
	return 0;
}

size_t _CDECL stub_read(int _fildes, void *_buf, size_t _nbyte)
{
	if (_fildes >= 3)
		return fread(_buf,1,_nbyte,openHandles[_fildes]);
	return 0;
}

int _CDECL stub_unlink(const char *_path)
{
	WORD error;

	if (initComplete) {
		if (R0_DeleteFile((char*)_path,0,&error))
			return 0;
		return -1;
		}
	else
		return i_remove(_path);
}

size_t _CDECL stub_write(int _fildes, const void *_buf, size_t _nbyte)
{
	if (_fildes >= 3)
		return fwrite(_buf,1,_nbyte,openHandles[_fildes]);
	return _nbyte;
}

int _CDECL stub_isatty(int _fildes)
{ return 0; }

/* <stdio.h> stub functions */
int _CDECL stub_remove(const char *_filename)
{ return stub_unlink(_filename); }

int _CDECL stub_rename(const char *_old, const char *_new)
{ return -1; }

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{
	WORD error;
	if (initComplete)
		R0_SetFileAttributes((char*)filename,attrib,&error);
}

/* Return the current date in days since 1/1/1980 */
ulong _CDECL _OS_getcurrentdate(void)
{
	RMREGS		regs;
	short		i,year,month,day,days;
	static char Days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	regs.h.ah = 0x2A;
	PM_int86(0x21,&regs,&regs);
	year = regs.x.cx;
	month = regs.h.dh;
	day = regs.h.dl;
	days = (year - 1980) * 365;
	i = month - 1;
	while (i > 0) {
		i--;
		days += Days[i];
		}
	days += day - 1;
	return days;
}

#elif	defined(__WINDOWS32__) || defined(__RTTARGET__)

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{ SetFileAttributes((LPSTR)filename, (DWORD)attrib); }

#elif	defined(__OS2__)

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{
	FILESTATUS3 s;
	if (DosQueryPathInfo((PSZ)filename,FIL_STANDARD,(PVOID)&s,sizeof(s)))
		return;
	s.attrFile = attrib;
	DosSetPathInfo((PSZ)filename,FIL_STANDARD,(PVOID)&s,sizeof(s),0L);
}

#else

void _CDECL _OS_setfileattr(const char *filename,unsigned attrib)
{ /* Unable to set hidden, system attributes on Unix. */ }

#endif

#ifndef	USE_LOCAL_FILEIO

/* <libc/file.h> stub functions */
int _CDECL stub_open(const char *_path, int _oflag, unsigned _mode)
{
	int oflag_tab[] = {
		___O_RDONLY,	O_RDONLY,
		___O_WRONLY,	O_WRONLY,
		___O_RDWR,		O_RDWR,
		___O_BINARY,	O_BINARY,
		___O_TEXT,		O_TEXT,
		___O_CREAT,		O_CREAT,
		___O_EXCL,		O_EXCL,
		___O_TRUNC,		O_TRUNC,
		___O_APPEND,	O_APPEND,
		};
	int i,oflag = 0;

	/* Translate the oflag's to the OS dependent versions */
	for (i = 0; i < sizeof(oflag_tab) / sizeof(int); i += 2) {
		if (_oflag & oflag_tab[i])
			oflag |= oflag_tab[i+1];
		}
	return open(_path,oflag,_mode);
}

int _CDECL stub_access(const char *_path, int _amode)
{ return access(_path,_amode); }

int _CDECL stub_close(int _fildes)
{ return close(_fildes); }

off_t _CDECL stub_lseek(int _fildes, off_t _offset, int _whence)
{ return lseek(_fildes,_offset,_whence); }

size_t _CDECL stub_read(int _fildes, void *_buf, size_t _nbyte)
{ return read(_fildes,_buf,_nbyte); }

int _CDECL stub_unlink(const char *_path)
{ return unlink(_path); }

size_t _CDECL stub_write(int _fildes, const void *_buf, size_t _nbyte)
{ return write(_fildes,_buf,_nbyte); }

int _CDECL stub_isatty(int _fildes)
{ return isatty(_fildes); }

/* <stdio.h> stub functions */
int _CDECL stub_remove(const char *_filename)
{ return remove(_filename); }

int _CDECL stub_rename(const char *_old, const char *_new)
{ return rename(_old,_new); }

#endif

#ifndef	USE_LOCAL_GETDATE

/* Return the current date in days since 1/1/1980 */
ulong _CDECL _OS_getcurrentdate(void)
{
	time_t 		today;
	struct tm 	*t;
	short		i,year,month,day,days;
	static char Days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	today = time(NULL);
	t = localtime(&today);
	year = t->tm_year + 1900;
	month = t->tm_mon + 1;
	day = t->tm_mday;
	days = (year - 1980) * 365;
	i = month - 1;
	while (i > 0) {
		i--;
		days += Days[i];
		}
	days += day - 1;
	return days;
}

#endif

int _CDECL stub_raise(int sig)
{
#if defined(__WIN32_VXD__) || defined(__SMX32__)
	return -1;
#else
	return raise(sig); 
#endif
}

typedef void (*__code_ptr)();

void * _CDECL stub_signal(int sig, void *handler)
{
#if defined(__WIN32_VXD__) || defined(__SMX32__)
	return NULL;
#else
	return (void*)signal(sig,(__code_ptr)handler);
#endif
}
