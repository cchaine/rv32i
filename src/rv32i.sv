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
  logic[31:0]  pc_pc;

  logic[31:0]  fetch_pc            =  '0;
  logic[31:0]  fetch_instruction;

  logic[31:0]  decode_instruction  =  '0;
  
  logic        regfile_write       =   0;
  logic[4:0]   regfile_waddr       =  '0;
  logic[31:0]  regfile_wdata       =  '0;
  logic[4:0]   regfile_raddra      =  '0;
  logic[4:0]   regfile_raddrb      =  '0;
  logic[31:0]  regfile_rdataa      =  '0;
  logic[31:0]  regfile_rdatab      =  '0;
  
  logic[2:0]   alu_op              =  '0;
  logic        alu_alt_op          =   0;
  logic[31:0]  alu_operand1        =  '0;
  logic[31:0]  alu_operand2        =  '0;
  logic[31:0]  alu_result;
  
  logic        loadstore_write     =   0;
  logic[31:0]  loadstore_addr      =  '0;
  logic[31:0]  loadstore_wdata     =  '0;
  logic[31:0]  loadstore_rdata;

  pc #(
    .START_ADDRESS ( 32'h0 )
  ) inst_pc (
    .clk_i  (  clk_i  ),
    .rst_i  (  rst_i  ),
    .pc_o   (  pc_pc  )
  );

  fetch #(
    .MEMORY_FILE ( "" )
  ) inst_fetch (
    .clk_i          (  clk_i                ),
    .pc_i           (  fetch_pc             ),
    .instruction_o  (  fetch_instruction_o  ),
  );

  decode inst_decode (
    .instruction_i  (  decode_instruction  ),
  );
  
  regfile inst_regfile (
    .clk_i     (  clk_i           ),
    .rst_i     (  rst_i           ),
    .write_i   (  regfile_write   ),
    .waddr_i   (  regfile_waddr   ),
    .wdata_i   (  regfile_wdata   ),
    .raddra_i  (  regfile_raddra  ),
    .rdataa_o  (  regfile_rdataa  ),
    .raddrb_i  (  regfile_raddrb  ),
    .rdatab_o  (  regfile_rdatab  )
  );

  alu inst_alu (
    .op_i        (  alu_op        ),
    .alt_op_i    (  alu_alt_op    ),
    .operand1_i  (  alu_operand1  ),
    .operand2_i  (  alu_operand2  ),
    .result_o    (  alu_result    )
  );

  loadstore inst_loadstore (
    .clk_i    (  clk_i              ),
    .write_i  (  loadstore_write    ),
    .addr_i   (  loadstore_addr_i   ),
    .wdata_i  (  loadstore_wdata_i  ),
    .rdata_o  (  loadstore_rdata_o  )
  );

endmodule // rv32i
