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

module loadstore (
  input   logic        clk_i,
  input   logic        write_i,
  input   logic[31:0]  addr_i,
  input   logic[31:0]  wdata_i,
  output  logic[31:0]  rdata_o
);

logic [31:0] dmem [4096];

always_ff @(posedge clk_i) begin
  if(write_i) begin
    dmem[addr_i[11:0]] <= wdata_i;
  end else begin
    rdata_o <= dmem[addr_i[11:0]];
  end
end

endmodule // fetch
