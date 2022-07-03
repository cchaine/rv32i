#!/usr/bin/python3
import sys

def main():
    fh_in = sys.stdin
    fh_out = sys.stdout

    while True:
        # incoming values have newline
        line = fh_in.readline()
        if not line:
            return 0

        # process the value
        line = "Filtered " + line

        # outgoing filtered values must have a newline
        fh_out.write("filtered: %s" % line)
        fh_out.flush()

if __name__ == '__main__':
	sys.exit(main())
