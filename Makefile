.PHONY: all
all: task stdinExample 

task:	codec.h basic_main.c
	gcc basic_main.c -L. -l Codec -o encoder -pthread

stdinExample:	stdin_main.c priorityQ.h priorityQ.c
		gcc stdin_main.c priorityQ.c -L. -l Codec -o tester -pthread

.PHONY: clean
clean:
	-rm encoder tester 2>/dev/null
