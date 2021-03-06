LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lssl -lcrypto

all: test testdbg iron_curtain

test: iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp libggfonts.a core.cpp -Wall -std=c++11 -g -Wextra $(LFLAGS) -o test\
		-D USE_OPENAL_SOUND \
		 /usr/lib/x86_64-linux-gnu/libopenal.so \
		/usr/lib/x86_64-linux-gnu/libalut.so

testdbg: iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp libggfonts.a core.cpp -Wall -std=c++11 -Wextra $(LFLAGS) -o testdbg\
		-DDEBUG -DUSE_OPENAL_SOUND \
		 /usr/lib/x86_64-linux-gnu/libopenal.so \
		/usr/lib/x86_64-linux-gnu/libalut.so

iron_curtain : iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp
	g++ iron_curtain.cpp timers.cpp nickJA.cpp benjaminG.cpp chadM.cpp andrewF.cpp spencerA.cpp libggfonts.a core.cpp -Wall -std=c++11 -Wextra $(LFLAGS) -o Iron_Curtain\
		 -DUSE_OPENAL_SOUND \
		 /usr/lib/x86_64-linux-gnu/libopenal.so \
		/usr/lib/x86_64-linux-gnu/libalut.so

clean:
	rm -f test
	rm -f testdbg
	rm -f Iron_Curtain
	rm -f *.o
