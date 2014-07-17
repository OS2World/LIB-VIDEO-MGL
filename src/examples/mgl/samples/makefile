############################################################################
#
#               Copyright (C) 1991-1997 SciTech Software, Inc.
#                            All rights reserved.
#
# Descripton:   Makefile for sample programs. Requires the SciTech
#               Software makefile definitions package to be installed.
#
#############################################################################

.INCLUDE: "../common.mak"

MGLSAMP_OBJ     = mglsamp$O getopt$O
.IF $(MGLWIN)
MGLSAMP_OBJ     += mglsamp$R
.ENDIF
EXELIBS         = $(BASELIBS)

EXEFILES        = bitblt$E cliprgn$E ellipses$E lines$E mgldemo$E mouse$E 	\
				  mousedb$E pageflip$E palette$E pixelfmt$E polys$E rects$E	\
				  regions$E sprites$E stretch$E textdemo$E viewport$E 		\
                  loadbmp$E loadpcx$E loadjpeg$E loadpng$E truetype$E 

all: $(EXEFILES)

bitblt$E:   bitblt$O $(MGLSAMP_OBJ)
cliprgn$E:  cliprgn$O $(MGLSAMP_OBJ)
ellipses$E: ellipses$O $(MGLSAMP_OBJ)
lines$E:    lines$O $(MGLSAMP_OBJ)
mgldemo$E:  mgldemo$O $(MGLSAMP_OBJ)
mouse$E:    mouse$O $(MGLSAMP_OBJ)
mousedb$E:  mousedb$O $(MGLSAMP_OBJ)
pageflip$E: pageflip$O $(MGLSAMP_OBJ)
palette$E:  palette$O $(MGLSAMP_OBJ)
pixelfmt$E: pixelfmt$O $(MGLSAMP_OBJ)
polys$E:    polys$O $(MGLSAMP_OBJ)
rects$E:    rects$O $(MGLSAMP_OBJ)
regions$E:  regions$O $(MGLSAMP_OBJ)
sprites$E:  sprites$O $(MGLSAMP_OBJ)
stretch$E:  stretch$O $(MGLSAMP_OBJ)
textdemo$E: textdemo$O $(MGLSAMP_OBJ)
viewport$E: viewport$O $(MGLSAMP_OBJ)
loadbmp$E:  loadbmp$O $(MGLSAMP_OBJ)
loadpcx$E:  loadpcx$O $(MGLSAMP_OBJ)
loadjpeg$E: loadjpeg$O $(MGLSAMP_OBJ)
loadpng$E:  loadpng$O $(MGLSAMP_OBJ)
truetype$E: truetype$O $(MGLSAMP_OBJ)
weird$E:    weird$O $(MGLSAMP_OBJ)
test$E:     test$O $(MGLSAMP_OBJ)

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
