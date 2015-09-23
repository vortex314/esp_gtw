/*
 * debug.h
 *
 *  Created on: Dec 4, 2014
 *      Author: Minh
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_
#include "util.h"
#ifndef INFO
//#define INFO os_printf

#define IINFO(fmt, ...) do {        \
		char str[200]; \
        static const char flash_str[] ICACHE_RODATA_ATTR __attribute__((aligned(4))) = fmt;     \
        ets_snprintf(str,200,flash_str, ##__VA_ARGS__);  \
        info(str); \
        } while(0)
#endif

#endif /* USER_DEBUG_H_ */
