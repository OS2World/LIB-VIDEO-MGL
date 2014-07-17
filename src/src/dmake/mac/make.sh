mkdir objects
c -c -I. -I :mac -d _MPW -s infer -sym off infer.c
mv infer.o objects
c -c -I. -I :mac -d _MPW -s make -sym off make.c
mv make.o objects
c -c -I. -I :mac -d _MPW -s stat -sym off stat.c
mv stat.o objects
c -c -I. -I :mac -d _MPW -s expand -sym off expand.c
mv expand.o objects
c -c -I. -I :mac -d _MPW -s dmstring -sym off dmstring.c
mv dmstring.o objects
c -c -I. -I :mac -d _MPW -s hash -sym off hash.c
mv hash.o objects
c -c -I. -I :mac -d _MPW -s dag -sym off dag.c
mv dag.o objects
c -c -I. -I :mac -d _MPW -s dmake -sym off dmake.c
mv dmake.o objects
c -c -I. -I :mac -d _MPW -s path -sym off path.c
mv path.o objects
c -c -I. -I :mac -d _MPW -s imacs -sym off imacs.c
mv imacs.o objects
c -c -I. -I :mac -d _MPW -s sysintf -sym off sysintf.c
mv sysintf.o objects
c -c -I. -I :mac -d _MPW -s parse -sym off parse.c
mv parse.o objects
c -c -I. -I :mac -d _MPW -s getinp -sym off getinp.c
mv getinp.o objects
c -c -I. -I :mac -d _MPW -s quit -sym off quit.c
mv quit.o objects
c -c -I. -I :mac -d _MPW -s state -sym off state.c
mv state.o objects
c -c -I. -I :mac -d _MPW -s dmdump -sym off dmdump.c
mv dmdump.o objects
c -c -I. -I :mac -d _MPW -s macparse -sym off macparse.c
mv macparse.o objects
c -c -I. -I :mac -d _MPW -s rulparse -sym off rulparse.c
mv rulparse.o objects
c -c -I. -I :mac -d _MPW -s percent -sym off percent.c
mv percent.o objects
c -c -I. -I :mac -d _MPW -s function -sym off function.c
mv function.o objects
c -c -I. -I :mac -d _MPW -s arlib -sym off :mac:arlib.c
mv arlib.o objects
c -c -I. -I :mac -d _MPW -s bogus -sym off :mac:bogus.c
mv bogus.o objects
c -c -I. -I :mac -d _MPW -s dirbrk -sym off :mac:dirbrk.c
mv dirbrk.o objects
c -c -I. -I :mac -d _MPW -s directry -sym off :mac:directry.c
mv directry.o objects
c -c -I. -I :mac -d _MPW -s environ -sym off :mac:environ.c
mv environ.o objects
c -c -I. -I :mac -d _MPW -s main -sym off :mac:main.c
mv main.o objects
c -c -I. -I :mac -d _MPW -s rmprq -sym off :mac:rmprq.c
mv rmprq.o objects
c -c -I. -I :mac -d _MPW -s ruletab -sym off :mac:ruletab.c
mv ruletab.o objects
c -c -I. -I :mac -d _MPW -s tempnam -sym off :mac:tempnam.c
mv tempnam.o objects
c -c -I. -I :mac -d _MPW -s tomacfil -sym off :mac:tomacfil.c
mv tomacfil.o objects
link -w -c 'MPS ' -t MPST -sym off -o dmake  :objects:infer.o :objects:make.o \
:objects:stat.o :objects:expand.o :objects:dmstring.o :objects:hash.o \
:objects:dag.o :objects:dmake.o :objects:path.o :objects:imacs.o \
:objects:sysintf.o :objects:parse.o :objects:getinp.o :objects:quit.o \
:objects:state.o :objects:dmdump.o :objects:macparse.o :objects:rulparse.o \
:objects:percent.o :objects:function.o :objects:arlib.o :objects:bogus.o \
:objects:dirbrk.o :objects:directry.o :objects:environ.o :objects:main.o \
:objects:rmprq.o :objects:ruletab.o :objects:tempnam.o :objects:tomacfil.o \
"CSANELib.o" "Math.o"                 "StdCLib.o" "Runtime.o"                 "Interface.o" "Toollibs.o" 
cp :mac:template.mk startup/config.mk
