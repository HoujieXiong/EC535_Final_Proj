/* 
 * EC535 Final Project - Smart Gardening System
 */

// Includes
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>


// Macros (LIGHT)
#define LIGHT_GPIO 			66			    // GPIO number used for controlling flood light relay
#define TEMP_LOW            21              // Low temperature bound
#define TEMP_HIGH           23              // High temperature bound
#define TEMP_THRESHOLD		27			    // Threshold temperature (in Celsius) below which the grow lights should turn on


// Function declarations
int read_adc_raw (int channel);             // Taking ADC data and outputting digital reading of the voltage for temperature sensors
double raw_to_degree (int raw_value);       // Raw digital reading to degrees
void digitalWrite (int gpio, int value);    // Write HIGH or LOW to a selected GPIO pin (doesn't ensure GPIO pin is set up!!)



int main()
{

	/*************Hardware Setup*****************/

	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-2";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}

    // GPIO SETUP MIGRATED TO SHELL SCRIPT


    
    // Gardening system operation
    while (1) {

        /*************Humidity sensor processes*****************/
        
        // Get I2C device, SI7021 I2C address is 0x40(64)
        ioctl(file, I2C_SLAVE, 0x40);

        // Send humidity measurement command(0xF5)
        char config[1] = {0xF5};
        write(file, config, 1);
        sleep(1);
        float humidity;

        // Read 2 bytes of humidity data
        // humidity msb, humidity lsb
        char data[2] = {0};
        if (read(file, data, 2) != 2)
        {
            printf("Error : Input/output Error \n");
        }
        else
        {
            // Convert the data
            humidity = (((data[0] * 256 + data[1]) * 125.0) / 65536.0) - 6;

            // Output data to screen (at the end)
        }


        // Send temperature measurement command(0xF3) - part of humidity sensor
        config[0] = 0xF3;
        write(file, config, 1); 
        sleep(1);

        // Read 2 bytes of temperature data
        // temp msb, temp lsb
        float cTemp;                            // Celsius temperature (from humidity sensor)
        float fTemp;                            // Fahrenheit temperature 
        FILE* fp;
        if ((fp= fopen ("output.txt","w"))<0) {
            printf("Failed to open output.txt");
        }
        
        if (read(file, data, 2) != 2)
        {
            printf("Error : Input/output Error \n");
        }
        else
        {
            // Convert the data
            cTemp = (((data[0] * 256 + data[1]) * 175.72) / 65536.0) - 46.85;
            fTemp = cTemp * 1.8 + 32;

            // Output data to screen (done after getting average)
        }

        // Finally writing the relative humidity
        fprintf(fp,"Relative Humidity : %.2f RH \n", humidity);


        /*************Temperature sensor (TMP36) readings*****************/
        double degree_avg = 0;
        for (int channel=0 ; channel<2; channel++){
            double degree;
            int raw = read_adc_raw(channel);
            degree = raw_to_degree(raw);
            degree_avg += degree;
            if (raw < 0) {
                fprintf(stderr, "Failed to read ADC channel %d\n", channel);
                return 1;
            }
        }
        // NOTE: Averageing TMP36 readings only because we want to isolate the one on the humidity sensor
        //degree_avg += cTemp;
        degree_avg = degree_avg / 2;
        

        // Print out average temperature to output file
        fprintf(fp,"Temperature: %.2f C \n", degree_avg);   


        // /*************Grow Light Control*****************/

        // If the temperature falls below 27 degrees C, then the grow lights should come on
        if (degree_avg < TEMP_LOW) {

            digitalWrite(LIGHT_GPIO, 1);
            fprintf(fp, "Grow light is on! Too COLD!\n");
        } else if (degree_avg > TEMP_HIGH) {

            digitalWrite(LIGHT_GPIO, 0);
            fprintf(fp, "Grow light is off! Too HOT!\n");
        }

        // Close output file
        fclose(fp);

        sleep(1);
    }

    return 0;
}



/***************************** Function Implementatios *********************************/

// Taking ADC data and outputting digital reading of the voltage for temperature sensors
int read_adc_raw(int channel) {
    char path[64];
    char buf[16];
    int fd, len;

    // build the sysfs path for in_voltage<channel>_raw
    snprintf(path, sizeof(path),
             "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",
             channel);

    // open for read
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open adc raw");
        return -1;
    }

    // read the ASCII number
    len = read(fd, buf, sizeof(buf)-1);
    close(fd);
    if (len <= 0) {
        perror("read adc raw");
        return -1;
    }
    buf[len] = '\0';

    return atoi(buf);
}



// Raw digital reading to degrees
double raw_to_degree(int raw_value){
    double voltage= 1.8 * (double) raw_value /4096;
    double tmp = (voltage-0.5)*100;
    return tmp;
}



// Write HIGH or LOW to a selected GPIO pin (doesn't ensure GPIO pin is set up!!)
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