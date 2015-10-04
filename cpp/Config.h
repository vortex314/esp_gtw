/*
 * Config.h
 *
 *  Created on: Oct 1, 2015
 *      Author: lieven
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stdint.h"

class Config {
public:
	virtual ~Config(){};
	virtual bool set(const char* key, const char*s)=0;
	virtual bool set(const char* key, int value)=0;
	virtual void get(int& value, const char* key, int dflt)=0;
	virtual void get(char* value, int length, const char* key,
			const char* dflt)=0;
};

#endif /* CONFIG_H_ */
