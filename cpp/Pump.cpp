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
const char* LED_ID="LED";

class LedBlink: public Handler {
	bool _isOn;
	uint32_t _msecInterval;
	void* _mqtt;

public:
	IROM LedBlink() :
			Handler("LedBlink") {
		_isOn = false;
		_msecInterval = 100;
		_mqtt = (void*)MQTT_ID;
	}

	IROM void init(){
		gpio16_output_conf();
	}

	IROM virtual ~LedBlink() {
	}

	IROM bool dispatch(Msg& msg) {
		PT_BEGIN()
		while (true) {
			timeout(_msecInterval);
			PT_YIELD_UNTIL(
					msg.is(_mqtt, SIG_CONNECTED) || msg.is(_mqtt, SIG_DISCONNECTED) || timeout());
			switch (msg.signal()) {
			case SIG_TICK: {
				gpio16_output_set(_isOn);
				_isOn = !_isOn;
				Msg::publish((void*)LED_ID,SIG_TXD);
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
LedBlink *led;

extern "C"   void  MsgInit() {
	Msg::init();
	msg = new Msg(256);
	led = new LedBlink();
	led->init();
}

extern "C"   void MsgPump() {
	Msg::init();
	while (msg->receive()) {
		if ( !msg->is(0,SIG_TICK))
			INFO(">>>>>>>>>>   %s , %s ",(const char* )msg->src(),strSignal[msg->signal()]);
		msg->rewind();
		Handler::dispatchToChilds(*msg);
		msg->free();
		if ((__count++) % 1000 == 0) {
			INFO( "Count loop : %d Committed size :%d ", __count,msg->_bb->getCommittedSize());
		}
	}
}



extern "C"   void MsgPublish(void* src, Signal signal) {
	Msg::publish(src, signal);
}





