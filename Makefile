CFLAGS=  -O3  -std=c99 -I/usr/include/malloc/ -no-pie
LIBRARIES=-lm
CC=gcc 
AR=xiar
OBJS = bin/PolyMult-seq.o lib/PolyMultGold-seq.o 
PAR_OBJS = bin/PolyMult-par.o lib/PolyMultGold-par.o # Gold is still sequential
FAST_OBJS = bin/PolyMultFAST.o lib/PolyMultGold.o


VERSIONS = GSQ
TIME_TARGETS = $(patsubst %,bin/PolyMult%.seq,$(VERSIONS))
TIME_PAR_TARGETS = $(patsubst %,bin/PolyMult%.par,$(VERSIONS))
CHECK_TARGETS = $(patsubst %,bin/PolyMult%.check-seq,$(VERSIONS))
CHECK_PAR_TARGETS = $(patsubst %,bin/PolyMult%.check-par,$(VERSIONS))
DATA = $(patsubst %,bin/PolyMult%.data,$(VERSIONS))


all: bin $(TIME_TARGETS) $(TIME_PAR_TARGETS) $(CHECK_TARGETS) $(CHECK_PAR_TARGETS) $(DATA)


debug: CFLAGS =-DDEBUG -g -Wall -Wextra -std=c99 -I/usr/include/malloc/
debug: all


# TASK BINARIES

PolyMult%.seq: $(OBJS) src/PolyMult-wrapper.c
	$(CC) src/PolyMult-wrapper.c -o $@ $(OBJS) $(CFLAGS) $(LIBRARIES) -D$(patsubst bin/PolyMult%.seq,%,$@)

PolyMult%.par: $(PAR_OBJS) src/PolyMult-wrapper.c
	$(CC) src/PolyMult-wrapper.c -o $@ $(PAR_OBJS) $(CFLAGS) $(LIBRARIES) -fopenmp -DPAR -DRANDOM -D$(patsubst bin/PolyMult%.par,%,$@)

PolyMult%.check-seq: $(OBJS) src/PolyMult-wrapper.c
	$(CC) src/PolyMult-wrapper.c -o $@ $(OBJS) $(CFLAGS) $(LIBRARIES) -DCHECKING -D$(patsubst bin/PolyMult%.check-seq,%,$@)

PolyMult%.check-par: $(OBJS) src/PolyMult-wrapper.c
	$(CC) src/PolyMult-wrapper.c -o $@ $(PAR_OBJS) $(CFLAGS) $(LIBRARIES) -DCHECKING -fopenmp -DPAR -D$(patsubst bin/PolyMult%.check-par,%,$@)

PolyMult%.data: $(PAR_OBJS) src/PolyMult-wrapper.c
	$(CC) src/PolyMult-wrapper.c -o $@ $(PAR_OBJS) $(CFLAGS) $(LIBRARIES) -fopenmp -DPAR -D$(patsubst bin/PolyMult%.data,%,$@) -DDATA

# OBJECT FILES

bin/PolyMult-seq.o: src/PolyMult.c
	$(CC) src/PolyMult.c -o bin/PolyMult-seq.o $(CFLAGS) $(LIBRARIES) -c

bin/PolyMult-par.o: src/PolyMult.c
	$(CC) src/PolyMult.c -o bin/PolyMult-par.o -fopenmp -DPAR $(CFLAGS) $(LIBRARIES) -c

#lib/PolyMultGold-seq.o: src/PolyMultGold.c
#	$(CC) src/PolyMultGold.c -o lib/PolyMultGold-seq.o $(CFLAGS) $(LIBRARIES) -c

#lib/PolyMultGold-par.o: src/PolyMultGold.c
#	$(CC) src/PolyMultGold.c -o lib/PolyMultGold-par.o -fopenmp -DPAR $(CFLAGS) $(LIBRARIES) -c


# MISC

bin:
	mkdir -p bin

PA1.tar: report.pdf src/PolyMult.c lib/collect_data.py
	tar -cvf PA1.tar $^

clean:
	rm -f bin/*

scratch:
	gcc src/scratch.cpp -o bin/scratch