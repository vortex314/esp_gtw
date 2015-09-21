/*
 * UartEsp8266.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: lieven
 */

#include "UartEsp8266.h"

UartEsp8266* UartEsp8266::_uart0 = 0;
UartEsp8266* UartEsp8266::_uart1 = 0;

UartEsp8266::UartEsp8266(int index) : _in(256),_out(256){
	_index = index;
	if (_index == 0)
		_uart0 = this;
	if (_index == 1)
		_uart1 = this;

}

UartEsp8266::~UartEsp8266() {
	if (_index == 0)
		_uart0 = 0;
	if (_index == 1)
		_uart1 = 0;
}

void UartEsp8266::init(uint32_t baudrate) {
//	uart_init((UartBautRate)baudrate,(UartBautRate) baudrate);
}

Erc UartEsp8266::write(uint8_t b) {
//	uart_tx_one_char(_index,b);
	return E_OK;
}

void UartEsp8266::receive(uint8_t b){

}

extern "C" void uart0Receive(uint8_t b) {
	if (UartEsp8266::_uart0)
		UartEsp8266::_uart0->receive(b);

}

extern "C" void uart1Receive(uint8_t b) {
	if (UartEsp8266::_uart1)
		UartEsp8266::_uart1->receive(b);
}
