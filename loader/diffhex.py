#!/usr/bin/env python3
#=============================================================================
#Copyright (C) 2016, Anonymous Maarten
#
#This file is part of ADuC832 demoproject.
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#=============================================================================

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
