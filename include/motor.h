#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include <arpa/inet.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <math.h> 

#define MAGIC_NUMBER    'k'
#define CMD_standby _IO(MAGIC_NUMBER    ,1)
#define CMD_forward  _IO(MAGIC_NUMBER    ,2)
#define CMD_backward  _IO(MAGIC_NUMBER    ,3)
#define CMD_brake _IO(MAGIC_NUMBER    ,4)

int motor_open();
void motor_close();

void motor_forward();
void motor_backward();