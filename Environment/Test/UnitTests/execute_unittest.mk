################################################################################
#
# @file execute_unittest.mk
#
# @brief Makefile to build and run an unit test
#			> Builds and Runs Unit test
#			> Runs Code Coverage
#			> Runs Static Analysis Tools
#			> TODO Runs Sanity Checks
#
#
#
#	- Download GCOV (Code Coverage)
#
#	- Clone ThirdPartyTools next to the OSCore Directory
#		Includes
#			- SPLINT
#			- Unity
#
#
#################################################################################
#
# Copyright (c) 2025 Microservice Store - All rights reserved.
#
# Unauthorised copying of this file, via any medium is strictly prohibited.
#
################################################################################


################################################################################
#                    		DEFINITIONS & INCLUDES                             #
################################################################################

PRINT_WARNING=\033[1;33m⚠️
PRINT_RESET=\033[0m

ROOT_DIR := ../../..
CURR_RELATIVE_DIR = Environment/Test/UnitTests
CURR_DIR = $(ROOT_DIR)/$(CURR_RELATIVE_DIR)
AIGEN_DIR = Include/AIGenerated

CC = gcc
UNITTEST_TARGET_EXTENSION = .out
MODULE?= Microservice

UNITTEST_TOOLS_REPO = https://github.com/MicroserviceStore/uSUnitTestTools.git
UNITTEST_TOOLS_DIR = Tools/uSUnitTestTools
UNITTEST_TOOLS_FULL_PATH = $(CURR_RELATIVE_DIR)/$(UNITTEST_TOOLS_DIR)

TEST_DIR = $(CURR_RELATIVE_DIR)

# Path of Unity Tool
UNITY_ROOT = $(UNITTEST_TOOLS_FULL_PATH)/Unity

# Path of out files after Unity Build
UNITY_OUT_PATH = $(CURR_RELATIVE_DIR)/out

# Path of SPLINT (Static Code Analysis) Tool
SPLINT_PATH=$(UNITTEST_TOOLS_DIR)/splint-3.1.1
# Path of SPLINT App
SPLINT_EXE_PATH=$(CURR_RELATIVE_DIR)/$(SPLINT_PATH)/bin/splint.exe
# Path of SPLINT Lib
SPLINT_LIB_PATH="$(CURR_RELATIVE_DIR)/$(SPLINT_PATH)/lib"

# Test file name
TEST_FILE_NAME = unittest
# Test source file
TEST_FILE = $(TEST_FILE_NAME).c
DEST_TEST_FILE = $(TEST_FILE)
# Test runner file
TEST_RUNNER_FILE = $(UNITY_OUT_PATH)/$(TEST_FILE_NAME)_runner.c

#
# Test output (executable) file
#
TARGET = $(UNITY_OUT_PATH)/$(MODULE)$(UNITTEST_TARGET_EXTENSION)

UNITTEST_CFLAGS = \
		-m32 -std=c99 -Wall -Wextra -Werror -Wpointer-arith -Wcast-align -Wwrite-strings \
		-Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers \
		-Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition -Wno-unused-function \
		-Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-switch-default

#
# All source file for unit test
#	- Unity.c : Source code of Unity tool
#	- Unit Test source file
#	- Test Runner source file
#
ALL_SRC_FILES= \
	$(UNITY_ROOT)/unity.c \
	$(TEST_DIR)/$(TEST_FILE) \
	$(TEST_RUNNER_FILE)

#
# Include Directories
#	- Header files of Unity Tool
#	- Project Common path for common type definitions
#
INC_DIRS = \
	-I$(UNITY_ROOT) \
	-I$(TEST_DIR) \
	-I$(TEST_DIR)/Mock \
	-I$(AIGEN_DIR)

#
# CFLAGS
#	- Code Coverity Flags for GCOV Tool
#	- Environment specific Unit test flags (UNITTEST_CFLAGS)
#
CFLAGS += \
	-ftest-coverage -fprofile-arcs -g -O0 --coverage -Wno-int-to-pointer-cast \
	$(UNITTEST_CFLAGS)

ifeq ($(AIGENERATED),1)
    CFLAGS += -DUS_AI_GENERATED=1
endif

#
# Compiler Symbols
#
UNITTEST_SYMBOLS += \
	-DUNIT_TEST

SPLINT_SYMBOLS = \
	-DUNIT_TEST \
	-DSP_LINT

ifeq ($(AIGENERATED),1)
    SPLINT_SYMBOLS += -DUS_AI_GENERATED=1
endif
#
# splint (static code analysis) Flags
#
SPLINT_FLAGS = \

################################################################################
#                    		     RULES                                   	   #
################################################################################

#
# Default Rule
#	- Runs Unit Test (Unity)
#	- Runs Code Coverity (GCOV)
#	- Static Code Analyis (splint)
#
default: \
	intro \
	prerequisites \
	run_unittest \
	run_codecovarege \
	run_codeanalysis
	rm -rf 

prerequisites: $(UNITTEST_TOOLS_FULL_PATH)
	@test -f $(AIGEN_DIR)/unittest.c && cp $(AIGEN_DIR)/unittest.c $(CURR_RELATIVE_DIR)

#
# Introduction for Tested Module
#
intro:
	@echo
	@echo
	@echo
	@echo "********************************************************************"
	@echo "********************************************************************"
	@echo
	@echo "  >> Testing \" $(MODULE) \" Module"
	@echo
	@echo "********************************************************************"
	@echo "********************************************************************"
	@echo

#
# Rule to run Unit Test
#
run_unittest:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "----------------- WHITEBOX SW TESTS ---------------------------"
	@echo "---------------------------------------------------------------"
	@echo

# Create directory for Test out
	@test -d $(UNITY_OUT_PATH) || mkdir -p $(UNITY_OUT_PATH)

# Unit Test source needs unity.h file but relative location of unity.h can
# be different for each module unit and unit test developer needs to
# include unity.h with its relative path in unit test source file.
# To avoid that, we copy the unity.h into Unit Test directory temproralily
# In this way, developer just include without relative path.
# When compiling is finished, we remove unity.h
	@test -d $(TEST_DIR)/unity.h || cp $(UNITY_ROOT)/unity.h $(TEST_DIR)

#
# Unity requires Test Runner Module and following RUBY script extracts test
# runner modules automatically from Unit Test file
#
	@ruby \
		$(UNITY_ROOT)/scripts/generate_test_runner.rb \
		$(TEST_DIR)/$(TEST_FILE) \
		$(TEST_RUNNER_FILE)

#
# Compile all unit test
#
	@rm -rf *.gcno
	@rm -rf *.gcda
	@$(CC) $(CFLAGS) $(INC_DIRS) $(UNITTEST_SYMBOLS) $(ALL_SRC_FILES) -o $(TARGET)
	@ls *.gcno 2>/dev/null 1>&2 && mv *.gcno $(UNITY_OUT_PATH)

# After compiling, we do not need unity.h in Unit Test directory, remove it.
	@rm $(TEST_DIR)/unity.h

#
# Run Unit Test and see results
#
	@./$(TARGET)
	@echo
# GCOV tool requires unit test code for code coverage analysis, to work on
# single directory, we also copy unit test source file into unity out directory
	@ls *.gcda 2>/dev/null 1>&2 && mv *.gcda $(UNITY_OUT_PATH)

#
# Rule to run code coverage on tested module
#
run_codecovarege:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "---------------- TEST CODE COVERAGE ---------------------------"
	@echo "---------------------------------------------------------------"
# Run GCOV tool to see code coverage
	@cp $(TEST_DIR)/$(TEST_FILE) $(UNITY_OUT_PATH)/$(DEST_TEST_FILE)
	@gcov $(UNITY_OUT_PATH)/$(DEST_TEST_FILE) > $(UNITY_OUT_PATH)/code_coverage.txt
	@mv *.gcov $(UNITY_OUT_PATH)
	@python $(CURR_RELATIVE_DIR)/Tools/code_coverage.py $(UNITY_OUT_PATH)/code_coverage.txt
	@echo

#
# Rule to run Static Code Analysis
#
run_codeanalysis:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "--------------- SOURCE CODE ANALYSIS --------------------------"
	@echo "---------------------------------------------------------------"
	@echo
	@LARCH_PATH=$(SPLINT_LIB_PATH) ./$(SPLINT_EXE_PATH) $(TEST_DIR)/$(TEST_FILE) $(INC_DIRS) $(SPLINT_SYMBOLS) $(SPLINT_FLAGS)
	@echo
	@echo "---------------------------------------------------------------"
	@echo

# Check if the third-party repos exists. Otherwise, clone
$(UNITTEST_TOOLS_FULL_PATH):
	test -d $(UNITTEST_TOOLS_FULL_PATH) || { \
	    echo -e "$(PRINT_WARNING) Unittest Helper Tools are missing! Cloning to $(UNITTEST_TOOLS_FULL_PATH)...$(PRINT_RESET)"; \
	    git clone $(UNITTEST_TOOLS_REPO) $(UNITTEST_TOOLS_FULL_PATH) && \
		cd $(UNITTEST_TOOLS_FULL_PATH) && unzip -q Unity.zip && unzip -q splint-3.1.1.win32.zip; \
	}
