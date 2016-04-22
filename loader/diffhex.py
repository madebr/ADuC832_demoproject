#!/usr/bin/env python3

import intelhex
import sys

def diff_dumps(ih1, ih2, tofile=None, name1="a", name2="b", n_context=3):
    """Diff 2 IntelHex objects and produce unified diff output for their
    hex dumps.
    @param ih1        first IntelHex object to compare
    @param ih2        second IntelHex object to compare
    @param tofile     file-like object to write output
    @param name1      name of the first hex file to show in the diff header
    @param name2      name of the first hex file to show in the diff header
    @param n_context  number of context lines in the unidiff output
    """
    from io import StringIO
    def prepare_lines(ih):
        sio = StringIO()
        ih.dump(sio)
        dump = sio.getvalue()
        lines = dump.splitlines()
        return lines
    a = prepare_lines(ih1)
    b = prepare_lines(ih2)
    import difflib
    result = list(difflib.unified_diff(a, b, fromfile=name1, tofile=name2, n=n_context, lineterm=''))
    if tofile is None:
        tofile = sys.stdout
    output = '\n'.join(result)+'\n'
    tofile.write(output)

ihx1 = intelhex.IntelHex(sys.argv[1])
ihx2 = intelhex.IntelHex(sys.argv[2])

ihx1.padding = b'\xff'
ihx2.padding = b'\xff'

diff_dumps(ihx1, ihx2, name1=sys.argv[1], name2=sys.argv[2])
