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
 */
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
const char* WIFI_ID = "TCP";

void wifiConnectCb(uint8_t status) {
	if (status == STATION_GOT_IP) {
		MsgPublish(WIFI_ID, SIG_CONNECTED);
		MQTT_Connect(&mqttClient);
	} else {
		MsgPublish(WIFI_ID, SIG_DISCONNECTED);
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

	MsgPublish(MQTT_ID, SIG_CONNECTED);

//	os_timer_arm(&hello_timer, DELAY, 1);
	mqttConnected = TRUE;

}

void mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
//	INFO("MQTT: Disconnected");
	mqttConnectCounter++;
	os_timer_disarm(&hello_timer);
	MsgPublish(MQTT_ID, SIG_CONNECTED);
	mqttConnected = FALSE;
}

void mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
//	INFO("MQTT: Published");
	MsgPublish(MQTT_ID, SIG_TXD);
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
extern uint32_t conflicts;
uint64_t timeoutValue = 0;
/*uint32_t millis() {
	return (system_get_time() / 1000);
}*/
extern uint64_t SysUpTime;
LOCAL void IROM tick_cb(void *arg) {

//	char buf[100];
//	char topic[30];
	MsgPublish(CLOCK_ID, SIG_TICK);
	MsgPump();
	if (SysMillis() > timeoutValue) {
		if (mqttConnected) {
			publish("count", count++);
			publish("mqtt/connections", mqttConnectCounter);
			publish("wifi/connections", wifiConnectCounter);
			publish("tcp/connections", tcpConnectCounter);
			publish("mqtt/published", messagesPublished);
			publishStr("system/log", lastLog);
			publishStr("system/build", __DATE__ " " __TIME__);
			publish("system/heapSize", system_get_free_heap_size());
			publish("system/bootTime", SysUpTime);
			timeoutValue = SysMillis() + 2000;
		}
	}

//	os_timer_arm(&hello_timer, DELAY, 1);

}
/*
 LOCAL os_timer_t tick_timer;

 LOCAL void IRAM tick_cb(void *arg) {
 //	MsgPublish(CLOCK_ID,SIG_TICK);
 MsgPump();
 } */

IROM void user_init(void) {
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	gpio_init();
	gpio16_output_conf();
	os_delay_us(1000000);
	INFO("Starting version : " __DATE__ " " __TIME__);
	MsgInit();
	ets_sprintf(mqttPrefix, "/limero314/ESP_%08X", system_get_chip_id());
	uart_div_modify(0, UART_CLK_FREQ / 115200);

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
	/*	os_timer_disarm(&tick_timer);
	 os_timer_setfn(&tick_timer, (os_timer_func_t *) tick_cb, (void *) 0);
	 os_timer_arm(&tick_timer, 1, 1);*/
	// 1 msec repeat
	clockInit();
	INFO("System started ...");
}

