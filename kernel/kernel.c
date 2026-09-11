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
 * Custom implementation of the standard print_integer function.
 * Converts an integer into a null-terminated string based on the given base.
 */
void print_integer(int number) {
    if(number == 0) {
        print_string("0");
        return;
    }

    if(number < 0) {
        print_string("-");
        number = -number;
    }

    char buffer[12];
    int index = 0;

    while(number > 0) {
        buffer[index++] = '0' + (number % 10);
        number /= 10;
    }
    while(index > 0) {
        char text[2] = { buffer[--index], '\0'};
        print_string(text);
    }
}

/**
 * Custom implementation of the standard itoa function.
 * Converts an integer into a null-terminated string based on the given base.
 */
char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // 1. Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // 2. Handle negative numbers only if base is 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // 3. Process individual digits (extracts them backwards)
    while (num != 0) {
        int rem = num % base;
        // If remainder is greater than 9, map to letters 'a'-'f' (for Hexadecimal)
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // 4. Append the negative sign if necessary
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append null-terminator to seal the string boundary

    // 5. Flip the string array in place because it was extracted backwards
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

/**
 * Kernel Panic Engine
 * Forces the CPU to freeze and locks down the monitor screen to output diagnostics.
 */
void kernel_panic(char *expression, char *file, int line) {
    // 1. Give yourself a fresh slate
    // clear_screen();

    // 2. Format a clear error interface
    print_string("!!! KERNEL PANIC: ASSERTION FAILED !!!\n");
    print_string("--------------------------------------\n");
    
    print_string("Condition broken: ");
    print_string(expression);
    print_string("\n");

    print_string("Location:         ");
    print_string(file);
    print_string(":");
    print_integer(line); // Using your custom dynamic integer printer!
    print_string("\n\n");

    print_string("System execution halted to prevent data corruption.");

    // 3. CRITICAL INTERRUPT: Force the CPU to freeze forever
    while (1) {
        // Complete lockdown
    }
}

/**
 * The Assert Macro Layout
 * If 'cond' is false, it triggers a kernel panic immediately.
 * #cond turns the code logic statement into a printable text string.
 */
#define ASSERT(cond) \
    if (!(cond)) { \
        kernel_panic(#cond, __FILE__, __LINE__); \
    }

/**
 * The main entry point of your operating system engine
 */
void kernel_main() {
    clear_screen();

    print_string("Booting system managers...\n");

    // Simulate checking a hardware condition
    int system_memory_total = 512; 
    
    print_string("Validating minimum hardware requirements...\n");
    
    // TEST PASS: This condition is true, nothing will happen!
    ASSERT(system_memory_total > 256); 
    print_string("Hardware check 1 passed.\n");

    // TEST FAIL: We will trigger a failure by stating memory must be 1024
    ASSERT(system_memory_total == 1024); 

    // This line will NEVER run because the assert will trip and freeze the CPU!
    print_string("This sentence will never be seen."); 

    while (1) {}
}



