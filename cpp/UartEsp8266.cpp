/*
 * UartEsp8266.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: lieven
 */

/*
 * Uart.cpp
 *
 *  Created on: 9-okt.-2013
 *      Author: lieven2
 */

#include "UartEsp8266.h"

IROM UartEsp8266::UartEsp8266(int idx) :
		_rxd(256), _txd(256) {
	_bytesRxd = 0;
	_bytesTxd = 0;
}

IROM UartEsp8266::~UartEsp8266() {
	ERROR(" dtor called ");
}
extern "C" void uart_tx_intr_enable(int bol);
IROM Erc UartEsp8266::write(Bytes& bytes) {
	bytes.offset(0);
	while (_txd.hasSpace() && bytes.hasData()) {
		_txd.write(bytes.read());
	};
	if (_txd.hasData()) {
		uart_tx_intr_enable(1);
	}
	return E_OK;
}

IROM Erc UartEsp8266::write(uint8_t data) {
	if (_txd.hasSpace())
		_txd.write(data);
	if (_txd.hasData()) {
		uart_tx_intr_enable(1);
	}
	return E_OK;
}

IROM uint8_t UartEsp8266::read() {
	return _rxd.read();
}

IROM bool UartEsp8266::hasData() {
	return _rxd.hasData();
}

IROM bool UartEsp8266::hasSpace() {
	return _txd.hasSpace();
}

IROM void UartEsp8266::receive(uint8_t b) {
	_rxd.write(b);
	_bytesRxd++;
}

//__________________________________________________________________ HARDWARE SPECIFIC

IROM void UartEsp8266::init(uint32_t baud) {

}

UartEsp8266* UartEsp8266::_uart0 = 0;

void checkUart0() {
	if (UartEsp8266::_uart0 == 0) {
		UartEsp8266::_uart0 = new UartEsp8266(0);
		for (int i = 0; i < 26; i++)
			UartEsp8266::_uart0->write('A' + i);
	}
}

/**********************************************************
 * USART1 interrupt request handler:
 *********************************************************/
extern "C" IROM void uart0RecvByte(uint8_t b) {
	UartEsp8266::_uart0 = 0;
	UartEsp8266::_uart0->receive(b);
}

extern "C" IROM int uart0SendByte() {
	checkUart0();
	if (UartEsp8266::_uart0->_txd.hasData()) {
		UartEsp8266::_uart0->_bytesTxd++;
		return UartEsp8266::_uart0->_txd.read();

	} else uart_tx_intr_enable(0);
	return -1;
}
