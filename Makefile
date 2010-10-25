CC=gcc
CCFLAG=-Wall
CLEAN=rm -f
PROGRAM_NAME=neurt
OBJECTS=neurt.c

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) $(CCFLAG) -lm -o $(PROGRAM_NAME) $^

clean:
	$(CLEAN) *.o
	rm -f $(PROGRAM_NAME)
