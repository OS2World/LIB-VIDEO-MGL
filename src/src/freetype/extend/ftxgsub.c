/*******************************************************************
 *
 *  ftxgsub.c
 *
 *    GSUB table support.
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

#include "tttypes.h"
#include "tttags.h"
#include "ttload.h"
#include "ttextend.h"
#include "ttmemory.h"
#include "ttfile.h"

#include "ftxopen.h"
#include "ftxopenf.h"


#define GSUB_ID  Build_Extension_ID( 'G', 'S', 'U', 'B' )


#define ADD_STRING( in, num_in, out, num_out, data )        \
          ( ( error = Add_String( (in), (num_in),           \
                                  (out), (num_out),         \
                                  (data) ) ) != TT_Err_Ok )

#define String_Send_Begin( s )     (s)->pos = 0;
#define String_Send_End( s )       (s)->pos = 0;
#define String_Receive_Begin( s )  (s)->pos = 0;
#define String_Receive_End( s )    (s)->length = (s)->pos; \
                                   (s)->pos    = 0;



  /**********************
   * Auxiliary functions
   **********************/


  /* The following function copies `num_out' elements from `data' to
     `out', advancing the array pointer in the `in' structure by `num_in'
     elements and in `out' by `num_out' elements.  If the string array
     in `out' is empty or too small, it allocates resp. reallocates the
     string array.                                                        */

  static TT_Error  Add_String( TTO_String*  in,
                               UShort       num_in,
                               TTO_String*  out,
                               UShort       num_out,
                               UShort*      data )
  {
    TT_Error  error = TT_Err_Ok;


    /* sanity check */

    if ( !in || !out ||
         in->length == 0 || in->pos >= in->length ||
         in->length >= in->pos + num_in )
      return TT_Err_Invalid_Argument;

    if ( out->allocated == 0 )
    {
      if ( ALLOC( out->string, in->length - ( in->pos + num_in ) ) )
        return error;
      out->allocated = in->length - ( in->pos + num_in );
    }
    else if ( out->pos + num_out >= out->allocated )
    {
      if ( REALLOC( out->string, out->pos + num_out +
                                 in->length - ( in->pos + num_in ) ) )
        return error;
      out->allocated = out->pos + num_out +
                       in->length - ( in->pos + num_in );
    }

    MEM_Copy( &out->string[in->pos], data, num_out * sizeof ( UShort ) );

    in->pos  += num_in;
    out->pos += num_out;

    return TT_Err_Ok;
  }



  /**********************
   * Extension Functions
   **********************/


  static TT_Error  GSUB_Create( void*  ext,
                                PFace  face )
  {
    DEFINE_LOAD_LOCALS( face->stream );

    TTO_GSUBHeader*  gsub = (TTO_GSUBHeader*)ext;
    Long             table;


    /* by convention */
    if ( !gsub )
      return TT_Err_Ok;

    /* a null offset indicates that there is no GSUB table */
    gsub->offset = 0;

    /* we store the start offset and the size of the subtable */
    table = TT_LookUp_Table( face, TTAG_GSUB );
    if ( table < 0 )
      return TT_Err_Ok;     /* The table is optional */

    if ( FILE_Seek( face->dirTables[table].Offset ) ||
         ACCESS_Frame( 4L ) )
      return error;

    gsub->offset  = FILE_Pos();
    gsub->Version = GET_ULong();

    FORGET_Frame();

    gsub->loaded = FALSE;

    return TT_Err_Ok;
  }


  static TT_Error  GSUB_Destroy( void*  ext,
                                 PFace  face )
  {
    TTO_GSUBHeader*  gsub = (TTO_GSUBHeader*)ext;


    /* by convention */
    if ( !gsub )
      return TT_Err_Ok;

    if ( gsub->loaded )
    {
      Free_LookupList( &gsub->LookupList, GSUB );
      Free_FeatureList( &gsub->FeatureList );
      Free_ScriptList( &gsub->ScriptList );
    }

    return TT_Err_Ok;
  }


  TT_Error  TT_Init_GSUB_Extension( TT_Engine  engine )
  {
    PEngine_Instance  _engine = HANDLE_Engine( engine );

    TT_Error  error;


    if ( !_engine )
      return TT_Err_Invalid_Engine;

    error = TT_Register_Extension( _engine,
                                   GSUB_ID,
                                   sizeof ( TTO_GSUBHeader ),
                                   GSUB_Create,
                                   GSUB_Destroy );
    return error;
  }


  TT_Error  TT_Load_GSUB_Table( TT_Face          face,
                                TTO_GSUBHeader*  gsub )
  {
    TT_Error   error;
    TT_Stream  stream;

    PFace  faze = HANDLE_Face( face );


    if ( !faze )
      return TT_Err_Invalid_Face_Handle;

    error = TT_Extension_Get( faze, GSUB_ID, (void**)&gsub );
    if ( error )
      return error;

    if ( gsub->offset == 0 )
      return TT_Err_Ok;             /* no GSUB table; nothing to do */

    /* now access stream */
    if ( USE_Stream( faze->stream, stream ) )
      return error;

    /* the next three functions access the current file position
       to get the real table offset                              */

    if ( FILE_Seek( gsub->offset ) ||
         ( error = Load_ScriptList( &gsub->ScriptList,
                                    faze ) ) != TT_Err_Ok )
      goto Fail1;

    if ( FILE_Seek( gsub->offset + 2L ) ||
         ( error = Load_FeatureList( &gsub->FeatureList,
                                     faze ) ) != TT_Err_Ok )
      goto Fail2;

    if ( FILE_Seek( gsub->offset + 4L ) ||
         ( error = Load_LookupList( &gsub->LookupList,
                                    faze, GSUB ) ) != TT_Err_Ok )
      goto Fail3;

    gsub->loaded = TRUE;
    DONE_Stream( stream );

    return TT_Err_Ok;


  Fail3:
    Free_LookupList( &gsub->LookupList, GSUB );

  Fail2:
    Free_FeatureList( &gsub->FeatureList );

  Fail1:
    Free_ScriptList( &gsub->ScriptList );

    /* release stream */
    DONE_Stream( stream );

    return error;
  }



  /*****************************
   * SubTable related functions
   *****************************/


  /* LookupType 1 */

  /* SingleSubstFormat1 */
  /* SingleSubstFormat2 */

  TT_Error  Load_LookupSingle( TTO_SingleSubst*  ss,
                               PFace             input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort   n, count;
    ULong    cur_offset, new_offset, base_offset;

    UShort*  s;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ss->SubstFormat = GET_UShort();
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ss->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    switch ( ss->SubstFormat )
    {
    case 1:
      if ( ACCESS_Frame( 2L ) )
        return error;

      ss->ssf.ssf1.DeltaGlyphID = GET_UShort();

      FORGET_Frame();

      break;

    case 2:
      if ( ACCESS_Frame( 2L ) )
        return error;

      count = ss->ssf.ssf2.GlyphCount = GET_UShort();

      FORGET_Frame();

      ss->ssf.ssf2.Substitute = NULL;

      if ( ALLOC_ARRAY( ss->ssf.ssf2.Substitute, count, TT_UShort ) )
        return error;

      s = ss->ssf.ssf2.Substitute;

      if ( ACCESS_Frame( count * 2L ) )
        return error;

      for ( n = 0; n < count; n++ )
        s[n] = GET_UShort();

      FORGET_Frame();

      break;

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;
  }


  void  Free_LookupSingle( TTO_SingleSubst*  ss )
  {
    switch ( ss->SubstFormat )
    {
    case 1:
      break;
    case 2:
      if ( ss->ssf.ssf2.Substitute )
        FREE( ss->ssf.ssf2.Substitute );
      break;
    default:
      break;
    }

    Free_Coverage( &ss->Coverage );
  }


  static TT_Error  Lookup_Single( TTO_SingleSubst*  ss,
                                  TTO_String*       in,
                                  TTO_String*       out )
  {
    UShort    index, value[1];
    TT_Error  error;


    /* sanity check */

    if ( !in || !out || in->length == 0 || in->pos >= in->length )
      return TT_Err_Invalid_Argument;

    error = Coverage_Index( &ss->Coverage, in->string[in->pos], &index );
    if ( error == TTO_Err_Not_Covered )
    {
      if ( ADD_STRING( in, 1, out, 1, &in->string[in->pos] ) )
        return error;
    }
    else
    {
      switch ( ss->SubstFormat)
      {
      case 1:
        value[0] = in->string[in->pos] + ss->ssf.ssf1.DeltaGlyphID;
        if ( ADD_STRING( in, 1, out, 1, value ) )
          return error;
        break;
      case 2:
        if ( index >= ss->ssf.ssf2.GlyphCount )
          return TTO_Err_Invalid_GSUB_SubTable;
        if ( ADD_STRING( in, 1, out, 1, &ss->ssf.ssf2.Substitute[index] ) )
          return error;
        break;
      default:
        return TTO_Err_Invalid_GSUB_SubTable;
      }
    }

    return TT_Err_Ok;
  }


  /* LookupType 2 */

  /* Sequence */

  static TT_Error  Load_Sequence( TTO_Sequence*  s,
                                  PFace          input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort   n, count;

    UShort*  sub;


    if ( ACCESS_Frame( 2L ) )
      return error;

    count = s->GlyphCount = GET_UShort();

    FORGET_Frame();

    s->Substitute = NULL;

    if ( ALLOC_ARRAY( s->Substitute, count, TT_UShort ) )
      return error;

    sub = s->Substitute;

    if ( ACCESS_Frame ( count * 2L ) )
      return error;

    for ( n = 0; n < count; n++ )
      sub[n] = GET_UShort();

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_Sequence( TTO_Sequence*  s )
  {
    if ( s->Substitute )
      FREE( s->Substitute );
  }


  /* MultipleSubstFormat1 */

  TT_Error  Load_LookupMultiple( TTO_MultipleSubst*  ms,
                                 PFace               input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort         n, count;
    ULong          cur_offset, new_offset, base_offset;

    TTO_Sequence*  s;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ms->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ms->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = ms->SequenceCount = GET_UShort();

    FORGET_Frame();

    ms->Sequence = NULL;

    if ( ALLOC_ARRAY( ms->Sequence, count, TTO_Sequence ) )
      return error;

    s = ms->Sequence;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Sequence( &s[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  void  Free_LookupMultiple( TTO_MultipleSubst*  ms )
  {
    UShort  n, count;


    if ( ms->Sequence )
    {
      count = ms->SequenceCount;

      for ( n = 0; n < count; n++ )
        Free_Sequence( &ms->Sequence[n] );

      FREE( ms->Sequence );
    }

    Free_Coverage( &ms->Coverage );
  }


  /* LookupType 3 */

  /* AlternateSet */

  static TT_Error  Load_AlternateSet( TTO_AlternateSet*  as,
                                      PFace              input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort   n, count;

    UShort*  a;


    if ( ACCESS_Frame( 2L ) )
      return error;

    count = as->GlyphCount = GET_UShort();

    FORGET_Frame();

    as->Alternate = NULL;

    if ( ALLOC_ARRAY( as->Alternate, count, TT_UShort ) )
      return error;

    a = as->Alternate;

    if ( ACCESS_Frame( count * 2L ) )
      return error;

    for ( n = 0; n < count; n++ )
      a[n] = GET_UShort();

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_AlternateSet( TTO_AlternateSet*  as )
  {
    if ( as->Alternate )
      FREE( as->Alternate );
  }


  /* AlternateSubstFormat1 */

  TT_Error  Load_LookupAlternate( TTO_AlternateSubst*  as,
                                  PFace                input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort             n, count;
    ULong              cur_offset, new_offset, base_offset;

    TTO_AlternateSet*  aset;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    as->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &as->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = as->AlternateSetCount = GET_UShort();

    FORGET_Frame();

    as->AlternateSet = NULL;

    if ( ALLOC_ARRAY( as->AlternateSet, count, TTO_AlternateSet ) )
      return error;

    aset = as->AlternateSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_AlternateSet( &aset[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  void  Free_LookupAlternate( TTO_AlternateSubst*  as )
  {
    UShort  n, count;


    if ( as->AlternateSet )
    {
      count = as->AlternateSetCount;

      for ( n = 0; n < count; n++ )
        Free_AlternateSet( &as->AlternateSet[n] );

      FREE( as->AlternateSet );
    }

    Free_Coverage( &as->Coverage );
  }


  /* LookupType 4 */

  /* Ligature */

  static TT_Error  Load_Ligature( TTO_Ligature*  l,
                                  PFace          input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort   n, count;

    UShort*  c;


    if ( ACCESS_Frame( 4L ) )
      return error;

    l->LigGlyph       = GET_UShort();
    l->ComponentCount = GET_UShort();

    FORGET_Frame();

    l->Component = NULL;

    count = l->ComponentCount - 1;      /* only ComponentCount - 1 elements */

    if ( ALLOC_ARRAY( l->Component, count, TT_UShort ) )
      return error;

    c = l->Component;

    if ( ACCESS_Frame( count * 2L ) )
      return error;

    for ( n = 0; n < count; n++ )
      c[n] = GET_UShort();

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_Ligature( TTO_Ligature*  l )
  {
    if ( l->Component )
      FREE( l->Component );
  }


  /* LigatureSet */

  static TT_Error  Load_LigatureSet( TTO_LigatureSet*  ls,
                                     PFace             input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort         n, count;
    ULong          cur_offset, new_offset, base_offset;

    TTO_Ligature*  l;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = ls->LigatureCount = GET_UShort();

    FORGET_Frame();

    ls->Ligature = NULL;

    if ( ALLOC_ARRAY( ls->Ligature, count, TTO_Ligature ) )
      return error;

    l = ls->Ligature;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Ligature( &l[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  static void  Free_LigatureSet( TTO_LigatureSet*  ls )
  {
    UShort  n, count;


    if ( ls->Ligature )
    {
      count = ls->LigatureCount;

      for ( n = 0; n < count; n++ )
        Free_Ligature( &ls->Ligature[n] );

      FREE( ls->Ligature );
    }
  }


  /* LigatureSubstFormat1 */

  TT_Error  Load_LookupLigature( TTO_LigatureSubst*  ls,
                                 PFace               input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort            n, count;
    ULong             cur_offset, new_offset, base_offset;

    TTO_LigatureSet*  lset;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ls->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ls->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = ls->LigatureSetCount = GET_UShort();

    FORGET_Frame();

    ls->LigatureSet = NULL;

    if ( ALLOC_ARRAY( ls->LigatureSet, count, TTO_LigatureSet ) )
      return error;

    lset = ls->LigatureSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_LigatureSet( &lset[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  void  Free_LookupLigature( TTO_LigatureSubst*  ls )
  {
    UShort  n, count;


    if ( ls->LigatureSet )
    {
      count = ls->LigatureSetCount;

      for ( n = 0; n < count; n++ )
        Free_LigatureSet( &ls->LigatureSet[n] );

      FREE( ls->LigatureSet );
    }

    Free_Coverage( &ls->Coverage );
  }


  /* LookupType 5 */

  /* SubRule */

  static TT_Error  Load_SubRule( TTO_SubRule*  sr,
                                 PFace         input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort                  n, count;

    UShort*                 i;
    TTO_SubstLookupRecord*  slr;


    if ( ACCESS_Frame( 4L ) )
      return error;

    sr->GlyphCount = GET_UShort();
    sr->SubstCount = GET_UShort();

    FORGET_Frame();

    sr->Input = NULL;

    count = sr->GlyphCount - 1;         /* only GlyphCount - 1 elements */

    if ( ALLOC_ARRAY( sr->Input, count, TT_UShort ) )
      return error;

    i = sr->Input;

    if ( ACCESS_Frame( count * 2L ) )
      return error;

    for ( n = 0; n < count; n++ )
      i[n] = GET_UShort();

    FORGET_Frame();

    sr->SubstLookupRecord = NULL;

    count = sr->SubstCount;

    if ( ALLOC_ARRAY( sr->SubstLookupRecord, count, TTO_SubstLookupRecord ) )
      return error;

    slr = sr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      return error;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_SubRule( TTO_SubRule*  sr )
  {
    if ( sr->SubstLookupRecord )
      FREE( sr->SubstLookupRecord );
    if ( sr->Input )
      FREE( sr->Input );
  }


  /* SubRuleSet */

  static TT_Error  Load_SubRuleSet( TTO_SubRuleSet*  srs,
                                    PFace            input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort        n, count;
    ULong         cur_offset, new_offset, base_offset;

    TTO_SubRule*  sr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = srs->SubRuleCount = GET_UShort();

    FORGET_Frame();

    srs->SubRule = NULL;

    if ( ALLOC_ARRAY( srs->SubRule, count, TTO_SubRule ) )
      return error;

    sr = srs->SubRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubRule( &sr[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  static void  Free_SubRuleSet( TTO_SubRuleSet*  srs )
  {
    UShort  n, count;


    if ( srs->SubRule )
    {
      count = srs->SubRuleCount;

      for ( n = 0; n < count; n++ )
        Free_SubRule( &srs->SubRule[n] );

      FREE( srs->SubRule );
    }
  }


  /* ContextSubstFormat1 */

  static TT_Error  Load_Context1( TTO_ContextSubstFormat1*  csf1,
                                  PFace                     input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort           n, count;
    ULong            cur_offset, new_offset, base_offset;

    TTO_SubRuleSet*  srs;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &csf1->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = csf1->SubRuleSetCount = GET_UShort();

    FORGET_Frame();

    csf1->SubRuleSet = NULL;

    if ( ALLOC_ARRAY( csf1->SubRuleSet, count, TTO_SubRuleSet ) )
      return error;

    srs = csf1->SubRuleSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubRuleSet( &srs[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  static void  Free_Context1( TTO_ContextSubstFormat1*  csf1 )
  {
    UShort  n, count;


    if ( csf1->SubRuleSet )
    {
      count = csf1->SubRuleSetCount;

      for ( n = 0; n < count; n++ )
        Free_SubRuleSet( &csf1->SubRuleSet[n] );

      FREE( csf1->SubRuleSet );
    }

    Free_Coverage( &csf1->Coverage );
  }


  /* SubClassRule */

  static TT_Error  Load_SubClassRule( TTO_SubClassRule*  scr,
                                      PFace              input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort                  n, count;

    UShort*                 c;
    TTO_SubstLookupRecord*  slr;


    if ( ACCESS_Frame( 4L ) )
      return error;

    scr->GlyphCount = GET_UShort();
    scr->SubstCount = GET_UShort();

    FORGET_Frame();

    scr->Class = NULL;

    count = scr->GlyphCount - 1;        /* only GlyphCount - 1 elements */

    if ( ALLOC_ARRAY( scr->Class, count, TT_UShort ) )
      return error;

    c = scr->Class;

    if ( ACCESS_Frame( count * 2L ) )
      return error;

    for ( n = 0; n < count; n++ )
      c[n] = GET_UShort();

    FORGET_Frame();

    scr->SubstLookupRecord = NULL;

    count = scr->SubstCount;

    if ( ALLOC_ARRAY( scr->SubstLookupRecord, count, TTO_SubstLookupRecord ) )
      return error;

    slr = scr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      return error;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_SubClassRule( TTO_SubClassRule*  scr )
  {
    if ( scr->SubstLookupRecord )
      FREE( scr->SubstLookupRecord );
    if ( scr->Class )
      FREE( scr->Class );
  }


  /* SubClassSet */

  static TT_Error  Load_SubClassSet( TTO_SubClassSet*  scs,
                                     PFace             input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort             n, count;
    ULong              cur_offset, new_offset, base_offset;

    TTO_SubClassRule*  scr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = scs->SubClassRuleCount = GET_UShort();

    FORGET_Frame();

    scs->SubClassRule = NULL;

    if ( ALLOC_ARRAY( scs->SubClassRule, count, TTO_SubClassRule ) )
      return error;

    scr = scs->SubClassRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubClassRule( &scr[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;
  }


  static void  Free_SubClassSet( TTO_SubClassSet*  scs )
  {
    UShort  n, count;


    if ( scs->SubClassRule )
    {
      count = scs->SubClassRuleCount;

      for ( n = 0; n < count; n++ )
        Free_SubClassRule( &scs->SubClassRule[n] );

      FREE( scs->SubClassRule );
    }
  }


  /* ContextSubstFormat2 */

  static TT_Error  Load_Context2( TTO_ContextSubstFormat2*  csf2,
                                  PFace                     input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort            n, count;
    ULong             cur_offset, new_offset, base_offset;

    TTO_SubClassSet*  scs;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &csf2->Coverage, input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_ClassDefinition( &csf2->ClassDef,
                                         input ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = csf2->SubClassSetCount = GET_UShort();

    FORGET_Frame();

    csf2->SubClassSet = NULL;

    if ( ALLOC_ARRAY( csf2->SubClassSet, count, TTO_SubClassSet ) )
      return error;

    scs = csf2->SubClassSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        return error;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      if ( new_offset != base_offset )      /* not a NULL offset */
      {
        cur_offset = FILE_Pos();
        if ( FILE_Seek( new_offset ) ||
             ( error = Load_SubClassSet( &scs[n], input ) ) != TT_Err_Ok )
          return error;
        (void)FILE_Seek( cur_offset );
      }
      else
      {
        /* we create a SubClassSet table with no entries */
        csf2->SubClassSet[n].SubClassRuleCount = 0;
        csf2->SubClassSet[n].SubClassRule      = NULL;
      }
    }

    return TT_Err_Ok;
  }


  static void  Free_Context2( TTO_ContextSubstFormat2*  csf2 )
  {
    UShort  n, count;


    if ( csf2->SubClassSet )
    {
      count = csf2->SubClassSetCount;

      for ( n = 0; n < count; n++ )
        Free_SubClassSet( &csf2->SubClassSet[n] );

      FREE( csf2->SubClassSet );
    }

    Free_ClassDefinition( &csf2->ClassDef );
    Free_Coverage( &csf2->Coverage );
  }


  /* ContextSubstFormat3 */

  static TT_Error  Load_Context3( TTO_ContextSubstFormat3*  csf3,
                                  PFace                     input )
  {
    DEFINE_LOAD_LOCALS( input->stream );

    UShort                  n, count;
    ULong                   cur_offset, new_offset, base_offset;

    TTO_Coverage*           c;
    TTO_SubstLookupRecord*  slr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 6L ) )
      return error;

    csf3->GlyphCount = GET_UShort();
    csf3->SubstCount = GET_UShort();
    new_offset       = GET_UShort() + base_offset;

    FORGET_Frame();

    csf3->Coverage = NULL;

    count = csf3->GlyphCount;

    if ( ALLOC_ARRAY( csf3->Coverage, count, TTO_Coverage ) )
      return error;

    c = csf3->Coverage;

    for ( n = 0; n < count; n++ )
    {
      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Coverage( &c[n], input ) ) != TT_Err_Ok )
        return error;
      (void)FILE_Seek( cur_offset );
    }

    csf3->SubstLookupRecord = NULL;

    count = csf3->SubstCount;

    if ( ALLOC_ARRAY( csf3->SubstLookupRecord, count,
                      TTO_SubstLookupRecord ) )
      return error;

    slr = csf3->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      return error;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_Context3( TTO_ContextSubstFormat3*  csf3 )
  {
    UShort  n, count;


    if ( csf3->SubstLookupRecord )
      FREE( csf3->SubstLookupRecord );

    if ( csf3->Coverage )
    {
      count = csf3->GlyphCount;

      for ( n = 0; n < count; n++ )
        Free_Coverage( &csf3->Coverage[n] );

      FREE( csf3->Coverage );
    }
  }


  /* ContextSubst */

  TT_Error  Load_LookupContext( TTO_ContextSubst*  cs,
                                PFace              input )
  {
    DEFINE_LOAD_LOCALS( input->stream );


    if ( ACCESS_Frame( 2L ) )
      return error;

    cs->SubstFormat = GET_UShort();

    FORGET_Frame();

    switch ( cs->SubstFormat )
    {
    case 1:
      return Load_Context1( &cs->csf.csf1, input );
    case 2:
      return Load_Context2( &cs->csf.csf2, input );
    case 3:
      return Load_Context3( &cs->csf.csf3, input );
    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;               /* never reached */
  }


  void  Free_LookupContext( TTO_ContextSubst*  cs )
  {
    switch ( cs->SubstFormat )
    {
    case 1:
      Free_Context1( &cs->csf.csf1 );
      break;
    case 2:
      Free_Context2( &cs->csf.csf2 );
      break;
    case 3:
      Free_Context3( &cs->csf.csf3 );
      break;
    default:
      break;
    }
  }


  /***********
   * GSUB API
   ***********/


  TT_Error  TT_GSUB_Select_Script( TTO_GSUBHeader*  gsub,
                                   TT_ULong         script_tag,
                                   TT_UShort*       script_index )
  {
    UShort             n;

    TTO_ScriptList*    sl = &gsub->ScriptList;
    TTO_ScriptRecord*  sr = sl->ScriptRecord;


    for ( n = 0; n < sl->ScriptCount; n++ )
      if ( script_tag == sr[n].ScriptTag )
      {
        *script_index = n;
        return TT_Err_Ok;
      }

    return TTO_Err_Not_Covered;
  }


  TT_Error  TT_GSUB_Select_Language( TTO_GSUBHeader*  gsub,
                                     TT_ULong         language_tag,
                                     TT_UShort        script_index,
                                     TT_UShort*       language_index )
  {
    UShort              n;

    TTO_ScriptList*     sl = &gsub->ScriptList;
    TTO_ScriptRecord*   sr = sl->ScriptRecord;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;


    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    for ( n = 0; n < s->LangSysCount; n++ )
      if ( language_tag == lsr[n].LangSysTag )
      {
        *language_index = n;
        return TT_Err_Ok;
      }

    return TTO_Err_Not_Covered;
  }


  /* selecting 0xFFFF for language_index asks for the values of the
     default language (DefaultLangSys)                              */

  TT_Error  TT_GSUB_Select_Feature( TTO_GSUBHeader*  gsub,
                                    TT_ULong         feature_tag,
                                    TT_UShort        script_index,
                                    TT_UShort        language_index,
                                    TT_UShort*       feature_index )
  {
    UShort              n;

    TTO_ScriptList*     sl = &gsub->ScriptList;
    TTO_ScriptRecord*   sr = sl->ScriptRecord;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;
    TTO_LangSys*        ls;
    UShort*             fi;

    TTO_FeatureList*    fl = &gsub->FeatureList;
    TTO_FeatureRecord*  fr = fl->FeatureRecord;


    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( language_index == 0xFFFF )
      ls = &s->DefaultLangSys;
    else
    {
      if ( language_index >= s->LangSysCount )
        return TT_Err_Invalid_Argument;

      ls = &lsr[language_index].LangSys;
    }
    fi = ls->FeatureIndex;

    for ( n = 0; n < ls->FeatureCount; n++ )
    {
      if ( fi[n] >= fl->FeatureCount )
        return TTO_Err_Invalid_GSUB_SubTable_Format;

      if ( feature_tag == fr[fi[n]].FeatureTag )
      {
        *feature_index = n;
        return TT_Err_Ok;
      }
    }

    return TTO_Err_Not_Covered;
  }


  /* The next three functions return a null-terminated list */

  TT_Error  TT_GSUB_Query_Scripts( TTO_GSUBHeader*  gsub,
                                   TT_ULong**       script_tag_list )
  {
    UShort             n;
    TT_Error           error;
    ULong*             stl;

    TTO_ScriptList*    sl = &gsub->ScriptList;
    TTO_ScriptRecord*  sr = sl->ScriptRecord;


    if ( ALLOC_ARRAY( stl, sl->ScriptCount + 1, TT_ULong ) )
      return error;

    for ( n = 0; n < sl->ScriptCount; n++ )
      stl[n] = sr[n].ScriptTag;
    stl[n] = 0;

    *script_tag_list = stl;

    return TT_Err_Ok;
  }


  TT_Error  TT_GSUB_Query_Languages( TTO_GSUBHeader*  gsub,
                                     TT_UShort        script_index,
                                     TT_ULong**       language_tag_list )
  {
    UShort              n;
    TT_Error            error;
    ULong*              ltl;

    TTO_ScriptList*     sl = &gsub->ScriptList;
    TTO_ScriptRecord*   sr = sl->ScriptRecord;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;


    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( ALLOC_ARRAY( ltl, s->LangSysCount + 1, TT_ULong ) )
      return error;

    for ( n = 0; n < s->LangSysCount; n++ )
      ltl[n] = lsr[n].LangSysTag;
    ltl[n] = 0;

    *language_tag_list = ltl;

    return TT_Err_Ok;
  }


  /* selecting 0xFFFF for language_index asks for the values of the
     default language (DefaultLangSys)                              */

  TT_Error  TT_GSUB_Query_Features( TTO_GSUBHeader*  gsub,
                                    TT_UShort        script_index,
                                    TT_UShort        language_index,
                                    TT_ULong**       feature_tag_list )
  {
    UShort              n;
    TT_Error            error;
    ULong*              ftl;

    TTO_ScriptList*     sl = &gsub->ScriptList;
    TTO_ScriptRecord*   sr = sl->ScriptRecord;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;
    TTO_LangSys*        ls;
    UShort*             fi;

    TTO_FeatureList*    fl = &gsub->FeatureList;
    TTO_FeatureRecord*  fr = fl->FeatureRecord;


    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( language_index == 0xFFFF )
      ls = &s->DefaultLangSys;
    else
    {
      if ( language_index >= s->LangSysCount )
        return TT_Err_Invalid_Argument;

      ls = &lsr[language_index].LangSys;
    }
    fi = ls->FeatureIndex;

    if ( ALLOC_ARRAY( ftl, ls->FeatureCount + 1, TT_ULong ) )
      return error;

    for ( n = 0; n < ls->FeatureCount; n++ )
    {
      if ( fi[n] >= fl->FeatureCount )
      {
        FREE( ftl );
        return TTO_Err_Invalid_GSUB_SubTable_Format;
      }
      ftl[n] = fr[fi[n]].FeatureTag;
    }
    ftl[n] = 0;

    *feature_tag_list = ftl;

    return TT_Err_Ok;
  }


/* END */
