#define ESP_COREDUMP
#ifdef ESP_COREDUMP

//#include <stdio.h>
#include <string.h>
#include <ets_sys.h>
#include <xtensa/corebits.h>
#include <stdint.h>
#include "esp_gdb.h"
#include "esp_exc.h"
#include "v7_esp_hw.h"
#include "esp_uart.h"
#include "esp_missing_includes.h"
#include "v7_esp.h"
#include "base64.h"
#include "Sys.h"

#define uart_puts(__fd,__arg ) INFO(__arg);
#define uart_putchar(__fd,__arg ) uart0Write(__arg);

#define ESP_COREDUMP_UART_NO 1
#define ESP_COREDUMP_FILENO (ESP_COREDUMP_UART_NO + 1)

/* output an unsigned decimal integer */
static void uart_putdec(int fd, unsigned int n) {
	unsigned int tmp;
	unsigned long long p = 1;

	for (tmp = n; tmp > 0; tmp /= 10) {
		p *= 10;
	}
	p /= 10;
	if (p == 0) {
		p = 1;
	}

	for (; p > 0; p /= 10) {
		uart_putchar(fd, '0' + (unsigned int) (n / p) % 10);
	}
}

static int core_dump_emit_char_fd = 0;
static void core_dump_emit_char(char c, void *user_data) {
	int *col_counter = (int *) user_data;
#ifdef RTOS_SDK
	system_soft_wdt_feed();
#endif
	(*col_counter)++;
	uart_putchar(core_dump_emit_char_fd, c);
	if (*col_counter >= 160) {
		uart_putchar(core_dump_emit_char_fd, '\n');
		(*col_counter) = 0;
	}
}

#define __stringify_1(x...) #x
#define __stringify(x...) __stringify_1(x)
#define RSR(sr)                                       \
  ({                                                  \
    uint32_t r;                                       \
    asm volatile("rsr %0,"__stringify(sr) : "=a"(r)); \
    r;                                                \
  })

void dump_stack() {
	uint32_t lv = 0;
	uint32_t* start = &lv;
	uint32_t* end = 0x40000000;
	uint32_t* ptr = start;
	os_printf_plus("@(#):STACK_START 0x%X\n",start);
//	INFO("@(#):%8X:%8X", 0xA0, RSR(A0));
	while (ptr < end) {
		if ((*ptr > 0x40000000 && *ptr < 0x60000000) // only print CODE locations
		|| (*ptr > 0x3ff00000 && *ptr < 0x40000000) // data
				)
			os_printf_plus("@(#):%8X:%8X\n", ptr, *ptr);
//	    INFO("%8x : %8x %8x %8x %8x ",ptr,*ptr,*(ptr+1),*(ptr+2),*(ptr+3));
		ptr += sizeof(uint32_t);
	}
	os_printf_plus("@(#):STACK_END\n");

}

/* address must be aligned to 4 and size must be multiple of 4 */
void emit_core_dump_section(int fd, const char *name, uint32_t addr,
		uint32_t size) {
	struct cs_base64_ctx ctx;
	int col_counter = 0;
	uart_puts(fd, ",\"");
	uart_puts(fd, name);
	uart_puts(fd, "\": {\"addr\": ");
	uart_putdec(fd, addr);
//  uart_puts(fd, ", \"data\": \"");
	core_dump_emit_char_fd = fd;
//  cs_base64_init(&ctx, core_dump_emit_char, &col_counter);

	uint32_t end = addr + size;
	while (addr < end) {
		uint32_t* ptr;
		ptr = ((uint32_t *) addr);

		addr += sizeof(uint32_t) * 4;
		INFO("%8x : %8x %8x %8x %8x ",
				addr, *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
//    cs_base64_update(&ctx, (char *) &buf, sizeof(uint32_t));
	}
//  cs_base64_finish(&ctx);

//  uart_puts(fd, "\"}");
}

void esp_dump_core(int fd, struct regfile *regs) {
	uint32_t lv = 0;

	uart_puts(fd, "--- BEGIN CORE DUMP ---\n");

	uart_puts(fd, "{\"arch\": \"ESP8266\"");
	emit_core_dump_section(fd, "REGS", (uintptr_t) regs, sizeof(*regs));
	emit_core_dump_section(fd, "DRAM", 0x3FFE8000, 0x18000);
	emit_core_dump_section(fd, "STACK", 0x3FFFF000, 0x1000);
	emit_core_dump_section(fd, "STACK2", &lv, 0x40000000 - (uint32_t) &lv);
	/* rtos relocates vectors here */
//  emit_core_dump_section(fd, "VEC", 0x40100000, 0x1000);
//  emit_core_dump_section(fd, "ROM", 0x40000000, 0x10000);
	uart_puts(fd, "}\n");

	/*
	 * IRAM and IROM can be obtained from the firmware/ dir.
	 * We need the ELF binary anyway to do symbolic debugging anyway
	 * so we can avoid sending here huge amount of data that's available
	 * on the host where we run GDB.
	 */

	uart_puts(fd, "---- END CORE DUMP ----\n");
}

#endif /* ESP_COREDUMP */
