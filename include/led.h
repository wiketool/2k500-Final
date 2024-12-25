#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

int led_open();
void led_close();

void led_wrong_password();
void led_right_password();
void led_boom();