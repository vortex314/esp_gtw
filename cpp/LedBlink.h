/*
 * LedBlink1.h
 *
 *  Created on: Oct 7, 2015
 *      Author: lieven
 */

#ifndef LEDBLINK_H_
#define LEDBLINK_H_
#include <stdint.h>
#include "Sys.h"
#include "Msg.h"
#include "Handler.h"

class LedBlink: public Handler {
	bool _isOn;
	uint32_t _msecInterval;
	void* _mqtt;

public:
	IROM LedBlink() ;
	IROM void init() ;
	IROM virtual ~LedBlink() ;
	IROM bool dispatch(Msg& msg);
};

#endif /* LEDBLINK1_H_ */
