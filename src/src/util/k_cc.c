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
* Description:	Simple program for Unix systems to spawn a C or C++
*				compiler with arguments passed in a response file. We do
*				this so we can keep the build command lines clean under
*				Unix systems using dmake, so that we can more easily see
*				when errors show up.
*
*				If you need to see the command line being used, run dmake
*				with the -vt command line switch to leave the response file
*				behind.
*
****************************************************************************/

#include "pmapi.h"
#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __UNIX__
#include <unistd.h>
#else
#include <process.h>
#endif

/*------------------------- Global variables ------------------------------*/

#define	MAX_CMD_LINE	2048

char	cmdLine[MAX_CMD_LINE];
char	*ccArgv[MAXARG];
char	moduleName[PM_MAX_PATH];

/*-------------------------- Implementation -------------------------------*/

void usage(void)
{
	printf("Usage: k_cc <cc> @<rspfile> <source_file>\n");
	printf("\n");
	printf("Simple C/C++ compiler wrapper program to hide the command line. This\n");
	printf("program is used to keep the command line clean so it is easy to spot\n");
	printf("warnings and messages in build log files when using dmake.\n");
	printf("\n");
	printf("If you need to see the command line being used, run dmake with the -vt\n");
	printf("command line switch to leave the response file behind.\n");
	exit(-1);
}

int main(int argc,char *argv[])
{
	FILE	*f;

	if ((argc != 3 && argc != 4) || argv[2][0] != '@')
		usage();
	if ((f = fopen(&argv[2][1],"r")) == NULL) {
		printf("Unable to open response file '%s'\n", &argv[2][1]);
		exit(-1);
		}
	if (fgets(cmdLine,sizeof(cmdLine),f) == NULL) {
		printf("I/O error reading response file '%s'\n", &argv[2][1]);
		exit(-1);
		}
	fclose(f);
	if (argc == 4) {
		strcat(cmdLine," ");
		strcat(cmdLine,argv[3]);
		printf("%s %s %s\n", argv[1], argv[2], argv[3]);
		}
	else
		printf("%s %s\n", argv[1], argv[2]);
	parse_commandline(moduleName,cmdLine,&argc,ccArgv,MAXARG);
	ccArgv[argc] = NULL;
	return execvp(argv[1],ccArgv);
}

