/* main.c -- MQTT client example
 *
 * Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of Redis nor the names of its contributors may be used
 * to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * Lieven : some thoughts
 * - Cllback routines can be invoked from interrupts and as such can forget to feed the watchdog
 * - timers are also likely to be in this case
 * - SPI flash loading can influence timing
 *
 */
#include "stdint.h"
#include "ets_sys.h"
#include "uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "gpio16.h"
#include "Sys.h"

MQTT_Client mqttClient;
#define DELAY 100/* milliseconds */
static os_timer_t hello_timer;

uint32_t count = 0;
disconnectCounter = 0;
uint32_t messagesPublished = 0;
uint32_t tcpConnectCounter = 0;
uint32_t mqttConnectCounter = 0;
uint32_t wifiConnectCounter = 0;
char mqttPrefix[30];
uint8_t mqttConnected = FALSE;

#include "Msg.h"

extern void MsgInit();

const char* MQTT_ID = "MQTT";
const char* CLOCK_ID = "CLOCK";
const char* TCP_ID = "TCP";
const char* WIFI_ID = "WIFI";



#include "util.h"
#include "esp_coredump.h"


#include "esp_exc.h"// implement for all timers, callbacks from OS,


// in SysLog ???
// in uart0Write ??
IROM void user_init(void) {
	ThreadLockInit();

	uart_init(BIT_RATE_115200, BIT_RATE_115200);

	gpio_init();
	clockInit();
	os_delay_us(1000000);

	INFO("*****************************************");
	INFO("Starting version : " __DATE__ " " __TIME__);

	os_delay_us(1000000);
	ets_sprintf(mqttPrefix, "/limero314/ESP_%08X", system_get_chip_id());

	system_init_done_cb(MsgInit);
}

