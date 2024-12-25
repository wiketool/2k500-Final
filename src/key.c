/* 
这是一个4*4的键盘扫描程序，请你帮我优化它以实现全面的扫描功能；
驱动向用户空间传递的是一个8字节的数组，前4个字节代表第几行的按键按下，后4个字节代表列的屏蔽值
用户空间向驱动写入的是一个4字节的数组，每一个数字代表一列的屏蔽与否；设置为0表示屏蔽
1. 取消从命令行传入参数的方式设置键盘的屏蔽值
2. 优化键盘扫描的方式，使得可以同时检测多个按键的按下情况
3. 物理设备上一行有4个按键，有4行；将键盘按键按照行优先的顺序进行编码
4. 为了能够精准地检测按键的按下情况，需要不断切换列的屏蔽状态
 */
// Copyright (c) 2023 Shandong University
// Copyright (c) 2023 Junchi Ren, Jinrun Yang

#include "key.h"

void rotate_column_mask(unsigned char *mask)
{
	// Rotate through columns: enable one column at a time
	static int current_col = 0;

	memset(mask, 0, MATRIX_COLS);
	mask[current_col] = 1;

	current_col = (current_col + 1) % MATRIX_COLS;
}

int key_scan(void)
{
	int fd, ret;
	int keyvalue[KEY_BUFFER_SIZE];
	unsigned char col_mask[MATRIX_COLS] = {
		1, 0, 0, 0
	}; // Start with first column enabled

	fd = open("/dev/key", O_RDWR);
	if (fd < 0) {
		printf("Failed to open /dev/key!\n");
		return -1;
	}

	int read_key = -1;
	int same_cnt = 0;

	while (1) {
		// Update column mask
		rotate_column_mask(col_mask);
		ret = write(fd, col_mask, MATRIX_COLS);
		if (ret < 0) {
			printf("Failed to write column mask!\n");
			break;
		}

		// Read key states
		ret = read(fd, keyvalue, sizeof(keyvalue));
		if (ret < 0) {
			printf("Failed to read key states!\n");
			break;
		}

		const int *col_val = keyvalue + MATRIX_ROWS;
		int offset = -1;
		int flag = 0;
		for (size_t i = 0; i < MATRIX_COLS; i++) {
			if (col_val[i]) {
				offset = i;
				for (size_t j = 0; j < MATRIX_ROWS; j++) {
					if (keyvalue[j]) {
						int now = j * MATRIX_ROWS +
							  offset;
						if (now == read_key) {
							same_cnt++;

						} else {
							read_key = now;
							same_cnt = 0;
						}
						// 重复读到3次一样的视为按下
						if (same_cnt > 2) {
							flag = 1;
							break;
						}
					}
				}
			}
		}
		if (flag)
		{
			break;
		}
		
		usleep(50000); // 50ms delay for scanning rate
	}

	close(fd);
	return read_key;
}
