TGT = listener
CC  = g++

CFLAGS = -Wall -c
LFLAGS = -lOpenAL32 -lglut32 -lopengl32 -lglu32 -g

SRCS = \
	src/Sample.cc \
	src/ListenerException.cc \
	src/Listener.cc \
	src/main.cc \

OBJS = $(patsubst %,obj/%,$(subst src/,,$(SRCS:.cc=.o)))


all : $(TGT)
	@

$(TGT) : obj $(OBJS)
	$(CC) -o $(TGT) $(OBJS) $(LFLAGS) 

obj/%.o : src/%.cc
	$(CC) $(CFLAGS) -o $@ $<

obj :
	mkdir obj

clean :
	rm -f $(TGT) obj/*.o
