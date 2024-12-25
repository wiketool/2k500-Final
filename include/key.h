#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#define MATRIX_ROWS 4
#define MATRIX_COLS 4
#define KEY_BUFFER_SIZE 8

int key_scan(void);