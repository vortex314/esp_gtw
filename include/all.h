/*
 * all.h
 *
 *  Created on: Sep 5, 2015
 *      Author: lieven
 */

#ifndef ALL_H_
#define ALL_H_

//+++++++++++++++++++++++++++++ STANDARD C types
#include <sys/types.h>
#include <stdarg.h>
#include <stdint.h>
//+++++++++++++++++++++++++++++ ESP8266 types
#ifdef __cplusplus
extern "C" {
#endif

#include <c_types.h>

#define BOOL uint8_t

#include <osapi.h>
#include <ets_sys.h>
#include "user_interface.h"
#include "uart.h"
#include "mem.h"
#include "osapi.h"
//+++++++++++++++++++++++++++++ appl types
#include "debug.h"
#include "gpio16.h"
#include "config.h"
#include "queue.h"
#include "proto.h"
#include "mqtt.h"
#include "util.h"
#include "user_config.h"

#ifdef __cplusplus
}
#endif

#endif /* ALL_H_ */
