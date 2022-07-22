/*
 *           __        _
 *  ________/ /  ___ _(_)__  ___
 * / __/ __/ _ \/ _ `/ / _ \/ -_)
 * \__/\__/_//_/\_,_/_/_//_/\__/
 * 
 * Copyright (C) Clément Chaine
 * This file is part of rv32i <https://github.com/cchaine/rv32i>
 *
 * rv32i is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rv32i is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rv32i.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <svdpi.h>

#include <term.h>
#include <riscv.h>

#include "Vrv32i.h"

#define SKIP 1

void loadmem(Vrv32i * dut, const char * path) {
  svScope scope = svGetScopeFromName("TOP.rv32i.inst_ifstage.inst_imem");
  assert(scope);
  svSetScope(scope);
  dut->loadmem(path);
}

regs_t get_regs(Vrv32i * dut) {
  svScope scope = svGetScopeFromName("TOP.rv32i.inst_regfile");
  assert(scope);
  svSetScope(scope);
  regs_t regs;
  for(uint32_t i = 0; i < 32; i++) {
    dut->getreg((svBitVecVal*)&i, &regs.array[i]);
  } 
  return regs;
}

uint32_t get_mem(Vrv32i * dut, uint32_t addr) {
  svScope scope = svGetScopeFromName("TOP.rv32i.inst_ifstage.inst_imem");
  assert(scope);
  svSetScope(scope);
  uint32_t value;
  dut->getmem((svBitVecVal*)&addr, &value);
  return value;
}

typedef int (*action_t)(Vrv32i*);
typedef struct {
  std::string name;
  action_t run;
  char skip = 0;
} test_t;

/**
 * Tests:
 *  loading the upper 20bits of a register
 * Expects:
 *  a0 = 0x12345000
 */
int tb_lui(Vrv32i * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "./mem/lui.hex");
        break;
      case 1:
        dut->rst_i = 0;
        break;
      case 2:
      case 3:
      case 4:
        break;
      case 5: {
        regs_t regs = get_regs(dut); 
        if(regs.a0 != 0x12345000) {
          pfail("Failed tb_lui: expected 0x12345000, read 0x%08x\n", regs.a0);
          success = 0;
        }
        break;
      }
      case 6:
        if(success) {
          psuccess("Success tb_lui!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;
}

int tb_auipc(Vrv32i * dut) { return 1; }
int tb_jal(Vrv32i * dut) { return 1; }
int tb_jalr(Vrv32i * dut) { return 1; }
int tb_branch(Vrv32i * dut) { return 1; }
int tb_load(Vrv32i * dut) { return 1; }

/**
 * Tests:
 *  storing a byte in memory without offset
 *  storing a half-word in memory without offset
 *  storing a word in memory without offset
 *  storing in memory with positive offset
 *  storing in memory with negative offset
 *  storing in memory with missaligned address 
 * Expects:
 *  a0 = 1
 *  a1 = 9
 *  a2 = 0xFFFFFF00
 */
int tb_store(Vrv32i * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "./mem/store.hex");
        break;
      case 1:
        dut->rst_i = 0;
        break;
      case 2:
      case 3:
      case 4:
        break;
      case 5: {
        regs_t regs = get_regs(dut); 
        if(regs.a0 != 0x1) {
          pfail("Failed tb_alu_imm: expected a0 = 0x1, read 0x%08x\n", regs.a0);
          success = 0;
        }
        break;
      }
      case 6: break; // Bubble
      case 7: {
        regs_t regs = get_regs(dut); 
        if(regs.a1 != 0xa) {
          pfail("Failed tb_alu_imm: expected a1 = 0xa, read 0x%08x\n", regs.a1);
          success = 0;
        }
        break;
      }
      case 8: break; // Bubble
      case 9: {
        regs_t regs = get_regs(dut); 
        if(regs.a2 != 0xFFFFFF00) {
          pfail("Failed tb_alu_imm: expected a2 = 0xFFFFFF00, read 0x%08x\n", regs.a2);
          success = 0;
        }
        break;
      }
      case 10:
        if(success) {
          psuccess("Success tb_alu_imm!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;

}

/**
 * Tests:
 *  the alu with immediate values
 *  immediate values sign-extend
 * Expects:
 *  a0 = 1
 *  a1 = 9
 *  a2 = 0xFFFFFF00
 */
int tb_alu_imm(Vrv32i * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "./mem/alu_imm.hex");
        break;
      case 1:
        dut->rst_i = 0;
        break;
      case 2:
      case 3:
      case 4:
        break;
      case 5: {
        regs_t regs = get_regs(dut); 
        if(regs.a0 != 0x1) {
          pfail("Failed tb_alu_imm: expected a0 = 0x1, read 0x%08x\n", regs.a0);
          success = 0;
        }
        break;
      }
      case 6: break; // Bubble
      case 7: {
        regs_t regs = get_regs(dut); 
        if(regs.a1 != 0xa) {
          pfail("Failed tb_alu_imm: expected a1 = 0xa, read 0x%08x\n", regs.a1);
          success = 0;
        }
        break;
      }
      case 8: break; // Bubble
      case 9: {
        regs_t regs = get_regs(dut); 
        if(regs.a2 != 0xFFFFFF00) {
          pfail("Failed tb_alu_imm: expected a2 = 0xFFFFFF00, read 0x%08x\n", regs.a2);
          success = 0;
        }
        break;
      }
      case 10:
        if(success) {
          psuccess("Success tb_alu_imm!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;
}

/**
 * Tests:
 *  the alu with register operands
 *  the sub instruction decoding (f7 field)
 * Expects:
 *  a0 = 0x1
 *  a1 = 0x9
 *  a2 = 0xa
 *  a3 = 0xFFFFFFF8
 */
int tb_alu(Vrv32i * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "./mem/alu.hex");
        break;
      case 1:
        dut->rst_i = 0;
        break;
      case 2:
      case 3:
      case 4:
        break;
      case 5: {
        regs_t regs = get_regs(dut); 
        if(regs.a0 != 0x1) {
          pfail("Failed tb_alu: expected a0 = 0x1, read 0x%08x\n", regs.a0);
          success = 0;
        }
        break;
      }
      case 6: break; // Bubble
      case 7: {
        regs_t regs = get_regs(dut); 
        if(regs.a1 != 0x9) {
          pfail("Failed tb_alu: expected a1 = 0x9, read 0x%08x\n", regs.a1);
          success = 0;
        }
        break;
      }
      case 8: break; // Bubble
      case 9: {
        regs_t regs = get_regs(dut); 
        if(regs.a2 != 0xa) {
          pfail("Failed tb_alu: expected a2 = 0xa, read 0x%08x\n", regs.a2);
          success = 0;
        }
        break;
      }
      case 10: break; // Bubble
      case 11: {
        regs_t regs = get_regs(dut); 
        if(regs.a3 != 0xFFFFFFF8) {
          pfail("Failed tb_alu: expected a3 = 0xFFFFFFF8, read 0x%08x\n", regs.a3);
          success = 0;
        }
        break;
      }
      case 12:
        if(success) {
          psuccess("Success tb_alu!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;
}

int tb_misc_mem(Vrv32i * dut) { return 1; }
int tb_system(Vrv32i * dut) { return 1; }

vluint64_t sim_time = 0;

// List of tests to execute
#define num_tests 11
test_t tests[] = {
  {"lui",      tb_lui},
  {"auipc",    tb_auipc,    SKIP},
  {"jal",      tb_jal,      SKIP},
  {"jalr",     tb_jalr,     SKIP},
  {"branch",   tb_branch,   SKIP},
  {"load",     tb_load,     SKIP},
  {"store",    tb_store,    SKIP},
  {"alu_imm",  tb_alu_imm},
  {"alu",      tb_alu},
  {"misc_mem", tb_misc_mem, SKIP},
  {"system",   tb_system,   SKIP}
};

int main(int argc, char ** argv, char ** env) {
  Vrv32i *dut = new Vrv32i;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);

  test_t * current_test = tests;
  std::string trace_name;
  // We are done when there are no more actions
  while(current_test < (tests + num_tests)) {
    // Skip the test if it should be skipped
    if(current_test->skip) {
      pwarn("Skipping %s\n", current_test->name.c_str());
      current_test += 1;
    } else {
      // Open the VCD trace if not already open
      if(!m_trace->isOpen()) {
        trace_name = "waves/rv32i_" + current_test->name + ".vcd";
        m_trace->open(trace_name.c_str());
      }

      // Update Clock
      dut->clk_i ^= 1;

      // Perform actions 
      int current_done = current_test->run(dut);

      // Update the trace
      dut->eval();
      m_trace->dump(sim_time);
      sim_time++;

      // Close the trace if test is over
      if(current_done) {
        m_trace->close();
        current_test += 1;
      }
    }
  }

  dut->final();
  delete dut;
  exit(EXIT_SUCCESS);
}
