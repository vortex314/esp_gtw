/*
 * Sender.h
 *
 *  Created on: Sep 6, 2015
 *      Author: lieven
 */

#ifndef SENDER_H_
#define SENDER_H_

#include <Handler.h>
#include "all.h"

class Sender: public Handler {
	Handler* _mqtt;
	uint32_t _myProp;
public:
	Sender(Handler* mqtt);
	virtual ~Sender();
	bool dispatch(Msg& msg);
};

#endif /* SENDER_H_ */
