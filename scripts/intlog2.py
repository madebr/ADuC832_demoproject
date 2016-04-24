#!/usr/bin/env python3
#=============================================================================
#Copyright (C) 2015-2016, Anonymous Maarten
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

import sys

if __name__ == '__main__':
  import argparse
  parser = argparse.ArgumentParser(description='Create C Integer log2 include file. The definition can only return log2 values of values >= 1. It will return negative values if the input is out of range.')
  parser.add_argument(dest='nbbits', metavar='NBBITS', type=int, help='Number of bits.', default=32, nargs='?')
  parser.add_argument('-D', dest='define', metavar='DEFINENAME', type=str, help='Name of the define', required=False, default='INTLOG2')
  _gh=object()
  parser.add_argument('-g', dest='guard', type=str, required=False, nargs='?', default=_gh)
  args = parser.parse_args()

  if args.guard is _gh:
    args.guard = None
  elif args.guard is None:
    args.guard = '_INTLOG2_H'

  output =  sys.stdout

  if args.guard:
    output.write('#ifndef {}\n'.format(args.guard))
    output.write('#define {}\n\n'.format(args.guard))

  output.write('#undef __VALUE_TOO_LOW\n')
  output.write('#undef __VALUE_TOO_BIG\n')

  output.write('#define _{}_{}_(VAL) {}\n\n'
    .format(args.define, 1 << (args.nbbits+1), '-2'))

  for exp2 in range(args.nbbits, 0, -1):
    lowerbound = 1 << exp2
    output.write('#define _{}_{}_(VAL) \\\n'.format(args.define, lowerbound))
    output.write('    (((VAL) <= {}ULL) ? {} : _{}_{}_(VAL))'
      .format(lowerbound-1, exp2 - 1, args.define, lowerbound << 1))
    output.write('\n\n')

  output.write('#define {}(VAL) \\\n'.format(args.define))
  output.write('     (((VAL) < 1) ? {} : _{}_2_(VAL))'
    .format('-1', args.define))

  output.write('\n\n')

  if args.guard:
    output.write('#endif //{}'.format(args.guard))

