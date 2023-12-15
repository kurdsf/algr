
CC := g++
CFLAGS := -Wall -Ofast 
TSTFLAGS := -Wall -fsanitize=leak,undefined,address -Og -ggdb -Wextra -Wno-sign-compare -fno-omit-frame-pointer 


SRCS := lexer.cc presedence.cc parser.cc vec.cc rule.cc algr.cc optimizer.cc
	

.DELETE_ON_ERROR:  repl
test: 
	$(CC)  $(TSTFLAGS) -o test  test.c $(SRCS)
	./test
   

repl:
	$(CC)  $(TSTFLAGS) -o repl  repl.c $(SRCS)
	./repl
	

clean:
	-rm -r  test repl
	

