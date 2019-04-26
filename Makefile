LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: test

test: iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp libggfonts.a core.cpp -Wall -std=c++11 -Wextra $(LFLAGS) -o test\
		-D USE_OPENAL_SOUND \
        /usr/lib/libopenal.so \
        /usr/lib/libalut.so

	
clean:
	rm -f test
	rm -f *.o
