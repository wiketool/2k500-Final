#include "led.h"

int led_open()
{
	int fd, retvalue;
	fd = open("/dev/led", O_RDWR);
	if (fd < 0) {
		printf("file /dev/led open failed!\r\n");
		return -1;
	}
	return fd;
};

void led_close(int fd)
{
	int retvalue;
	retvalue = close(fd); /* 关闭文件 */
	if (retvalue < 0) {
		printf("file %s close failed!\r\n", "/dev/led");
	}
};

void led_write(int fd, unsigned char databuf[3])
{
	int retvalue;
	retvalue = write(fd, databuf, 3 * sizeof(unsigned char));
	if (retvalue < 0) {
		printf("LED Control Failed!\r\n");
		close(fd);
	}
};

void reset_led(int fd)
{
	unsigned char databuf[3];
	databuf[0] = 0; /* 要执行的操作：打开或关闭 */
	databuf[1] = 0; /* 要执行的操作：打开或关闭 */
	databuf[2] = 0; /* 要执行的操作：打开或关闭 */
	// printf("databuf[0]:%d,databuf[1]:%d,databuf[2]:%d\r\n", databuf[0],
	//    databuf[1], databuf[2]);
	/* 向/dev/led文件写入数据 */
	led_write(fd, databuf);
}

void led_wrong_password()
{
	int fd = led_open();
	unsigned char databuf[3];
	databuf[0] = 0; /* 要执行的操作：打开或关闭 */
	databuf[1] = 0; /* 要执行的操作：打开或关闭 */
	databuf[2] = 1; /* 要执行的操作：打开或关闭 */
	for (size_t i = 0; i < 5; i++) {
		led_write(fd, databuf);
		usleep(100000);
		reset_led(fd);
		usleep(100000);
	}
	led_close(fd);
};

void led_right_password()
{
	int fd = led_open();
	unsigned char databuf[3];
	databuf[0] = 1; /* 要执行的操作：打开或关闭 */
	databuf[1] = 0; /* 要执行的操作：打开或关闭 */
	databuf[2] = 0; /* 要执行的操作：打开或关闭 */
	for (size_t i = 0; i < 5; i++) {
		led_write(fd, databuf);
		usleep(100000);
		reset_led(fd);
		usleep(100000);
	}
	led_close(fd);
};

void led_boom()
{
	int fd = led_open();
	unsigned char databuf[3];
	databuf[0] = 1; /* 要执行的操作：打开或关闭 */
	databuf[1] = 1; /* 要执行的操作：打开或关闭 */
	databuf[2] = 1; /* 要执行的操作：打开或关闭 */
	for (size_t i = 0; i < 5; i++) {
		led_write(fd, databuf);
		usleep(100000);
		reset_led(fd);
		usleep(100000);
	}
	led_close(fd);
}