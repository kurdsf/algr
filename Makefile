
CC := gcc
CFLAGS := -Wall -Ofast 
TSTFLAGS := -Wall -fsanitize=leak,undefined,address -Og -ggdb -Wextra -Wno-sign-compare -fno-omit-frame-pointer 



SRCS := lexer.c presedence.c parser.c vec.c rule.c algr.c optimizer.c
	

.DELETE_ON_ERROR:  repl
test: 
	$(CC)  $(TSTFLAGS) -o test  test.c $(SRCS)
	./test
   

repl:
	$(CC)  $(TSTFLAGS) -o repl  repl.c $(SRCS)
	./repl
	

clean:
	-rm -r  test repl
	

