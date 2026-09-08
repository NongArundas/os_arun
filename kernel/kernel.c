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

/**
 * Advanced print function that understands grid coordinates and newlines (\n)
 */
void print_string(char *str) {
    // Base address of the VGA video memory card
    volatile char *video_memory = (volatile char *) 0xB8000;

    // Loop through each character until we hit the null-terminator
    while (*str != '\0') {
        
        // Check for a Newline character
        if (*str == '\n') {
            cursor_x = 0;    // Reset back to the left wall
            cursor_y++;      // Move down 1 row
        } 
        // If it's a regular letter, print it
        else {
            // Formula: Find the exact byte index for the current (X, Y) coordinate
            int memory_offset = ((cursor_y * 80) + cursor_x) * 2;
            
            video_memory[memory_offset] = *str;         // Drop character into Slot A
            video_memory[memory_offset + 1] = 0x0F;     // Drop color byte into Slot B

            cursor_x++; // Move 1 column to the right
            
            // Safety Check: If we run off the right edge of the screen, wrap to next line
            if (cursor_x >= 80) {
                cursor_x = 0;
                cursor_y++;
            }
        }

        str++; // Move your finger to the next character in your text string
    }
}

/**
 * The main entry point of your operating system engine
 */
void kernel_main() {
    clear_screen();

    // Testing out our new multi-line system with \n!
    print_string("Welcome to Arun OS!\n");
    print_string("-------------------\n");
    print_string("Line 1: Your custom bare-metal kernel is running.\n");
    print_string("Line 2: Newlines are successfully working via coordinate math.");

    while (1) {
        // Keep the CPU safely running in place
    }
}
