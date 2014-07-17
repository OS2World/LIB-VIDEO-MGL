/****************************************************************************
*
*								 makedep
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
* Environment:	MSDOS
*
* Description:	Program to build a list of makefile dependencies given
*				a list of object files to be built. Will search the standard
*				set of include file directories for the include files,
*				and can be set to ignore all system include files.
*
*				Written specifically for use with the DMAKE program.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pmapi.h"
#include "getopt.h"
#ifdef	__LINUX__
#include <unistd.h>
#endif

#define	MAX_SRC_DIR     50
#define	MAX_INC_DIR		50
#define	MAX_FILES		1000
#define	MAX_DEPEND		1000
#define	LINESIZE		256
#define	INCLUDE_VAR		"INCLUDE"
#define	ASM_INC			"include"
#define	ASM_INC_LEN		7
#define	C_INC			"#include"
#define	C_INC_LEN		8
#define	IGNORE_STR		"IGNORE DEPEND"
#define IGNORE_LEN      13

/*------------------------- Global variables ------------------------------*/

char	filenames[MAX_FILES][PM_MAX_PATH];
int		incnum = 0;
char 	incdir[MAX_SRC_DIR][PM_MAX_PATH];
int 	srcnum = 1;
char 	srcdir[MAX_SRC_DIR][PM_MAX_PATH] = {""};
int 	numDepend;
char 	dependList[MAX_DEPEND][PM_MAX_PATH];
char 	*srcexts[] = {".c",".cc",".cpp",".cxx",".asm"};
int		type;
ibool 	ignoreSys;
ibool 	relativePaths;
ibool 	debug = false;

#define	NUMEXT	(sizeof(srcexts) / sizeof(char*))

/*-------------------------- Implementation -------------------------------*/

#ifdef	__UNIX__
/* Unix file systems are case sensitive */
#define	stricmp 	strcasecmp
#define	strnicmp	strncasecmp
#define	LWR_FILENAME(s)
#define	UPR_FILENAME(s)
#define SEPARATOR	'/'
#else
#define	LWR_FILENAME(s)	strlwr(s)
#define	UPR_FILENAME(s)	strupr(s)
#define SEPARATOR	'\\'
#endif

void convertSeparator(char *s)
{
	while (*s) {
#ifdef	__UNIX__
		if (*s == '\\')
			*s = '/';
#else
		if (*s == '/')
			*s = '\\';
#endif
		s++;
		}
}

int openfile(FILE **in,char *filename,char *mode)
{
	if ((*in = fopen(filename,mode) ) == NULL)
		return false;
	else
		return true;
}

void error(char *msg)
{
	fprintf(stderr,"FATAL: %s\n", msg);
	exit(1);
}

char *skipwhite(char *s)
{
	while (*s && isspace(*s))
		s++;
	return *s ? s : NULL;
}

char *skiptowhite(char *s)
{
	while (*s && !isspace(*s))
		s++;
	return *s ? s : NULL;
}

void readfilenames(
	char *name,
	char filenames[MAX_FILES][PM_MAX_PATH],
	int *numfiles)
{
	char	buf[PM_MAX_PATH];
	FILE	*f;

	*numfiles = 0;
	if (!openfile(&f,name,"r")) {
		/* Unable to open the file - simply return 0 for number of files
		 * to process
		 */
		return;
		}
	while (!feof(f) && (fscanf(f," %s ",buf) == 1)) {
		LWR_FILENAME(buf);
		strcpy(filenames[*numfiles],buf);
		(*numfiles)++;
		}
	fclose(f);
}

char *readrsp(char *name)
{
	FILE		*f;
	static char	rspbuf[512];

	if (!openfile(&f,name,"r"))
		error("Unable to open response file!\n");
	fgets(rspbuf,512,f);
	fclose(f);
	return rspbuf;
}

void help(void)
{
	printf("Usage: makedep [-arsSIu @rspfile] <objects>\n\n");
	printf("where 'objects' is the list of all object files to build the dependency\n");
	printf("information for. You can use @rspfile to obtain the objects file names from\n");
	printf("a response file rather than from the command line. Include files are\n");
	printf("in the current directory or in the directories specified by the INCLUDE\n");
	printf("environment variable.\n");
	printf("\n");
	printf("Options are:\n");
	printf("\n");
	printf("  -a<file> Append the output to the specified file (stdout is default)\n");
	printf("  -r       Generate relative pathnames for dependencies (not system includes)\n");
	printf("  -s       Ignore all system include files (eg: <stdio.h>)\n");
	printf("  -S<dir>  List of directories to search for source files (-S@rsp also valid)\n");
	printf("  -I<inc>  List of directories to search for include files (-I@rsp also valid)\n");
	printf("  -u       Generate dependencies in all uppercase (lowercase is default)\n");
	exit(1);
}

int getSrcName(char *srcname,char *dependname)
/****************************************************************************
*
* Function:		getSrcName
* Parameters:	srcname		- Place to store the source file name
*				dependname	- Dependent file name (.obj) being used
* Returns:		0 for C files, 1 for assembler files, -1 if not found.
*
* Description:	Attempts to locate the source file for the specified
*				object file dependency and return the pathname. If one
*				cannot be found we bomb out.
*
****************************************************************************/
{
	int			i,j;
	char		tmp[PM_MAX_PATH],drive[PM_MAX_DRIVE],dir[PM_MAX_PATH];
	char		name[PM_MAX_PATH],ext[PM_MAX_PATH];
	PM_findData	findData;
	void		*hfile;

	findData.dwSize = sizeof(findData);
	PM_splitpath(dependname,drive,dir,name,ext);
	if (stricmp(ext,".res") == 0) {
		for (i = 0; i < srcnum; i++) {
			PM_makepath(tmp,"",srcdir[i],name,".rc");
			if (debug)
				fprintf(stderr,"Searching for %s ... \n", tmp);
			if ((hfile = PM_findFirstFile(tmp,&findData)) != PM_FILE_INVALID) {
				PM_findClose(hfile);
				strcpy(srcname, tmp);
				if (debug)
					fprintf(stderr,"found\n");
				return 0;
				}
			if (debug)
				fprintf(stderr,"not found\n");
			}
		}
	else {
		for (i = 0; i < srcnum; i++) {
			for (j = 0; j < NUMEXT; j++) {
				PM_makepath(tmp,"",srcdir[i],name,srcexts[j]);
				if (debug)
					fprintf(stderr,"Searching for %s ... \n", tmp);
				if ((hfile = PM_findFirstFile(tmp,&findData)) != PM_FILE_INVALID) {
					PM_findClose(hfile);
					strcpy(srcname, tmp);
					if (debug)
						fprintf(stderr,"found\n");
					return (srcexts[j][1] == 'a');
					}
				if (debug)
					fprintf(stderr,"not found\n");
				}
			}
		}
	fprintf(stderr,"Unable to locate dependencies for %s.\n", dependname);
	return -1;
}

ibool processInclude(char *incname,char *incpath,int sysInclude)
/****************************************************************************
*
* Function:		processInclude
* Parameters:	incname		- Name of include file to add
*				sysInclude	- True if it is a system include file
*
* Description:	Attempts to search for the specified include file and
*				builds the full pathname to it.
*
****************************************************************************/
{
	int			j;
	char		tmp[PM_MAX_PATH],cwd[PM_MAX_PATH],drive[PM_MAX_DRIVE];
	char		dir[PM_MAX_PATH],name[PM_MAX_PATH],ext[PM_MAX_PATH];
	char		cdrive[PM_MAX_DRIVE],cdir[PM_MAX_PATH],cname[PM_MAX_PATH];
	char		cext[PM_MAX_PATH],tdir[PM_MAX_PATH],*p,*cp;
	PM_findData	findData;
	void		*hfile;

	findData.dwSize = sizeof(findData);
	LWR_FILENAME(incname);
	if (incname[0] == '\\' || incname[0] == '/')
		strcpy(tmp,incname);
	else {
		strcpy(tmp,incpath);
		strcat(tmp,incname);
		}
	if (debug)
		fprintf(stderr,"Searching for %s ... \n", tmp);
	if ((hfile = PM_findFirstFile(tmp,&findData)) != PM_FILE_INVALID) {
		PM_findClose(hfile);
		if (debug)
			fprintf(stderr,"found\n");
		if (relativePaths && !sysInclude) {
			PM_splitpath(tmp,drive,dir,name,ext);
			PM_getCurrentPath(cwd,PM_MAX_PATH);
			LWR_FILENAME(cwd);
			PM_backslash(cwd);
			PM_splitpath(cwd,cdrive,cdir,cname,cext);
			if (drive[0] == cdrive[0]) {
				p = dir;
				cp = cdir;
				/* Skip common directory paths */
				while (*p && *cp  && (*p == *cp)) {
					p++; cp++;
					}
				if (p != dir && cp != cdir) {
					/* Skip back to start of uncommon directory name */
					while (*(p-1) != SEPARATOR && p != dir && cp != cdir) {
						p--;
						cp--;
						}
					strcpy(tdir,p);
					p = cdir;
					while ((cp = strchr(cp,SEPARATOR)) != NULL) {
						*p++ = '.';
						*p++ = '.';
						*p++ = SEPARATOR;
						cp++;
						}
					*p = 0;
					strcat(cdir,tdir);
					PM_makepath(tmp,NULL,cdir,name,ext);
					}
				else
					PM_makepath(tmp,NULL,dir,name,ext);
				}
			}
		for (j = 0; j < numDepend; j++)
			if (strcmp(dependList[j],tmp) == 0)
				return true;
		if (numDepend == MAX_DEPEND)
			error("Too many dependant files!");
		strcpy(dependList[numDepend++],tmp);
		return true;
		}
	if (debug)
		fprintf(stderr,"not found\n");
	return false;
}

void addIncName(char *incname,char *srcname,char *srcPath,ibool sysInclude)
/****************************************************************************
*
* Function:		addIncName
* Parameters:	incname		- Name of include file to add
*				sysInclude	- True if it is a system include file
*
* Description:	Attempts to search for the specified include file and
*				builds the full pathname to it.
*
****************************************************************************/
{
	int	i;

	LWR_FILENAME(incname);
	if (!sysInclude) {
		if (processInclude(incname,srcPath,sysInclude))
			return;
		}
	for (i = (sysInclude ? 1 : 0); i < incnum; i++) {
		if (processInclude(incname,incdir[i],sysInclude))
			return;
		}
	fprintf(stderr,"Unable to locate include file %s (from %s).\n", incname, srcname);
}

void buildDependList(char *srcname)
/****************************************************************************
*
* Function:		buildDependList
* Parameters:	srcname			- Name of source file to build dependencies
*
* Description:	Builds the list of dependencies for the source file.
*
****************************************************************************/
{
	FILE	*f;
	int		i,first,last,sysInclude;
	char	line[LINESIZE],*s,*e;
	char	srcPath[PM_MAX_PATH],drive[PM_MAX_DRIVE],name[PM_MAX_PATH],ext[PM_MAX_PATH];

	if (!openfile(&f,srcname,"r")) {
		printf("Unable to open source file %s!!\n", srcname);
		exit(1);
		}

	/* Find path to source file as first include directory in list */
	PM_splitpath(srcname,drive,srcPath,name,ext);

	/* Now process all include files for this file */
	first = numDepend;
	while (fgets(line,LINESIZE,f)) {
		if ((s = skipwhite(line)) == NULL)
			continue;

		if (type) {				/* Assembler source file		*/
			if (strnicmp(s,ASM_INC,ASM_INC_LEN) == 0) {
				if (strstr(s,IGNORE_STR) != NULL)
					continue;
				if ((s = skipwhite(s + ASM_INC_LEN)) == NULL)
					continue;
				if (*s == '"')
					s++;
				if ((e = skiptowhite(s)) == NULL)
					continue;
				if (*(e-1) == '"')
					e--;
				*e = 0;
				addIncName(s,srcname,srcPath,false);
				}
			}
		else {					/* C style source file			*/
			if (strnicmp(s,C_INC,C_INC_LEN) == 0) {
				if (strstr(s,IGNORE_STR) != NULL)
					continue;
				if ((s = skipwhite(s + C_INC_LEN)) == NULL)
					continue;
				if (*s == '<') {
					if (ignoreSys)
						continue;
					sysInclude = true;
					}
				else
					sysInclude = false;
				s++;
				if ((e = skiptowhite(s)) == NULL)
					continue;
				e--;
				*e = 0;
				addIncName(s,srcname,srcPath,sysInclude);
				}
			}
		}
	last = numDepend;
	fclose(f);

	/* Recursively parse each include file found */
	if (first < last) {
		for (i = first; i < last; i++)
			buildDependList(dependList[i]);
		}
}

void processFiles(FILE *out,char filenames[MAX_FILES][PM_MAX_PATH],
	int numfiles,ibool uppercase)
/****************************************************************************
*
* Function:		processFiles
* Parameters:	out				- File to send output list to
*               filenames		- List of object file names to process
*               numfiles		- Numer of files to process
*
* Description:	Processes the list of files generating the list of
*				dependencies in the output list.
*
****************************************************************************/
{
	int		i,j;
	char    srcname[PM_MAX_PATH];

	for (i = 0; i < numfiles; i++) {
		if ((type = getSrcName(srcname,filenames[i])) == -1)
			continue;
		numDepend = 0;
		buildDependList(srcname);

		/* Last one in list is the original source file */
		strcpy(dependList[numDepend++],srcname);

		/* Convert to uppercase or lower case as needed */
		if (uppercase) {
			UPR_FILENAME(filenames[i]);
			for (j = 0; j < numDepend; j++)
				UPR_FILENAME(dependList[j]);
			}

		/* Write the list of dependant files to the output file */
		for (j = 0; j < numDepend; j++) {
			convertSeparator(dependList[j]);
			fprintf(out, "%s: %s\n", filenames[i],dependList[j]);
			}
		}
}

int main(int argc,char *argv[])
{
	int		i,numfiles,option;
	char	*argument,*s,*d;
	char	tmp[255];
	FILE	*out = stdout;
	ibool	uppercase;

	uppercase = ignoreSys = relativePaths = false;

	/* Read include file directories from the INCLUDE evironment var */

	PM_getCurrentPath(tmp,PM_MAX_PATH);
	LWR_FILENAME(tmp);
	strcpy(incdir[incnum],tmp);
	PM_backslash(incdir[incnum++]);
	if (getenv(INCLUDE_VAR)) {
		strcpy(tmp,getenv(INCLUDE_VAR));
		LWR_FILENAME(tmp);
		s = tmp;
#ifdef __UNIX__
		d = s;
		while (d[0] && d[1] && d[0] == '-' && (d[1] == 'i' || d[1] == 'I')) {
			d += 2;
			s = d;
			while (*s && !isspace(*s))
				s++;
			i = *s;				
			*s = 0;
			strcpy(incdir[incnum],d);
			PM_backslash(incdir[incnum++]);
			d = s+1;
			if (i == 0)
				break;
			}
#else		
		while ((d = strchr(s,';')) != NULL) {
			*d++ = '\0';
			strcpy(incdir[incnum],s);
			PM_backslash(incdir[incnum++]);
			s = d;
			}
		if (strlen(s) > 0) {
			strcpy(incdir[incnum],s);
			PM_backslash(incdir[incnum++]);
			}
#endif			
		}

	/* Parse command line options */
	do {
		option = getopt(argc,argv,"a:rsS:I:uD",&argument);
		switch(option) {
			case 'a':
				if (!openfile(&out,argument,"at"))
					error("Unable to open output file");
				break;
			case 'r':	relativePaths = true;		break;
			case 's':	ignoreSys = true;			break;
			case 'u':   uppercase = true;			break;
			case 'D': 	debug = true; 				break;
			case 'S':
				if (argument[0] == '@')
					argument = readrsp(argument+1);
				LWR_FILENAME(argument);
				s = argument;
				while ((d = strchr(s,';')) != NULL) {
					*d++ = '\0';
					strcpy(srcdir[srcnum],s);
					PM_backslash(srcdir[srcnum++]);
					s = d;
					}
				strcpy(srcdir[srcnum],s);
				PM_backslash(srcdir[srcnum++]);
				break;
			case 'I':
				if (argument[0] == '@')
					argument = readrsp(argument+1);
				LWR_FILENAME(argument);
				s = argument;
				while ((d = strchr(s,';')) != NULL) {
					*d++ = '\0';
					strcpy(incdir[incnum],s);
					PM_backslash(incdir[incnum++]);
					s = d;
					}
				strcpy(incdir[incnum],s);
				PM_backslash(incdir[incnum++]);
				break;
			case INVALID:
				help();
			}
		} while (option != ALLDONE && option != PARAMETER);

	if ((argc - nextargv) < 1)
		help();

	/* Read list of files to process from command line or from response
	 * file.
	 */
	if (argv[nextargv][0] == '@')
		readfilenames(&argv[nextargv][1],filenames,&numfiles);
	else {
		numfiles = 0;
		for (i = nextargv; i < argc; i++) {
			strcpy(filenames[numfiles],argv[i]);
			LWR_FILENAME(filenames[numfiles++]);
			}
		}

	if (debug) {
		fprintf(stderr,"Source directories:\n");
		for (i = 0; i < srcnum; i++)
			fprintf(stderr,"%s\n", srcdir[i]);
		fprintf(stderr,"Include directories:\n");
		for (i = 0; i < incnum; i++)
			fprintf(stderr,"%s\n", incdir[i]);
		fprintf(stderr,"Object files:\n");
		for (i = 0; i < numfiles; i++)
			fprintf(stderr,"%s\n", filenames[i]);
		}
	processFiles(out,filenames,numfiles,uppercase);

	if (out != stdout)
		fclose(out);
	return 0;
}
