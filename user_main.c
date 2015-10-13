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
#define DELAY 20/* milliseconds */
LOCAL os_timer_t hello_timer;

uint32_t count = 0;
disconnectCounter = 0;
uint32_t messagesPublished = 0;
uint32_t tcpConnectCounter = 0;
uint32_t mqttConnectCounter = 0;
uint32_t wifiConnectCounter = 0;
char mqttPrefix[30];
uint8_t mqttConnected = FALSE;

#include "Msg.h"
extern void MsgPump();
extern void MsgPublish(void* src, Signal signal);
extern void MsgInit();

const char* MQTT_ID = "MQTT";
const char* CLOCK_ID = "CLOCK";
const char* TCP_ID = "TCP";
const char* WIFI_ID = "WIFI";
#define MSG_TASK_PRIO        		1
#define MSG_TASK_QUEUE_SIZE    	100
#define MSG_SEND_TIMOUT			5
os_event_t MsgQueue[MSG_TASK_QUEUE_SIZE];
typedef enum {
	MsgTick, MsgUart
} MsgType;

void Post(const char* src, Signal signal) {
//	INFO("POST %s : %d ",src,signal);
	system_os_post(MSG_TASK_PRIO, signal, (ETSParam) src);
}

void wifiConnectCb(uint8_t status) {
	if (status == STATION_GOT_IP) {
		Post(WIFI_ID, SIG_CONNECTED);
		MQTT_Connect(&mqttClient);
	} else {
		Post(WIFI_ID, SIG_DISCONNECTED);
		MQTT_Disconnect(&mqttClient);
	}
}

void mqttConnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	INFO("MQTT: Connected");

	char topic[30];

	ets_sprintf(topic, "PUT%s/#", mqttPrefix);
	MQTT_Subscribe(client, topic, 0);

	ets_sprintf(topic, "PUT%s/realTime", mqttPrefix);
	MQTT_Publish(client, topic, "12234578", 6, 0, 0);

	Post(MQTT_ID, SIG_CONNECTED);

//	os_timer_arm(&hello_timer, DELAY, 1);
	mqttConnected = TRUE;

}

void mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
//	INFO("MQTT: Disconnected");
	mqttConnectCounter++;
	os_timer_disarm(&hello_timer);
	Post(MQTT_ID, SIG_CONNECTED);
	mqttConnected = FALSE;
}

void mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
//	INFO("MQTT: Published");
	Post(MQTT_ID, SIG_TXD);
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len,
		const char *data, uint32_t data_len) {
	char *topicBuf = (char*) os_zalloc(topic_len + 1), *dataBuf =
			(char*) os_zalloc(data_len + 1);

	MQTT_Client* client = (MQTT_Client*) args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s ", topicBuf, dataBuf);
	os_free(topicBuf);
	os_free(dataBuf);
}

//bool ledOn = true;

LOCAL void IROM publish(const char* topicName, uint32_t value) {
	char buf[100];
	char topic[40];

	ets_sprintf(buf, "%d", value);
	ets_sprintf(topic, "%s/%s", mqttPrefix, topicName);
	MQTT_Publish(&mqttClient, topic, buf, strlen(buf), 0, 0);
	messagesPublished++;
}

LOCAL void IROM publishStr(const char* topicName, char* buf) {
	char topic[40];

	ets_sprintf(topic, "%s/%s", mqttPrefix, topicName);
	MQTT_Publish(&mqttClient, topic, buf, strlen(buf), 0, 0);
	messagesPublished++;
}
extern char lastLog[];
#include "util.h"
#include "esp_coredump.h"
extern uint32_t conflicts;
extern uint32_t uartTxdCount;
extern uint32_t uartRxdCount;
extern uint32_t uartErrorCount;
extern uint32_t overflowTxd;

/*uint32_t millis() {
 return (system_get_time() / 1000);
 }*/

extern uint64_t SysUpTime;
extern uint64_t SysWatchDog;

extern IROM int HandlerTimeouts();

void IROM tick_cb(void *arg) {

	if (HandlerTimeouts())
		system_os_post(MSG_TASK_PRIO, SIG_TICK, CLOCK_ID);
	SysWatchDog = SysMillis() + 1000;	// if not called within 1 second calls dump_stack;
}

void IROM MSG_TASK(os_event_t *e) {

//	INFO("event received : %s :%d ",e->par,e->sig);
	static uint64_t timeoutValue = 0;
	MsgPublish(e->par, e->sig);
	MsgPump();
	if (SysMillis() > timeoutValue) {
		if (mqttConnected) {
			publish("count", count++);
			publish("system/conflicts", conflicts);
			publish("mqtt/connections", mqttConnectCounter);
			publish("wifi/connections", wifiConnectCounter);
			publish("tcp/connections", tcpConnectCounter);
			publish("mqtt/published", messagesPublished);
			publishStr("system/log", lastLog);
			publishStr("system/build", __DATE__ " " __TIME__);
			publish("system/heapSize", system_get_free_heap_size());
			publish("system/bootTime", SysUpTime);
			publish("uart0/txdCount", uartTxdCount);
			publish("uart0/rxdCount", uartRxdCount);
			publish("uart0/errorCount", uartErrorCount);
			publish("uart0/overflowTxd", overflowTxd);
			timeoutValue = SysMillis() + 5000;
		}
	}
}
/*
 LOCAL os_timer_t tick_timer;

 LOCAL void IRAM tick_cb(void *arg) {
 //	Publish(CLOCK_ID,SIG_TICK);
 MsgPump();
 } */

#include "esp_exc.h"
IROM void user_init(void) {

	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	esp_exception_handler_init();
	gpio_init();
	gpio16_output_conf();
	clockInit();
	os_delay_us(1000000);
	INFO("");
	INFO("Starting version : " __DATE__ " " __TIME__);
	dump_stack();

	MsgInit();
	os_delay_us(1000000);
	ets_sprintf(mqttPrefix, "/limero314/ESP_%08X", system_get_chip_id());
//	uart_div_modify(0, UART_CLK_FREQ / 115200);

	CFG_Load();

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port,
			sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user,
			sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
	// Set up a timer to send the message

	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *) tick_cb, (void *) 0);
	os_timer_arm(&hello_timer, DELAY, 1);

	INFO("System started ...");
	system_os_task(MSG_TASK, MSG_TASK_PRIO, MsgQueue, MSG_TASK_QUEUE_SIZE);
	system_os_post(MSG_TASK_PRIO, 0, 0);
}

