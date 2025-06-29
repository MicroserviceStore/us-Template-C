#***************************************************************************
# 
# Microservice (uService) Make file
#
#***************************************************************************

# We support only GCC for now
TOOLCHAIN=GCC

#***************************************************************************
# Includes
#***************************************************************************
include Configurations/$(CONFIG).config
include Environment/Toolchain/$(TOOLCHAIN)/toolchain.mk
include Environment/CPU/$(uSERVICE_CPU_CORE)/uServicePackage/Toolchain/$(TOOLCHAIN)/flags.mk

#***************************************************************************
# Defines
#***************************************************************************
uSERVICE_PACKAGE_PATH := Environment/CPU/$(uSERVICE_CPU_CORE)/uServicePackage
OUTPUT_PATH:=Output/$(uSERVICE_CPU_CORE)/$(TOOLCHAIN)
OUTPUT_IMAGE:=$(OUTPUT_PATH)/Image
OUTPUT_IMAGE_DETAILS:=$(OUTPUT_PATH)/ImageDetails
USERLIB_NAME:=libuS-$(uSERVICE_NAME)UserLib_$(TOOLCHAIN).a
OUTPUT_USERLIB_PATH:=$(OUTPUT_PATH)/UserLib
OUTPUT_MSSTOREPACK_FOLDER:=MicroserviceStorePackage
OUTPUT_MSSTOREPACK_PATH:=$(OUTPUT_PATH)/$(OUTPUT_MSSTOREPACK_FOLDER)
MICROSERVICE_ATTRIBUTES_PATH:=$(OUTPUT_PATH)/MicroserviceAttributes.json

#***************************************************************************
# Default values for optional defines
#***************************************************************************
uSERVICE_VERSION_STR ?= 0.1.0
CPU_MEM_RANGES_NEEDS_EXP_ROUNDING ?= 1
uSERVICE_GCC_LD_PATH ?= $(uSERVICE_PACKAGE_PATH)/Toolchain/$(TOOLCHAIN)/microservice_template.ld

#***************************************************************************
# Build Objects
#***************************************************************************
SOURCE_FILES := \
	$(uSERVICE_PACKAGE_PATH)/Toolchain/$(TOOLCHAIN)/vectors.s \
	$(uSERVICE_SOURCE_FILES)

INCLUDE_DIRS := \
	-I$(uSERVICE_PACKAGE_PATH)/Include \
	-I$(uSERVICE_PACKAGE_PATH)/Include/SysCall \
	-I$(uSERVICE_PACKAGE_PATH)/Include/uService \
	-IConfigurations \
	$(uSERVICE_INCLUDE_DIRS)

CFLAGS := \
	$(CPU_CFLAGS) \
	$(uSERVICE_CFLAGS) \
	-DUSERVICE_NAME=\"$(uSERVICE_NAME)\" \
	-DUSERVICE_NAME_NONSTR=$(uSERVICE_NAME) \
	-DUSERVICE_PUBLIC_HEADER=\"us-$(uSERVICE_NAME).h\" \
	-DUSERVICE_INTERNAL_HEADER=\"us-$(uSERVICE_NAME)_Internal.h\" \
	-DUSERVICE_VERSION_STR=\"$(uSERVICE_VERSION_STR)\"

LD_FLAGS := \
	$(CPU_LDFLAGS) \
	$(uSERVICE_LDFLAGS) \
	-Wl,-defsym=MICROSERVICE_CODE_CAPACITY=$(uSERVICE_CODE_SIZE) \
	-Wl,-defsym=MICROSERVICE_RAM_CAPACITY=$(uSERVICE_RAM_SIZE) \
	-Wl,-defsym=MICROSERVICE_MAINSTACK_SIZE=$(uSERVICE_MAINSTACK_SIZE) \

SOURCE_FILES_USERLIB := \
	Source/UserLib/UserLib.c

INCLUDE_DIRS_USERLIB := \
	$(INCLUDE_DIRS)

CFLAGS_USERLIB := \
	$(CFLAGS)

#***************************************************************************
# Rules
#***************************************************************************
.PHONY: all package microservice userlib output output_userlib

all: microservice userlib

microservice: $(uSERVICE_NAME).elf
$(uSERVICE_NAME).elf: output
	@echo -e "\nBuilding" $(uSERVICE_NAME) "Microservice..."
	@echo -e "---------------------------------------------"
	@echo -e " - Code Capacity     : " $(uSERVICE_CODE_SIZE)
	@echo -e " - RAM Capacity      : " $(uSERVICE_RAM_SIZE)
	@echo -e " - Main Stack Size   : " $(uSERVICE_MAINSTACK_SIZE) "\n"
### Build The Microservice
	@$(CC) $(CFLAGS) $(SOURCE_FILES) $(INCLUDE_DIRS) $(LD_FLAGS) -T $(uSERVICE_GCC_LD_PATH) -L"$(uSERVICE_PACKAGE_PATH)" -lZAYASysCall_GCC -Xlinker -Map=$(OUTPUT_PATH)/$@.map -o $(OUTPUT_PATH)/$@
	@echo -e "---------------------------------------------"
	@$(SZ) $(OUTPUT_PATH)/$@
	@echo -e "\n$(PRINT_OK)Build Completed...$(PRINT_RESET)"

### Helper Outputs
	@$(OD) -D $(OUTPUT_PATH)/$@ > $(OUTPUT_IMAGE_DETAILS)/$(patsubst %.elf,%.objdump,$@)
	@$(OC) -S $(OUTPUT_PATH)/$@ -O binary $(OUTPUT_IMAGE)/$(patsubst %.elf,%.bin,$@)
	@$(OC) -S $(OUTPUT_PATH)/$@ -O ihex $(OUTPUT_IMAGE)/$(patsubst %.elf,%.hex,$@)
	@$(SZ) $(OUTPUT_PATH)/$@ > $(OUTPUT_IMAGE_DETAILS)/$(patsubst %.elf,%.size,$@)
	@$(NM) -S --size-sort $(OUTPUT_PATH)/$@ > $(OUTPUT_IMAGE_DETAILS)/$(patsubst %.elf,%.map,$@)
	@$(NM) -n -l -C $(OUTPUT_PATH)/$@ > $(OUTPUT_IMAGE_DETAILS)/$(patsubst %.elf,%.sym,$@)

	@rm -rf $(MICROSERVICE_ATTRIBUTES_PATH)
	@echo -e "{\n\t\"Name\":\"$(uSERVICE_NAME)\",\n\t\"Version\":\"$(uSERVICE_VERSION_STR)\"," > $(MICROSERVICE_ATTRIBUTES_PATH)

### Optimisation Recommendations for Developers
#   Check Code Capacity vs Code Size
	@CODE_USAGE=$$($(SZ) $(OUTPUT_PATH)/$@ | tail -n1 | awk '{print $$1 + $$2}'); \
	P=$(uSERVICE_CODE_SIZE); \
	if [ "$(CPU_MEM_RANGES_NEEDS_EXP_ROUNDING)" = "1" ]; then \
		P=1; \
		while [ $$P -lt $$CODE_USAGE ]; do P=$$(($$P * 2)); done; \
		if [ $$P != $(shell printf "%d\n" $(uSERVICE_CODE_SIZE)) ]; then \
			echo -e "$(PRINT_RECOMMENDATION) RECOMMENDATION: Microservice Code Capacity($(uSERVICE_CODE_SIZE)) could be ($$P) for memory efficiency.$(PRINT_RESET)"; \
		fi; \
	fi; \
	echo -e "\t\"CodeCapacity\":$$P," >> $(MICROSERVICE_ATTRIBUTES_PATH)

#   Check RAM Capacity vs RAM Size
	@RAM_USAGE=$$($(SZ) $(OUTPUT_PATH)/$@ | tail -n1 | awk '{print $$2 + $$3}'); \
	P=$(uSERVICE_RAM_SIZE); \
	if [ "$(CPU_MEM_RANGES_NEEDS_EXP_ROUNDING)" = "1" ]; then \
		P=1; \
		while [ $$P -lt $$RAM_USAGE ]; do P=$$(($$P * 2)); done; \
		if [ $$P != $(shell printf "%d\n" $(uSERVICE_RAM_SIZE)) ]; then \
			echo -e "$(PRINT_RECOMMENDATION) RECOMMENDATION: Microservice RAM Capacity($(uSERVICE_RAM_SIZE)) could be ($$P) for memory efficiency.$(PRINT_RESET)"; \
		fi; \
	fi; \
	echo -e "\t\"RAMCapacity\":$$P," >> $(MICROSERVICE_ATTRIBUTES_PATH)

	@echo -e "}" >> $(MICROSERVICE_ATTRIBUTES_PATH)

$(USERLIB_NAME): output_userlib
	@$(CC) $(CFLAGS_USERLIB) $(INCLUDE_DIRS_USERLIB) -c $(SOURCE_FILES_USERLIB) && $(AR) rcs $(OUTPUT_USERLIB_PATH)/$@ *.o && rm -f *.o
	@cp Include/us-$(uSERVICE_NAME).h $(OUTPUT_USERLIB_PATH)/
	@echo -e "\n$(PRINT_OK)User Package Generation Completed...$(PRINT_RESET)"

userlib: $(USERLIB_NAME)

output:
	@mkdir -p $(OUTPUT_PATH)
	@mkdir -p $(OUTPUT_IMAGE)
	@mkdir -p $(OUTPUT_IMAGE_DETAILS)
	@mkdir -p $(OUTPUT_USERLIB_PATH)

output_userlib:
	@mkdir -p $(OUTPUT_PATH)
	@mkdir -p $(OUTPUT_USERLIB_PATH)

output_msstorepackage:
	@mkdir -p $(OUTPUT_PATH)
	@mkdir -p $(OUTPUT_MSSTOREPACK_PATH)

clean:
	@rm -rf $(OUTPUT_PATH)

package: output_msstorepackage all check_zip
	@cp $(OUTPUT_IMAGE)/$(uSERVICE_NAME).bin $(OUTPUT_MSSTOREPACK_PATH)/
	@cp -r $(OUTPUT_USERLIB_PATH)/ $(OUTPUT_MSSTOREPACK_PATH)/
	@cp $(MICROSERVICE_ATTRIBUTES_PATH) $(OUTPUT_MSSTOREPACK_PATH)/
	@cd $(OUTPUT_PATH) && zip -rq MSStore_$(uSERVICE_NAME)_$(uSERVICE_VERSION_STR).zip $(OUTPUT_MSSTOREPACK_FOLDER)
	@echo -e "\n$(PRINT_OK)Microservice Store Package Generation Completed...$(PRINT_RESET)"
