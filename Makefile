#           __        _
#  ________/ /  ___ _(_)__  ___
# / __/ __/ _ \/ _ `/ / _ \/ -_)
# \__/\__/_//_/\_,_/_/_//_/\__/
# 
# Copyright (C) Clément Chaine
# This file is part of rv32i <https://github.com/cchaine/rv32i>
# 
# rv32i is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# rv32i is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with rv32i.  If not, see <http://www.gnu.org/licenses/>.

.PHONY: unit

PROJECT_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

AS = riscv64-unknown-elf-as
OBJCOPY = riscv64-unknown-elf-objcopy

BUILD_DIR = build
SRC_DIR = src
TB_DIR = tests
MEM_DIR = mem

INCLUDE = $(SRC_DIR)/include/riscv_pkg.svh
INCLUDE := $(addprefix $(PROJECT_ROOT), $(INCLUDE))

SRC = $(shell find $(SRC_DIR) -name '*.sv')
SRC := $(addprefix $(PROJECT_ROOT), $(SRC))

TOP_MODULE = rv32i
MODULES = pc regfile alu loadstore

MEMORY_FILES_SRC = $(wildcard $(TB_DIR)/$(MEM_DIR)/*.S)
MEMORY_FILES = $(addprefix $(BUILD_DIR)/$(MEM_DIR)/, $(notdir $(MEMORY_FILES_SRC:.S=.hex)))

VERILATOR_OPTS = --cc --trace
VERILATOR_WARNINGS = -Wall -Wno-unused -Wno-pinmissing

all:
	verilator \
		--lint-only \
		${VERILATOR_WARNINGS} \
	 	${INCLUDE} ${SRC} \
		--top-module ${TOP_MODULE}

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/waves
	@mkdir -p $(BUILD_DIR)/mem

$(TOP_MODULE): $(BUILD_DIR) $(MEMORY_FILES)
	@echo "Testing $@..."
	@verilator \
		${VERILATOR_OPTS} ${VERILATOR_WARNINGS} \
		--Mdir $(BUILD_DIR)/ \
		--exe ${INCLUDE} ${SRC} ${PROJECT_ROOT}${TB_DIR}/tb_$@.cpp \
		--top-module $@ --prefix V$@ \
		-CFLAGS -I${PROJECT_ROOT}/tests/lib -CFLAGS -g
	@make -C $(BUILD_DIR) -f V$@.mk V$@ > /dev/null
	@cd $(BUILD_DIR)/ && ./V$@

$(MODULES): $(BUILD_DIR)
	@echo "Testing $@..."
	@verilator \
		${VERILATOR_OPTS} ${VERILATOR_WARNINGS} \
		--Mdir $(BUILD_DIR)/ \
		-exe ${INCLUDE} ${SRC_DIR}/$@.sv ${PROJECT_ROOT}${TB_DIR}/tb_$@.cpp \
		--top-module $@ --prefix V$@ \
		-CFLAGS -I${PROJECT_ROOT}/tests/lib -CFLAGS -g
	@make -C $(BUILD_DIR) -f V$@.mk V$@ > /dev/null
	@cd $(BUILD_DIR)/ && ./V$@

$(MEMORY_FILES): $(MEMORY_FILES_SRC)
	@$(AS) $(TB_DIR)/$(MEM_DIR)/$(notdir $(@:.hex=.S)) -o $(BUILD_DIR)/$(notdir $(@:.hex=.o))
	@$(OBJCOPY) -O binary -j .text $(BUILD_DIR)/$(notdir $(@:.hex=.o)) $(BUILD_DIR)/$(notdir $(@:.hex=.bin))
	@xxd -g 4 -e -c 4 $(BUILD_DIR)/$(notdir $(@:.hex=.bin)) | awk '{print $$2}' > $@

unit : $(MODULES) $(TOP_MODULE)

clean:
	rm -rf $(BUILD_DIR)
