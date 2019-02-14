LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: test

test: iron_curtain.cpp timers.cpp nickJA.cpp ChadM.cpp andrewF.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp ChadM.cpp andrewF.cpp -Wall -Wextra $(LFLAGS) -o test

clean:
	rm -f test
	rm -f *.o
