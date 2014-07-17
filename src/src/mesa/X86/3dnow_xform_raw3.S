#include "assyntax.h"

   SEG_TEXT


#if !defined(NASM_ASSEMBLER) && !defined(MASM_ASSEMBLER)
#define LLBL(a) .L ## a
#else
#define LLBL(a) a
#endif



ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_general_raw)
GLNAME( gl_3dnow_transform_points3_general_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(4), REGOFF(16, ECX) )
    OR_B      ( CONST(15), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TPGR_4 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(16, ECX), MM0 )  /* m10           | m00                */

    MOVD      ( REGOFF(4, ECX), MM1 )   /*               | m01                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM1 )  /* m11           | m01                */

    MOVQ      ( REGOFF(32, ECX), MM2 )  /* m21           | m20                */
    MOVQ      ( REGOFF(48, ECX), MM3 )  /* m31           | m30                */

    PUSH_L    ( EAX )
    PUSH_L    ( EDX )
    PUSH_L    ( ESI )

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TPGR_2 ):

    MOVQ      ( MM4, MM5 )              /* x1            | x0                 */
    ADD_L     ( EDI, EAX )              /* next vertex                        */

    PFMUL     ( MM0, MM4 )              /* x1*m10        | x0*m00             */
    PUNPCKLDQ ( MM6, MM6 )              /* x2            | x2                 */

    PFMUL     ( MM1, MM5 )              /* x1*m11        | x0*m01             */
    PFACC     ( MM5, MM4 )              /* x0*m01+x1*m11 | x0*m00+x1*m10      */

    PFMUL     ( MM2, MM6 )              /* x2*m21        | x2*m20             */
    PFADD     ( MM3, MM6 )              /* x2*m21+m31    | x2*m20+m30         */

    PFADD     ( MM4, MM6 )              /* r1            | r0                 */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVQ      ( MM6, REGOFF(-16, EDX) ) /* write r0, r1                       */

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */

    JA        ( LLBL( G3TPGR_2 ) )      /* cnt > 0 ? -> process next vertex   */
                                        /* and now the second stripe ...      */
    POP_L     ( ESI )                   /* reset counter & pointers           */
    POP_L     ( EDX )
    POP_L     ( EAX )

    MOVD      ( REGOFF(8, ECX), MM0 )   /*               | m02                */
    PUNPCKLDQ ( REGOFF(24, ECX), MM0 )  /* m12           | m02                */

    MOVD      ( REGOFF(12, ECX), MM1 )  /*               | m03                */
    PUNPCKLDQ ( REGOFF(28, ECX), MM1 )  /* m13           | m03                */

    MOVQ      ( REGOFF(40, ECX), MM2 )  /* m23           | m22                */
    MOVQ      ( REGOFF(56, ECX), MM3 )  /* m33           | m32                */

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TPGR_3 ):

    ADD_L     ( EDI, EAX )              /* next vertex                        */
    MOVQ      ( MM4, MM5 )              /* x1            | x0                 */

    PFMUL     ( MM0, MM4 )              /* x1*m12        | x0*m02             */
    PUNPCKLDQ ( MM6, MM6 )              /* x2            | x2                 */

    PFMUL     ( MM1, MM5 )              /* x1*m13        | x0*m03             */
    PFACC     ( MM5, MM4 )              /* x0*m03+x1*m13 | x0*m02+x1*m12      */

    PFMUL     ( MM2, MM6 )              /* x2*m23        | x2*m22             */
    PFADD     ( MM3, MM6 )              /* x2*m23+m33    | x2*m22+m32         */

    PFADD     ( MM4, MM6 )              /* r3            | r2                 */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVQ      ( MM6, REGOFF(-8, EDX) )  /* write r2, r3                       */

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */

    JA        ( LLBL( G3TPGR_3 ) )      /* cnt > 0 ? -> process next vertex   */

LLBL( G3TPGR_4 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET






ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_identity_raw)
GLNAME( gl_3dnow_transform_points3_identity_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(3), REGOFF(16, ECX) )
    OR_B      ( CONST(7), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TPIR_4 ) )

    MOVQ      ( REGIND(EAX), MM0 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM1 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TPIR_3 ):

    ADD_L     ( EDI, EAX )              /* next vertex                        */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVQ      ( MM0, REGOFF(-16, EDX) ) /* r1            | r0                 */

    MOVD      ( MM1, REGOFF(-8, EDX) )  /*               | r2                 */
    MOVQ      ( REGIND(EAX), MM0 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM1 )   /*               | x2                 */
    JA        ( LLBL( G3TPIR_3 ) )      /* cnt > 0 ? -> process next vertex   */

LLBL( G3TPIR_4 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_2d_raw)
GLNAME( gl_3dnow_transform_points3_2d_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(3), REGOFF(16, ECX) )
    OR_B      ( CONST(7), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TP2R_3 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(16, ECX), MM0 )  /* m10           | m00                */

    MOVD      ( REGOFF(4, ECX), MM1 )   /*               | m01                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM1 )  /* m11           | m01                */

    MOVQ      ( REGOFF(48, ECX), MM2 )  /* m31           | m30                */
    MOVQ      ( REGIND(EAX), MM3 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */

ALIGNTEXT32
LLBL( G3TP2R_2 ):

    MOVQ      ( MM3, MM4 )              /* x1            | x0                 */
    PFMUL     ( MM0, MM3 )              /* x1*m10        | x0*m00             */

    ADD_L     ( EDI, EAX )              /* next vertex                        */
    PFMUL     ( MM1, MM4 )              /* x1*m11        | x0*m01             */

    PFACC     ( MM4, MM3 )              /* x0*m00+x1*m10 | x0*m01+x1*m11      */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    PFADD     ( MM2, MM3 )              /* x0*...*m10+m30| x0*m01+x1*m11+m31  */
    DEC_L     ( ESI )                   /* decrement vertex counter           */

    MOVD      ( MM5, REGOFF(-8, EDX) )  /* write r2 (=x2)                     */
    MOVQ      ( MM3, REGOFF(-16, EDX) ) /* write r0, r1                       */

    MOVQ      ( REGIND(EAX), MM3 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */

    NOP
    JA        ( LLBL( G3TP2R_2 ) )      /* cnt > 0 ? -> process next vertex   */

LLBL( G3TP2R_3 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET





ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_2d_no_rot_raw)
GLNAME( gl_3dnow_transform_points3_2d_no_rot_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(3), REGOFF(16, ECX) )
    OR_B      ( CONST(7), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TP2NRR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )  /* m11           | m00                */

    MOVQ      ( REGOFF(48, ECX), MM1 )  /* m31           | m30                */
    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TP2NRR_2 ):
    ADD_L     ( CONST(16), EDX )        /* next r                             */
    PFMUL     ( MM0, MM4 )              /* x1*m11        | x0*m00             */

    ADD_L     ( EDI, EAX )              /* next vertex                        */
    PFADD     ( MM1, MM4 )              /* x1*m11+m31    | x0*m00+m30         */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVD      ( MM5, REGOFF(-8, EDX) )  /* write r2 (=x2)                     */

    MOVQ      ( MM4, REGOFF(-16, EDX) ) /* write r0, r1                       */
    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */
    JA        ( LLBL( G3TP2NRR_2 ) )    /* cnt > 0 ? -> process next vertex   */

LLBL( G3TP2NRR_3 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_3d_raw)
GLNAME( gl_3dnow_transform_points3_3d_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(3), REGOFF(16, ECX) )
    OR_B      ( CONST(7), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TP3R_4 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(16, ECX), MM0 )  /* m10           | m00                */

    MOVD      ( REGOFF(4, ECX), MM1 )   /*               | m01                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM1 )  /*               | m11                */

    MOVQ      ( REGOFF(32, ECX), MM2 )  /* m21           | m20                */
    MOVQ      ( REGOFF(48, ECX), MM3 )  /* m31           | m30                */

    PUSH_L    ( EAX )
    PUSH_L    ( EDX )
    PUSH_L    ( ESI )

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */


ALIGNTEXT32
    NOP
    NOP

LLBL( G3TP3R_2 ):
    MOVQ      ( MM4, MM5 )              /* x1            | x0                 */
    PFMUL     ( MM0, MM4 )              /* x1*m10        | x0*m00             */

    PUNPCKLDQ ( MM6, MM6 )              /* x2            | x2                 */
    PFMUL     ( MM1, MM5 )              /* x1*m11        | x0*m01             */

    ADD_L     ( EDI, EAX )              /* next vertex                        */
    PFMUL     ( MM2, MM6 )              /* x2*m21        | x2*m20             */

    PFACC     ( MM5, MM4 )              /* x0*m01+x1*m11 | x0*m00+x1*m10      */
    PFADD     ( MM3, MM6 )              /* x2*m21+m31    | x2*m20+m30         */

    PFADD     ( MM4, MM6 )              /* r1            | r0                 */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVQ      ( MM6, REGOFF(-16, EDX) ) /* write r0, r1                       */

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */

    JA        ( LLBL( G3TP3R_2 ) )      /* cnt > 0 ? -> process next vertex   */
                                        /* and now the second stripe ...      */
    MOVD      ( REGOFF(8, ECX), MM0 )   /*               | m02                */
    PUNPCKLDQ ( REGOFF(24, ECX), MM0 )  /* m12           | m02                */

    MOVD      ( REGOFF(40, ECX), MM2 )  /*               | m22                */
    MOVD      ( REGOFF(56, ECX), MM3 )  /*               | m32                */

    POP_L     ( ESI )                   /* reset counter & pointers           */
    POP_L     ( EDX )
    POP_L     ( EAX )

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TP3R_3 ):
    PFMUL     ( MM2, MM6 )              /*               | x2*m22             */
    MOVQ      ( MM4, MM5 )              /* x1            | x0                 */

    PFMUL     ( MM0, MM4 )              /* x1*m12        | x0*m02             */
    PFADD     ( MM3, MM6 )              /*               | x2*m22+m32         */

    PFMUL     ( MM1, MM5 )              /* x1*m13        | x0*m03             */
    ADD_L     ( EDI, EAX )              /* next vertex                        */

    PFACC     ( MM5, MM4 )              /* x0*m03+x1*m13 | x0*m02+x1*m12      */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    PFADD     ( MM4, MM6 )              /*               | r2                 */
    DEC_L     ( ESI )                   /* decrement vertex counter           */

    MOVD      ( MM6, REGOFF(-8, EDX) )  /* write r2                           */
    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM6 )   /*               | x2                 */
    JA        ( LLBL( G3TP3R_3 ) )      /* cnt > 0 ? -> process next vertex   */

LLBL( G3TP3R_4 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_3d_no_rot_raw)
GLNAME( gl_3dnow_transform_points3_3d_no_rot_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(3), REGOFF(16, ECX) )
    OR_B      ( CONST(7), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TP3NRR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )  /* m11           | m00                */

    MOVD      ( REGOFF(40, ECX), MM2 )  /*               | m22                */
    PUNPCKLDQ ( MM2, MM2 )              /* m22           | m22                */

    MOVQ      ( REGOFF(48, ECX), MM1 )  /* m31           | m30                */
    MOVD      ( REGOFF(56, ECX), MM3 )  /*               | m32                */

    PUNPCKLDQ ( MM3, MM3 )              /* m32           | m32                */
    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */


ALIGNTEXT32
LLBL( G3TP3NRR_2 ):
    PFMUL     ( MM0, MM4 )              /* x1*m11        | x0*m00             */
    ADD_L     ( EDI, EAX )              /* next vertex                        */

    PFADD     ( MM1, MM4 )              /* x1*m11+m31    | x0*m00+m30         */
    PFMUL     ( MM2, MM5 )              /*               | x2*m22             */

    PFADD     ( MM3, MM5 )              /*               | x2*m22+m32         */
    MOVQ      ( MM4, REGIND(EDX) )      /* write r0, r1                       */

    ADD_L     ( CONST(16), EDX )        /* next r                             */
    DEC_L     ( ESI )                   /* decrement vertex counter           */

    MOVD      ( MM5, REGOFF(-8, EDX) )  /* write r2                           */
    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */

    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */
    JA        ( LLBL( G3TP3NRR_2 ) )    /* cnt > 0 ? -> process next vertex   */

LLBL( G3TP3NRR_3 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME(gl_3dnow_transform_points3_perspective_raw)
GLNAME( gl_3dnow_transform_points3_perspective_raw ):

    PUSH_L    ( ESI )
    MOV_L     ( REGOFF(8, ESP), ECX )
    MOV_L     ( REGOFF(12, ESP), ESI )
    MOV_L     ( REGOFF(16, ESP), EAX )
    MOV_L     ( CONST(4), REGOFF(16, ECX) )
    OR_B      ( CONST(15), REGOFF(20, ECX) )
    MOV_L     ( REGOFF(8, EAX), EDX )
    MOV_L     ( EDX, REGOFF(8, ECX) )

ALIGNTEXT32

    PUSH_L    ( EDI )
    PUSH_L    ( ESI )

    MOV_L     ( REGOFF(4, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(8, EAX), ESI )
    MOV_L     ( REGOFF(12, EAX), EDI )
    MOV_L     ( REGOFF(4, EAX), EAX )

    FEMMS

    CMP_L     ( CONST(0), ESI )
    JE        ( LLBL( G3TPPR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )      /*               | m00                */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )  /* m11           | m00                */

    MOVQ      ( REGOFF(32, ECX), MM1 )  /* m21           | m20                */
    MOVD      ( REGOFF(40, ECX), MM2 )  /*               | m22                */

    MOVD      ( REGOFF(56, ECX), MM3 )  /*               | m32                */

ALIGNTEXT32
    NOP                                 /* align label                        */
    NOP
LLBL( G3TPPR_2 ):

    PXOR      ( MM7, MM7 )              /* 0             | 0                  */
    MOVD      ( REGOFF(8, EAX), MM5 )   /*               | x2                 */

    MOVQ      ( REGIND(EAX), MM4 )      /* x1            | x0                 */
    ADD_L     ( EDI, EAX )              /* next vertex                        */

    MOVQ      ( MM5, MM6 )              /*               | x2                 */
    PFMUL     ( MM0, MM4 )              /* x1*m11        | x0*m00             */

    PFSUB     ( MM5, MM7 )              /* -x2           | -x2                */
    PFMUL     ( MM2, MM6 )              /*               | x2*m22             */

    PUNPCKLDQ ( MM5, MM5 )              /* x2            | x2                 */
    MOVD      ( MM7, REGOFF(12, EDX) )  /* write r3                           */

    PFMUL     ( MM1, MM5 )              /* x2*m21        | x2*m20             */
    PFADD     ( MM3, MM6 )              /*               | x2*m22+m32         */

    PFADD     ( MM4, MM5 )              /* x1*m11+x2*m21 | x0*m00+x2*m20      */
    ADD_L     ( CONST(16), EDX )        /* next r                             */

    DEC_L     ( ESI )                   /* decrement vertex counter           */
    MOVD      ( MM6, REGOFF(-8, EDX) )  /* write r2                           */

    MOVQ      ( MM5, REGOFF(-16, EDX) ) /* write r0, r1                       */
    JA        ( LLBL( G3TPPR_2 ) )      /* cnt > 0 ? -> process next vertex   */

LLBL( G3TPPR_3 ):
    FEMMS

    POP_L     ( ESI )
    POP_L     ( EDI )

    POP_L     ( ESI )
    RET





