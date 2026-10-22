CC = gcc
AS = nasm
override CFLAGS += -nostdlib -nostartfiles -ffreestanding -Wl,-e,entry -Wl,--oformat,binary -Wl,-T,src/kernel.ld -Wl,-Map,bin/kernel.map -m32 -fno-pie
override ASFLAGS += -f bin
SRC_DIR = src
BIN_DIR = bin
OUT_DIR = out
BOOT = boot
KERNEL = kernel
IMG = nos.img

all: $(IMG)

$(BOOT): $(SRC_DIR)/$(BOOT).asm
	$(AS) $< $(ASFLAGS) -o $(BIN_DIR)/$(BOOT)
$(KERNEL): $(SRC_DIR)/$(KERNEL).c
	$(CC) $< $(CFLAGS) -o $(BIN_DIR)/$(KERNEL)
$(IMG): $(BOOT) $(KERNEL)
	cat $(BIN_DIR)/boot $(BIN_DIR)/kernel > $(OUT_DIR)/$(IMG)

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OUT_DIR)/*

.PHONY: all clean
