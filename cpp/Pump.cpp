/*
 * Pump.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: lieven
 */
#include "Msg.h"
#include "Handler.h"

//#include "all.h"
#include "Sys.h"

extern "C" {
//#include "gpio16.h"
#include "osapi.h"
#include "ets_sys.h"
#include "user_interface.h"
#include "espmissingincludes.h"
}

extern "C" const char* CLOCK_ID;
extern "C" const char* MQTT_ID;
extern "C" const char* TCP_ID;
extern "C" const char* WIFI_ID;

const char* LED_ID = "LED";
#include "UartEsp8266.h"
#include "mutex.h"
#include "Flash.h"
#include "LedBlink.h"
#include "Wifi.h"
#include "Sys.h"
#include "Tcp.h"

// uint32_t __count = 0;
//Sender sender();
mutex_t mutex;
extern "C" uint32_t conflicts;
Flash* flash;
LedBlink *led;
Msg* msg;
Wifi* wifi;
Tcp* tcp;

#define MSG_TASK_PRIO        		1
#define MSG_TASK_QUEUE_SIZE    	100
#define MSG_SEND_TIMOUT			5

extern "C" void MsgPump();
extern "C" void MsgPublish(void* src, Signal signal);
extern "C" void MsgInit();
extern uint64_t SysWatchDog;


os_event_t MsgQueue[MSG_TASK_QUEUE_SIZE];
os_timer_t pumpTimer;

inline void Post(const char* src, Signal signal) {
	system_os_post((uint8_t) MSG_TASK_PRIO, (os_signal_t) signal,
			(os_param_t) src);
}

void IROM MSG_TASK(os_event_t *e) {
	MsgPublish((void*)e->par, (Signal) e->sig);
	MsgPump();
	SysWatchDog = Sys::millis() + 1000; // if not called within 1 second calls dump_stack;
}

void IROM tick_cb(void *arg) {
	Post(CLOCK_ID, SIG_TICK);
}

extern "C" IROM void MsgInit() {
	INFO(" Start Message Pump ");
	Msg::init();
	Msg::publish((void*) __FUNCTION__, SIG_INIT);
	msg = new Msg(256);
	led = new LedBlink();
	wifi = new Wifi();
	wifi->config("Merckx", "LievenMarletteEwoutRonald");
	led->init();
	CreateMutex(&mutex);
	flash = new Flash();
	flash->init();
	tcp =  new Tcp();
//	tcp->config("iot.eclipse.org",1883);

	os_timer_disarm(&pumpTimer);
	os_timer_setfn(&pumpTimer, (os_timer_func_t *) tick_cb, (void *) 0);
	os_timer_arm(&pumpTimer, 100, 1);
	system_os_task(MSG_TASK, MSG_TASK_PRIO, MsgQueue, MSG_TASK_QUEUE_SIZE);
}

extern "C" IROM void MsgPump() {
	while (msg->receive()) {
		INFO(">>>> %s , %s ",
				(const char* )msg->src(), strSignal[msg->signal()]);
		Handler::dispatchToChilds(*msg);
		msg->free();
	}
}

extern "C" IROM void MsgPublish(void* src, Signal signal) {
	if (GetMutex(&mutex)) {
		Msg::publish(src, signal);
		ReleaseMutex(&mutex);
	} else {
		conflicts++;
	}
}




extern uint64_t SysWatchDog;

