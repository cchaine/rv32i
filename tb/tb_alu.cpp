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

#include "Valu.h"

typedef int (*action_t)(Valu *);

int reset(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting reset\n");
    case 1:
    case 2:
    case 3:
    case 4:
      break;
    case 5:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_add(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_add\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sub(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_sub\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sll(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_sll\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_slt(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_slt\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sltu(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_sltu\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_xor(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_xor\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_sra(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_sra\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_srl(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_srl\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_or(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_or\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

int tb_and(Valu * dut) {
  static int time = 0;
  switch(time) {
    case 0:
      printf("Starting tb_and\n");
    case 1:
      return 1;
  }

  time += 1;
  return 0;
}

vluint64_t sim_time = 0;

// List of tests to execute
#define num_actions 11
action_t actions[] = { reset, tb_add, tb_sub, tb_sll, tb_slt, tb_sltu, tb_xor, tb_sra, tb_srl, tb_or, tb_and };

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
