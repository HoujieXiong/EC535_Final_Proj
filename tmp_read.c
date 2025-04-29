// read_adc2_raw.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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
    double tmp = (voltage-0.5)*100;
    return tmp;
}

int main(void) {
    int channel = 0;  // AIN0
    double degree;
    int raw = read_adc_raw(channel);
    degree = raw_to_degree(raw);
    if (raw < 0) {
        fprintf(stderr, "Failed to read ADC channel %d\n", channel);
        return 1;
    }
    printf("AIN%d raw = %d\n degree = %f", channel, raw, degree);
    return 0;
}
