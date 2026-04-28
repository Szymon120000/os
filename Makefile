CC     := clang
LDLINK := lld-link

BUILD_DIR := build
EFI_OBJ   := $(BUILD_DIR)/uefi_main.obj
GFX_OBJ   := $(BUILD_DIR)/gfx.obj
GUI_OBJ   := $(BUILD_DIR)/gui.obj
EXU_OBJ   := $(BUILD_DIR)/exu.obj
EFI_BIN   := $(BUILD_DIR)/BOOTX64.EFI
ESP_DIR   := $(BUILD_DIR)/esp/EFI/BOOT

CFLAGS  := -target x86_64-pc-windows-msvc -ffreestanding -fshort-wchar -mno-red-zone -fno-stack-protector -Wall -Wextra -O2 -Isrc
LDFLAGS := /subsystem:efi_application /entry:efi_main /nodefaultlib /out:$(EFI_BIN)

.PHONY: all esp run clean

all: $(EFI_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EFI_OBJ): src/uefi_main.c src/efi_min.h src/gfx.h src/gui.h src/exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(GFX_OBJ): src/gfx.c src/gfx.h src/efi_min.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(GUI_OBJ): src/gui.c src/gui.h src/gfx.h src/exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXU_OBJ): src/exu.c src/exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EFI_BIN): $(EFI_OBJ) $(GFX_OBJ) $(GUI_OBJ) $(EXU_OBJ)
	$(LDLINK) $(LDFLAGS) $^

esp: $(EFI_BIN)
	mkdir -p $(ESP_DIR)
	cp $(EFI_BIN) $(ESP_DIR)/BOOTX64.EFI

run: $(EFI_BIN)
	@echo "Copy $(EFI_BIN) to EFI/BOOT/BOOTX64.EFI on a FAT32 ESP and boot in UEFI mode."

clean:
	rm -rf $(BUILD_DIR)
