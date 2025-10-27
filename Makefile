# Compilers & tools
ARMGNU		= aarch64-linux-gnu
CC			= $(ARMGNU)-gcc
LD			= $(ARMGNU)-ld
OBJCOPY		= $(ARMGNU)-objcopy
QEMU		= qemu-system-aarch64
RM			= rm -f

# Flags
CFLAGS		= -Iinclude -fno-stack-protector
LDFLAGS		= -T src/linker.ld
QEMUFLAGS	= -M raspi3b -kernel $(TARGET).img -display none -S -s

# Files & directories
TARGET		= kernel8
BUILD_DIR	= build
SRC_DIR		= src
SRC_C		= $(wildcard $(SRC_DIR)/*.c)
SRC_S		= $(wildcard $(SRC_DIR)/*.S)
OBJ_C		= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_C))
OBJ_S		= $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%.o, $(SRC_S))
OBJ			= $(OBJ_C) $(OBJ_S)

# Rules
all: $(TARGET).img

$(TARGET).img: $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).img

$(TARGET).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) $(OBJ) $(TARGET).elf $(TARGET).img

run: $(TARGET).img
	$(QEMU) $(QEMUFLAGS)

.PHONY: all clean run
