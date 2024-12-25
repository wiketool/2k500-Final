#include "aip1944.h"

int aip1944_open()
{
	int aip_fd, ret;
	aip_fd = open("/dev/aip", O_RDWR);
	// printf("test: aip_fd=%d\n", aip_fd);
	if (aip_fd == -1) {
		printf("test: Error Opening aip1944\n");
		return (-1);
	}
	return aip_fd;
};

void aip1944_close(int fd)
{
	close(fd);
	return;
};

void huanyinghuijia()
{
	int fd = aip1944_open();
	int ret;
	int frames = sizeof(arr_huanyinghuijia) / sizeof(u_int8_t) - 32;
	// printf("frames: %d\n", frames);
	int begin = 0;
	while (frames >= 0) {
		ret = write(fd, arr_huanyinghuijia + begin,
			    32 * sizeof(u_int8_t));
		usleep(1000 * 33);
		frames -= 2;
		begin += 2;
	}
	aip1944_close(fd);
};

void aip1944_reset()
{
	int fd = aip1944_open();
	int ret;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	ret = write(fd, buf, 32 * sizeof(char));
	aip1944_close(fd);
}