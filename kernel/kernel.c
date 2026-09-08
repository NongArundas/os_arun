/**
 * Custom function to print an entire text string to the top-left of the screen.
 * It manually loops through characters and shoves them into the VGA hardware matrix.
 */
 void print_string(char *str) {
    // Point directly to the hardware video memory address (Top-Left cell)
    volatile char *video_memory = (volatile char *) 0xB8000;

    // Loop through each character until we hit the null-terminator ('\0')
    while (*str != '\0') {
        
        *video_memory = *str;          // Drop the character byte into Slot A (Message)
        *(video_memory + 1) = 0x0F;    // Drop the color byte into Slot B (White on Black)

        video_memory += 2;             // Walk 2 full steps forward to the next mailbox
        str++;                         // Move your finger to the next letter on the shelf
    }
}

/**
 * The main entry point of your operating system engine
 */
void kernel_main() {
    
    // Clear any leftover BIOS text by calling our printing function with a message
    print_string("Welcome to Arun OS! Your custom bare-metal kernel is running successfully. \n");

    // Keep the CPU safely running in place forever
    while (1) {
        // Wait here for future hardware interrupts
    }
}

