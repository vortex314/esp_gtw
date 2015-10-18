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

// uint32_t __count = 0;
//Sender sender();
mutex_t mutex;
 extern "C" uint32_t conflicts;
Flash* flash;
LedBlink *led;
Msg* msg;


extern "C" IROM void MsgInit() {
	INFO(" Start Message Pump ");
	Msg::init();
	msg = new Msg(256);
	led = new LedBlink();
	led->init();
	CreateMutex(&mutex);
	flash = new Flash();
	flash->init();
}

IROM void logMsg(Msg* msg) {

/*	static uint32_t sigCount = 0;
	static void* src = 0;
	static Signal signal = SIG_IDLE;
	if (msg->is(src, signal)) {
		sigCount++;
	} else {
		if (sigCount) {
			INFO(">>>>>>>>>>   %s , %s x %d ",
					(const char*) src, strSignal[signal], sigCount);
		}*/
		INFO(">>>> %s , %s ",	(const char* )msg->src(), strSignal[msg->signal()]);
/*		src = msg->src();
		signal = msg->signal();
		sigCount = 0;
	}*/
}

extern "C" IROM void MsgPump() {

	if (GetMutex(&mutex)) {
		while (msg->receive()) {
			logMsg(msg);
			Handler::dispatchToChilds(*msg);
			msg->free();
		}
		ReleaseMutex(&mutex);
	} else {
		conflicts++;
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

