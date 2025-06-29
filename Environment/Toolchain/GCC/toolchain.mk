# TOOLCHAIN TOOLS
uSERVICE_TOOLCHAIN_GCC_PATH ?=
uSERVICE_TOOLCHAIN_GCC_TOOLCHAIN_PREFIX ?= arm-none-eabi-
CROSS_COMPILE := $(uSERVICE_TOOLCHAIN_GCC_PATH)$(uSERVICE_TOOLCHAIN_GCC_TOOLCHAIN_PREFIX)
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)gcc
NM := $(CROSS_COMPILE)nm
OD := $(CROSS_COMPILE)objdump
OC := $(CROSS_COMPILE)objcopy
SZ := $(CROSS_COMPILE)size
RE := $(CROSS_COMPILE)readelf
STRIP := $(CROSS_COMPILE)strip

# Print Coloring
PRINT_WARNING=\033[1;33m⚠️
PRINT_ERROR=\033[1;31m❌
PRINT_OK_GREEN=\033[0;32m✅
PRINT_OK=\033[0;32m✅\033[0m
PRINT_RECOMMENDATION=\033[1;33m🌟
PRINT_RESET=\033[0m

check_zip:
	@if ! command -v zip >/dev/null 2>&1; then \
		echo "📦 zip not found. Attempting to install..."; \
		if command -v pacman >/dev/null 2>&1; then \
			echo "🔧 Installing with pacman..."; \
			pacman -S --noconfirm zip; \
		elif command -v apt-get >/dev/null 2>&1; then \
			echo "🔧 Installing with apt..."; \
			sudo apt-get update && sudo apt-get install -y zip; \
		elif command -v yum >/dev/null 2>&1; then \
			echo "🔧 Installing with yum..."; \
			sudo yum install -y zip; \
		else \
			echo "❌ Could not detect package manager. Please install 'zip' manually."; \
			exit 1; \
		fi; \
	fi
