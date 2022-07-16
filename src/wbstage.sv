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

module wbstage import riscv_pkg::*; (
  input   logic          clk_i,
  input   logic          rst_i,
  input   riscv_pkg::instruction_t  instruction_i,
  input   logic[31:0]    data_i,
  output  logic          reg_write_o,
  output  logic[4:0]     reg_waddr_o,
  output  logic[31:0]    reg_wdata_o
);
  function logic should_write(input logic[4:0] opcode);
    unique case(opcode)
      OP_LUI,
      OP_AUIPC,
      OP_LOAD,
      OP_ALU_IMM,
      OP_ALU:
        return 1;
      default:
        return 0;
    endcase
  endfunction

  always_ff @(posedge clk_i) begin
    if(rst_i) begin
      reg_write_o <= 0;
    end else begin
      if(should_write(instruction_i.opcode)) begin
        reg_write_o <= 1;
      end else begin
        reg_write_o <= 0;
      end
    end
  end

  always_comb begin
    reg_waddr_o = instruction_i.rd;
    reg_wdata_o = data_i;
  end

endmodule // wbstage
