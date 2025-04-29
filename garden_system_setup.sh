# EC535 Gardening System shell script


# Open GPIO 66 to the file system and set it as output
echo 66 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio66/direction

# Run program
make clean
make 
./write_tmp_hum