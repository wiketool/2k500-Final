#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>


struct key_table_t {
	unsigned char key_val;
	unsigned char key_code;
	unsigned char key_name[20];
    unsigned int key;
};

int get_ir();

#define KEY_NUM sizeof(key_table) / sizeof(key_table[0])