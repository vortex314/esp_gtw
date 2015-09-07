/*
 * Sender.cpp
 *
 *  Created on: Sep 6, 2015
 *      Author: lieven
 */

#include "Sender.h"
#include "all.h"

Sender::Sender(Handler* mqtt) {
	_mqtt = mqtt;
	_myProp = 0;
}

Sender::~Sender() {

}


extern "C" void ets_sprintf(char *s,char* fmt,...);

bool irom Sender::dispatch(Msg& msg) {
	PT_BEGIN()
	DISCONNECTED: {
		PT_YIELD_UNTIL( msg.is(_mqtt, SIG_CONNECTED));
		goto CONNECTED;
	}
	CONNECTED: {
		PT_YIELD_UNTIL( msg.is(_mqtt, SIG_DISCONNECTED) || timeout());
		if (timeout()) {
			char sValue[10];
//			ets_sprintf(sValue,(const char*)"%d",_myProp);
//			MQTT_Publish(&mqttClient, "/mqtt/topic/0", "hello0", 6, 0, 0);
		} else {
			goto DISCONNECTED;
		}
	}

PT_END();
}

