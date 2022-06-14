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
  input  logic        clk_i,
  input  logic[2:0]   alu_op_i,
  input  logic        alu_alt_op_i,
  input  logic[31:0]  operand1_i,
  input  logic[31:0]  operand2_i,
  input  logic[4:0]   rd_i
);
  logic[31:0] result;

  alu inst_alu (
    .op_i ( alu_op_i ),
    .alt_op_i ( alu_alt_op_i ),
    .operand1_i ( operand1_i ),
    .operand2_i ( operand2_i ),
    .result_o ( result )
  );

endmodule // exstage
