#pragma once
#ifndef _IO_H
#define _IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value) {
	__asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t value;
	__asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

#ifdef __cplusplus
}
#endif

#endif /*!_IO_H*/
