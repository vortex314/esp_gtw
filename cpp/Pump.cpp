/*
 * Pump.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: lieven
 */
#include "Msg.h"
#include "Handler.h"

#include "all.h"
#include "Sys.h"

extern "C" {
#include "gpio16.h"
}

extern "C" const char* CLOCK_ID;
extern "C" const char* MQTT_ID;
extern "C" const char* TCP_ID;
extern "C" const char* WIFI_ID;
const char* LED_ID = "LED";

class LedBlink: public Handler {
	bool _isOn;
	uint32_t _msecInterval;
	void* _mqtt;

public:
	IROM LedBlink() :
			Handler("LedBlink") {
		_isOn = false;
		_msecInterval = 200;
		_mqtt = (void*) MQTT_ID;
	}

	IROM void init() {
		gpio16_output_conf();
	}

	IROM virtual ~LedBlink() {
	}

#include "UartEsp8266.h"
	extern UartEsp8266* UartEsp8266::_uart0;

	IROM bool dispatch(Msg& msg) {
		PT_BEGIN()
		while (true) {
			timeout(_msecInterval);
			PT_YIELD_UNTIL(
					msg.is(_mqtt, SIG_CONNECTED) || msg.is(_mqtt, SIG_DISCONNECTED) || timeout());
			switch (msg.signal()) {
			case SIG_TICK: {
				_uart0->write('L');
				_uart0->write('\n');
				gpio16_output_set(_isOn);
				_isOn = !_isOn;
				Msg::publish((void*) LED_ID, SIG_TXD);
//				INFO( "Led Tick ");
				break;
			}
			case SIG_CONNECTED: {
				_msecInterval = 1000;
				break;
			}
			case SIG_DISCONNECTED: {
				_msecInterval = 200;
				break;
			}
			default: {
			}
			}

		}
	PT_END();
}
};

uint32_t __count = 0;
//Sender sender();
static Msg* msg;

#include "mutex.h"
mutex_t mutex;
extern "C" uint32_t conflicts;
#include "Flash.h"

Flash* flash;
LedBlink *led;



extern "C" void MsgInit() {
	INFO(" Start Messge Pump ");
	Msg::init();
	msg = new Msg(256);
	led = new LedBlink();
	led->init();
	CreateMutex(&mutex);
	flash=new Flash();
	flash->init();
}


void logMsg(Msg* msg){

	static uint32_t sigCount = 0;
	static void* src = 0;
	static Signal signal = SIG_IDLE;
	if (msg->is(src, signal)) {
		sigCount++;
	} else {
		if (sigCount) {
			INFO(">>>>>>>>>>   %s , %s x %d ",
					(const char*) src, strSignal[signal], sigCount);
		}
		INFO(">>>>>>>>>>   %s , %s ",
				(const char* )msg->src(), strSignal[msg->signal()]);
		src = msg->src();
		signal = msg->signal();
		sigCount = 0;
	}
}

extern "C" void MsgPump() {

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

extern "C" void MsgPublish(void* src, Signal signal) {
	if (GetMutex(&mutex)) {
		Msg::publish(src, signal);
		ReleaseMutex(&mutex);
	} else {
		conflicts++;
	}
}

