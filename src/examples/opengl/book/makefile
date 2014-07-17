############################################################################
#
#                   Copyright (c) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for the OpenGL demos.
#
# $Date:   04 Apr 1997 19:20:02  $ $Author:   KendallB  $
#
#############################################################################

.INCLUDE: "../common.mak"

# Names of all executable files built

EXEFILES        = accanti$E anti$E bezmesh$E checker$E depthcue$E       \
                  dof$E fog$E fogindex$E material$E mipmap$E nurbs$E    \
                  pickdp$E plane$E polys$E scclight$E scene$E sceneba$E \
                  scenef$E stencil$E stroke$E surface$E teaamb$E        \
                  texture$E
EXELIBS         = $(BASELIBS)

.IF $(USE_WIN32)
RCFILE          = demo$R
.ELSE
RCFILE          =
.ENDIF

all: $(EXEFILES)

accanti$E: accanti$O $(RCFILE)
anti$E: anti$O $(RCFILE)
bezmesh$E: bezmesh$O $(RCFILE)
checker$E: checker$O $(RCFILE)
depthcue$E: depthcue$O $(RCFILE)
dof$E: dof$O $(RCFILE)
fog$E: fog$O $(RCFILE)
fogindex$E: fogindex$O $(RCFILE)
material$E: material$O $(RCFILE)
mipmap$E: mipmap$O $(RCFILE)
nurbs$E: nurbs$O $(RCFILE)
pickdp$E: pickdp$O $(RCFILE)
plane$E: plane$O $(RCFILE)
polys$E: polys$O $(RCFILE)
scclight$E: scclight$O $(RCFILE)
scene$E: scene$O $(RCFILE)
sceneba$E: sceneba$O $(RCFILE)
scenef$E: scenef$O $(RCFILE)
stencil$E: stencil$O $(RCFILE)
stroke$E: stroke$O $(RCFILE)
surface$E: surface$O $(RCFILE)
teaamb$E: teaamb$O $(RCFILE)
teapots$E: teapot$O $(RCFILE)
texture$E: texture$O $(RCFILE)

.IF $(UNIX)
.INCLUDE: "$(SCITECH)/makedefs/common.mk"
.ELSE
.INCLUDE: "$(SCITECH)\makedefs\common.mk"
.ENDIF
