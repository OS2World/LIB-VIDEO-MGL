md objects
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx infer.c
copy infer.obj objects
del infer.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx make.c
copy make.obj objects
del make.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx stat.c
copy stat.obj objects
del stat.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx expand.c
copy expand.obj objects
del expand.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx dmstring.c
copy dmstring.obj objects
del dmstring.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx hash.c
copy hash.obj objects
del hash.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx dag.c
copy dag.obj objects
del dag.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx dmake.c
copy dmake.obj objects
del dmake.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx path.c
copy path.obj objects
del path.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx imacs.c
copy imacs.obj objects
del imacs.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx sysintf.c
copy sysintf.obj objects
del sysintf.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx parse.c
copy parse.obj objects
del parse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx getinp.c
copy getinp.obj objects
del getinp.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx quit.c
copy quit.obj objects
del quit.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx state.c
copy state.obj objects
del state.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx dmdump.c
copy dmdump.obj objects
del dmdump.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx macparse.c
copy macparse.obj objects
del macparse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx rulparse.c
copy rulparse.obj objects
del rulparse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx percent.c
copy percent.obj objects
del percent.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx function.c
copy function.obj objects
del function.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx os2\ruletab.c
copy ruletab.obj objects
del ruletab.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx os2\dchdir.c
copy dchdir.obj objects
del dchdir.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx os2\switchar.c
copy switchar.obj objects
del switchar.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx msdos\arlib.c
copy arlib.obj objects
del arlib.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx msdos\dstrlwr.c
copy dstrlwr.obj objects
del dstrlwr.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx msdos\runargv.c
copy runargv.obj objects
del runargv.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /O- /Ti/Tm/Tx os2\ibm\tempnam.c
copy tempnam.obj objects
del tempnam.obj
ilink @os2\ibm\icc\obj_dbg.rsp
copy os2\ibm\icc\template.mk startup\config.mk
