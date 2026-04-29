ifeq ($(OS),Windows_NT)
SHELL := cmd
.SHELLFLAGS := /C

CC     := clang
LDLINK := lld-link

BUILD_DIR := build
EFI_OBJ   := $(BUILD_DIR)\uefi_main.obj
GFX_OBJ   := $(BUILD_DIR)\gfx.obj
GUI_OBJ   := $(BUILD_DIR)\gui.obj
EXU_OBJ   := $(BUILD_DIR)\exu.obj
EFI_BIN   := $(BUILD_DIR)\BOOTX64.EFI
ESP_DIR   := $(BUILD_DIR)\esp\EFI\BOOT
ISO_ROOT  := $(BUILD_DIR)\iso_root
EFI_IMG   := $(BUILD_DIR)\efiboot.img
ISO_FILE  := $(BUILD_DIR)\nebulaos-x64-uefi.iso

CFLAGS  := -target x86_64-pc-windows-msvc -ffreestanding -fshort-wchar -mno-red-zone -fno-stack-protector -Wall -Wextra -O2 -Isrc
LDFLAGS := /subsystem:efi_application /entry:efi_main /nodefaultlib /out:$(EFI_BIN)

.PHONY: all help check-tools esp espimg iso fulliso run clean

all: check-tools $(EFI_BIN)

help:
	@echo Targets: all, esp, espimg, iso, fulliso, run, clean
	@echo If clang is missing: install LLVM and ensure clang/lld-link are in PATH.

check-tools:
	@where $(CC) >NUL 2>NUL || (echo ERROR: $(CC) not found in PATH. & exit /b 1)
	@where $(LDLINK) >NUL 2>NUL || (echo ERROR: $(LDLINK) not found in PATH. & exit /b 1)

$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(EFI_OBJ): src\uefi_main.c src\efi_min.h src\gfx.h src\gui.h src\exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/uefi_main.c -o $(EFI_OBJ)

$(GFX_OBJ): src\gfx.c src\gfx.h src\efi_min.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/gfx.c -o $(GFX_OBJ)

$(GUI_OBJ): src\gui.c src\gui.h src\gfx.h src\exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/gui.c -o $(GUI_OBJ)

$(EXU_OBJ): src\exu.c src\exu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/exu.c -o $(EXU_OBJ)

$(EFI_BIN): $(EFI_OBJ) $(GFX_OBJ) $(GUI_OBJ) $(EXU_OBJ)
	$(LDLINK) $(LDFLAGS) $(EFI_OBJ) $(GFX_OBJ) $(GUI_OBJ) $(EXU_OBJ)

esp: $(EFI_BIN)
	@if not exist $(ESP_DIR) mkdir $(ESP_DIR)
	@copy /Y $(EFI_BIN) $(ESP_DIR)\BOOTX64.EFI >NUL

espimg: esp
	@echo ERROR: espimg on native cmd.exe requires dd+mkfs.fat+mtools tools.
	@echo Use MSYS2 shell or WSL for espimg/iso targets.
	@exit /b 1

iso:
	@echo ERROR: iso on native cmd.exe requires xorriso + FAT image tools.
	@echo Use MSYS2 shell or WSL, then run: make iso
	@exit /b 1

fulliso: iso

run: $(EFI_BIN)
	@echo Copy $(EFI_BIN) to EFI\BOOT\BOOTX64.EFI on a FAT32 ESP and boot in UEFI mode.

clean:
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)

else
CC     := clang
LDLINK := lld-link

BUILD_DIR := build
EFI_OBJ   := $(BUILD_DIR)/uefi_main.obj
GFX_OBJ   := $(BUILD_DIR)/gfx.obj
GUI_OBJ   := $(BUILD_DIR)/gui.obj
EXU_OBJ   := $(BUILD_DIR)/exu.obj
EFI_BIN   := $(BUILD_DIR)/BOOTX64.EFI
ESP_DIR   := $(BUILD_DIR)/esp/EFI/BOOT
ISO_ROOT  := $(BUILD_DIR)/iso_root
EFI_IMG   := $(BUILD_DIR)/efiboot.img
ISO_FILE  := $(BUILD_DIR)/nebulaos-x64-uefi.iso

CFLAGS  := -target x86_64-pc-windows-msvc -ffreestanding -fshort-wchar -mno-red-zone -fno-stack-protector -Wall -Wextra -O2 -Isrc
LDFLAGS := /subsystem:efi_application /entry:efi_main /nodefaultlib /out:$(EFI_BIN)

.PHONY: all help esp espimg iso fulliso run clean

all: $(EFI_BIN)

help:
	@echo Targets: all, esp, espimg, iso, fulliso, run, clean

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

espimg: esp
	dd if=/dev/zero of=$(EFI_IMG) bs=1M count=64 status=none
	mkfs.fat -F 32 $(EFI_IMG)
	mmd -i $(EFI_IMG) ::/EFI ::/EFI/BOOT
	mcopy -i $(EFI_IMG) $(EFI_BIN) ::/EFI/BOOT/BOOTX64.EFI

iso: espimg
	mkdir -p $(ISO_ROOT)
	cp $(EFI_IMG) $(ISO_ROOT)/efiboot.img
	xorriso -as mkisofs \
		-R -J -V "NEBULAOS" \
		-e efiboot.img -no-emul-boot \
		-o $(ISO_FILE) $(ISO_ROOT)

fulliso: iso
	@echo ISO ready at $(ISO_FILE)

run: $(EFI_BIN)
	@echo "Copy $(EFI_BIN) to EFI/BOOT/BOOTX64.EFI on a FAT32 ESP and boot in UEFI mode."

clean:
	rm -rf $(BUILD_DIR)
endif
