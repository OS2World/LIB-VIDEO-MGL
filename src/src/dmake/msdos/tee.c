/* RCS  $Id: tee.c,v 1.1.1.1 1997/07/15 16:02:27 dvadura Exp $
--
-- SYNOPSIS
--      Hook_std_writes() dummy call for non swapping MSDOS versions.
-- 
-- DESCRIPTION
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

#include "extern.h"

PUBLIC void
Hook_std_writes( file )
char *file;
{
}
