LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: test

test: iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp
	clear; g++ iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp libggfonts.a core.cpp -g -Wall -std=c++11 -Wextra $(LFLAGS) -o test

clean:
	rm -f test
	rm -f *.o
