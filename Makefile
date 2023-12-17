CC := g++
CFLAGS := -Wall -Ofast 
TSTFLAGS := -Wall -fsanitize=leak,undefined,address -Og -ggdb -Wextra -Wno-sign-compare -fno-omit-frame-pointer 

SRCS := lexer.cc parser.cc repl.cc 

repl:
	$(CC)  $(TSTFLAGS) -o repl  repl.c $(SRCS)
	./repl
	
clean:
	-rm -r  repl
	

