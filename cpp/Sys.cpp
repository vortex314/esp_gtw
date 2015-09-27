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

uint64_t Sys::millis() {
return system_get_time() / 1000;
}

#include "stdarg.h"

IROM
void Sys::log(const char* level,const char* file, const	char * function, const char *	format,	... )
{
	va_list args;
	va_start(args , format);
	SysLog( level,file,function, format,args );
	va_end(args);
}

#endif

