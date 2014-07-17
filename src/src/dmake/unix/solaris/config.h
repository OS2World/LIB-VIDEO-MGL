/* RCS  $Id: config.h,v 1.1.1.1 1997/07/15 16:02:36 dvadura Exp $
--
-- SYNOPSIS
--      Configurarion include file.
-- 
-- DESCRIPTION
-- 	There is one of these for each specific machine configuration.
--	It can be used to further tweek the machine specific sources
--	so that they compile.
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

/* define this for configurations that don't have the coreleft function
 * so that the code compiles.  To my knowledge coreleft exists only on
 * Turbo C, but it is needed here since the function is used in many debug
 * macros. */
#define coreleft() 0L

/* Define the getcwd function that is used in the code, since BSD does
 * not have getcwd, but call it getwd instead. */
/*extern	char*	getwd ANSI((char*));
#define	getcwd(buf,siz)	getwd(buf)*/

/* We don't care about CONST */
#define CONST const

/* a small problem with pointer to voids on some unix machines needs this */
#define PVOID void *

/* Sequent dynix doesn't have tzset() Arrrrgh! */
#define tzset()

#ifdef  _POSIX_NAME_MAX
#undef  _POSIX_NAME_MAX
#define _POSIX_NAME_MAX 255
#endif
