/*
 * rboot_loc.c
 *
 *  Created on: Sep 20, 2015
 *      Author: lieven
 */
#include "c_types.h"
#include "rboot.h"

const rboot_config rom_config ={
		BOOT_CONFIG_MAGIC,
		BOOT_CONFIG_VERSION,	// std version
		MODE_STANDARD, 			// not controlled by gpio
		0,						// current rom
		0,						// gpio rom ( same as std single boot )
		1,						// nbr of roms in use
		{0,0},					// padding
		{0x00002000}

};


