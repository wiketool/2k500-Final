#include "ch422.h"

const unsigned char BCD_decode_tab[ 29 ] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71, 0x00, 0x46, 0x40, 0x41, 0x39, 0x0F, 0x08, 0x76, 0x38, 0x73, 0x80, 0xFF, 0x00 };

int count_max = 9999, count_curr = 0;
unsigned char databuf[5];

void set_single_tube(int* value){
	int fd = open_digital_tube();

    for (int i = 0; i < 4; i++)
	{
		// printf("value[%d]: %d\n", i, value[i]);
		databuf[i+1] = BCD_decode_tab[value[i]];
	}

	write(fd, databuf, sizeof(databuf));
    close_digital_tube(fd);
};


void set_digital_tube(int count){
    int fd = open_digital_tube();

    for (int i = 4; i >= 1; i--)
	{
		databuf[i] = BCD_decode_tab[count % 10];
		count /= 10;
	}

	write(fd, databuf, sizeof(databuf));
    close_digital_tube(fd);
}


void reset_digital_tube(){
    int fd = open_digital_tube();

    databuf[1] = 0x0;
	databuf[2] = 0x0;
	databuf[3] = 0x0;
	databuf[4] = 0x0;

	write(fd, databuf, sizeof(databuf));
    close_digital_tube(fd);
}


int open_digital_tube(){
    int fd, ret, i, tmp;

	fd = open("/dev/ch422g", O_RDWR); 
	// printf("test: fd=%d\n", fd); 
	if(fd == -1) 
	{ 
		printf("test: Error Opening ch422g\n"); 
		return(-1); 
	}
    return fd;
}

int close_digital_tube(int fd){
    return close(fd);
}
