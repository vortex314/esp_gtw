/*
 * Sys.cpp
 *
 *  Created on: Sep 13, 2015
 *      Author: lieven
 */
#include "errno.h"
#include "string.h"
#include "Sys.h"

#ifdef __ESP8266__
extern "C" {
#include "user_interface.h"
#include "osapi.h"
}
#include "util.h"

uint64_t Sys::millis() {
	return system_get_time() / 1000;
}

#include "stdarg.h"

static const char* space = "                                 ";

IROM void strAlign(char *dst, int dstLength, const char* src, int srcLength) {
	int copyLength = (srcLength < dstLength) ? srcLength : dstLength;
	strncat(dst, src, copyLength);
	strncat(dst, space, dstLength - copyLength);
}

//extern "C" void SysLog(const char* file, const char* function, const char * format, va_list);

IROM void Sys::log(const char* file, const char* function, const char * format,
		...) {

//	uint32_t time = millis();
//	char buffer[256];
	va_list args;
	va_start(args, format);
	SysLog(file,function,format,args);
//	ets_vsnprintf(buffer, 256, format, args);
	va_end(args);
	/*
	char dst[50];
#ifdef __ESP8266__
	dst[0] = '\0';
	strAlign(dst, 15, file, strlen(file));
	strAlign(&dst[15], 15, function, strlen(function));
#else
	const char *ps = strrchr(file,'/');
	const char *pf=strchr(function,' ');
//	pf  = strchr(pf,'(');
	sprintf(dst,"%10.10s - %10.10s",ps+1,pf+1);
#endif
	os_printf("%06d.%03d | %s | %s\r\n", time / 1000, time % 1000, dst,
			buffer);*/
}

#endif

