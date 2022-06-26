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

module rv32i (
  input   logic         clk_i,
  input   logic         rst_i
);
  logic[31:0] instruction;

  logic[2:0] id_alu_op;
  logic id_alu_alt_op;
  logic[4:0] id_reg_raddra, id_reg_raddrb;
  logic[31:0] id_imm;
  logic id_is_imm;

  logic[4:0] reg_raddra, reg_raddrb;
  logic[31:0] reg_rdataa, reg_rdatab;

  logic[2:0] ex_alu_op;
  logic ex_alu_alt_op;
  logic[31:0] ex_operand1, ex_operand2;

  ifstage inst_ifstage (
    .clk_i ( clk_i ),
    .rst_i ( rst_i ),
    .instruction_o ( instruction )
  );

  idstage inst_idstage (
    .clk_i          (  clk_i          ),
    .instruction_i  (  instruction    )
    .alu_op_o       (  id_alu_op      ),
    .alu_alt_op_o   (  id_alu_alt_op  ),
    .reg_raddra_o   (  id_reg_addra   ),
    .reg_raddrb_o   (  id_reg_addrb   ),
    .imm_o          (  id_imm         ),
    .is_imm_o       (  id_is_imm      )
  );

  regfile inst_regfile (
    .clk_i ( clk_i ),
    .rst_i ( rst_i ),
    .write_i ( ),
    .waddr_i ( ),
    .wdata_i ( ),
    .raddra_i ( reg_raddra ),
    .raddrb_i ( reg_raddrb ),
    .rdataa_o ( reg_rdataa ),
    .rdatab_o ( reg_rdatab )
  );
  always_comb begin
    reg_raddra = id_reg_raddra;
    reg_raddrb = id_reg_raddrb;
  end

  exstage inst_exstage (
    .alu_op_i      (  ex_alu_op      ),
    .alu_alt_op_i  (  ex_alu_alt_op  ),
    .operand1_i    (  ex_operand1    ),
    .operand2_i    (  ex_operand2    ),
  );
  always_comb begin
    ex_alu_op      =  id_alu_op,
    ex_alu_alt_op  =  id_alu_alt_op,
    ex_operand1    =  reg_rdataa;
    ex_operand2    =  id_is_imm ? id_imm : reg_datab;
  end

  wbstage inst_wbstage (
  
  );

endmodule // rv32i
