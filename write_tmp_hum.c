#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>



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


double raw_to_degree(int raw_value){
    double voltage= 1.8 * (double) raw_value /4096;
    double tmp = (voltage-0.5)/10;
    return tmp;
}





int main()
{
    // write file destination 

    FILE* fp;
    if((fp= fopen ("output.txt","w"))<0){
        printf("Failed to open output.txt");
    }


	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}


    while (1){
        // Get I2C device, SI7021 I2C address is 0x40(64)
        ioctl(file, I2C_SLAVE, 0x40);

        // Send humidity measurement command(0xF5)
        char config[1] = {0xF5};
        write(file, config, 1);
        sleep(1);

        // Read 2 bytes of humidity data
        // humidity msb, humidity lsb
        char data[2] = {0};
        if(read(file, data, 2) != 2)
        {
            printf("Error : Input/output Error \n");
        }
        else
        {
            // Convert the data
            float humidity = (((data[0] * 256 + data[1]) * 125.0) / 65536.0) - 6;

            // Output data to screen
            fprintf(fp,"Relative Humidity : %.2f RH \n", humidity);
        }

        // Send temperature measurement command(0xF3)
        config[0] = 0xF3;
        write(file, config, 1); 
        sleep(1);

        // Read 2 bytes of temperature data
        // temp msb, temp lsb
        if(read(file, data, 2) != 2)
        {
            printf("Error : Input/output Error \n");
        }
        else
        {
            // Convert the data
            float cTemp = (((data[0] * 256 + data[1]) * 175.72) / 65536.0) - 46.85;
            float fTemp = cTemp * 1.8 + 32;

            // Output data to screen
            fprintf(fp,"Temperature in Celsius : %.2f C \n", cTemp);
            fprintf(fp,"Temperature in Fahrenheit : %.2f F \n", fTemp);
        }




        for (int channel=0 ; channel<3;channel++){
            double degree;
            int raw = read_adc_raw(channel);
            degree = raw_to_degree(raw);
            if (raw < 0) {
                fprintf(stderr, "Failed to read ADC channel %d\n", channel);
                return 1;
            }
            printf("AIN%d raw = %d\n degree = %f", channel, raw, degree);   
        }
        sleep(1);
    }   
}




