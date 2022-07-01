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

module exstage (
  input   logic          clk_i,
  input   riscv_pkg::instruction_t  instruction_i,
  input   logic[31:0]    reg_rdataa_i,
  input   logic[31:0]    reg_rdatab_i,
  output  logic[31:0]    result_o,
  output  riscv_pkg::instruction_t  instruction_o
);
  logic[2:0]   alu_op;
  logic        alu_alt_op;
  logic[31:0]  operand1, operand2;
  logic[31:0]  result;

  alu inst_alu (
    .op_i        (  alu_op      ),
    .alt_op_i    (  alu_alt_op  ),
    .operand1_i  (  operand1    ),
    .operand2_i  (  operand2    ),
    .result_o    (  result      )
  );

  always_comb begin
    alu_op = instruction_i.f3;
    alu_alt_op = 0;
    operand1 = reg_rdataa_i;
    operand2 = instruction_i.is_imm ? instruction_i.imm : reg_rdatab_i;

    result_o = result;
  end

  always_ff @(posedge clk_i) begin
    instruction_o <= instruction_i;
  end
endmodule // exstage
