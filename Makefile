LDFLAGS=-F/Library/Frameworks -framework SDL2 -framework Cocoa -framework SDL2_mixer -framework SDL2_ttf -I/System/Library/Frameworks/OpenGL.framework/Headers -framework OpenGL
CFLAGS=-g -O2 -Wall -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers
#CFLAGS=-g -Wall `sdl2-config --cflags`
#LDFLAGS=-g -Wall `sdl2-config --libs` -lm -lSDL2_mixer -lSDL2_ttf


all: commute

commute: engine.o timer.o graphics.o commute.o
	gcc -o $@ ${LDFLAGS} $^

engine.o: engine.h
graphics.o: engine.h graphics.h timer.h
commute.o: engine.h graphics.h timer.h
timer.o: timer.h
.c.o:
	gcc -c ${CFLAGS} $<

clean: 
	rm commute *.o *~
