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

module wbstage (
  input   logic          clk_i,
  input   logic          rst_i,
  input   riscv_pkg::instruction_t  instruction_i,
  input   logic[31:0]    data_i,
  output  logic          reg_write_o,
  output  logic[4:0]     reg_waddr_o,
  output  logic[31:0]    reg_wdata_o
);

  always_comb begin
    reg_write_o = 0;
    reg_waddr_o = '0;
    reg_wdata_o = '0;
  end

endmodule // wbstage
