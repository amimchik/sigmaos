#include <sigmaos/vga.h>

void kernel_main(void) {
	vga_init();
	vga_clear();
	vga_print("Hello, World!\nanother line");
}
