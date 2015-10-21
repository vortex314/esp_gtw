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
#include "espmissingincludes.h"
#include "osapi.h"
#include "mem.h"
#include "util.h"

extern uint64_t SysUpTime;
#include "mutex.h"
mutex_t mallocMutex=1;



IROM void* malloc(size_t size) {
	while(!GetMutex(&mallocMutex));
	void* pv = pvPortMalloc(size);
	INFO("malloc(%d)=> 0x%X", size, pv);
	ReleaseMutex(&mallocMutex);
	return pv;
}

IROM void free(void* ptr) {
	INFO("free(0x%X)", ptr);
	vPortFree(ptr);
}


IRAM uint64_t SysMillis() {
	return SysUpTime;
}

IRAM uint64_t Sys::millis() {
	return SysMillis();
}

#include "stdarg.h"

IROM
void Sys::log(SysLogLevel level, const char* file, const char * function,
		const char * format, ...) {
	va_list args;
	va_start(args, format);
	SysLog(level, file, function, format, args);
	va_end(args);
}

#ifdef __cplusplus
}
#endif

#endif

