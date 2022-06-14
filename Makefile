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

.PHONY: sim

PROJECT_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SRC_DIR = src
TB_DIR = tb

INCLUDE = include/riscv_pkg.sv 
INCLUDE := $(addprefix $(PROJECT_ROOT), $(INCLUDE))

SRC = $(shell find $(SRC_DIR) -name '*.sv')
SRC := $(addprefix $(PROJECT_ROOT), $(SRC))

TOP_MODULE = rv32i
TEST_MODULES = pc regfile alu loadstore
TEST_STAGES = ifstage
TESTS = ${TEST_MODULES} ${TEST_STAGES}

VERILATOR_OPTS = -Wall --cc --trace -Wno-unused -Wno-pinmissing

all:
	verilator --lint-only -Wall ${INCLUDE} ${SRC} --top-module ${TOP_MODULE}

build:
	mkdir -p build

$(TESTS): build
	@echo "Simulating module $@"
	verilator ${VERILATOR_OPTS} --Mdir build/ --exe ${INCLUDE} ${SRC} ${PROJECT_ROOT}${TB_DIR}/tb_$@.cpp --top-module $@ --prefix V$@ -CFLAGS -I${PROJECT_ROOT}/tb/lib
	make -C build -f V$@.mk V$@

sim : $(TESTS)
	echo "$(TESTS)"

clean:
	rm -rf build
