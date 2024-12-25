#include "motor.h"

int motor_open(){
    int fd,ret; 
	fd = open("/dev/motor", O_RDWR); 
	// printf("test: fd=%d\n", fd);
	if(fd == -1) 
	{ 
		printf("test: Error Opening motor\n"); 
		return(-1); 
	}
    return fd;
};

void motor_close(int fd){
    close(fd);
};

void motor_forward(){
    int fd = motor_open();
    ioctl(fd,CMD_forward);
    sleep(1);
    ioctl(fd,CMD_brake);
    motor_close(fd);
};

void motor_backward(){
    int fd = motor_open();
    ioctl(fd,CMD_backward);
    sleep(1);
    ioctl(fd,CMD_brake);
    motor_close(fd);
};