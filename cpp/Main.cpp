/*
 * Main.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: lieven
 */
#include "Msg.h"
#include "Handler.h"

#include "all.h"
#include "Sys.h"

extern "C" {
#include "gpio16.h"
}

class LedBlink: public Handler {
	bool _isOn;
	uint32_t _msecInterval;
	void* _mqtt;

public:
	IROM LedBlink(void* mqtt) :
			Handler("LedBlink") {
		_isOn = false;
		_msecInterval = 100;
		_mqtt = mqtt;
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
				SysLog(__FILE__, __FUNCTION__, "Led Tick ");
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

LedBlink *led;

 extern "C"    void  MsgInit(void* mqtt) {
	Msg::init();
	led = new LedBlink(mqtt);
	led->init();
}

