export CC_PREFIX := /home/wiketool/2k500/toolchain-loongarch64-linux-gnu-gcc8-host-x86_64-2022-07-18
export PATH := $(CC_PREFIX)/bin:$(PATH)
export LD_LIBRARY_PATH := $(CC_PREFIX)/lib:$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH := $(CC_PREFIX)/loongarch64-linux-gnu/lib64:$(LD_LIBRARY_PATH)
export ARCH := loongarch
export CROSS_COMPILE := loongarch64-linux-gnu-
export MY_KDIR := /home/wiketool/2k500/linux-5.10-2k500-cbd-src

CC := $(CROSS_COMPILE)gcc

# Set the include directory
CFLAGS += -Iinclude

# Find all .c files in the src directory
SRC_FILES := $(wildcard src/*.c)

# Define the target binary
TARGET := final

# Define the rule to build the target
$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lrt

# Clean rule to remove the binary
clean:
	rm -f $(TARGET)



