#############################################################################
#
#                   Copyright (C) 1996 SciTech Software.
#                           All rights reserved.
#
# Descripton:   Makefile for command line utilities for SciTech makefiles.
#
#############################################################################

.IF $(UNIX)
all: loc$E makedep$E k_echo$E k_rm$E k_cp$E k_md$E k_cc$E
.ELSE
all: dj_ld$E expndinc$E loc$E makedef$E makedep$E makedepx$E \
	 trimlib$E k_echo$E k_rm$E k_cp$E rclink$E bclink$E k_md$E k_cc$E
.ENDIF

dj_ld$E: 	dj_ld$O
expndinc$E: expndinc$O
loc$E:      loc$O
makedef$E:  makedef$O
makedep$E:  makedep$O getopt$O
makedepx$E: makedepx$O getopt$O
rclink$E:   rclink$O
bclink$E:   bclink$O
trimlib$E:  trimlib$O
tolower$E:  tolower$O getopt$O
k_echo$E:   k_echo$O
k_rm$E:     k_rm$O
k_cp$E:     k_cp$O
k_md$E:     k_md$O
k_cc$E:		k_cc$O getopt$O
p4_cvs$E:   p4_cvs$O
cvs_date$E: cvs_date$O

DEPEND_OBJ = dj_ld$O expndinc$O loc$O makedef$O makedep$O getopt$O \
			 makedep$O rclink$O bclink$O trimlib$O tolower$O \
			 k_echo$O k_rm$O k_cp$O k_md$O 

.INCLUDE: "$(SCITECH)/makedefs/common.mk"
