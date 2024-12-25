#include "beep.h"

int beep_open(){
	int fd, ret;
	fd = open("/dev/beep", O_RDWR);
	if(fd == -1) 
	{ 
		printf("Error Opening beep\n"); 
		return(-1); 
	}
	return fd;
};

int beep_close(int fd){
	int retvalue;
	retvalue = close(fd); /* 关闭文件 */
	if (retvalue < 0) {
		printf("file %s close failed!\r\n", "/dev/beep");
		return -1;
	}
	return 0;
};

int beep_on(int fd)
{
	int retvalue;
	unsigned char databuf[1];
	databuf[0] = BEEPON;
	/* 向/dev/beep文件写入数据 */
	retvalue = write(fd, databuf, sizeof(databuf));
	if (retvalue < 0) {
		printf("BEEP Control Failed!\r\n");
		close(fd);
		return -1;
	}
	return 0;
};

int beep_off(int fd) {
	int retvalue;
	unsigned char databuf[1];
	databuf[0] = BEEPOFF;
	/* 向/dev/beep文件写入数据 */
	retvalue = write(fd, databuf, sizeof(databuf));
	if (retvalue < 0) {
		printf("BEEP Control Failed!\r\n");
		close(fd);
		return -1;
	}
	return 0;
};


int warning_beep(int times){
	int fd = beep_open();
	for (size_t i = 0; i < times; i++)
	{
		beep_on(fd);
		usleep(100000);
		beep_off(fd);
		usleep(100000);
	}
	beep_close(fd);
};

