/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
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
* Description:	Program to build .FON font libraries using the Borland
*				Resource Compiler.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <process.h>

/*------------------------- Implementation --------------------------------*/

int main(int argc,char *argv[])
{
	struct find_t	blk;
	char			basename[_MAX_PATH];
	char			suffix[_MAX_PATH] = "";
	char			findname[_MAX_PATH];
	char			findsuffix[_MAX_PATH];
	char			command[_MAX_PATH];
	int				id,result,suffixlen;
	FILE			*f;

	if (argc != 2 && argc != 3)
		return -1;
	strcpy(basename,argv[1]);
	if (argc > 2)
		strcpy(suffix,argv[2]);
	strcpy(findname,basename);
	strcat(findname,"*");
	strcat(findname,suffix);
	strcat(findname,".fnt");
	strcpy(findsuffix,suffix);
	strcat(findsuffix,".fnt");
	suffixlen = strlen(findsuffix);
	if ((f = fopen("font.rc","w")) == NULL)
		return -1;
	id = 1;
	result = _dos_findfirst(findname,_A_NORMAL,&blk);
	while (result == 0) {
		if (!(blk.attrib & _A_SUBDIR) &&
				strlen(blk.name) == strlen(findname)+1 &&
				stricmp(&blk.name[strlen(blk.name)-suffixlen],findsuffix) == 0) {
			fprintf(f,"%d FONT LOADONCALL MOVEABLE DISCARDABLE %s\n", id,blk.name);
			id++;
			}
		result = _dos_findnext(&blk);
		}
	fclose(f);

	sprintf(command,"cp empty.dll %s%s.fon", basename, suffix);
	system(command);

	sprintf(command,"c:\\c\\bc50\\bin\\brc -fe%s%s.fon font.rc", basename, suffix);
	system(command);

	unlink("font.rc");
	return 0;
}
