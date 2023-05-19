#define CIO_IMPLEMENTATION
#include "cio.h"
#include <string.h>
#include <stdlib.h>
#include <memory.h>

int main(void)
{
	cio_print("Hello, %s\n", "World");
	cio_println("Hello, %s", "World");
	const char* text = "This is an example text";
	size_t text_size = strlen(text);
	cio_println("len(%s) = %zu", text, text_size);
	cio_write_file("example.txt", text, text_size, false);
	char* loaded_text = malloc(text_size);
	memset(loaded_text, 0, text_size);
	cio_read_file("example.txt", loaded_text, false);
	cio_println("text = %s", text);
	cio_println("loaded_text = %s", loaded_text);
	cio_assert(strcmp(text, loaded_text) == 0 && "text and loaded_text is not the same");
}
