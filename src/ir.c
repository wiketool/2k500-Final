#include "ir.h"

struct key_table_t key_table[] = {
	{ 0x40, KEY_UP, "UP",0x10 },	      { 0x19, KEY_DOWN, "DOWN",0x11 },
	{ 0x7, KEY_LEFT, "LEFT",0x12 },    { 0x9, KEY_RIGHT, "RIGHT",0x13 },
	{ 0x15, KEY_ENTER, "ENTER",0x14 }, { 0xc, KEY_1, "Num:1",0x1 },
	{ 0x18, KEY_2, "Num:2",0x2 },     { 0x5e, KEY_3, "Num:3",0x3 },
	{ 0x8, KEY_4, "Num:4",0x4 },      { 0x1c, KEY_5, "Num:5",0x5 },
	{ 0x5a, KEY_6, "Num:6",0x6 },     { 0x42, KEY_7, "Num:7",0x7 },
	{ 0x52, KEY_8, "Num:8",0x8 },     { 0x4a, KEY_9, "Num:9",0x9 },
	{ 0x16, KEY_0, "Num:0",0x0 },     { 0x45, KEY_A, "A",0xA },
    { 0x46, KEY_B, "B",0xB },	      { 0x47, KEY_C, "C",0xC },
    { 0x44, KEY_D, "D",0xD },	      { 0x43, KEY_E, "E",0xE },
    { 0xd, KEY_F, "F",0xF }
};

static struct input_event inputevent;

static unsigned int get_key_table_id(unsigned int code)
{
	int id;
	for (id = 0; id < KEY_NUM; id++) {
		if (code == key_table[id].key_code) {
			if (KEY_NUM >= id)
				return id;
		}
	}
	return 0xFFFF;
}

int get_ir()
{
	int fd, id = 0;
	int err = 0;
	char *filename = "/dev/input/event3";

	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("Can't open file %s\r\n", filename);
		return -1;
	}
    int result = 0;
    int flag = 0;
	while (1) {
		err = read(fd, &inputevent, sizeof(inputevent));
		if (err > 0) { /* 读取数据成功 */
			//printf("inputevent.type:%x,EV_KEY:%x,EV_REL:%x,EV_ABS:%x,EV_MSC:%x,EV_SW:%x,BTN_MISC:%x\r\n",inputevent.type,EV_KEY,EV_REL,EV_ABS,EV_MSC,EV_SW,BTN_MISC);
			//printf("key %d %s\r\n", inputevent.code, inputevent.value ? "press" : "release");
			id = get_key_table_id(inputevent.code);
			switch (inputevent.type) {
			case EV_KEY:
				if (inputevent.code < BTN_MISC) { /* 键盘键值 */
					printf("key %s %s\r\n",
					       key_table[id].key_name,
					       inputevent.value ? "press" :
								  "release");
                    result = key_table[id].key;
                    flag = 1;
				}
				break;

			/* 其他类型的事件，自行处理 */
			case EV_REL:
				break;
			case EV_ABS:
				break;
			case EV_MSC:
				break;
			case EV_SW:
				break;
			}
		}
        if (flag)
        {
            break;
        }
	}
	return result;
}