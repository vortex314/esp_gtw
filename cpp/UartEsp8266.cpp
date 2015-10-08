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

extern "C" uint32_t overflowTxd=0;

 IROM UartEsp8266::UartEsp8266() :
		_rxd(256), _txd(2048) {
	_bytesRxd = 0;
	_bytesTxd = 0;
	_overflowTxd=0;
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
	if ( bytes.hasData()) {
		_overflowTxd++;
		overflowTxd++;
	}
	if (_txd.hasData()) {
		uart_tx_intr_enable(1);
	}
	return E_OK;
}

IROM Erc UartEsp8266::write(uint8_t data) {
	if (_txd.hasSpace())
		_txd.write(data);
	else {
		_overflowTxd++;
		overflowTxd++;
	}
	if (_txd.hasData()) {
		uart_tx_intr_enable(1);
	}
	return E_OK;
}

IROM uint8_t UartEsp8266::read() {
	return _rxd.read();
}

bool UartEsp8266::hasData() { // not in IROM as it will be called in interrupt
	return _rxd.hasData();
}

IROM bool UartEsp8266::hasSpace() {
	return _txd.hasSpace();
}

void UartEsp8266::receive(uint8_t b) { // not in IROM as it will be called in interrupt
	_rxd.writeFromIsr(b);
	_bytesRxd++;
}

//__________________________________________________________________ HARDWARE SPECIFIC

IROM void UartEsp8266::init(uint32_t baud) {

}

UartEsp8266* UartEsp8266::_uart0 = 0;
UartEsp8266* UartEsp8266::_uart1 = 0;

void checkUart0() {
	if (UartEsp8266::_uart0 == 0) {
		UartEsp8266::_uart0 = new UartEsp8266();
		for (int i = 0; i < 26; i++)
			UartEsp8266::_uart0->write('a' + i);
	}
}

/**********************************************************
 * USART1 interrupt request handler:
 *********************************************************/
// incoming char enqueue it
extern "C" void uart0RecvByte(uint8_t b) {
	checkUart0();
	UartEsp8266::_uart0->receive(b);
}
// get next byte to transmit or return -1 if not available/empty circbuf
extern "C" int uart0SendByte() {
	checkUart0();
	if (UartEsp8266::_uart0->_txd.hasData()) {
		UartEsp8266::_uart0->_bytesTxd++;
		return UartEsp8266::_uart0->_txd.readFromIsr();
	}
	return -1;
}
// enqueue char to send in TXD
extern "C" void uart0Write(uint8_t b) {
	checkUart0();
	if (b == '\n') {
		UartEsp8266::_uart0->write('\r');
		UartEsp8266::_uart0->write(b);
	} else if (b == '\r') {

	} else {
		UartEsp8266::_uart0->write(b);
	}
}
