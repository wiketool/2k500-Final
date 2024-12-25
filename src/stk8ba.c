#include "stk8ba.h"

int stk8ba_open()
{
	int fd;
	char *filename = "/dev/stk8ba";
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("can't open file %s\r\n", filename);
		return -1;
	}
	return fd;
};

void stk8ba_close(int fd)
{
	close(fd);
};

short axis_out_encode(unsigned char out1, unsigned char out2)
{
	short val;
	val = (out1 >> 4) | (out2 << 4); 
	if (val & (1 << 11))
	{
		val |= 0xF000;
	}
	return val;
}


void stk8ba_moitor(int fd, short *xyz)
{
	unsigned char databuf[6];
	int ret;
	unsigned char xout1, xout2, yout1, yout2, zout1, zout2;
	short x, y, z;
	ret = read(fd, databuf, sizeof(databuf));
	if (ret == 0) { /* 数据读取成功 */
		xout1 = databuf[0];
		xout2 = databuf[1];
		yout1 = databuf[2];
		yout2 = databuf[3];
		zout1 = databuf[4];
		zout2 = databuf[5];
		xyz[0] = axis_out_encode(xout1, xout2);
		xyz[1] = axis_out_encode(yout1, yout2);
		xyz[2] = axis_out_encode(zout1, zout2);
	}
	// printf("x:%d, y:%d, z:%d\n", x, y, z);
	usleep(100000); /*500ms */
};