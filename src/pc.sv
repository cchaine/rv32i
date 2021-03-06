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

module pc #(
 logic[31:0] RESET_ADDR = 32'b0
)(
  input   logic        clk_i,
  input   logic        rst_i,
  output  logic[31:0]  pc_o
);
  logic[31:0] pc_d, pc_q;

  always_ff @(posedge clk_i) begin
    if(rst_i) begin
      pc_d <= RESET_ADDR;
      pc_q <= RESET_ADDR;
    end else begin
      pc_d <= pc_d + 32'h4;
      pc_q <= pc_d;
    end
  end

  always_comb begin
    pc_o = pc_q;
  end
endmodule // pc
