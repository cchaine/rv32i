addi a0, x0, 0x100  # addr
nop
lui  a1, 0x12345
addi a1, x0, 0x678  # data
nop
sb a1, 0(a0)        # store byte
nop
addi a0, a0, 4      # addr + 4
nop
sh a1, 0(a0)        # store half-word 
nop
addi a0, a0, 4      # addr + 4
nop
sw a1, 0(a0)        # store word
