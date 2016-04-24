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

import collections
import re
import os
import sys
import shutil

eolname_eol = {'UNIX': b'\n', 'APPLE': b'\r', 'MICROSOFT': b'\r\n'}
eol_eolname = dict((value, key) for key, value in eolname_eol.items())

EOLregex = re.compile(b'(\r\n|\n|\r)')

def detectEOL(src):
  coll = collections.Counter()
  for match in EOLregex.findall(src):
    coll[match] += 1
  occurences = coll.most_common()
  if len(occurences) > 1:
    print('WARNING: file has EOL styles mixed', file=sys.stderr)
  mostcommon = occurences[0][0]
  return eol_eolname[mostcommon]

def toUNIX(src):
  return src.replace(b'\r\n', b'\n').replace(b'\r', b'\n')

def convertToEOL(src, style):
  if style == 'UNIX':
    return toUNIX(src)
  elif style == 'MICROSOFT':
    return toUNIX(src).replace(b'\n', b'\r\n')
  elif style == 'APPLE':
    return toUNIX(src).replace(b'\n', b'\r')

class Commenter(object):
  def __init__(self, extensions, extensionIgnoreCase = True):
    self._extensionIgnoreCase = extensionIgnoreCase
    self._extensions = extensions

  def commentarize(self, commentUnix, outputEOLname):
    eol = eolname_eol[outputEOLname]
    result = []
    blockPrefix = self.blockPrefix()
    linePrefix = self.linePrefix()
    blockSuffix = self.blockSuffix()
    if blockPrefix is not None:
      result += [blockPrefix, eol]
    for line in commentUnix.split(b'\n'):
      if linePrefix is not None:
        result += [linePrefix]
      result += [line, eol]
    if blockSuffix is not None:
      result += [blockSuffix, eol]
    return b''.join(result)

  def blockPrefix(self):
    return None

  def blockSuffix(self):
    return None

  def linePrefix(self):
    return None

  def parses(self, filename):
    _, extension = os.path.splitext(sourcefn)
    extension = extension[1:]
    if self._extensionIgnoreCase:
      extensions = [ext.lower() for ext in self._extensions]
      extension = extension.lower()
    extensions = self._extensions
    return extension in extensions

class CommenterC(Commenter):
  def __init__(self):
    super(CommenterC, self).__init__(['c', 'cpp', 'cxx', 'h', 'hpp', 'hxx'])

  def blockPrefix(self):
    return b'/*'

  def blockSuffix(self):
    return b'*/'

class CommenterPy(Commenter):
  def __init__(self):
    super(CommenterPy, self).__init__(['py'], False)

  def linePrefix(self):
    return b'#'

class CommenterCMake(Commenter):
  def __init__(self):
    super(CommenterCMake, self).__init__(['cmake'], False)

  def linePrefix(self):
    return b'#'

  def parses(self, filename):
    if os.path.basename(filename) == 'CMakeLists.txt':
      return self
    return super(CommenterCMake, self).parses(filename)

class CommenterSh(Commenter):
  def __init__(self):
    super(CommenterSh, self).__init__(['sh'], False)

  def linePrefix(self):
    return b'#'

class MasterCommenter(object):
  def __init__(self):
    self._commenters = []

  def register(self, commenter):
    self._commenters.append(commenter)

  def parses(self, filename):
    for commenter in self._commenters:
      if commenter.parses(filename):
        return commenter

    return None

def alreadyIncluded(haystack, needleUNIX):
  occurences = 0
  nblines = 0
  lines = needleUNIX.split(b'\n')
  for line in lines:
    line = line.strip()
    if line:
      nblines += 1
      if line in haystack:
        occurences += 1
  return occurences / nblines

def extractSheBang(src):
  sheBang = None
  rest = src
  for newline in EOLregex.finditer(src):
    firstline = src[0:newline.start()]
    if firstline.startswith(b'#!'):
      sheBang = firstline
      rest = src[newline.end():]
    break
  return sheBang, rest

if __name__ == '__main__':
  import argparse
  parser = argparse.ArgumentParser(description='Prepend files with license')
  parser.add_argument('-l', dest='header', type=argparse.FileType('rb'),
    help='The header file to prepend to each SOURCE.'
      'Read from stdin if not supplied.', required=True)
  parser.add_argument('sources', metavar='SOURCE', nargs='+',
    help='Sources to which the header has to be prepended.')
  parser.add_argument('-N', dest='backup', action='store_false',
    help='Do not create backup files.')
  args = parser.parse_args()

  headerUnix = toUNIX(args.header.read()).strip()

  doubleHeaderIncludeGuardProbability = 0.8

  masterCommenter = MasterCommenter()
  masterCommenter.register(CommenterC())
  masterCommenter.register(CommenterPy())
  masterCommenter.register(CommenterCMake())
  masterCommenter.register(CommenterSh())

  for sourcefn in args.sources:
    commenter = masterCommenter.parses(sourcefn)
    if not commenter:
      print('WARNING ({}): cannot parse file, no parsers available.'.format(sourcefn), file=sys.stderr)
      continue
    src = open(sourcefn, 'rb').read()
    if alreadyIncluded(src, headerUnix) > doubleHeaderIncludeGuardProbability:
      print('WARNING ({}): header already included.'.format(sourcefn), file=sys.stderr)
      continue
    if args.backup:
      backupsourcefn = sourcefn + '.bak'
      os.rename(sourcefn, backupsourcefn)
    src_eolname = detectEOL(src)
    src_eol = eolname_eol[src_eolname]
    targetfn = sourcefn
    sheBang, src = extractSheBang(src)
    output = open(targetfn, 'wb')
    header_srceol = commenter.commentarize(headerUnix, src_eolname)
    if sheBang:
      output.write(sheBang + src_eol)
    output.write(header_srceol)
    output.write(src)
    output.close()
    shutil.copymode(backupsourcefn, targetfn)

