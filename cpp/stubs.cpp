#include "Sys.h"




void * operator new(size_t size) {
 return malloc(size);
}

void operator delete(void * ptr) {
	free(ptr);
}

void * operator new[](size_t size) {
	return malloc(size);
}

void operator delete[](void * ptr) {
	free(ptr);
}

#include "errno.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "fcntl.h"
extern int errno;

#ifdef __cplusplus
extern "C" {
#endif

void _exit(int code) {
	DEBUG("SHouldn't arrive here !");
	while(1);
}

void __aeabi_atexit() {
	return;
}

void __exidx_end() {
	DEBUG("SHouldn't arrive here !");
	while(1);
}

void __exidx_start() {
	DEBUG("SHouldn't arrive here !");
	while(1);
}
/*
 kill
 Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) {
	DEBUG("SHouldn't arrive here !");
	errno = EINVAL;
	return (-1);
}

//void abort(){};

/*
 link
 Establish a new name for an existing file. Minimal implementation:
 */

/*
 getpid
 Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation, for a system without processes:
 */

int _getpid() {
	return 1;
}

int _ctype(char c) {
	return 0;
}
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifndef STDOUT_USART
#define STDOUT_USART 2
#endif

#ifndef STDERR_USART
#define STDERR_USART 2
#endif

#ifndef STDIN_USART
#define STDIN_USART 2
#endif
/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
	DEBUG("SHouldn't arrive here !");
	switch (file) {
		case STDOUT_FILENO:
		case STDERR_FILENO:
		case STDIN_FILENO:
		return 1;
		default:
		//errno = ENOTTY;
		errno = EBADF;
		return 0;
	}
}
/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
	DEBUG("SHouldn't arrive here !");
	return 0;
}
/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */

int _read(int file, void *ptr, size_t len) {
	DEBUG("SHouldn't arrive here !");

	return 0;
}
/*
 int stat(const char *filepath, struct stat *st) {
 st->st_mode = S_IFCHR;
 return 0;
 }
 */
/*
 lseek
 Set position in a file. Minimal implementation:
 */
off_t lseek(int file, off_t ptr, int dir) {
	return 0;
}

int _close(int file) {
	return -1;
}
/*
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

/*
 link
 Establish a new name for an existing file. Minimal implementation:
 */

int _link(char *old, char *nw) {
	errno = EMLINK;
	return -1;
}

/*
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
	return 0;
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));


int __cxa_guard_acquire(__guard *g) {
	return !*(char *) (g);
}

void __cxa_guard_release(__guard *g) {
	*(char *) g = 1;
}

void __cxa_guard_abort(__guard *) {
}

void __cxa_pure_virtual(void) {
}

int _getpid_r(){
	return 1;
};
void _kill_r(int id){

}

// register char *stack_ptr =0;
#include "osapi.h"
#include "util.h"
caddr_t
_sbrk_rr (int incr)
{
	DEBUG("SHouldn't arrive here !");
	return 0;
	/*
  extern char end;		// Defined by the linker
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0)
    {
      heap_end = &end;
    }
  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr)
    {
      _write (1, "Heap and stack collision\n", 25);
      abort ();
    }
  heap_end += incr;
  return (caddr_t) prev_heap_end;
  */
}
#ifdef MEMCPY
void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t* dp = (uint8_t*) dest;
	const uint8_t* sp = (uint8_t*) src;
	while (n--)
		*dp++ = *sp++;
	return dest;
}
#endif

#ifdef __cplusplus
};
#endif
