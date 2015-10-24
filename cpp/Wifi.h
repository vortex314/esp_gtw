/*
 * Wifi.h
 *
 *  Created on: Oct 24, 2015
 *      Author: lieven
 */

#ifndef WIFI_H_
#define WIFI_H_
#include "Handler.h"

 class Wifi : public Handler{
	 char _ssid[32];
	 char _pswd[64];
	 uint32_t _connections;
	 bool _connected;
	static void callback();
public:
	Wifi();
	virtual ~Wifi();
	 void config(const char* ssid,const char* pswd);
	 bool dispatch(Msg& msg);
	 void init();
	 bool isConnected() const;
}
;

#endif /* WIFI_H_ */
