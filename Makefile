####################################################
# Usage : (XX = day number)
#   make run day=XX  => runs part1&2 of the day
#   make in day=XX   => shows input of the day
#   make clean       => remove all run files
####################################################

all: run

DAY = $(day)

run : day${DAY}.run
	@./day${DAY}.run "p1" < ./day${DAY}.in
	@./day${DAY}.run "p2" < ./day${DAY}.in

%.run : %.c
	@gcc day${DAY}.c -o day${DAY}.run

in:
	cat day${DAY}.in

clean:
	rm -f *.run
