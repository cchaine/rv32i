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

module alu (
  input   logic[2:0]   op_i,
  // funct7 field used for SUB and SRA operations
  input   logic        alt_op,
  input   logic[31:0]  operand1,
  input   logic[31:0]  operand2,
  output  logic[31:0]  result
);
import riscv_pkg::*;

always_comb begin
  case (op_i)
    F3_ALU_ADD:   begin
      if (alt_op) // SUB
        result = operand1 - operand2;
      else        // ADD
        result = operand1 + operand2;
      end
    F3_ALU_SLL:   begin
      result = operand1 << operand2;
      end
    F3_ALU_SLT:   begin
      result = $signed(operand1) < $signed(operand2) ? 1 : 0;
      end
    F3_ALU_SLTU:  begin
      result = operand1 < operand2 ? 1 : 0;
      end
    F3_ALU_XOR:   begin
      result = operand1 ^ operand2;
      end
    F3_ALU_SR:    begin
      if (alt_op) // SRA
        result = operand1 >>> operand2;
      else        // SRL
        result = operand1 >> operand2;
      end
    F3_ALU_OR:    begin
      result = operand1 | operand2;
      end
    F3_ALU_AND:   begin
      result = operand1 & operand2;
      end
  endcase
end
endmodule // alu
