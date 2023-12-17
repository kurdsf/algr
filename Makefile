CC := g++
CFLAGS := -Ofast 
TSTFLAGS := -Wall -fsanitize=leak,undefined,address -Og -ggdb -Wno-sign-compare -fno-omit-frame-pointer
SRCS := lexer.cc parser.cc 

repl:
	$(CC)  $(TSTFLAGS) -o repl  repl.cc $(SRCS)
	./repl
	
clean:
	-rm -r  repl
	

