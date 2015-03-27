TGT = listener
CC  = g++

CFLAGS = -std=c++0x -Wall -c -flto -O3 -Ikissfft/
LFLAGS = -lopenal -lalut -lSDL2 -g

SRCS = \
	kissfft/kiss_fft.cc \
	src/sample.cc \
	src/gui/element.cc \
	src/gui/gui.cc \
	src/gui/grid.cc \
	src/gui/sampleElement.cc \
	src/gui/fftElement.cc \
	src/gui/waterfallElement.cc \
	src/listenerException.cc \
	src/listener.cc \
	src/main.cc \

DIRS = \
	obj \
	obj/gui \
	obj/kissfft

OBJS = $(patsubst %,obj/%,$(subst src/,,$(SRCS:.cc=.o)))

all : $(DIRS) $(TGT)
	@

$(TGT) : obj $(OBJS)
	$(CC) -o $(TGT) $(OBJS) $(LFLAGS) 

obj/kissfft/%.o : kissfft/%.cc
	$(CC) $(CFLAGS) -o $@ $<

obj/%.o : src/%.cc
	$(CC) $(CFLAGS) -o $@ $<

$(DIRS) :
	mkdir obj
	mkdir obj/gui
	mkdir obj/kissfft

clean :
	rm -rf $(DIRS) $(TGT) obj/*.o obj/*/*.o
