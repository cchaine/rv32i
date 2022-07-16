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

#include "Vregfile.h"

typedef int (*action_t)(Vregfile *);

int reset(Vregfile * dut) {
  static int time = 0;
  if(dut->clk_i) {
    switch(time) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
        dut->rst_i = 1;
        dut->waddr_i = 0;
        dut->raddra_i = 0;
        dut->raddrb_i = 0;
        dut->write_i = 0;
        dut->wdata_i = 0;
        break;
      case 5:
        dut->rst_i = 0;
        return 1;
    }

    time += 1;
  }
  return 0;
}

int tb_write_read(Vregfile * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        // Set first data to be written
        dut->waddr_i = 0x1;
        dut->wdata_i = 0xAAAAAAAA;
        dut->write_i = 1;
        break;
      case 1:
        // Set second data to be written
        dut->waddr_i = 0x2;
        dut->wdata_i = 0xBBBBBBBB;
        dut->write_i = 1;
        break;
      case 2:
        // Stop writing
        dut->write_i = 0;
        break;
      case 3:
        // Set read address
        dut->raddra_i = 0x1;
        dut->raddrb_i = 0x2;

        dut->eval();

        if(dut->rdataa_o != 0xAAAAAAAA) {
          pfail("Failed tb_write_read: written 0xAAAAAAAA, read %x @ 0x1\n", dut->rdataa_o);
          success = 0;
        }
        if(dut->rdatab_o != 0xBBBBBBBB) {
          pfail("Failed tb_write_read: written 0xAAAAAAAA, read %x @ 0x2\n", dut->rdataa_o);
          success = 0;
        }
        break;
      case 4:
        if(success) {
          psuccess("Success tb_write_read!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;
}

int tb_write_read_x0(Vregfile * dut) {
  static int time = 0;
  static int success = 1;
  if(dut->clk_i) {
    switch(time) {
      case 0:
        // Write to x0
        dut->waddr_i = 0;
        dut->wdata_i = 0xAAAAAAAA;
        dut->write_i = 1;
        break;
      case 1:
        // Stop writing
        dut->write_i = 0;
        break;
      case 2:
        // Read x0
        dut->raddra_i = 0;

        dut->eval();

        if(dut->rdataa_o != 0) {
          pfail("Failed tb_write_read_x0\n");
          success = 0;
        }
        break;
      case 3:
        if(success) {
          psuccess("Success tb_write_read_x0!\n");
        }
        return 1;
    }

    time += 1;
  }
  return 0;     
}

#define MAX_SIM_TIME 20
vluint64_t sim_time = 0;

// List of tests to execute
#define num_actions 3
action_t actions[] = {reset, tb_write_read, tb_write_read_x0};

int main(int argc, char ** argv, char ** env) {
  Vregfile *dut = new Vregfile;

  Verilated::traceEverOn(true);
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("regfile.vcd");

  action_t * current_action = actions;
  // We are done when there are no more actions and the clk is low 
  while(current_action < (actions + num_actions) || dut->clk_i == 1) {
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
  dut->final();
  delete dut;
  exit(EXIT_SUCCESS);
}
