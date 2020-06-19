# @author Swapnil Acharya
# @since 01/23/2020
# @file Makefile

all: compute controller

compute: P1.h P1_compute.c
	gcc -o compute P1.h P1_compute.c

controller: P1.h P1_controller.c
	gcc -o controller P1.h P1_controller.c

clean:
	rm -f compute
	rm -f controller
