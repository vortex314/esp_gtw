/*
 * LedBlink1.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: lieven
 */

#include "LedBlink.h"
extern "C" {
#include "c_types.h"
#include "gpio16.h"
}

IROM LedBlink::LedBlink() :
		Handler("LedBlink") {
	_isOn = false;
	_msecInterval = 200;
	_mqtt = (void*) MQTT_ID;
}

IROM void LedBlink::init() {
	gpio16_output_conf();
}

IROM  LedBlink::~LedBlink() {
}

IROM bool LedBlink::dispatch(Msg& msg) {
	PT_BEGIN()
	while (true) {
		timeout(_msecInterval);
		PT_YIELD_UNTIL(
				msg.is(_mqtt, SIG_CONNECTED) || msg.is(_mqtt, SIG_DISCONNECTED)
						|| timeout());
		switch (msg.signal()) {
		case SIG_TICK: {
			gpio16_output_set(_isOn);
			_isOn = !_isOn;
//				Msg::publish((void*) LED_ID, SIG_TXD);
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

