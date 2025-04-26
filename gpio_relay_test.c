/* GPIO Relay Test - Making sure the C program can write 
 * to a GPIO pin to control a relay that turns ON/OFF a light */ 


// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


// Macros 
#define LIGHT_GPIO 			66			// GPIO number used for controlling flood light relay


void digitalWrite(int gpio, int value) {
    char path[64];
    int fd;

    // Set GPIO value
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", LIGHT_GPIO);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open gpio value");
        return;
    }

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
}


int main () {

    // GPIO setup
	FILE *GPIO_SETUP;
	GPIO_SETUP = fopen("/sys/class/gpio/export", "w");
	if (GPIO_SETUP == NULL) {

		perror("Failed to set up GPIO!\n");
		return -1;
	}
	fprintf(GPIO_SETUP, "%d", LIGHT_GPIO);
	fclose(GPIO_SETUP);

	// Set GPIO direction
    sleep(1);
    int GPIO_DIRECTION;
    char gpio_path[128];
    char out[] = "out";
	snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%d/direction", LIGHT_GPIO);
	printf("Snprintf\n"); // DEBUG
    GPIO_DIRECTION = open(gpio_path, O_WRONLY);
    printf("Opening GPIO_DIRECTION file\n"); // DEBUG
	if (GPIO_DIRECTION < 0) {

		perror("Failed to set up GPIO! Check GPIO number.\n");
		return -1;
	}
    printf("Writing to file\n"); // DEBUG
	write(GPIO_DIRECTION, out, sizeof(out)); 
    printf("Closing file\n"); // DEBUG
    close(GPIO_DIRECTION);

    while (1) {

        sleep(1);
        digitalWrite(LIGHT_GPIO, 1);
        sleep(1);
        digitalWrite(LIGHT_GPIO, 0);
    }

    return 0;
}