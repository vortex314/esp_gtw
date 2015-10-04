/*
 * Flash.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: lieven
 */
#include "Flash.h"
extern "C" {
#include "esp8266.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"
#include "mqtt_msg.h"
#include "debug.h"
#include "user_config.h"
#include "mqtt.h"
#include "queue.h"
#include "ets_sys.h"
};
#include "Sys.h"

uint32_t roundQuad(uint32_t value) {
if ( value & 0x3 ) return (value & 0xFFFFFFFC )+0x4;
else return value;
}

Flash::Flash() {
_keyMax=0;
_freePos=0;
_pageIdx=0;
_sequence=0;
}

Flash::~Flash() {
};

bool Flash::set(const char* key, const char*s) {
return true;
}
bool Flash::set(const char* key, int value) {
return true;
}
void Flash::get(int& value, const char* key, int dflt) {

}
void Flash::get(char* value, int length, const char* key, const char* dflt) {

	}

	void Flash::init() {
		INFO("");
		findOrCreateActivePage();
//		writeItem(0,(uint8_t*)"ABBA",5);
		uint16_t pos= findItem(2);
		Quad w;
		spi_flash_read(pageAddress(_pageIdx)+pos,&w.w,4);
		INFO(" pos : %u ,index : %u , length : %u",pos,w.index,w.length);
		char str[20];
		os_strcpy(str,"NOPE\0");
		for(int i=0;i<w.length;i++) {
			str[i]=flashReadByte(pos+4+i);
		}
		INFO(" loadItem : %s ",str);
	}

	bool Flash::isValidPage(uint32_t pageIdx,uint32_t& sequence) {
		uint32_t magic;
		uint32_t index=0;
		if ( spi_flash_read(pageAddress(pageIdx),&magic,sizeof(magic))!=SPI_FLASH_RESULT_OK ) {
			return false;
		}
		if ( magic != PAGE_SIGNATURE) {
			return false;
		}
		if ( spi_flash_read(pageAddress(pageIdx)+4,&index,sizeof(index)) != SPI_FLASH_RESULT_OK ) {
			return false;
		}
		sequence = index;
		return true;
	}

	uint16_t Flash::findFreeBegin() {
		Quad temp;
		uint16_t offset=8; // after signature and sequence
		while(true) {
			if ( spi_flash_read(pageAddress(_pageIdx)+offset,&temp.w,4)!= SPI_FLASH_RESULT_OK )
			return 0;
			if ( temp.w == 0xFFFFFFFF ) break;
			if ( (temp.index & 1==0) && (temp.index >_keyMax)) _keyMax=temp.index;
			offset = offset + roundQuad(temp.length)+4;
			if ( offset > PAGE_SIZE) break;
		}
		_freePos=offset;
		return offset;
	}

	void Flash::findOrCreateActivePage() {
		INFO("");
		_sequence=0;
		uint32_t sequence;

		for(uint32_t i=0;i< PAGE_COUNT;i++ ) {
			if ( isValidPage(i,sequence) && sequence>_sequence ) {
				_pageIdx=i;
				_sequence=sequence;
			}
		}
		if ( _sequence ==0 ) {
			initializePage(0,1);
			_pageIdx=0;
			_sequence=1;
		}
		findFreeBegin();
		INFO(" pageIdx : %d freePos : %u ",_pageIdx,_freePos);
		/*		writeItem(2,(uint8_t*)"mqtt/host",10);
		 writeItem(4,(uint8_t*)"mqtt/port",10);*/
	}
#define FIRST "FIRST"

	bool Flash::initializePage(uint32_t pageIdx,uint32_t sequence) {
		INFO(" pageIdx : %d sequence : %d ",pageIdx,sequence);
		uint32_t sector = PAGE_START/PAGE_SIZE + pageIdx;
		if ( spi_flash_erase_sector(sector) != SPI_FLASH_RESULT_OK ) return false;
		uint32_t magic=PAGE_SIGNATURE;
		if ( spi_flash_write(pageAddress(pageIdx),&magic, sizeof(magic))!= SPI_FLASH_RESULT_OK ) return false;
		if ( spi_flash_write(pageAddress(pageIdx)+4,&sequence, sizeof(sequence))!= SPI_FLASH_RESULT_OK ) return false;
		return true;
	}

	uint32_t Flash::pageAddress(uint32_t pageIdx) {
		return PAGE_START + ( pageIdx * PAGE_SIZE );
	}

	uint32_t Flash::nextPage(uint32_t pageIdx) {
		return (pageIdx+1)%PAGE_COUNT;
	}

	bool Flash::writeItem( uint16_t index,uint8_t* start,uint32_t length) {
		INFO(" index : %u , length : %u ",index,length);
		uint32_t address = pageAddress(_pageIdx)+_freePos;
		Quad W;
		W.index = index;
		W.length=length;
		INFO(" address 0x%x length %d",address,length);
		if (spi_flash_write(address,&W.w,4)!= SPI_FLASH_RESULT_OK ) return false;
		for(uint32_t i=0;i<length+3;i+=4) {
			W.w=0xFFFFFFFF;
			for(uint32_t j=0;i+j<length && j<4;j++) W.b[j]=start[i+j];
			if (spi_flash_write(address+4+i,&W.w,4)!= SPI_FLASH_RESULT_OK ) return false;
		}
		_freePos += 4+roundQuad(length);
		return true;
	}

	bool Flash::loadItem(uint16_t& offset,uint16_t& index,uint16_t& length ) {
		Quad temp;

		if ( spi_flash_read(pageAddress(_pageIdx)+offset,&temp.w,4)!= SPI_FLASH_RESULT_OK ) return false;
		if ( temp.w == 0xFFFFFFFF ) return false;
		index = temp.index;
		length = temp.length;
		return true;
	}

	uint16_t Flash::findItem(uint16_t idx) {
		Quad temp;
		uint16_t offset=8; // after signature and sequence
		uint16_t lastOffset=0;
		while(true) {
			if ( spi_flash_read(pageAddress(_pageIdx)+offset,&temp.w,4)!= SPI_FLASH_RESULT_OK )
			return 0;
			if ( temp.w == 0xFFFFFFFF ) break;
			if ( temp.index==idx ) {
				lastOffset=offset;
			}
			offset = offset + roundQuad(temp.length)+4;
			if ( offset > PAGE_SIZE) break;
		}
		return lastOffset;
	}

	/*	bool Flash::scanPage(uint32_t pageIdx) {
	 Quad item;
	 _lastPos=8;
	 uint16_t offset=8;
	 uint32_t temp;
	 while ( offset < PAGE_SIZE ) {
	 spi_flash_read(pageAddress(_pageIdx)+item.offset,&temp,4);
	 if ( temp == 0xFFFFFFFF ) break;
	 item.length = temp & 0xFFFF;
	 item.index = temp >> 16;
	 item.offset += 4 + item.length;
	 }

	 return true;
	 }*/
	/*
	 <magic><sequence><idx+len><data..len mod 4 ><break>
	 idx==even : key
	 idx+1==uneven : value of idx+1
	 */

#define CACHE_SIZE	16
	int Flash::flashReadByte(uint32_t location) {
		static Quad W;
		static uint32_t lastLocation=0;
		uint32_t newLocation = location & 0xFFFFFFFC;
		if ( newLocation != lastLocation )
		{
			spi_flash_read(pageAddress(_pageIdx)+newLocation,&W.w,4);
			lastLocation=newLocation;
		}
		return W.b[location&0x03];
	}

