####################################################
# Usage : (XX = day number)
#   make run day=XX  => runs part1&2 of the day
#   make in day=XX   => shows input of the day
#   make clean       => remove all run files
####################################################

all: run

DAY = $(day)

CFLAGS=
ifeq ($(dbg1), 1)
	CFLAGS += -D DEBUG1
endif
ifeq ($(dbg2), 1)
	CFLAGS += -D DEBUG2
endif

run : day${DAY}.run
	@./day${DAY}.run "p1" < ./day${DAY}.in
	@./day${DAY}.run "p2" < ./day${DAY}.in

%.run : %.o main.o
	@gcc -o $@ $^

%.o: %.c
	@gcc ${CFLAGS} -c $^

in:
	cat day${DAY}.in

clean:
	rm -f *.o

mrproper: clean
	rm -f *.run

.PHONY: all run clean mrproper
.PRECIOUS: %.o