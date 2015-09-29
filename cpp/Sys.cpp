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
#ifdef __cplusplus
extern "C" {
#endif

#include "user_interface.h"
#include "osapi.h"
#include "mem.h"
#include "util.h"

extern uint64_t SysUpTime;

IROM void* malloc(size_t size) {
INFO("malloc(%d)",size);
return pvPortMalloc(size);
}

IROM void free(void* ptr) {
return vPortFree(ptr);
}
#ifdef __cplusplus
}
#endif

uint64_t SysMillis() {
/*	static uint32_t lastSample=0;
	static uint64_t major=0;
	uint32_t minor = system_get_time();
	if ( minor < lastSample ) {	// carry the overflow
		major += 0x100000000UL;
	}
	lastSample = minor ;
	return (major | minor)/1000;*/
	return SysUpTime;
}

uint64_t Sys::millis(){
	return SysMillis();
}

#include "stdarg.h"

IROM
void Sys::log(SysLogLevel level,const char* file, const	char * function, const char *	format,	... )
{
	va_list args;
	va_start(args , format);
	SysLog( level,file,function, format,args );
	va_end(args);
}

#endif

