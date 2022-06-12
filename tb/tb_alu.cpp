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

#include <term.h>

#include "Valu.h"

typedef int (*action_t)(Valu *);

#define  OP_ADD   0
#define  OP_SLL   1
#define  OP_SLT   2
#define  OP_SLTU  3
#define  OP_XOR   4
#define  OP_SR    5
#define  OP_OR    6
#define  OP_AND   7

int tb_add(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0: // Simple add test
      dut->op_i = OP_ADD;
      dut->alt_op_i = 0;
      dut->operand1_i = 10;
      dut->operand2_i = 5;

      dut->eval();

      if(dut->result_o != 0xf) {
        pfail("Failed tb_add\n");
        pfail("  0xa + 0x5 : expected 0xf, got 0x%x\n", dut->result_o);
        success = 0;
      }
      break;
    case 1: // Overflow test
      dut->op_i = OP_ADD;
      dut->alt_op_i = 0;
      dut->operand1_i = 0xffffffff;
      dut->operand2_i = 5;

      dut->eval();

      if(dut->result_o != 0x4) {
        pfail("Failed tb_add_overflow\n");
        pfail("  0xffffffff + 0x5 : expected 0x4, got 0x%x\n", dut->result_o);
        success = 0;
      }
      break;
    case 2:
      if(success) {
        psuccess("Success tb_add !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sub(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0: // Simple sub test
      dut->op_i = OP_ADD;
      dut->alt_op_i = 1;
      dut->operand1_i = 10;
      dut->operand2_i = 5;

      dut->eval();

      if(dut->result_o != 0x5) {
        pfail("Failed tb_sub\n");
        pfail("  0xa - 0x5 : expected 0x5, got 0x%x\n", dut->result_o);
        success = 0;
      }
      break;
    case 1: // Overflow test
      dut->operand1_i = 0;
      dut->operand2_i = 5;

      dut->eval();

      if(dut->result_o != 0xfffffffb) {
        pfail("Failed tb_sub\n");
        pfail("  0x0 - 0x5 : expected 0xfffffffb, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 2:
      if(success) {
        psuccess("Success tb_sub !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sll(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_SLL;
      dut->alt_op_i = 0;
      dut->operand1_i = 0x55555555;
      dut->operand2_i = 5;

      dut->eval();

      if(dut->result_o != 0xAAAAAAA0) {
        pfail("Failed tb_sll\n");
        pfail("  0x55555555 << 5 : expected 0xAAAAAAA0, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_sll !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_slt(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0: // Two positives, expect true
      dut->op_i = OP_SLT;
      dut->alt_op_i = 0;
      dut->operand1_i = 0x1;
      dut->operand2_i = 0x2;

      dut->eval();

      if(dut->result_o != 0x1) {
        pfail("Failed tb_slt\n");
        pfail("  0x1 < 0x2 : expected 1, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 1: // One positive, one negative
      dut->operand1_i = 0x1;
      dut->operand2_i = -0x1;

      dut->eval();

      if(dut->result_o != 0x0) {
        pfail("Failed tb_slt\n");
        pfail("  0x1 < -0x1 : expected 0, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 2: // Two negatives
      dut->operand1_i = -0x2;
      dut->operand2_i = -0x1;

      dut->eval();

      if(dut->result_o != 0x1) {
        pfail("Failed tb_slt\n");
        pfail("  -0x2 < -0x1 : expected 1, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 3: // Equals
      dut->operand1_i = -0x2;
      dut->operand2_i = -0x2;

      dut->eval();

      if(dut->result_o != 0x0) {
        pfail("Failed tb_slt\n");
        pfail("  -0x2 < -0x2 : expected 0, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 4:
      if(success) {
        psuccess("Success tb_slt !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sltu(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0: // Two positives, expect true
      dut->op_i = OP_SLTU;
      dut->alt_op_i = 0;
      dut->operand1_i = 0x1;
      dut->operand2_i = 0x2;

      dut->eval();

      if(dut->result_o != 0x1) {
        pfail("Failed tb_sltu\n");
        pfail("  0x1 < 0x2 : expected 1, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 1: // One positive, one negative
      dut->operand1_i = 0x1;
      dut->operand2_i = -0x1;

      dut->eval();

      if(dut->result_o != 0x1) {
        pfail("Failed tb_sltu\n");
        pfail("  0x1 < -0x1(0xffffffff) : expected 1, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 2: // Two negatives
      dut->operand1_i = -0x2; // 0xFFFFFFFE
      dut->operand2_i = -0x1; // 0xFFFFFFFF

      dut->eval();

      if(dut->result_o != 0x1) {
        pfail("Failed tb_sltu\n");
        pfail("  -0x2(0xFFFFFFFE) < -0x1(0xFFFFFFFF) : expected 1, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 3: // Equals
      dut->operand1_i = -0x2;
      dut->operand2_i = -0x2;

      dut->eval();

      if(dut->result_o != 0x0) {
        pfail("Failed tb_sltu\n");
        pfail("  0xFFFFFFFE < 0xFFFFFFFE : expected 0, got %d\n", dut->result_o);
        success = 0;
      }
      break;
    case 4:
      if(success) {
        psuccess("Success tb_sltu !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_xor(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_XOR;
      dut->alt_op_i = 0;
      dut->operand1_i = 0xdeadbeef;
      dut->operand2_i = 0xcafecafe;

      dut->eval();

      if(dut->result_o != 0x14537411) {
        pfail("Failed tb_xor\n");
        pfail("  0xdeadbeef ^ 0xcafecafe : expected 0x14537411, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_xor !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sra(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_SR;
      dut->alt_op_i = 1;
      dut->operand1_i = -0x8;
      dut->operand2_i = 0x2;

      dut->eval();

      if(dut->result_o != -0x2) {
        pfail("Failed tb_sra\n");
        pfail("  -0x8 >>> 0x2 : expected -0x2, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_sra !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_srl(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_SR;
      dut->alt_op_i = 0;
      dut->operand1_i = 0x8;
      dut->operand2_i = 0x2;

      dut->eval();

      if(dut->result_o != 0x2) {
        pfail("Failed tb_srl\n");
        pfail("  0x8 >> 0x2 : expected 0x2, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_srl !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_or(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_OR;
      dut->alt_op_i = 0;
      dut->operand1_i = 0xF0F0F0F0;
      dut->operand2_i = 0x0F0F0F0F;

      dut->eval();

      if(dut->result_o != 0xFFFFFFFF) {
        pfail("Failed tb_or\n");
        pfail("  0xF0F0F0F0 | 0x0F0F0F0F : expected 0xFFFFFFFF, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_or !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

int tb_and(Valu * dut) {
  static int time = 0;
  static int success = 1;
  switch(time) {
    case 0:
      dut->op_i = OP_AND;
      dut->alt_op_i = 0;
      dut->operand1_i = 0xFFFF0000;
      dut->operand2_i = 0x000F0000;

      dut->eval();

      if(dut->result_o != 0x000F0000) {
        pfail("Failed tb_and\n");
        pfail("  0xFFFF0000 & 0x000F0000 : expected 0x000F0000, got 0x%x\n", dut->result_o);
        success = 0;
      }
    case 1:
      if(success) {
        psuccess("Success tb_and !\n");
      }
      return 1;
  }

  time += 1;
  return 0;
}

vluint64_t sim_time = 0;

// List of tests to execute
#define num_actions 10
action_t actions[] = { tb_add, tb_sub, tb_sll, tb_slt, tb_sltu, tb_xor, tb_sra, tb_srl, tb_or, tb_and };

int main(int argc, char ** argv, char ** env) {
  Valu *dut = new Valu;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("alu.vcd");

  action_t * current_action = actions;
  // We are done when there are no more actions and the clk is low 
  while(current_action < (actions + num_actions)) {
    // Perform actions 
    int current_done = (*current_action)(dut);
    if(current_done) {
      current_action += 1;
    }

    dut->eval();
    m_trace->dump(sim_time);
    sim_time++;
  }

  m_trace->close();
  delete dut;
  exit(EXIT_SUCCESS);
}
