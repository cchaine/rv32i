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

#include "Vloadstore.h"

typedef int (*action_t)(Vloadstore *);

int wait(Vloadstore * dut) {
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

int tb_loadstore(Vloadstore * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0: // Write @ 0xA
        dut->write_i = 1;
        dut->addr_i = 0xA;
        dut->wdata_i = 0xCAFEBABE;

        break;
      case 1: // Read @ 0xA
        dut->write_i = 0;

        dut->eval();

        if(dut->rdata_o != 0xCAFEBABE) {
          pfail("Failed tb_loadstore\n");
          pfail("  expected 0xCAFEBABE, got 0x%x\n", dut->rdata_o);
          success = 0;
        }
      case 16:
        if(success) {
          psuccess("Success tb_loadstore !\n");
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
action_t actions[] = { wait, tb_loadstore };

int main(int argc, char ** argv, char ** env) {
  Vloadstore *dut = new Vloadstore;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("loadstore.vcd");

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
