CFLAGS=  -O3  -std=c99 -I/usr/include/malloc/ 
LIBRARIES=-lm
CC=gcc 
AR=xiar
OBJS = bin/PolyMult.o lib/PolyMultGold.o 
PAR_OBJS = bin/PolyMultPAR.o lib/PolyMultGold.o # Gold is still sequential
FAST_OBJS = bin/PolyMultFAST.o lib/PolyMultGold.o


TASKS = GSQ INQ OPQ BLQ DCQ # DCK DCNew
TIME_TARGETS = $(patsubst %,bin/PolyMult%.time,$(TASKS))
TIME_PAR_TARGETS = $(patsubst %,bin/PolyMult%.time-par,$(TASKS))
CHECK_TARGETS = $(patsubst %,bin/PolyMult%.check,$(TASKS))
CHECK_RAND_TARGETS = $(patsubst %,bin/PolyMult%.check-rand,$(TASKS))
CHECK_PAR_TARGETS = $(patsubst %,bin/PolyMult%.check-par,$(TASKS))
# one could add CHECK_PAR_RAND_TARGETS to be very thurough

all: bin $(TIME_TARGETS) $(TIME_PAR_TARGETS) $(CHECK_TARGETS) $(CHECK_RAND_TARGETS) $(CHECK_PAR_TARGETS) $(FAST_TARGETS)


debug: CFLAGS =-DDEBUG -g -Wall -Wextra -std=c99 -I/usr/include/malloc/
debug: all


# TASK BINARIES

PolyMult%.time: $(OBJS) src/PolyMult.h 
	$(CC) src/PolyMult-wrapper.c -o $@ $^ $(CFLAGS) $(LIBRARIES) -DRANDOM -D$(patsubst bin/PolyMult%.time,%,$@)

PolyMult%.time-par: $(PAR_OBJS) src/PolyMult.h 
	$(CC) src/PolyMult-wrapper.c -o $@ $^ $(CFLAGS) $(LIBRARIES) -fopenmp -DPAR -DRANDOM -D$(patsubst bin/PolyMult%.time-par,%,$@)

PolyMult%.check: $(OBJS) src/PolyMult.h 
	$(CC) src/PolyMult-wrapper.c -o $@ $^ $(CFLAGS) $(LIBRARIES) -DCHECKING -D$(patsubst bin/PolyMult%.check,%,$@)

PolyMult%.check-rand: $(OBJS) src/PolyMult.h 
	$(CC) src/PolyMult-wrapper.c -o $@ $^ $(CFLAGS) $(LIBRARIES) -DRANDOM -DCHECKING -D$(patsubst bin/PolyMult%.check-rand,%,$@)

PolyMult%.check-par: $(PAR_OBJS) src/PolyMult.h 
	$(CC) src/PolyMult-wrapper.c -o $@ $^ $(CFLAGS) $(LIBRARIES) -fopenmp -DPAR -DCHECKING -D$(patsubst bin/PolyMult%.check-par,%,$@)

# OBJECT FILES

bin/PolyMult.o: src/PolyMult.c
	$(CC) src/PolyMult.c -o bin/PolyMult.o $(CFLAGS) $(LIBRARIES) -c

bin/PolyMultPAR.o: src/PolyMult.c
	$(CC) src/PolyMult.c -o bin/PolyMultPAR.o -fopenmp -DPAR $(CFLAGS) $(LIBRARIES) -c

#lib/PolyMultGold.o: src/PolyMultGold.c
#	$(CC) src/PolyMultGold.c -o lib/PolyMultGold.o $(CFLAGS) $(LIBRARIES) -c


# MISC

bin:
	mkdir -p bin

PA2.tar: report.pdf src/PolyMult.c lib/PolyMultGold.o
	tar -cvf PA2.tar $^

clean:
	rm -f bin/*
