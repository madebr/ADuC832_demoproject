#!/usr/bin/env python3

import intelhex
import sys

ihx = intelhex.IntelHex(sys.argv[1])
ihx.padding = b'\ff'
ihx.dump()
