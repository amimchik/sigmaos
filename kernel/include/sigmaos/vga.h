#pragma once
#ifndef _VGA_H
#define _VGA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sigmaos/io.h>

#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25

typedef unsigned char vga_c_t;
typedef unsigned char vga_attr_t ;

struct vga_cursor_pos {
	int x;
	int y;
};

int vga_init();

int vga_putc(vga_c_t c);
int vga_clear();

struct vga_cursor_pos vga_get_cursor_pos();
int vga_set_cursor_pos(struct vga_cursor_pos cursor);

int vga_print(vga_c_t *s);

#ifdef __cplusplus
}
#endif

#endif /*!_VGA_H*/
