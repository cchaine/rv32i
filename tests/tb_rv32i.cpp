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

typedef int (*action_t)(Vrv32i*);
typedef struct {
  std::string name;
  action_t run;
  char skip = 0;
} test_t;

int tb_nop(Vrv32i * dut) {
  static int time = 0;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "../tests/mem/nop.mem");
        break;
      case 1:
        dut->rst_i = 0;
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        return 1;
    }

    time += 1;
  }
  return 0;
}

int tb_lui(Vrv32i * dut) {
  static int time = 0;
  static int success = 1;
  svScope scope;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        loadmem(dut, "../tests/mem/lui.mem");
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
int tb_beq(Vrv32i * dut) { return 1; }
int tb_bne(Vrv32i * dut) { return 1; }
int tb_blt(Vrv32i * dut) { return 1; }
int tb_bge(Vrv32i * dut) { return 1; }
int tb_bltu(Vrv32i * dut) { return 1; }
int tb_bgeu(Vrv32i * dut) { return 1; }
int tb_lb(Vrv32i * dut) { return 1; }
int tb_lh(Vrv32i * dut) { return 1; }
int tb_lw(Vrv32i * dut) { return 1; }
int tb_lbu(Vrv32i * dut) { return 1; }
int tb_lhu(Vrv32i * dut) { return 1; }
int tb_sb(Vrv32i * dut) { return 1; }
int tb_sh(Vrv32i * dut) { return 1; }
int tb_sw(Vrv32i * dut) { return 1; }
int tb_add(Vrv32i * dut) { return 1; }
int tb_sub(Vrv32i * dut) { return 1; }
int tb_sll(Vrv32i * dut) { return 1; }
int tb_slt(Vrv32i * dut) { return 1; }
int tb_sltu(Vrv32i * dut) { return 1; }
int tb_xor(Vrv32i * dut) { return 1; }
int tb_srl(Vrv32i * dut) { return 1; }
int tb_sra(Vrv32i * dut) { return 1; }
int tb_or(Vrv32i * dut) { return 1; }
int tb_and(Vrv32i * dut) { return 1; }
int tb_fence(Vrv32i * dut) { return 1; }
int tb_fencei(Vrv32i * dut) { return 1; }
int tb_ecall(Vrv32i * dut) { return 1; }
int tb_ebreak(Vrv32i * dut) { return 1; }
int tb_csrrw(Vrv32i * dut) { return 1; }
int tb_csrrs(Vrv32i * dut) { return 1; }
int tb_csrrc(Vrv32i * dut) { return 1; }
int tb_csrrwi(Vrv32i * dut) { return 1; }
int tb_csrrsi(Vrv32i * dut) { return 1; }
int tb_csrrci(Vrv32i * dut) { return 1; }

vluint64_t sim_time = 0;

// List of tests to execute
#define num_tests 39
test_t tests[] = {
  {"nop",      tb_nop},
  {"lui",      tb_lui},
  {"auipc",    tb_auipc,   SKIP},
  {"jal",      tb_jal,     SKIP},
  {"jalr",     tb_jalr,    SKIP},
  {"beq",      tb_beq,     SKIP},
  {"bne",      tb_bne,     SKIP},
  {"blt",      tb_blt,     SKIP},
  {"bge",      tb_bge,     SKIP},
  {"bltu",     tb_bltu,    SKIP},
  {"bgeu",     tb_bgeu,    SKIP},
  {"lb",       tb_lb,      SKIP},
  {"lh",       tb_lh,      SKIP},
  {"lw",       tb_lw,      SKIP},
  {"lbu",      tb_lbu,     SKIP},
  {"lhu",      tb_lhu,     SKIP},
  {"sb",       tb_sb,      SKIP},
  {"sh",       tb_sh,      SKIP},
  {"sw",       tb_sw,      SKIP},
  {"add",      tb_add,     SKIP},
  {"sub",      tb_sub,     SKIP},
  {"sll",      tb_sll,     SKIP},
  {"slt",      tb_slt,     SKIP},
  {"sltu",     tb_sltu,    SKIP},
  {"xor",      tb_xor,     SKIP},
  {"srl",      tb_srl,     SKIP},
  {"sra",      tb_sra,     SKIP},
  {"or",       tb_or,      SKIP},
  {"and",      tb_and,     SKIP},
  {"fence",    tb_fence,   SKIP},
  {"fence.i",  tb_fencei,  SKIP},
  {"ecall",    tb_ecall,   SKIP},
  {"ebreak",   tb_ebreak,  SKIP},
  {"csrrw",    tb_csrrw,   SKIP},
  {"csrrs",    tb_csrrs,   SKIP},
  {"csrrc",    tb_csrrc,   SKIP},
  {"csrrwi",   tb_csrrwi,  SKIP},
  {"csrrsi",   tb_csrrsi,  SKIP},
  {"csrrci",   tb_csrrci,  SKIP},
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
