#ifndef LIB_DPI_H
#define LIB_DPI_H

#include <riscv.h>

typedef void (*dpi_func0_t)(const svBitVecVal*, svBitVecVal*);

uint32_t get_reg(dpi_func0_t getreg, uint32_t address) { 
  uint32_t value;
  getreg(&address, &value);
  return value;
}

regs_t get_regs(dpi_func0_t getreg) {
  regs_t regs;
  for(uint32_t i = 0; i < 32; i++) {
    getreg((svBitVecVal*)&i, &regs.array[i]);
  } 
  return regs;
}

#endif
