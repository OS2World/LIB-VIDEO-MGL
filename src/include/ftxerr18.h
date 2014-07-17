/****************************************************************************/
/*                                                                          */
/*  Erwin Dieterich,  15. 10. 1997                                          */
/*                  - 06. 07. 1998                                          */
/*                                                                          */
/*  TT_ErrToString: translate error codes to character strings              */
/*                                                                          */
/*  This extension provides internationalised error strings from the        */
/*  various error messages. It uses the "gettext" package where available   */
/*  or returns english/american message strings if not.                     */
/*                                                                          */
/*  If you do not want to use it, or if you encounter some problems         */
/*  compiling this file, try to disable nls support by configuring          */
/*  FreeType with ./configure --disable-nls                                 */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#ifndef FTXERR18_H
#define FTXERR18_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "freetype.h"

TT_String*  TT_ErrToString18( TT_Error  i );

#ifdef __cplusplus
  }
#endif

#endif /* FTXERR18_H */


/* END */
