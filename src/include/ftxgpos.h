/*******************************************************************
 *
 *  ftxgpos.h
 *
 *    TrueType Open GPOS table support
 *
 *  Copyright 1996-1998 by
 *  David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 *  This file is part of the FreeType project, and may only be used
 *  modified and distributed under the terms of the FreeType project
 *  license, LICENSE.TXT.  By continuing to use, modify, or distribute
 *  this file you indicate that you have read the license and
 *  understand and accept it fully.
 *
 ******************************************************************/

#ifndef FTXOPEN_H
#error "Don't include this file! Use ftxopen.h instead."
#endif

#ifndef FTXGPOS_H
#define FTXGPOS_H

#ifdef __cplusplus
extern "C" {
#endif

#define TTO_Err_Invalid_GPOS_SubTable_Format  0x1020


  /* to be written */

  union  TTO_GPOS_SubTable_
  {
    int  dummy;
  };

  typedef union TTO_GPOS_SubTable_  TTO_GPOS_SubTable;

#ifdef __cplusplus
}
#endif

#endif /* FTXGPOS_H */


/* END */
