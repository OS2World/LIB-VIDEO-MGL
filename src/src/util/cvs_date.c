/****************************************************************************
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
* Environment:	any
*
* Description:	Program to change the date and time stamps on all
*				files in a CVS directory to those contained within the
*				CVS repository files on the local system. This is used
*				to reset the date and time stamps on local CVS files
*				after they are checked into Perforce so that CVS does
*				not think the files have been modified.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "pmapi.h"

#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>

/*------------------------- Global variables ------------------------------*/

const char *months[] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec",NULL,
	};

/*-------------------------- Implementation -------------------------------*/

int lookupString(
	const char *table[],
	const char *str)
{
	int i;

	for (i = 1; *table != NULL; table++, i++) {
		if (strcmp(*table,str) == 0)
			return i;
		}
	return i;
}

ibool parseTime(
	const char *buf,
	ushort *date,
	ushort *time)
{
	char 	*p;

	if (strlen(buf) != 24)
		return false;
	*date = *time = 0;
	p = buf + 4;
	p[3] = 0;
	*date |= lookupString(months,p) << 5;
	p = p + 4;
	p[2] = 0;
	*date |= atoi(p);
	p = p + 3;
	p[2] = 0;
	*time |= atoi(p) << 11;
	p = p + 3;
	p[2] = 0;
	*time |= atoi(p) << 5;
	p = p + 3;
	p[2] = 0;
	*time |= atoi(p)/2;
	p = p + 3;
	*date |= (atoi(p) - 1980) << 9;
	return true;
}

extern unsigned _dos_setftime(int __handle,unsigned short __date,unsigned short __time);

void recurseResetDate(
	const char *path,
	const char *fileMask)
{
	PM_findData	findData;
	void		*hfile;
	int 		done,handle;
	char 		files[PM_MAX_PATH];
	char 		name[PM_MAX_PATH];
	char 		*p,*q,buf[PM_MAX_PATH];
	FILE		*f;
	ushort		date,time;
	unsigned	attrib;

	// Open CVS/Entries file and process all files contained within
	strcpy(files,path);
	if (strlen(path) > 0)
		PM_backslash(files);
	strcat(files,"CVS/Entries");
	if ((f = fopen(files,"r")) != NULL) {
		while (!feof(f) && fgets(buf,sizeof(buf),f)) {
			p = buf;
			q = name;
			if (*p++ != '/')
				continue;
			while (*p && *p != '\n' && *p != '/')
				*q++ = *p++;
			*q = 0;
			if (*p++ != '/')
				continue;
			while (*p && *p != '\n' && *p != '/')
				p++;
			if (*p++ != '/')
				continue;
			q = p;
			while (*p && *p != '\n' && *p != '/')
				p++;
			*p = 0;
			if (!parseTime(q,&date,&time))
				continue;
			strcpy(files,path);
			if (strlen(path) > 0)
				PM_backslash(files);
			strcat(files,name);
#ifdef	DEBUG
			printf("Changing date for '%s'\n", files);
#endif

// TODO: This needs to be made portable!!
			_dos_getfileattr(files,&attrib);
			_dos_setfileattr(files,0);
			if ((handle = open(files,O_RDWR | O_BINARY)) == -1) {
				sprintf(buf,"Unable to open '%s' %d!", files, errno);
				PM_fatalError(buf);
				}
			_dos_setftime(handle,date,time);
			close(handle);
			_dos_setfileattr(files,attrib);
			}
		fclose(f);
		}

	// Recurse all subdirectories
	findData.dwSize = sizeof(findData);
	strcpy(files,path);
	if (strlen(path) > 0)
		PM_backslash(files);
#ifdef	__UNIX__
	strcat(files,"*");
#else
	strcat(files,"*.*");
#endif
	done = (hfile = PM_findFirstFile(files,&findData)) == PM_FILE_INVALID;
	while (!done) {
		if ((strcmp(findData.name,".") != 0) && (strcmp(findData.name,"..") != 0)) {
			if (findData.attrib & PM_FILE_DIRECTORY) {
				strcpy(name,path);
				if (strlen(path) > 0)
					PM_backslash(name);
				strcat(name,findData.name);
				recurseResetDate(name,fileMask);
				}
			}
		done = !PM_findNextFile(hfile,&findData);
		}
}

int main(int argc,char *argv[])
{
	chdir("c:\\cvs\\mesa");

	printf("Resetting date and time to CVS settings ... ");
	fflush(stdout);
	recurseResetDate("","*");
	printf("Done\n");
	fflush(stdout);

	chdir("c:\\scitech\\src\\util");
	return 0;
}
