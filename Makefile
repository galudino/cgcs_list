all: cgcs_list_test cgcs_list.o

cgcs_list_test: cgcs_list_test.c cgcs_list.o
	cc -std=c11 -pedantic-errors -Wall -Werror -I ./ -g -o cgcs_list_test cgcs_list_test.c cgcs_list.o

cgcs_list.o: cgcs_list.c
	cc -std=c11 -pedantic-errors -Wall -Werror -I ./ -c cgcs_list.c -o cgcs_list.o

clean:
	rm -rf *.o cgcs_list_test

