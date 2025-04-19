/* Xiang Jin, Houjie Xiong
 * EC535 Final Project
 * 
 * BeagleBone Black on-board software (to be run on startup) */


// Includes
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>


int main() {

    while (1) {

        int uart0_filestream = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
        if (uart0_filestream == -1) {
            perror("Failed to open UART");
            return 1;
        }

        // Configure UART
        struct termios options;
        tcgetattr(uart0_filestream, &options);

        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     // Baud rate: 9600, 8-bit, enable receiver
        options.c_iflag = IGNPAR;                           // Ignore parity errors
        options.c_oflag = 0;
        options.c_lflag = 0;

        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);

        // Write to UART
        const char *message = "Hello from BeagleBone!\n";
        int count = write(uart0_filestream, message, strlen(message));
        if (count < 0) {
            perror("UART TX error");
        }

        // Read from UART (non-blocking for demo)
        char rx_buffer[256];
        memset(rx_buffer, 0, sizeof(rx_buffer));
        int rx_length = read(uart0_filestream, rx_buffer, sizeof(rx_buffer) - 1);
        if (rx_length < 0) {
            printf("No data received\n");
        } else if (rx_length == 0) {
            printf("No data available\n");
        } else {
            rx_buffer[rx_length] = '\0';
            printf("Received: %s\n", rx_buffer);
        }

        close(uart0_filestream);
    }
    
    return 0;
}
