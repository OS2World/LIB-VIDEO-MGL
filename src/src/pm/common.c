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
* Environment:	Any
*
* Description:	Module containing code common to all platforms.
*
****************************************************************************/

#include "pmapi.h"
#include "drvlib/os/os.h"
#ifdef __WIN32_VXD__
#include "sdd/sddhelp.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

/*---------------------------- Global variables ---------------------------*/

/* {secret} */
long _VARAPI ___drv_os_type = _OS_UNSUPPORTED;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
DESCRIPTION:
Sets the location of the debug log file.

HEADER:
pmapi.h

PARAMETERS:
dllname	- Name of the Binary Portable DLL to load
bpdpath	- Place to store the actual path to the file

RETURNS:
True if found, false if not.

REMARKS:
Finds the location of a specific Binary Portable DLL, by searching all
the standard SciTech Nucleus driver locations.
****************************************************************************/
ibool PMAPI PM_findBPD(
	const char *dllname,
	char *bpdpath)
{
	char	filename[256];
	FILE	*f;
#ifndef	__WIN32_VXD__
	char	*mgl_root = getenv("MGL_ROOT");
#endif

	PM_init();
	strcpy(bpdpath,PM_getNucleusPath());
	PM_backslash(bpdpath);
	strcpy(filename,bpdpath);
	strcat(filename,dllname);
	if ((f = fopen(filename,"rb")) != NULL)
		goto Found;

	/* Next try in the MGL drivers directory relative to application */
	PM_getCurrentPath(bpdpath,PM_MAX_PATH);
	PM_backslash(bpdpath);
	strcat(bpdpath,"drivers");
	PM_backslash(bpdpath);
	strcpy(filename,bpdpath);
	strcat(filename,dllname);
	if ((f = fopen(filename,"rb")) != NULL)
		goto Found;

#ifndef	__WIN32_VXD__
	/* Next try in the directory relative to the MGL_ROOT
	 * environment variable.
	 */
	if (mgl_root) {
		strcpy(bpdpath, mgl_root);
		PM_backslash(bpdpath);
		strcat(bpdpath,"drivers");
		PM_backslash(bpdpath);
		strcpy(filename,bpdpath);
		strcat(filename,dllname);
		if ((f = fopen(filename,"rb")) != NULL)
			goto Found;
		}
#endif

	/* Whoops, couldn't open the BPD file! */
	return false;

Found:
	fclose(f);
	return true;
}

/****************************************************************************
REMARKS:
Copies a string into another, and returns dest + strlen(src).
****************************************************************************/
static char *_stpcpy(
	char *_dest,
	const char *_src)
{
	if (!_dest || !_src)
		return 0;
	while ((*_dest++ = *_src++) != 0)
		;
	return --_dest;
}

/****************************************************************************
REMARKS:
Copies a string into another, stopping at the maximum length. The string
is properly terminated (unlike strncpy).
****************************************************************************/
static void safe_strncpy(
	char *dst,
	const char *src,
	unsigned maxlen)
{
	if (dst) {
		if(strlen(src) >= maxlen) {
			strncpy(dst, src, maxlen);
			dst[maxlen] = 0;
			}
		else
			strcpy(dst, src);
		}
}

/****************************************************************************
REMARKS:
Determins if the dot separator is present in the string.
****************************************************************************/
static int findDot(
	char *p)
{
	if (*(p-1) == '.')
		p--;
	switch (*--p) {
		case ':':
			if (*(p-2) != '\0')
				break;
		case '/':
		case '\\':
		case '\0':
			return true;
		}
	return false;
}

/****************************************************************************
DESCRIPTION:
Make a full pathname from split components.

HEADER:
pmapi.h

PARAMETERS:
path	- Place to store full path
drive	- Drive component for path
dir		- Directory component for path
name	- Filename component for path
ext		- Extension component for path

REMARKS:
Function to make a full pathname from split components. Under Unix the
drive component will usually be empty. If the drive, dir, name, or ext
parameters are null or empty, they are not inserted in the path string.
Otherwise, if the drive doesn't end with a colon, one is inserted in the
path. If the dir doesn't end in a slash, one is inserted in the path.
If the ext doesn't start with a dot, one is inserted in the path.

The maximum sizes for the path string is given by the constant PM_MAX_PATH,
which includes space for the null-terminator.

SEE ALSO:
PM_splitPath
****************************************************************************/
void PMAPI PM_makepath(
	char *path,
	const char *drive,
	const char *dir,
	const char *name,
	const char *ext)
{
	if (drive && *drive) {
		*path++ = *drive;
		*path++ = ':';
		}
	if (dir && *dir) {
		path = _stpcpy(path,dir);
		if (*(path-1) != '\\' && *(path-1) != '/')
#ifdef	__UNIX__
			*path++ = '/';
#else
			*path++ = '\\';
#endif
		}
	if (name)
		path = _stpcpy(path,name);
	if (ext && *ext) {
		if (*ext != '.')
			*path++ = '.';
		path = _stpcpy(path,ext);
		}
	*path = 0;
}

/****************************************************************************
DESCRIPTION:
Split a full pathname into components.

HEADER:
pmapi.h

PARAMETERS:
path	- Full path to split
drive	- Drive component for path
dir		- Directory component for path
name	- Filename component for path
ext		- Extension component for path

RETURNS:
Flags indicating what components were parsed.

REMARKS:
Function to split a full pathmame into separate components in the form

	X:\DIR\SUBDIR\NAME.EXT

and splits path into its four components. It then stores those components
in the strings pointed to by drive, dir, name and ext. (Each component is
required but can be a NULL, which means the corresponding component will be
parsed but not stored).

The maximum sizes for these strings are given by the constants PM_MAX_DRIVE
and PM_MAX_PATH. PM_MAX_DRIVE is always 4, and PM_MAX_PATH is usually at
least 256 characters. Under Unix the dir, name and ext components may be
up to the full path in length.

SEE ALSO:
PM_makePath
****************************************************************************/
int PMAPI PM_splitpath(
	const char *path,
	char *drive,
	char *dir,
	char *name,
	char *ext)
{
	char	*p;
	int    	temp,ret;
	char 	buf[PM_MAX_PATH+2];

	/* Set all string to default value zero */
	ret = 0;
	if (drive) 	*drive = 0;
	if (dir) 	*dir = 0;
	if (name) 	*name = 0;
	if (ext)	*ext = 0;

	/* Copy filename into template up to PM_MAX_PATH characters */
	p = buf;
	if ((temp = strlen(path)) > PM_MAX_PATH)
		temp = PM_MAX_PATH;
	*p++ = 0;
	strncpy(p, path, temp);
	*(p += temp) = 0;

	/* Split the filename and fill corresponding nonzero pointers */
	temp = 0;
	for (;;) {
		switch (*--p) {
			case '.':
				if (!temp && (*(p+1) == '\0'))
					temp = findDot(p);
				if ((!temp) && ((ret & PM_HAS_EXTENSION) == 0)) {
					ret |= PM_HAS_EXTENSION;
					safe_strncpy(ext, p, PM_MAX_PATH - 1);
					*p = 0;
					}
				continue;
			case ':':
				if (p != &buf[2])
					continue;
			case '\0':
				if (temp) {
					if (*++p)
						ret |= PM_HAS_DIRECTORY;
					safe_strncpy(dir, p, PM_MAX_PATH - 1);
					*p-- = 0;
					break;
					}
			case '/':
			case '\\':
				if (!temp) {
					temp++;
					if (*++p)
						ret |= PM_HAS_FILENAME;
					safe_strncpy(name, p, PM_MAX_PATH - 1);
					*p-- = 0;
					if (*p == 0 || (*p == ':' && p == &buf[2]))
						break;
					}
				continue;
			case '*':
			case '?':
				if (!temp)
					ret |= PM_HAS_WILDCARDS;
			default:
				continue;
			}
		break;
		}
	if (*p == ':') {
		if (buf[1])
			ret |= PM_HAS_DRIVE;
		safe_strncpy(drive, &buf[1], PM_MAX_DRIVE - 1);
		}
	return ret;
}
