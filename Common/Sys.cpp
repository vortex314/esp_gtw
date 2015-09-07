/*
 * Sys.cpp
 *
 *  Created on: 20-aug.-2014
 *      Author: lieven2
 */
#include "Sys.h"
//*****************************************************************************
//
// Interrupt handler for the system tick counter.
//
//*****************************************************************************

extern "C" void SysTickIntHandler(void) {
	Sys::_upTime++;
}

extern "C" uint64_t getSysUpTime() {
	return Sys::_upTime;
}

uint64_t Sys::_upTime = 0;

uint64_t Sys::upTime() {
	return Sys::_upTime;
}

uint32_t Sys::_errorCount = 0;
void Sys::warn(int err, const char* s) {
	_errorCount++;
}



void Sys::interruptDisable(){
//	__disable_irq();
}

void Sys::interruptEnable(){
//	__enable_irq();
}
