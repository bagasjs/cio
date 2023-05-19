CFLAGS = -Wall -Wextra -pedantic
example: ./example.c
	$(CC) $(CFLAGS) -o $@ $^
