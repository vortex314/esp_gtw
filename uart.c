/*
 * File	: uart.c
 * Copyright (C) 2013 - 2016, Espressif Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * LMR REDUCED TO THE MAX
 */
#include "stdint.h"
#include "ets_sys.h"
#include "osapi.h"
#include "uart.h"
#include "osapi.h"
#include "uart_register.h"
#include "mem.h"
#include "os_type.h"

extern UartDevice UartDev;

extern void uart0RecvByte(uint8_t b);
extern void uart0Write(uint8_t b);

LOCAL void uart0_rx_intr_handler(void *para);

/******************************************************************************
 * FunctionName : uart_config
 * Description  : Internal used function
 *                UART0 used for data TX/RX, RX buffer size is 0x100, interrupt enabled
 *                UART1 just used for debug output
 * Parameters   : uart_no, use UART0 or UART1 defined ahead
 * Returns      : NONE
 *******************************************************************************/LOCAL void ICACHE_FLASH_ATTR
uart0_config() {
	uint8 uart_no = UART0;
	/* rcv_buff size if 0x100 */
	ETS_UART_INTR_ATTACH(uart0_rx_intr_handler, &(UartDev.rcv_buff));
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
	uart_div_modify(uart_no, UART_CLK_FREQ / (UartDev.baut_rate)); //SET BAUDRATE

	WRITE_PERI_REG(UART_CONF0(uart_no),
			((UartDev.exist_parity & UART_PARITY_EN_M) << UART_PARITY_EN_S) //SET BIT AND PARITY MODE
			| ((UartDev.parity & UART_PARITY_M) <<UART_PARITY_S ) | ((UartDev.stop_bits & UART_STOP_BIT_NUM) << UART_STOP_BIT_NUM_S) | ((UartDev.data_bits & UART_BIT_NUM) << UART_BIT_NUM_S));

	SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
	//clear rx and tx fifo,not ready
	CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
	//RESET FIFO

	WRITE_PERI_REG(
			UART_CONF1(uart_no), //set rx fifo trigger
			((100 & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) | (0x02 & UART_RX_TOUT_THRHD) << UART_RX_TOUT_THRHD_S | UART_RX_TOUT_EN| ((0x10 & UART_TXFIFO_EMPTY_THRHD)<<UART_TXFIFO_EMPTY_THRHD_S));

	SET_PERI_REG_MASK(UART_INT_ENA(uart_no),
			UART_RXFIFO_TOUT_INT_ENA |UART_FRM_ERR_INT_ENA);

	//clear all interrupt
	WRITE_PERI_REG(UART_INT_CLR(uart_no), 0xffff);
	//enable rx_interrupt
//	SET_PERI_REG_MASK(UART_INT_ENA(uart_no),
//			UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_OVF_INT_ENA|UART_RXFIFO_TOUT_INT_ENA|UART_TXFIFO_EMPTY_INT_ENA);
	SET_PERI_REG_MASK(UART_CONF1(UART0),
			(UART_TX_EMPTY_THRESH_VAL & UART_TXFIFO_EMPTY_THRHD)<<UART_TXFIFO_EMPTY_THRHD_S);
//	SET_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
	SET_PERI_REG_MASK(UART_INT_ENA(uart_no),
			UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_OVF_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
//	uart_tx_intr_enable(1);
	uart_rx_intr_enable(UART0);
}

LOCAL void ICACHE_FLASH_ATTR
uart1_config() {
	uint8 uart_no = UART1;
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
	uart_div_modify(uart_no, UART_CLK_FREQ / (UartDev.baut_rate)); //SET BAUDRATE
	WRITE_PERI_REG(UART_CONF0(uart_no),
			((UartDev.exist_parity & UART_PARITY_EN_M) << UART_PARITY_EN_S) //SET BIT AND PARITY MODE
			| ((UartDev.parity & UART_PARITY_M) <<UART_PARITY_S ) | ((UartDev.stop_bits & UART_STOP_BIT_NUM) << UART_STOP_BIT_NUM_S) | ((UartDev.data_bits & UART_BIT_NUM) << UART_BIT_NUM_S));

	//clear rx and tx fifo,not ready
	SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
	//RESET FIFO
	CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);

	WRITE_PERI_REG(UART_CONF1(uart_no),
			((UartDev.rcv_buff.TrigLvl & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S));

	//clear all interrupt
	WRITE_PERI_REG(UART_INT_CLR(uart_no), 0xffff);
	//enable rx_interrupt

	SET_PERI_REG_MASK(UART_INT_ENA(uart_no),
			UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_OVF_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
	uart_tx_intr_enable(1);
}

static unsigned int uart_rx_fifo_length(void) {
	return ((READ_PERI_REG(UART_STATUS(0)) >> UART_RXFIFO_CNT_S)
			& UART_RXFIFO_CNT);
}

static unsigned int uart_tx_fifo_length(void) {
	return ((READ_PERI_REG(UART_STATUS(0)) >> UART_TXFIFO_CNT_S)
			& UART_TXFIFO_CNT);
}

void uart_tx_intr_enable(int bol) {
	if (bol) {
		SET_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
	}
	else
		CLEAR_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
}

void uart_rx_intr_disable(uint8 uart_no) {
	CLEAR_PERI_REG_MASK(UART_INT_ENA(uart_no),
			UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
}

void uart_rx_intr_enable(uint8 uart_no) {
	SET_PERI_REG_MASK(UART_INT_ENA(uart_no),
			UART_RXFIFO_FULL_INT_ENA|UART_RXFIFO_TOUT_INT_ENA);
}

uint32_t uartRxdCount = 0;
uint32_t uartTxdCount = 0;
uint32_t uartErrorCount = 0;

/******************************************************************************
 * FunctionName : uart0_rx_intr_handler
 * Description  : Internal used function
 *                UART0 interrupt handler, add self handle code inside
 * Parameters   : void *para - point to ETS_UART_INTR_ATTACH's arg
 * Returns      : NONE
 *******************************************************************************/

LOCAL void uart0_rx_intr_handler(void *para) {
	ETS_UART_INTR_DISABLE();

	uint32_t int_status = READ_PERI_REG(UART_INT_ST(UART0));

	if (int_status & UART_FRM_ERR_INT_ST) { // FRAMING ERROR
		// clear rx fifo (apparently this is not optional at this point)
		SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST);
		CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST);
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_FRM_ERR_INT_CLR);
		uartErrorCount++;

	};
	if (int_status & UART_RXFIFO_FULL_INT_ST) { // RXD FIFO FULL

		while (uart_rx_fifo_length()) {
			uart0RecvByte(READ_PERI_REG(UART_FIFO(0)));
			uartRxdCount++;
		}

		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);

	};

	if (int_status & UART_RXFIFO_TOUT_INT_ST) { // RXD FIFO TIMEOUT

		while (uart_rx_fifo_length()) {
			uart0RecvByte(READ_PERI_REG(UART_FIFO(0)));
			uartRxdCount++;
		}
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);

	};
	if (int_status & UART_TXFIFO_EMPTY_INT_ST) { // TXD FIFO EMPTY

		int b;
		while ((uart_tx_fifo_length() < 126) && ((b = uart0SendByte()) >= 0)) {
			uartTxdCount++;
			WRITE_PERI_REG(UART_FIFO(UART0), (uint8_t) b);
		}
		if (b < 0) { // no more chars to send
			uart_tx_intr_enable(0);
		}
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_TXFIFO_EMPTY_INT_CLR);

	};
	if (int_status & UART_RXFIFO_OVF_INT_ST) {

		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_OVF_INT_CLR);
		uartErrorCount++;
	}

	if (READ_PERI_REG(UART_INT_ST(UART1))) {
		WRITE_PERI_REG(UART_INT_CLR(UART1), 0xFFFF);
	}
	ETS_UART_INTR_ENABLE();

}

void ICACHE_FLASH_ATTR
uart_init(UartBautRate uart0_br, UartBautRate uart1_br) {

	UartDev.baut_rate = uart0_br;
	uart0_config();
	UartDev.baut_rate = uart1_br;
	uart1_config();
	ETS_UART_INTR_ENABLE();

	os_install_putc1((void *) uart0Write);
}

