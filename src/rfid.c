#include "rfid.h"

static enum IO_CMD {
	READ_CARD = 3,
	CHANGE_PASSWD = 1,
	CHANGE_BLOCK = 2,
	CHANGE_KEY = 5,
	WRITE_CARD = 4,
	GET_ID = 6,
};

int rfid_open()
{
	int rc522_fd = open("/dev/rfid_dev", O_RDWR);
	// printf("test: rc522_fd=%d\n", rc522_fd);
	if (rc522_fd == -1) {
		printf("test: Error Opening rc522\n");
		return (-1);
	}
	return rc522_fd;
};

void rfid_close(int fd)
{
	close(fd);
};

int read_rfid()
{
	int fd = rfid_open();
	unsigned char a[16];
	int result = -1;
	memset(a, 0, sizeof(a));
	while (1) {
		int retvalue = ioctl(fd, GET_ID, &(a[0])); //参数3：选第0块 */
		if (retvalue == 0) {
			for (size_t i = 0; i < 4; i++) {
				memcpy((char *)(&result) + i, &a[i],
				       sizeof(int));
			}
			break;
		}
		usleep(1000000);
	}

	// printf("%d%d%d%d", a[0], a[1], a[2], a[3]);

	rfid_close(fd);
	return result;
};