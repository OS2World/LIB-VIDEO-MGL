/* RCS  $Id: config.h,v 1.1.1.1 1997/07/15 16:02:26 dvadura Exp $
--
-- SYNOPSIS
--      Configurarion include file for the Macintosh.
-- 
-- DESCRIPTION
--  There is one of these for each specific machine configuration.
--  It can be used to further tweek the machine specific sources
--  so that they compile.
--
-- AUTHOR
--      Dennis Vadura, dvadura@dmake.wticorp.com
--
-- WWW
--      http://dmake.wticorp.com/
--
-- COPYRIGHT
--      Copyright (c) 1996,1997 by WTI Corp.  All rights reserved.
-- 
--      This program is NOT free software; you can redistribute it and/or
--      modify it under the terms of the Software License Agreement Provided
--      in the file <distribution-root>/readme/license.txt.
--
-- LOG
--      Use cvs log to obtain detailed change logs.
*/

/* Definitions left out of StdArg.h */
#define va_alist     ...
#define va_dcl

#define CONST const

#define FALSE 0
#define TRUE 1

/* signal extensions */
#define SIGQUIT SIGTERM


/* Mac doesn't have a stat function or structure so we have to add one in. */
typedef long off_t;
struct stat {
    unsigned short st_mode;
    off_t st_size;
    time_t st_mtime;
    time_t st_ctime;
    /* struct stat has lots of other fields, but we don't need them for dmake */
}; /* struct stat */
#define S_IFDIR  0040000    /* directory */
#define S_IFREG  0100000    /* regular */
#define S_IFMT   (S_IFDIR | S_IFREG)    /* Format */
#define S_IREAD  0000400    /* read owner permission */
#define S_IWRITE 0000200    /* write owner permission */
#define S_IEXEC  0000100    /* execute owner permission */


/* Global for environmental variables */
extern char **environ;


/* We really want main to be in the mac directory
   so that we get the envp argument */
#define main(argc, argv)    dmakemain(argc, argv)

/* Directory/file info. and directory moving */
int stat (char *pPath, struct stat *pStat);
char *getcwd (char *pPath, size_t pathSize);
int chdir (char *pPath);
int utime (char *pPath, time_t *pTimes);

/* Routines to handle conversion from Unix file names to Mac file names */
char *Unix2MacFName(char *pUnixName);
FILE *MacFOpen (char *pName, char *pMode);
#define fopen(pFName, pMode)    MacFOpen(pFName, pMode)

/* a small problem with pointer to voids on some unix machines needs this */
#define PVOID void *
