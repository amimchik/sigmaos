#include <sigmaos/vga.h>

static struct vga_cursor_pos cursor_pos;
static vga_c_t *video = (char *)0xb8000;
static vga_attr_t current_attr;

int vga_init()
{
	cursor_pos.x = 0;
	cursor_pos.y = 0;
}

/*
 * shifting this shit up times times
 * should probably work
 */
static int scroll_back(int times)
{
	vga_c_t *video = (vga_c_t *)0xb8000;
	const int cols = 80;
	const int rows = 25;
	const int bytes_per_char = 2;
	const int bytes_per_row = cols * bytes_per_char;
	if (times <= 0 || times > rows) {
		return 0;
	}
	for (int i = 0; i < (rows - times) * bytes_per_row; i++) {
		video[i] = video[i + times * bytes_per_row];
	}
	for (int i = (rows - times) * bytes_per_row; i < rows * bytes_per_row; i += bytes_per_char) {
		video[i] = ' ';
		video[i + 1] = 0x07;
	}
	return times;
}

static int vga_setc(vga_c_t c, vga_attr_t attr)
{
	int pos = cursor_pos.y * VGA_SCREEN_WIDTH + cursor_pos.x;
	video[pos * 2] = c;
	video[pos * 2 + 1] = attr;
}

int vga_putc(vga_c_t c)
{
	if (c == '\r' || c == '\n') {
		struct vga_cursor_pos pos = vga_get_cursor_pos();
		pos.x = 0;
		if (vga_set_cursor_pos(pos))
			return 1;
		if (c == '\r')
			return 0;
	}
	if (c == '\n' || cursor_pos.x >= VGA_SCREEN_WIDTH - 1) {
		if (cursor_pos.y >= VGA_SCREEN_HEIGHT - 1) {
			scroll_back(1);
		} else {
			struct vga_cursor_pos pos = vga_get_cursor_pos();
			pos.y++;
			if (vga_set_cursor_pos(pos))
				return 1;
		}
	}
	if (c == '\n')
		return 0;
	vga_setc(c, 0x07);
	struct vga_cursor_pos pos = vga_get_cursor_pos();
	pos.x++;
	vga_set_cursor_pos(pos);
}

int vga_clear()
{
	current_attr = 0x07;
	for (int x = 0; x < VGA_SCREEN_WIDTH; x++) {
		for (int y = 0; y < VGA_SCREEN_HEIGHT; y++) {
			struct vga_cursor_pos pos;
			pos.x = x;
			pos.y = y;
			vga_set_cursor_pos(pos);
			vga_setc(' ', current_attr);
		}
	}
	struct vga_cursor_pos pos;
	pos.x = 0;
	pos.y = 0;
	vga_set_cursor_pos(pos);
}

int vga_print(vga_c_t *s)
{
	while (*s) {
		vga_putc(*s);
		s++;
	}
}


struct vga_cursor_pos vga_get_cursor_pos()
{
	return cursor_pos;
}

int vga_set_cursor_pos(struct vga_cursor_pos cursor)
{
	cursor_pos = cursor;
	uint16_t pos = cursor.x + cursor.y * VGA_SCREEN_WIDTH;
		if (pos >= 80 * 25) {
		pos = 80 * 25 - 1;
	}
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
