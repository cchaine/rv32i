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

  ifstage inst_ifstage (
    .clk_i ( clk_i ),
    .rst_i ( rst_i ),
    .instruction_o ( instruction )
  );

  idstage inst_idstage (
    .clk_i ( clk_i ),
    .instruction_i ( instruction )
  );

  exstage inst_exstage (

  );

  wbstage inst_wbstage (
  
  );

endmodule // rv32i
