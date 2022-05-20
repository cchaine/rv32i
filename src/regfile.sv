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

module regfile (
  input   logic           clk_i,
  input   logic           rst_ni,
  input   logic           write_i,
  input   logic  [4:0]    waddr_i,
  input   logic  [31:0]   wdata_i,
  input   logic  [4:0]    raddra_i,
  input   logic  [4:0]    raddrb_i,
  output  logic  [31:0]   rdataa_o,
  output  logic  [31:0]   rdatab_o
);
  logic [31:0] regs [31:0];

  always_comb begin
    rdataa_o = regs[raddra_i];
    rdatab_o = regs[raddrb_i];
  end

  always_ff @(posedge clk_i, negedge rst_ni) begin
    if (rst_ni == 0) begin
      foreach (regs[i]) begin
        regs[i] <= '0;
      end
    end else if(write_i == 1) begin
      if(waddr_i != 0) begin
        regs[waddr_i] <= wdata_i;
      end
    end
  end

endmodule //regfile
