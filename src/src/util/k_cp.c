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
* Description:	Program to copy files. This program is similar to the
*				GNU cp program, and it does not do any wildcard expansion.
*				The primary use of this program is to copy files in dmake.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pmapi.h"

/*-------------------------- Implementation -------------------------------*/

#ifdef	__16BIT__
#define	BLOCK_SIZE	(32 * 1024)
#else
#define	BLOCK_SIZE  (512 * 1024)
#endif

/****************************************************************************
PARAMETERS:
sourceName	- Name for source file
destName	- Name for destination file

RETURNS:
True on success, false on failure.

REMARKS:
Copies the source file to the destination file. We copy the file in chunks
for compatibility (32K for 16-bit, 512K for 32-bit).
****************************************************************************/
int copyBinaryFile(
	char *sourceName,
	char *destName)
{
	char    *data;
	FILE	*in,*out;
	size_t	size,blocksize,sizeleft,i,numblocks;

	/* Open the source and destination files */
	if ((in = fopen(sourceName, "rb")) == NULL)
		return false;
	if ((out = fopen(destName, "wb")) == NULL)
		return false;
	if ((data = (char*)malloc(blocksize = BLOCK_SIZE)) == NULL)
		return false;

	/* Get the size of the source file */
	fseek(in, 0, SEEK_END);
	size = ftell(in);
	fseek(in, 0, SEEK_SET);

	/* Now copy the contents of the file 32k at a time */
	if (size > 0) {
		if (size < blocksize)
			blocksize = size;
		numblocks = size / blocksize;
		sizeleft = size;
		for (i = 0; i < numblocks; i++) {
			if (fread(data, 1, blocksize, in) != blocksize)
				return false;
			if (fwrite(data, 1, blocksize, out) != blocksize)
				return false;
			sizeleft -= blocksize;
			}
		if (sizeleft > 0) {
			if (fread(data, 1, sizeleft, in) != sizeleft)
				return false;
			if (fwrite(data, 1, sizeleft, out) != sizeleft)
				return false;
			}
		}

	free(data);
	fclose(in);
	fclose(out);
	return true;
}

ibool copyFile(
	char *source,
	char *dest)
{
	PM_findData	findData;
	void		*hfile;
	char		drive[PM_MAX_DRIVE];
	char		dir[PM_MAX_PATH];
	char		name[PM_MAX_PATH];
	char		ext[PM_MAX_PATH];
	char		destPath[PM_MAX_PATH];

	findData.dwSize = sizeof(findData);
	strcpy(destPath,dest);
	if ((hfile = PM_findFirstFile(dest,&findData)) != PM_FILE_INVALID) {
		if (findData.attrib & PM_FILE_DIRECTORY) {
			PM_backslash(destPath);
			PM_splitpath(source,drive,dir,name,ext);
			PM_makepath(dir,NULL,NULL,name,ext);
			strcat(destPath,dir);
			}
		PM_findClose(hfile);
		}
	if (!copyBinaryFile(source,destPath)) {
		fprintf(stderr,"Unable to copy file '%s' -> '%s'\n", source, dest);
		return false;
		}
	return true;
}

ibool copyFiles(
	char *source,
	char *dest)
{
	PM_findData	findData;
	void		*hfile;
	int 		done,success = true;
	char		drive[PM_MAX_DRIVE];
	char		dir[PM_MAX_PATH];
	char 		name[PM_MAX_PATH];
	char		ext[PM_MAX_PATH];
	char		path[PM_MAX_PATH];

	PM_splitpath(source,drive,dir,name,ext);
	PM_makepath(path,drive,dir,NULL,NULL);
	findData.dwSize = sizeof(findData);
	done = (hfile = PM_findFirstFile(source,&findData)) == PM_FILE_INVALID;
	while (!done) {
		if ((strcmp(findData.name,".") != 0) && (strcmp(findData.name,"..") != 0)) {
			if (!(findData.attrib & PM_FILE_DIRECTORY)) {
				/* Copy the file */
				strcpy(name,path);
				if (strlen(path) > 0)
					PM_backslash(name);
				strcat(name,findData.name);
				if (!copyFile(name,dest)) {
					success = false;
					break;
					}
				}
			}
		done = !PM_findNextFile(hfile,&findData);
		}
	if (hfile != PM_FILE_INVALID)
		PM_findClose(hfile);
	return success;
}

void usage(void)
{
	printf("Usage: k_cp <source> <destination>\n");
	exit(0);
}

int main(int argc,char *argv[])
{
	char		source[PM_MAX_PATH];
	char		dest[PM_MAX_PATH];

	if (argc == 3) {
		strcpy(source,argv[1]);
		strcpy(dest,argv[2]);
		if (dest[strlen(dest)-1] == '\\' || dest[strlen(dest)-1] == '/')
			dest[strlen(dest)-1] = 0;
		if (!copyFiles(source,dest))
			return -1;
		}
	else
		usage();
	return 0;
}
