#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#define BEEPOFF 0
#define BEEPON 1

int beep_on();
int beep_off();

int warning_beep(int times);

int beep_open();
int beep_close();