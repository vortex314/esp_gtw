/*
 * util.c
 *
 *  Created on: Sep 5, 2015
 *      Author: lieven
 */
#include "all.h"

const char* space = "                                 ";

void IROM strAlign(char *dst, int dstLength, char* src, int srcLength) {
	int copyLength = (srcLength < dstLength) ? srcLength : dstLength;
	strncat(dst, src, copyLength);
	strncat(dst, space, dstLength - copyLength);
}

void IROM SysLog(const char* file, const char* function, const char * format, ...) {
	uint32_t time = system_get_time() / 1000;
	char buffer[256];
	va_list args;
	va_start(args, format);
	ets_vsnprintf(buffer, 256, format, args);
	va_end(args);
	char dst[30];
	dst[0]='\0';
	strAlign(dst, 15, file, strlen(file));
	strAlign(&dst[15], 15, function, strlen(function));
	ets_printf("%06d.%03d | %s | %s\r\n", time / 1000, time % 1000, dst,
			buffer);
}
