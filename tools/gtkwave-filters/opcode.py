#!/usr/bin/python3
import sys

opcodes = {
    0b01101 : "LUI", 
    0b00101 : "AUIPC", 
    0b11011 : "JAL", 
    0b11001 : "JALR", 
    0b11000 : "BRANCH", 
    0b00000 : "LOAD", 
    0b01000 : "STORE", 
    0b00100 : "ALU_IMM", 
    0b01100 : "ALU", 
    0b00011 : "MISC_MEM", 
    0b11100 : "SYSTEM" 
}

def main():
    fh_in = sys.stdin
    fh_out = sys.stdout

    while True:
        # incoming values have newline
        line = fh_in.readline()
        if not line:
            return 0

        if "x" in line:
            fh_out.write(l)
            fh_out.flush()
            continue

        # process the value
        opcode = int(line, 16)

        # output
        if opcode in opcodes:
            output = opcodes[opcode]
        else:
            output = "unknown"

        # outgoing filtered values must have a newline
        fh_out.write("%s\n" % output)
        fh_out.flush()

if __name__ == '__main__':
	sys.exit(main())
