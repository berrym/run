#!/usr/bin/env python

import sys
import subprocess

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('usage: {0} command args'.format(sys.argv[0]))
    else:
        subprocess.Popen(sys.argv[1:])
