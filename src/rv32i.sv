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

module rv32i import riscv_pkg::*; (
  input   logic         clk_i,
  input   logic         rst_i
);
  logic[31:0]    if_id_instruction;
  riscv_pkg::instruction_t  id_ex_instruction;
  logic[31:0]    reg_rdataa, reg_rdatab;
  riscv_pkg::instruction_t  ex_wb_instruction;
  logic[31:0]    ex_result;
  logic          reg_write;
  logic[4:0]     reg_waddr;
  logic[31:0]    reg_wdata;

  ifstage inst_ifstage (
    .clk_i          (  clk_i              ),
    .rst_i          (  rst_i              ),
    .instruction_o  (  if_id_instruction  )
  );

  idstage inst_idstage (
    .clk_i          (  clk_i              ),
    .rst_i          (  rst_i              ),
    .instruction_i  (  if_id_instruction  ),
    .instruction_o  (  id_ex_instruction  )
  );

  regfile inst_regfile (
    .clk_i     (  clk_i                  ),
    .rst_i     (  rst_i                  ),
    .write_i   (  reg_write ),
    .waddr_i   (  reg_waddr ),
    .wdata_i   (  reg_wdata ),
    .raddra_i  (  id_ex_instruction.rs1  ),
    .raddrb_i  (  id_ex_instruction.rs2  ),
    .rdataa_o  (  reg_rdataa             ),
    .rdatab_o  (  reg_rdatab             )
  );

  exstage inst_exstage (
    .clk_i          (  clk_i              ),
    .rst_i          (  rst_i              ),
    .instruction_i  (  id_ex_instruction  ),
    .reg_rdataa_i   (  reg_rdataa         ),
    .reg_rdatab_i   (  reg_rdatab         ),
    .result_o       (  ex_result          ),
    .instruction_o  (  ex_wb_instruction  )
  );

  wbstage inst_wbstage (
    .clk_i          (  clk_i              ),
    .rst_i          (  rst_i              ),
    .instruction_i  (  ex_wb_instruction  ),
    .data_i         (  ex_result          ),
    .reg_write_o    (  reg_write          ),
    .reg_waddr_o    (  reg_waddr          ),
    .reg_wdata_o    (  reg_wdata          )
  );
endmodule // rv32i
