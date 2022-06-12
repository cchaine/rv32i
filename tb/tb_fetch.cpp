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

#include "Vfetch.h"

typedef int (*action_t)(Vfetch *);

int wait(Vfetch * dut) {
  static int time = 0;
  if(dut->clk_i) {
    switch(time) {
      case 0:
      case 1:
        return 1;
    }
    time += 1;
  }
  return 0;
}

int tb_fetch(Vfetch * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0: 
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
        dut->pc_i = time;

        dut->eval();

        if(dut->instruction_o != time + 1) {
          pfail("Failed tb_fetch\n");
          pfail("  expected 0x%x, got 0x%x\n", time+1, dut->instruction_o);
          success = 0;
        }
        break;
      case 16:
        if(success) {
          psuccess("Success tb_fetch !\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;
}

vluint64_t sim_time = 0;

// List of tests to execute
#define num_actions 2
action_t actions[] = { wait, tb_fetch };

int main(int argc, char ** argv, char ** env) {
  Vfetch *dut = new Vfetch;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("fetch.vcd");

  action_t * current_action = actions;
  // We are done when there are no more actions and the clk is low 
  while(current_action < (actions + num_actions)) {
    // Clock
    dut->clk_i ^= 1;

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
