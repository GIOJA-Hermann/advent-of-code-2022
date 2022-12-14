####################################################
# Usage : (XX = day number)
#   make run day=XX  => runs part1&2 of the day
#     opts: -B       => required if changing args
#           dbg1=1   => activates PRINTF1
#           dbg2=1   => activates PRINTF2
#   make in day=XX   => shows input of the day
#   make clean       => remove all object files
#   make mrproper    => remove all binaries
####################################################

all: run

DAY = $(day)

INCLUDES = -I./includes
CFLAGS=
ifeq ($(dbg1), 1)
	CFLAGS += -D DEBUG1
endif
ifeq ($(dbg2), 1)
	CFLAGS += -D DEBUG2
endif

run : day${DAY}.run
	@./day${DAY}.run "p1" < inputs/day${DAY}.in
	@./day${DAY}.run "p2" < inputs/day${DAY}.in

%.run : obj/%.o obj/main.o
	@gcc -o $@ $^

obj/%.o: %.c
	@mkdir -p obj
	@gcc ${CFLAGS} ${INCLUDES} -c $^ -o $@

in:
	cat inputs/day${DAY}.in

clean:
	rm -f obj/*.o

mrproper: clean
	rm -f *.run

.PHONY: all run clean mrproper
.PRECIOUS: obj/%.o