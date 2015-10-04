/*
 * Flash.h
 *
 *  Created on: Oct 1, 2015
 *      Author: lieven
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "Config.h"

#define PAGE_SIZE 0x1000
#define PAGE_START	0x78000
#define PAGE_COUNT 4		// 78000,79000,7A000,7B000
#define PAGE_SIGNATURE 0xDEADBEEF

typedef struct  {
	union {
		uint32_t w;
		struct {
			uint16_t index;
			uint16_t length;
		};
		uint8_t b[4];
	};
} Quad;

class Flash: public Config {
private:
	uint32_t _pageIdx;
	uint32_t _sequence;
	uint32_t _freePos;
	uint16_t _keyMax;

	void findOrCreateActivePage();
	bool initializePage(uint32_t pageIdx, uint32_t sequence);
	bool scanPage(uint32_t pageIdx);
	static uint32_t pageAddress(uint32_t pageIdx);
	bool isValidPage(uint32_t pageIdx, uint32_t& sequence);
	uint32_t nextPage(uint32_t pageIdx);
	bool loadItem(uint16_t& offset,uint16_t& index,uint16_t& length );
	uint16_t findItem(uint16_t index );
	bool writeItem( uint16_t sequence, uint8_t* start,uint32_t length) ;
	uint16_t findFreeBegin();
	int flashReadByte(uint32_t location) ;
	int findKey(const char*s);
	int findOrCreateKey(const char*s);
	uint32_t flashReadQuad(uint32_t location);
	bool loadItem(uint16_t offset,uint8_t* start,uint16_t& length );

public:
	Flash();
	~Flash();
	void init();
	bool set(const char* key, const char*s);
	bool set(const char* key, int value);
	void get(int& value, const char* key, int dflt);
	void get(char* value, int length, const char* key, const char* dflt);
};

#endif /* FLASH_H_ */
