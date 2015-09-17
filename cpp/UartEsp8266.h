/*
 * UartEsp8266.h
 *
 *  Created on: Sep 17, 2015
 *      Author: lieven
 */

#ifndef UARTESP8266_H_
#define UARTESP8266_H_

#include <Uart.h>
#include "uart.h"
// EXPOSED C routines for C interrupt handlers
extern "C" void uart0Receive(uint8_t b);
extern "C" void uart1Receive(uint8_t b);

class UartEsp8266: public Uart {
private:

	uint8_t _index;
public:
	static UartEsp8266* _uart0;
	static UartEsp8266* _uart1;
	UartEsp8266(int idx);
	virtual ~UartEsp8266();
	void receive(uint8_t b);
	void init(uint32_t baudrate);
	virtual Erc write(Bytes& data)=0;
	virtual Erc write(uint8_t b)=0;
	virtual bool hasData()=0;
	virtual bool hasSpace()=0;
	virtual uint8_t read()=0;

};

#endif /* UARTESP8266_H_ */
