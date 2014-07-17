/*******************************************************************
 *
 *  ftxopenf.h
 *
 *    internal TrueType Open functions
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

#ifndef FTXOPENF_H
#define FTXOPENF_H

#include "ftxopen.h"

#ifdef __cplusplus
extern "C" {
#endif

  /* functions from ftxopen.c */

  TT_Error  Load_ScriptList( TTO_ScriptList*  sl,
                             PFace            input );
  TT_Error  Load_FeatureList( TTO_FeatureList*  fl,
                              PFace             input );
  TT_Error  Load_LookupList( TTO_LookupList*  ll,
                             PFace            input,
                             TTO_Type         type );

  TT_Error  Load_Coverage( TTO_Coverage*  c,
                           PFace          input );
  TT_Error  Load_ClassDefinition( TTO_ClassDefinition*  cd,
                                  PFace                 input );

  void  Free_ScriptList( TTO_ScriptList*  sl );
  void  Free_FeatureList( TTO_FeatureList*  fl );
  void  Free_LookupList( TTO_LookupList*  ll,
                         TTO_Type         type );

  void  Free_Coverage( TTO_Coverage*  c );
  void  Free_ClassDefinition( TTO_ClassDefinition*  cd );


  /* functions from ftxgsub.c */

  TT_Error  Load_LookupSingle( TTO_SingleSubst*  ss,
                               PFace             input );
  TT_Error  Load_LookupMultiple( TTO_MultipleSubst*  ms,
                                 PFace               input );
  TT_Error  Load_LookupAlternate( TTO_AlternateSubst*  as,
                                  PFace                input );
  TT_Error  Load_LookupLigature( TTO_LigatureSubst*  ls,
                                 PFace               input );
  TT_Error  Load_LookupContext( TTO_ContextSubst*  cs,
                                PFace              input );

  void  Free_LookupSingle( TTO_SingleSubst*  ss );
  void  Free_LookupMultiple( TTO_MultipleSubst*  ms );
  void  Free_LookupAlternate( TTO_AlternateSubst*  as );
  void  Free_LookupLigature( TTO_LigatureSubst*  ls );
  void  Free_LookupContext( TTO_ContextSubst*  cs );

  TT_Error  Coverage_Index( TTO_Coverage*  c,
                            UShort         glyphID,
                            UShort*        index );
  UShort  Get_Class( TTO_ClassDefinition*  cd,
                     UShort                glyphID );


#ifdef __cplusplus
}
#endif

#endif /* FTXOPENF_H */


/* END */
