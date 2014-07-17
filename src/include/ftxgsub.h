/*******************************************************************
 *
 *  ftxgsub.h
 *
 *    TrueType Open GSUB table support
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

#ifndef FTXGSUB_H
#define FTXGSUB_H

#ifdef __cplusplus
extern "C" {
#endif

#define TTO_Err_Invalid_GSUB_SubTable_Format  0x1010
#define TTO_Err_Invalid_GSUB_SubTable         0x1011


/* Lookup types for glyph substitution */

#define LOOKUP_SINGLE     1
#define LOOKUP_MULTIPLE   2
#define LOOKUP_ALTERNATE  3
#define LOOKUP_LIGATURE   4
#define LOOKUP_CONTEXT    5


  struct  TTO_GSUBHeader_
  {
    TT_Bool          loaded;
    TT_ULong         offset;

    TT_Fixed         Version;

    TTO_ScriptList   ScriptList;
    TTO_FeatureList  FeatureList;
    TTO_LookupList   LookupList;
  };

  typedef struct TTO_GSUBHeader_  TTO_GSUBHeader;


  /* Lookup type 1 */

  struct  TTO_SingleSubstFormat1_
  {
    TT_Short  DeltaGlyphID;             /* constant added to get
                                           substitution glyph index */
  };

  typedef struct TTO_SingleSubstFormat1_  TTO_SingleSubstFormat1;


  struct  TTO_SingleSubstFormat2_
  {
    TT_UShort   GlyphCount;             /* number of glyph IDs in
                                           Substitute array              */
    TT_UShort*  Substitute;             /* array of substitute glyph IDs */
  };

  typedef struct TTO_SingleSubstFormat2_  TTO_SingleSubstFormat2;


  struct  TTO_SingleSubst_
  {
    TT_UShort     SubstFormat;          /* 1 or 2         */
    TTO_Coverage  Coverage;             /* Coverage table */

    union
    {
      TTO_SingleSubstFormat1  ssf1;
      TTO_SingleSubstFormat2  ssf2;
    } ssf;
  };

  typedef struct TTO_SingleSubst_  TTO_SingleSubst;


  /* Lookup type 2 */

  struct  TTO_Sequence_
  {
    TT_UShort   GlyphCount;             /* number of glyph IDs in the
                                           Substitute array           */
    TT_UShort*  Substitute;             /* string of glyph IDs to
                                           substitute                 */
  };

  typedef struct TTO_Sequence_  TTO_Sequence;


  struct  TTO_MultipleSubst_
  {
    TT_UShort      SubstFormat;         /* always 1                  */
    TTO_Coverage   Coverage;            /* Coverage table            */
    TT_UShort      SequenceCount;       /* number of Sequence tables */
    TTO_Sequence*  Sequence;            /* array of Sequence tables  */
  };

  typedef struct TTO_MultipleSubst_  TTO_MultipleSubst;


  /* Lookup type 3 */

  struct  TTO_AlternateSet_
  {
    TT_UShort   GlyphCount;             /* number of glyph IDs in the
                                           Alternate array              */
    TT_UShort*  Alternate;              /* array of alternate glyph IDs */
  };

  typedef struct TTO_AlternateSet_  TTO_AlternateSet;


  struct  TTO_AlternateSubst_
  {
    TT_UShort          SubstFormat;     /* always 1                      */
    TTO_Coverage       Coverage;        /* Coverage table                */
    TT_UShort          AlternateSetCount;
                                        /* number of AlternateSet tables */
    TTO_AlternateSet*  AlternateSet;    /* array of AlternateSet tables  */
  };

  typedef struct TTO_AlternateSubst_  TTO_AlternateSubst;


  /* Lookup type 4 */

  struct  TTO_Ligature_
  {
    TT_UShort   LigGlyph;               /* glyphID of ligature
                                           to substitute                    */
    TT_UShort   ComponentCount;         /* number of components in ligature */
    TT_UShort*  Component;              /* array of component glyph IDs     */
  };

  typedef struct TTO_Ligature_  TTO_Ligature;


  struct  TTO_LigatureSet_
  {
    TT_UShort      LigatureCount;       /* number of Ligature tables */
    TTO_Ligature*  Ligature;            /* array of Ligature tables  */
  };

  typedef struct TTO_LigatureSet_  TTO_LigatureSet;


  struct  TTO_LigatureSubst_
  {
    TT_UShort         SubstFormat;      /* always 1                     */
    TTO_Coverage      Coverage;         /* Coverage table               */
    TT_UShort         LigatureSetCount; /* number of LigatureSet tables */
    TTO_LigatureSet*  LigatureSet;      /* array of LigatureSet tables  */
  };

  typedef struct TTO_LigatureSubst_  TTO_LigatureSubst;


  /* Lookup type 5 */

  struct  TTO_SubstLookupRecord_
  {
    TT_UShort  SequenceIndex;           /* index into current
                                           glyph sequence               */
    TT_UShort  LookupListIndex;         /* Lookup to apply to that pos. */
  };

  typedef struct TTO_SubstLookupRecord_  TTO_SubstLookupRecord;


  struct  TTO_SubRule_
  {
    TT_UShort               GlyphCount; /* total number of input glyphs */
    TT_UShort               SubstCount; /* number of SubstLookupRecords */
    TT_UShort*              Input;      /* array of input glyph IDs     */
    TTO_SubstLookupRecord*  SubstLookupRecord;
                                        /* array of SubstLookupRecords  */
  };

  typedef struct TTO_SubRule_  TTO_SubRule;


  struct  TTO_SubRuleSet_
  {
    TT_UShort     SubRuleCount;         /* number of SubRule tables */
    TTO_SubRule*  SubRule;              /* array of SubRule tables  */
  };

  typedef struct TTO_SubRuleSet_  TTO_SubRuleSet;


  struct  TTO_ContextSubstFormat1_
  {
    TTO_Coverage     Coverage;          /* Coverage table              */
    TT_UShort        SubRuleSetCount;   /* number of SubRuleSet tables */
    TTO_SubRuleSet*  SubRuleSet;        /* array of SubRuleSet tables  */
  };

  typedef struct TTO_ContextSubstFormat1_  TTO_ContextSubstFormat1;


  struct  TTO_SubClassRule_
  {
    TT_UShort               GlyphCount; /* total number of context classes */
    TT_UShort               SubstCount; /* number of SubstLookupRecords    */
    TT_UShort*              Class;      /* array of classes                */
    TTO_SubstLookupRecord*  SubstLookupRecord;
                                        /* array of substitution lookups   */
  };

  typedef struct TTO_SubClassRule_  TTO_SubClassRule;


  struct  TTO_SubClassSet_
  {
    TT_UShort          SubClassRuleCount;
                                        /* number of SubClassRule tables */
    TTO_SubClassRule*  SubClassRule;    /* array of SubClassRule tables  */
  };

  typedef struct TTO_SubClassSet_  TTO_SubClassSet;


  struct  TTO_ContextSubstFormat2_
  {
    TTO_Coverage         Coverage;      /* Coverage table               */
    TTO_ClassDefinition  ClassDef;      /* ClassDef table               */
    TT_UShort            SubClassSetCount;
                                        /* number of SubClassSet tables */
    TTO_SubClassSet*     SubClassSet;   /* array of SubClassSet tables  */
  };

  typedef struct TTO_ContextSubstFormat2_  TTO_ContextSubstFormat2;


  struct  TTO_ContextSubstFormat3_
  {
    TT_UShort               GlyphCount; /* number of input glyphs        */
    TT_UShort               SubstCount; /* number of SubstLookupRecords  */
    TTO_Coverage*           Coverage;   /* array of Coverage tables      */
    TTO_SubstLookupRecord*  SubstLookupRecord;
                                        /* array of substitution lookups */
  };

  typedef struct TTO_ContextSubstFormat3_  TTO_ContextSubstFormat3;


  struct  TTO_ContextSubst_
  {
    TT_UShort  SubstFormat;             /* 1, 2, or 3 */

    union
    {
      TTO_ContextSubstFormat1  csf1;
      TTO_ContextSubstFormat2  csf2;
      TTO_ContextSubstFormat3  csf3;
    } csf;
  };

  typedef struct TTO_ContextSubst_  TTO_ContextSubst;


  union  TTO_GSUB_SubTable_
  {
    TTO_SingleSubst     single;
    TTO_MultipleSubst   multiple;
    TTO_AlternateSubst  alternate;
    TTO_LigatureSubst   ligature;
    TTO_ContextSubst    context;
  };

  typedef union TTO_GSUB_SubTable_  TTO_GSUB_SubTable;


  /* A simple string object.  It can both `send' and `receive' data.
     In case of sending, `length' and `pos' will be used.  In case of
     receiving, `pos' points to the latest glyph, and `allocated'
     specifies the amount of allocated memory (and the `length' field
     will be ignored).  The routine Add_String() will increase the
     amount of memory if necessary.  After end of receive, `length'
     will be set to the value of `pos', and `pos' will be set to zero.

     As a consequence, NEVER modify any elements of the structure!
     You should rather copy its contents if necessary.

     Add_String() will also handle allocation; you should use free()
     in case you want to destroy this object.                          */

  struct  TTO_String_
  {
    TT_ULong    length;
    TT_ULong    pos;
    TT_ULong    allocated;
    TT_UShort*  string;
  };

  typedef struct TTO_String_  TTO_String;


  /* finally, the GSUB API */

  TT_Error  TT_Init_GSUB_Extension( TT_Engine  engine );
  TT_Error  TT_Load_GSUB_Table( TT_Face          face,
                                TTO_GSUBHeader*  gsub );

  TT_Error  TT_GSUB_Select_Script( TTO_GSUBHeader*  gsub,
                                   TT_ULong         script_tag,
                                   TT_UShort*       script_index );
  TT_Error  TT_GSUB_Select_Language( TTO_GSUBHeader*  gsub,
                                     TT_ULong         language_tag,
                                     TT_UShort        script_index,
                                     TT_UShort*       language_index );
  TT_Error  TT_GSUB_Select_Feature( TTO_GSUBHeader*  gsub,
                                    TT_ULong         feature_tag,
                                    TT_UShort        script_index,
                                    TT_UShort        language_index,
                                    TT_UShort*       feature_index );

  TT_Error  TT_GSUB_Query_Scripts( TTO_GSUBHeader*  gsub,
                                   TT_ULong**       script_tag_list );
  TT_Error  TT_GSUB_Query_Languages( TTO_GSUBHeader*  gsub,
                                     TT_UShort        script_index,
                                     TT_ULong**       language_tag_list );
  TT_Error  TT_GSUB_Query_Features( TTO_GSUBHeader*  gsub,
                                    TT_UShort        script_index,
                                    TT_UShort        language_index,
                                    TT_ULong**       feature_tag_list );

  TT_Error  TT_GSUB_Apply( TTO_GSUBHeader*  gsub,
                           TT_UShort        script_index,
                           TT_UShort        language_index,
                           TT_UShort        feature_index,
                           TTO_String*      in,
                           TTO_String*      out );

#ifdef __cplusplus
}
#endif

#endif /* FTXGSUB_H */


/* END */
