md objects
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 infer.c
copy infer.obj objects
del infer.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 make.c
copy make.obj objects
del make.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 stat.c
copy stat.obj objects
del stat.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 expand.c
copy expand.obj objects
del expand.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 dmstring.c
copy dmstring.obj objects
del dmstring.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 hash.c
copy hash.obj objects
del hash.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 dag.c
copy dag.obj objects
del dag.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 dmake.c
copy dmake.obj objects
del dmake.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 path.c
copy path.obj objects
del path.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 imacs.c
copy imacs.obj objects
del imacs.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 sysintf.c
copy sysintf.obj objects
del sysintf.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 parse.c
copy parse.obj objects
del parse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 getinp.c
copy getinp.obj objects
del getinp.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 quit.c
copy quit.obj objects
del quit.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 state.c
copy state.obj objects
del state.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 dmdump.c
copy dmdump.obj objects
del dmdump.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 macparse.c
copy macparse.obj objects
del macparse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 rulparse.c
copy rulparse.obj objects
del rulparse.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 percent.c
copy percent.obj objects
del percent.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 function.c
copy function.obj objects
del function.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 os2\ruletab.c
copy ruletab.obj objects
del ruletab.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 os2\dchdir.c
copy dchdir.obj objects
del dchdir.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 os2\switchar.c
copy switchar.obj objects
del switchar.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 msdos\dirbrk.c
copy dirbrk.obj objects
del dirbrk.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 msdos\arlib.c
copy arlib.obj objects
del arlib.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 msdos\dstrlwr.c
copy dstrlwr.obj objects
del dstrlwr.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 msdos\runargv.c
copy runargv.obj objects
del runargv.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 msdos\rmprq.c
copy rmprq.obj objects
del rmprq.obj
icc /c /I. /Ios2 /Ios2\ibm /Ios2\ibm\icc /Sp1 /Q /Fi- /O /G5 os2\ibm\tempnam.c
copy tempnam.obj objects
del tempnam.obj
ilink @os2\ibm\icc\obj.rsp
copy os2\ibm\icc\template.mk startup\config.mk
