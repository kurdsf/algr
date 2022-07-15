test:
	gcc -o tst errors_test.c -Wall -Wextra -Wpedantic -fsanitize=address,undefined,leak
	-./tst
	-rm tst
