#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <time.h>

#include "key.h"
#include "ch422.h"
#include "beep.h"
#include "ir.h"
#include "motor.h"
#include "led.h"
#include "rfid.h"
#include "stk8ba.h"

#define MAX_MSG_SIZE 256
#define QUEUE_NAME "/input_queue"

unsigned int nfc_passwd = 3895543395;
unsigned int key_passwd = 0x1234;
unsigned int admin_passwd = 0x9561;

// Add global security flag
volatile int security_breach = 0;

// Event types
typedef enum { EVENT_KEY, EVENT_NFC } EventType;

// Event structure
typedef struct {
	EventType type;
	int value;
} InputEvent;

int get_passwd()
{
	// 0 1 2 3
	int key_scans[4];
	int key_scan_idx = 0;
	int count = 0;
	memset(key_scans, 00, sizeof(key_scans));
	while (1) {
		// 读取键盘输入
		int key = key_scan();
		if (key == 15) {
			memset(key_scans, 00, sizeof(key_scans));
			reset_digital_tube();
			key_scan_idx = 0;
		} else {
			key_scans[key_scan_idx] = key;
			key_scan_idx++;
			set_single_tube(key_scans);
		}
		warning_beep(1);
		if (key_scan_idx >= 4) {
			break;
		}
	}
	for (int i = 0; i < 4; i++) {
		count = count * 16 + key_scans[i];
	}
	return count;
};

void wrong_passwd()
{
	warning_beep(3);
	reset_digital_tube();
	led_wrong_password();
}

void right_passwd()
{
	reset_digital_tube();
	motor_forward();
	led_right_password();
	huanyinghuijia();
	aip1944_reset();
}

// Thread function for key monitoring
void *key_monitor(void *arg)
{
	mqd_t mq = (mqd_t)arg;
	InputEvent event;
	while (1) {
		int key = get_passwd(); // Assuming this blocks until key press
		event.type = EVENT_KEY;
		event.value = key;
		mq_send(mq, (char *)&event, sizeof(InputEvent), 0);
	}
	return NULL;
}

// Thread function for NFC monitoring
void *nfc_monitor(void *arg)
{
	mqd_t mq = (mqd_t)arg;
	InputEvent event;
	while (1) {
		int nfc = read_rfid(); // Assuming this blocks until NFC read
		event.type = EVENT_NFC;
		event.value = nfc;
		mq_send(mq, (char *)&event, sizeof(InputEvent), 0);
	}
	return NULL;
}

void security_moitor()
{
	int fd = stk8ba_open();
	short xyz_prev[3];
	stk8ba_moitor(fd, xyz_prev);
	short xyz[3];

	// 添加稳定状态计时
	time_t stable_start = 0;
	int is_stable = 1;

	while (1) {
		stk8ba_moitor(fd, xyz);
		// 与之前状态比较，检测是否稳定
		if (abs(xyz[0] - xyz_prev[0]) > 100 ||
		    abs(xyz[1] - xyz_prev[1]) > 100 ||
		    abs(xyz[2] - xyz_prev[2]) > 100) {
			warning_beep(3);
			security_breach = 1;
			is_stable = 0;
			stable_start = 0;
		} else {
			// 系统稳定
			if (!is_stable) {
				// 开始计时
				if (stable_start == 0) {
					stable_start = time(NULL);
				}
				// 检查是否稳定超过1秒
				if (time(NULL) - stable_start >= 1) {
					security_breach = 0;
					is_stable = 1;
					stable_start = 0;
					warning_beep(1); // 提示解除警报
				}
			}
		}
		memcpy(xyz_prev, xyz, sizeof(xyz));
		usleep(100000); // 100ms采样间隔
	}
	stk8ba_close(fd);
}

int read_passwd()
{
	// Check security status first
	if (security_breach) {
		warning_beep(3);
		return -1; // Return invalid password when system is locked
	}

	// Initialize message queue
	struct mq_attr attr = { .mq_flags = 0,
				.mq_maxmsg = 10,
				.mq_msgsize = sizeof(InputEvent),
				.mq_curmsgs = 0 };
	mqd_t mq;
	mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
	if (mq == (mqd_t)-1) {
		perror("mq_open");
		return -1;
	}

	// Create threads
	pthread_t key_thread, nfc_thread;
	pthread_create(&key_thread, NULL, key_monitor, (void *)mq);
	pthread_create(&nfc_thread, NULL, nfc_monitor, (void *)mq);

	// Main event loop
	int user_passwd = 0x1234;
	int admin_passwd = 0xFAB1;

	while (1) {
		InputEvent event;
		mq_receive(mq, (char *)&event, sizeof(InputEvent), NULL);

		int passwd;
		if (event.type == EVENT_KEY) {
			passwd = event.value;
		} else if (event.type == EVENT_NFC) {
			passwd = event.value;
		}

		// Cancel threads and cleanup
		pthread_cancel(key_thread);
		pthread_cancel(nfc_thread);

		// Wait for threads to finish
		pthread_join(key_thread, NULL);
		pthread_join(nfc_thread, NULL);

		// Close and unlink message queue
		mq_close(mq);
		mq_unlink(QUEUE_NAME);

		return passwd;
	}
}

void admin_mode()
{
	int admin_flag = 0;
	while (!admin_flag) {
		int mode = get_ir();
		switch (mode) {
		case 0x14: {
			admin_flag = 1;
			reset_digital_tube();
			warning_beep(2);
			break;
		}
		case 0x1: {
			right_passwd();
			break;
		}
		case 0x2: {
			wrong_passwd();
			break;
		}
		case 0x3: {
			led_boom();
			break;
		}
		case 0xA: {
			unsigned int key = get_passwd();
			admin_passwd = key;
			warning_beep(1);
			break;
		}
		case 0xB: {
			unsigned int key = get_passwd();
			key_passwd = key;
			warning_beep(1);
		}
		case 0xC: {
			unsigned int key = read_rfid();
			nfc_passwd = key;
			warning_beep(1);
		}
		default:
			break;
		}
	}
}