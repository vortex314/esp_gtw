/*
 * util.c
 *
 *  Created on: Sep 5, 2015
 *      Author: lieven
 */
#include "Sys.h"

const char* space = "                                 ";

void IROM strAlign(char *dst, int dstLength, char* src, int srcLength) {
	int copyLength = (srcLength < dstLength) ? srcLength : dstLength;
	strncat(dst, src, copyLength);
	strncat(dst, space, dstLength - copyLength);
}
#include <stdarg.h>
char lastLog[256];
char buffer[128];
uint32_t conflicts=0;

const char* SysLogLevelStr[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR",
		"FATAL" };
#include "mutex.h"
mutex_t logMutex=1;
void IROM SysLog(SysLogLevel level, const char* file, const char* function,
		const char * format, ...) {
	char *p;
	buffer[0]=0;
	va_list args;
	va_start(args, format);
	if ( GetMutex(&logMutex)==0) {
		for(p= "$$$:";*p!=0;p++) 	uart0WriteWait(*p);
		for(p=file;*p!=0;p++) 	uart0WriteWait(*p);
		for(p= "$$$";*p!=0;p++) 	uart0WriteWait(*p);
		for(p=function;*p!=0;p++) 	uart0WriteWait(*p);
//		uart0WriteBytes(":",1);
//		uart0WriteBytes(function,strlen(function));
//		os_printf_plus(" Syslog called by %s ",function);
		conflicts+=1000;
		va_end(args);
		return;

	}
	uint32_t time = SysMillis();


	ets_vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	char dst[40];
	dst[0] = '\0';
	strAlign(dst, 18, file, strlen(file));
	strAlign(&dst[18], 18, function, strlen(function));

//	if (level > LOG_INFO) { // put log in mqtt buffer
//		ets_sprintf(lastLog, "%s:%s:%s", SysLogLevelStr[level], dst, buffer);
//	}
	ets_snprintf(lastLog,256,"%10u | %s | %s\n", time , dst, buffer);
	uart0WriteBytes(lastLog,strlen(lastLog));
//	os_printf_plus();
	ReleaseMutex(&logMutex);
}
