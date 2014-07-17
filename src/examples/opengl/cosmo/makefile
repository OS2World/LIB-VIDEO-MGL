!include <win32.mak>

INCS = -I..\include
LIBS = ..\lib\CosmoGLU.lib ..\lib\CosmoGL.lib $(guilibsdll)

TARGETS = \
	cube.exe \
	indexfnc.exe \
	indexmat.exe \
	indextex.exe \
	simpleci.exe \
	sphere.exe \
	texenv.exe \
	torus.exe \
	vcull.exe \
	whichgl.exe

all: $(TARGETS)

.c.exe:
    $(cc) $(cdebug) $(INCS) $(cflags) $(cvarsdll) $(scall) -c $*.c
    $(link) $(ldebug) $(guilflags) -out:$*.exe $*.obj $(LIBS)

sphere.exe: sphere.c
    $(cc) $(cdebug) $(INCS) $(cflags) $(cvarsdll) $(scall) -c $*.c
    $(link) $(ldebug) $(guilflags) -out:$*.exe $*.obj winmm.lib $(LIBS)

whichgl.exe: whichgl.c
    $(cc) $(cdebug) $(INCS) $(cflags) $(cvarsdll) $(scall) -c $*.c
    $(link) $(ldebug) $(conlflags) -out:$*.exe $*.obj $(baselibs) $(LIBS)

clean:
    rm -f *.obj

clobber:
    rm -f *.obj *.exe



