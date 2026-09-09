// Track our position on the 80x25 screen grid globally
int cursor_x = 0;
int cursor_y = 0;

void clear_screen(void) {
    volatile char *video_memory = (volatile char *) 0xB8000;

    for (int cell = 0; cell < 80 * 25; cell++) {
        video_memory[cell * 2] = ' ';
        video_memory[cell * 2 + 1] = 0x0F;
    }
}

void scroll(void) {
    volatile char *video_memory = (volatile char *) 0xB8000;

    // 1. Shift the top 24 rows UP
    for (int cell = 0; cell < 80 * 24; cell++) {
        video_memory[cell * 2] = video_memory[(cell + 80) * 2];
        video_memory[cell * 2 + 1] = video_memory[(cell + 80) * 2 + 1];
    }

    // 2. Clear the bottom row cleanly (Your excellent loop!)
    for (int cell = 80 * 24; cell < 80 * 25; cell++) {
        video_memory[cell * 2] = ' ';
        video_memory[cell * 2 + 1] = 0x0F;
    }

    // 3. Reset coordinate system to the start of the bottom line
    cursor_x = 0;
    cursor_y = 24;
}

/**
 * Advanced print function that understands grid coordinates and newlines (\n)
 */
void print_string(char *str) {
    volatile char *video_memory = (volatile char *) 0xB8000;

    while (*str != '\0') {
        // TRIGGER CHECK: If the text goes past the bottom row, scroll up!
        if (cursor_y >= 25) {
            scroll();
        }

        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            int memory_offset = ((cursor_y * 80) + cursor_x) * 2;
            video_memory[memory_offset] = *str;
            video_memory[memory_offset + 1] = 0x0F;
            cursor_x++;

            if (cursor_x >= 80) {
                cursor_x = 0;
                cursor_y++;
            }
        }
        str++;
    }
}


/**
 * The main entry point of your operating system engine
 */
void kernel_main() {
    clear_screen();

    // Print 30 lines of text to test our engine
    for (int i = 1; i <= 30; i++) {
        print_string("Testing scroll layout engine... Line printed successfully!\n");
    }

    while (1) {}
}

