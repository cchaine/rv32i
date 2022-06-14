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

module idstage (
  input   logic        clk_i,
  input   logic[31:0]  instruction_i,
  output  logic[2:0]   alu_op_o,
  output  logic        alu_alt_op_o,
  output  logic[4:0]   rd_o,
  output  logic[31:0]  imm_o,
  output  logic        is_imm_o
);
  import riscv_pkg::*;

  logic[4:0]  opcode;
  logic[2:0]  f3;
  logic[4:0]  rd;
  logic[4:0]  rs1;
  logic[4:0]  rs2;

  always_ff @(posedge clk_i) begin
    opcode <= instruction_i[6:2];
    case(opcode)
      OP_LOAD : begin
        rd       <=  instruction_i[11:7];
        rs1      <=  instruction_i[19:15];
        imm_o    <=  { 20'b0, instruction_i[11:0] };
        is_imm_o <=  1;
      end
      default: begin
        alu_op_o <= '0;
        alu_alt_op_o <= 0;
        rd_o <= '0;
        imm_o <= '0;
        is_imm_o <= 0;
      end
    endcase 
  end
endmodule // idstage
