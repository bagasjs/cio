#ifndef CIO_H
#define CIO_H

#include <stddef.h>
#include <stdbool.h>

#define CIO_PRINT_MAXIMUM_CHARACTER_CAPACITY 32000

bool cio_has_failure(void);
const char* cio_failure_reason(void);

size_t cio_get_file_size(const char* file_path);
void cio_read_file(const char* file_path, char* writable, bool binary);
void cio_write_file(const char* file_path, const char* data, size_t data_size, bool binary);

void cio_print(const char* fmt, ...);
void cio_println(const char* fmt, ...);
void __cio_report_assertion_failure(const char* cond, const char* file, int line);
#define cio_assert(cond) do { \
    if((cond)) {} \
    else {\
        __cio_report_assertion_failure(#cond, __FILE__, __LINE__);\
    }\
} while(0) \

#endif // CIO_H

#if defined(CIO_IMPLEMENTATION)

#include <stdarg.h>
#include <stdio.h>

enum {
	CIO_NO_FAILURE = 0,
	CIO_FAILED_TO_OPEN_FILE,
};

static struct {
	int last_failure;		
} CIO = {0};

bool cio_has_failure(void)
{
	return CIO.last_failure != CIO_NO_FAILURE;
}

const char* cio_failure_reason(void)
{
	switch(CIO.last_failure) {
		case CIO_FAILED_TO_OPEN_FILE: return "Failed to open file";
		default: return NULL;
	}
	return NULL;
}

size_t cio_get_file_size(const char* file_path)
{
	FILE* f = fopen(file_path, "rb");
	if(f == NULL) {
		CIO.last_failure = CIO_FAILED_TO_OPEN_FILE;
		return 0;
	}

	fseek(f, 0L, SEEK_END);
	size_t sz = ftell(f);
	fseek(f, 0L, SEEK_SET);
	fclose(f);
	return sz;
}

void cio_read_file(const char* file_path, char* writable, bool binary)
{
	FILE* f = NULL;
	if(binary) f = fopen(file_path, "rb");
	else f = fopen(file_path, "r");

	if(f == NULL) {
		CIO.last_failure = CIO_FAILED_TO_OPEN_FILE;
		return;
	}

	fseek(f, 0L, SEEK_END);
	size_t fsz = ftell(f);
	fseek(f, 0L, SEEK_SET);

	for(size_t i = 0; i < fsz && !feof(f); ++i)
		writable[i] = fgetc(f);

	fclose(f);
}

void cio_write_file(const char* file_path, const char* data, size_t data_size, bool binary)
{
	FILE* f = NULL;
	if(binary) f = fopen(file_path, "wb");
	else f = fopen(file_path, "w");

	if(f == NULL) {
		CIO.last_failure = CIO_FAILED_TO_OPEN_FILE;
		return;
	}

	for(size_t i = 0; i < data_size && !feof(f); ++i)
		fputc(data[i], f);

	fclose(f);
}

void cio_print(const char* fmt, ...)
{
	char out_msg[CIO_PRINT_MAXIMUM_CHARACTER_CAPACITY] = {0};
	__builtin_va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(out_msg, CIO_PRINT_MAXIMUM_CHARACTER_CAPACITY, fmt, arg_ptr);
	va_end(arg_ptr);
	printf("%s", out_msg);
}

void cio_println(const char* fmt, ...)
{
	char out_msg[CIO_PRINT_MAXIMUM_CHARACTER_CAPACITY] = {0};
	__builtin_va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(out_msg, CIO_PRINT_MAXIMUM_CHARACTER_CAPACITY, fmt, arg_ptr);
	va_end(arg_ptr);
	printf("%s\n", out_msg);
}

void __cio_report_assertion_failure(const char* cond, const char* file, int line)
{
	cio_println("Assertion Failure in %s at line %d: %s", file, line, cond);
}

#endif