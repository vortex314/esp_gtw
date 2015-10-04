/*
 * UartEsp8266.h
 *
 *  Created on: Sep 17, 2015
 *      Author: lieven
 */

#ifndef UARTESP8266_H_
#define UARTESP8266_H_

#include <Uart.h>
#include "CircBuf.h"

// EXPOSED C routines for C interrupt handlers
extern "C" void uart0Receive(uint8_t b);
extern "C" void uart1Receive(uint8_t b);

class UartEsp8266: public Uart {
public:
	CircBuf _rxd;
	CircBuf _txd;
	uint8_t _index;
	uint32_t _bytesRxd;
	uint32_t _bytesTxd;
public:
	static UartEsp8266* _uart0;
	static UartEsp8266* _uart1;
	UartEsp8266(int idx);
	 ~UartEsp8266();
	void receive(uint8_t b);
	void init(uint32_t baud);
	 Erc write(Bytes& data);
	 Erc write(uint8_t b);
	 bool hasData();
	 bool hasSpace();
	 uint8_t read();

};

#endif /* UARTESP8266_H_ */
