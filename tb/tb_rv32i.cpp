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

#include "Vrv32i.h"

typedef int (*action_t)(Vrv32i*);
typedef struct {
  std::string name;
  action_t run;
} test_t;

int tb_nop(Vrv32i * dut) {
  static int time = 0;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        dut->rst_i = 1;
        dut->loadmem("../tb/mem/tb_nop.mem");
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

vluint64_t sim_time = 0;

// List of tests to execute
#define num_tests 1
test_t tests[] = {
  {"nop",    tb_nop}
};

int main(int argc, char ** argv, char ** env) {
  Vrv32i *dut = new Vrv32i;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);

  // Set scope for DPI interactions
  const svScope scope = svGetScopeFromName("TOP.rv32i.inst_ifstage.inst_imem");
  assert(scope);
  svSetScope(scope);

  test_t * current_test = tests;
  std::string trace_name;
  // We are done when there are no more actions
  while(current_test < (tests + num_tests)) {
    // Open the VCD trace if not already open
    if(!m_trace->isOpen()) {
      trace_name = "rv32i_" + current_test->name + ".vcd";
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

  dut->final();
  delete dut;
  exit(EXIT_SUCCESS);
}
