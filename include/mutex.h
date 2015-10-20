//////////////////////////////////////////////////
// Mutex support for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#ifndef __MUTEX_H__
#define __MUTEX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <c_types.h>

typedef int32 mutex_t;

void ICACHE_FLASH_ATTR CreateMutex(mutex_t *mutex);
bool ICACHE_FLASH_ATTR GetMutex(mutex_t *mutex);
void ICACHE_FLASH_ATTR ReleaseMutex(mutex_t *mutex);
bool ThreadLock(const char* name);
void ThreadUnlock();
void ThreadLockInit();
#ifdef __cplusplus
}
#endif
#endif
