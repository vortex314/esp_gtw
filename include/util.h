#ifndef util_h
#define util_h
#ifdef __cplusplus
extern "C" {
#endif
#include "ets_sys.h"

extern void os_printf_plus(const char* s, ...);
extern void logShort(const char * format, ...);

#define debug(fmt,...) SysLog(__FILE__,__FUNCTION__,fmt,##__VA_ARGS__)
#define info(fmt,...)  SysLog(__FILE__, __PRETTY_FUNCTION__ ,fmt,##__VA_ARGS__)
#define BOOL uint8_t

typedef enum __attribute__ ((__packed__))
{
	off = 0,
	no = 0,
	on = 1,
	yes = 1
} bool_t;

//_Static_assert(sizeof(bool_t) == 1, "sizeof(bool_t) != 1");

#define IROM __attribute__((section(".irom0.text")))
#define IRAM __attribute__((section(".text")))
#define noinline __attribute__ ((noinline))
#define attr_pure __attribute__ ((pure))
#define attr_const __attribute__ ((const))



// replacement for nasty #defines that give warnings

void pin_func_select(uint32_t pin_name, uint32_t pin_func);

// prototypes missing
#include <stdarg.h>
void *pvPortMalloc(size_t);
int ets_vsnprintf(char *, size_t, const char *, va_list);

void ets_isr_attach(int, void *, void *);
void ets_isr_mask(unsigned intr);
void ets_isr_unmask(unsigned intr);
//void ets_timer_arm_new(ETSTimer *, uint32_t, bool, int);
void ets_timer_disarm(ETSTimer *);
void ets_timer_setfn(ETSTimer *, ETSTimerFunc *, void *);
//void ets_delay_us(uint16_t);

// local utility functions missing from libc

int snprintf(char *, size_t, const char *, ...) __attribute__ ((format (printf, 3, 4)));
void *malloc(size_t);

// other convenience functions

void reset(void);
const char *yesno(bool_t value);
const char *onoff(bool_t value);
int dprintf(const char *fmt, ...);
void msleep(uint16_t);
uint16_t double_to_string(double value, uint8_t precision, double top_decimal, uint16_t size, char *dst);
double string_to_double(const char *);
#ifdef __cplusplus
};
#endif
#endif
