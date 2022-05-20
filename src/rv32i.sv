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

module rv32i (
  input   logic         clk_i,
  input   logic         rst_ni,
);
  logic  [4:0]    regfile_waddr = '0;
  logic  [4:0]    regfile_raddr = '0;
  logic           regfile_write = 0;
  logic  [31:0]   regfile_wdata = '0;
  
  regfile inst_regfile (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .waddr_i  (regfile_waddr),
    .raddr_i  (regfile_raddr),
    .write    (regfile_write),
    .wdata_i  (regfile_wdata),
    .rdata_o  (regfile_rdata_o)
  );
endmodule // rv32i
