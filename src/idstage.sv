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
  output  logic[4:0]   reg_raddra_o,
  output  logic[4:0]   reg_raddrb_o
  output  logic[31:0]  imm_o,
  output  logic        is_imm_o,
);
  import riscv_pkg::*;

  logic[4:0]   opcode;
  logic[2:0]   f3;
  logic[4:0]   rd;
  logic[4:0]   rs1;
  logic[4:0]   rs2;
  logic[6:0]   f7;
  logic[31:0]  imm;
  logic        is_imm;

  always_ff @(posedge clk_i) begin
    opcode <= instruction_i[6:2];
    f3   <=  '0;
    rd   <=  '0;
    rs1  <=  '0;
    rs2  <=  '0;
    f7   <=  '0;
    imm  <=  '0;

    unique case(opcode)
      // R-type instruction
      OP_ALU: begin
        rd   <=  instruction_i[11:7];
        f3   <=  instruction_i[14:12];
        rs1  <=  instruction_i[19:15];
        rs2  <=  instruction_i[24:20];
        f7   <=  instruction_i[31:25];
      end
      // I-type instruction
      OP_ALU_IMM,
      OP_LOAD,
      OP_JALR: begin
        rd         <=  instruction_i[11:7];
        f3         <=  instruction_i[14:12];
        rs1        <=  instruction_i[19:15];
        imm[11:0]  <=  instruction_i[31:20];
      end
      // S-type instruction
      OP_STORE: begin
        imm[4:0]   <=  instruction_i[11:7];
        f3         <=  instruction_i[14:12];
        rs1        <=  instruction_i[19:15];
        rs2        <=  instruction_i[24:20];
        imm[11:5]  <=  instruction_i[31:20];
      end
      // B-type instruction
      OP_BRANCH:
        imm[11]    <=  instruction_i[7];
        imm[4:1]   <=  instruction_i[11:8];
        f3         <=  instruction_i[14:12];
        rs1        <=  instruction_i[19:15];
        rs2        <=  instruction_i[24:20];
        imm[10:5]  <=  instruction_i[30:25];
        imm[12]    <=  instruction_i[31];
      // U-type instruction
      OP_LUI,
      OP_AUIPC: begin
        rd         <=  instruction_i[11:7];
        imm[31:12] <= instruction_i[31:12];
      end
      // J-type instruction
      OP_JAL: begin
        rd          <=  instruction_i[11:7];
        imm[19:12]  <=  instruction_i[19:12];
        imm[11]     <=  instruction_i[20];
        imm[10:1]   <=  instruction_i[30:21];
        imm[20]     <=  instruction_i[31];
      // Fence instructions
      OP_MISC_MEM:
      // CSR instructions
      OP_SYSTEM:
    endcase 
  end

  always_comb begin
    alu_op_o      =  f3;
    reg_raddra_o  =  rs1;
    reg_raddrb_o  =  rs2;
    imm_o         =  imm;
    is_imm_o      =  is_imm;
  end
endmodule // idstage
