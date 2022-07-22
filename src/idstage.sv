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
  input   logic          clk_i,
  input   logic          rst_i,
  input   logic[31:0]    instruction_i,
  output  riscv_pkg::instruction_t  instruction_o
);
  import riscv_pkg::*;

  riscv_pkg::instruction_t decoded_instr; 

  always_comb begin
    // Reset decoded instruction
    decoded_instr = '0;

    decoded_instr.opcode = instruction_i[6:2];
    case(instruction_i[6:2])
      // R-type instruction
      OP_ALU: begin
        decoded_instr.rd   =  instruction_i[11:7];
        decoded_instr.f3   =  instruction_i[14:12];
        decoded_instr.rs1  =  instruction_i[19:15];
        decoded_instr.rs2  =  instruction_i[24:20];
        decoded_instr.f7   =  instruction_i[31:25];
      end
      // I-type instruction
      OP_ALU_IMM,
      OP_LOAD,
      OP_JALR: begin
        decoded_instr.rd         =  instruction_i[11:7];
        decoded_instr.f3         =  instruction_i[14:12];
        decoded_instr.rs1        =  instruction_i[19:15];
        decoded_instr.imm[11:0]  =  instruction_i[31:20];
        // sign-extend
        decoded_instr.imm[31:12] = 
          instruction_i[31] == 1 ? 20'hFFFFF : '0;
        decoded_instr.is_imm     =  1;
      end
      // S-type instruction
      OP_STORE: begin
        decoded_instr.imm[4:0]   =  instruction_i[11:7];
        decoded_instr.f3         =  instruction_i[14:12];
        decoded_instr.rs1        =  instruction_i[19:15];
        decoded_instr.rs2        =  instruction_i[24:20];
        decoded_instr.imm[11:5]  =  instruction_i[31:25];
        decoded_instr.is_imm     =  1;
      end
      // B-type instruction
      OP_BRANCH: begin
        decoded_instr.imm[11]    =  instruction_i[7];
        decoded_instr.imm[4:1]   =  instruction_i[11:8];
        decoded_instr.f3         =  instruction_i[14:12];
        decoded_instr.rs1        =  instruction_i[19:15];
        decoded_instr.rs2        =  instruction_i[24:20];
        decoded_instr.imm[10:5]  =  instruction_i[30:25];
        decoded_instr.imm[12]    =  instruction_i[31];
        decoded_instr.is_imm     =  1;
      end
      // U-type instruction
      OP_LUI,
      OP_AUIPC: begin
        decoded_instr.rd         =  instruction_i[11:7];
        decoded_instr.imm[31:12] = instruction_i[31:12];
        decoded_instr.is_imm     =  1;
      end
      // J-type instruction
      OP_JAL: begin
        decoded_instr.rd          =  instruction_i[11:7];
        decoded_instr.imm[19:12]  =  instruction_i[19:12];
        decoded_instr.imm[11]     =  instruction_i[20];
        decoded_instr.imm[10:1]   =  instruction_i[30:21];
        decoded_instr.imm[20]     =  instruction_i[31];
        decoded_instr.is_imm      =  1;
      end
      // Fence instructions
      OP_MISC_MEM: begin end
      // CSR instructions
      OP_SYSTEM: begin end
      default: begin end
    endcase 
  end

  always_comb begin
    instruction_o = rst_i ? '0 : decoded_instr;
  end
endmodule // idstage
