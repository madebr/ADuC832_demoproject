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

def calculate_checksum(data):
  return (0x100 - sum(data)) % 0x100

def verify_checksum(data):
  expected = calculate_checksum(data[:-1])
  got = data[-1]
  if expected != got:
    return False
  return True

class LoaderInformation:
  def __init__(self):
    self.identifier = None
    self.firmwareversion = None
    self.hardwareconfig = None
    self.reserved = None
  def __repr__(self):
    return '[LoaderInformation:ident:{0},fw:{1},hwcfg:{2}]'.format(
      self.identifier,
      self.firmwareversion,
      self.hardwareconfig)
  def __str__(self):
    return 'Loader:\n' + \
      '  identifiier: {}\n'.format(self.identifier.decode('utf8')) + \
      '  firmware: {}\n'.format(self.firmwareversion.decode('utf8')) + \
      '  hardwareconfig: 0x{}'.format(bytesToHexString(self.hardwareconfig, ''))
  @classmethod
  def fromBytes(cls, data):
    if not verify_checksum(data):
      return None
    ldr = cls()
    ldr.identifier = data[0:10].strip()
    ldr.firmwareversion = data[10:14].strip()
    if data[14:16] != b'\r\n':
      print('Warning: expected CRLN, got {0})'.format(data[14:16]))
    ldr.hardwareconfig = data[16:18]
    ldr.reserved = data[18:24]
    return ldr

def wait_for_loader(conn):
  data = b''
  while True:
    data += conn.read(1)
    if len(data) > 25:
      data = data[-25:]
    if len(data) == 25:
      ldr = LoaderInformation.fromBytes(data)
      if not ldr:
        continue
      return ldr

def interrogate(conn):
  data = b'!Z\x00'
  checksum = calculate_checksum(data[1:])
  packet = data + bytes([checksum])
  conn.write(packet)
  conn.flushOutput()
  ldr = LoaderInformation.fromBytes(conn.read(25))
  if not ldr:
    print('WARNING: interrogation checksum failed')
  return ldr

def readProgramPagePacket(page):
  return createPacket(b'V' + bytes([page]))

def eraseProgramOnlyPacket():
  return createPacket(b'C')

def eraseProgramDataPacket():
  return createPacket(b'A')

def addressToUML(address):
  return bytes(
      [(address >> 16) & 0xff,
        (address >> 8) & 0xff,
        (address) & 0xff])

def writeProgramMemoryPackets(address, data):
  nbBytes = 0
  packets = []
  while nbBytes < len(data):
    subData = data[nbBytes:nbBytes+21]
    rawaddress = addressToUML(address + nbBytes)
    rawPacketData = b'W' + rawaddress + subData
    packets.append(createPacket(rawPacketData))
    nbBytes += len(subData)
  return packets

def writeDataMemoryPackets(address, data):
  if len(data) % 4 != 0:
    print('WARNING: len(data) must be a multiple of 4')
  rawaddress = addressToUML(address)
  rawdata = b'E' + rawaddress + data
  return createPacket(rawdata)

def jumpUserCodePacket(address):
  rawaddress = addressToUML(address)
  rawdata = b'U' + rawaddress
  return createPacket(rawdata)

def segments(ihx):
  addresses = ihx.addresses()
  if not addresses:
      return []
  elif len(addresses) == 1:
      return([(addresses[0], addresses[0]+1)])
  adjacent_differences = [(b - a) for (a, b) in zip(addresses[:-1], addresses[1:])]
  breaks = [i for (i, x) in enumerate(adjacent_differences) if x > 1]
  endings = [addresses[b] for b in breaks]
  endings.append(addresses[-1])
  beginings = [addresses[b+1] for b in breaks]
  beginings.insert(0, addresses[0])
  return [(a, b+1) for (a, b) in zip(beginings, endings)]

def eraseProgramOnly(conn):
  packet = eraseProgramOnlyPacket()
  sendPacket(conn, packet)

def verifyProgram(conn, reference):
  refIhx = intelhex.IntelHex()
  refIhx.loadhex(reference)
  nbPages = (refIhx.maxaddr() + 0x100 - 1) // 0x100
  ihx = readProgram(conn, nbPages)
  _, fn = tempfile.mkstemp()
  ihx.write_hex_file(fn)
  print('Read-back data written to temporary file "{}"'.format(fn))
  errors = []
  for start, stop in segments(refIhx):
    for addr in range(start, stop):
      if refIhx[addr] != ihx[addr]:
        errors.append(addr)
  if len(errors) == 0:
    print('Verification: OK')
  else:
    print('Verification: {} errors!'.format(len(errors)))

def readProgram(conn, nbPages = None):
  pagesMax = 256 - 8
  if nbPages == None:
    nbPages = pagesMax
  if nbPages > pagesMax:
    print('WARNING: a maximum of {} pages can be read'.format(pagesMax))
    nbPages = pagesMax
  ihx = intelhex.IntelHex()
  for page in range(nbPages):
    packet = readProgramPagePacket(page)
    conn.write(packet)
    bytesToRead = 0x100 + 1
    data = b''
    if page == 0:
      data += conn.read(1)
      bytesToRead -= 1
      if data == b'\x07':
        print('Cannot read program.')
        if conn.dev:
          leave(1)
    data += conn.read(bytesToRead)
    if not verify_checksum(data):
      print('Checksum fail')
      if conn.dev:
        leave(1)
        return None
    ihx.frombytes(data[:-1], page * 0x100)
  return ihx

def sendProgram(conn, f):
  ihx = intelhex.IntelHex()
  ihx.loadhex(f)
  for start, stop in segments(ihx):
    binstr = ihx[start:stop].tobinstr()
    packets = writeProgramMemoryPackets(start, binstr)
    for packet in packets:
      sendPacket(conn, packet)

def createPacket(data):
  start = b'\x07\x0e'
  if len(data) > 25:
    print('error:data is too long.')
    return False
  if bytes([data[0]]) not in b'CAVWESBU':
    print('Illegal command. ({0})'.format(command))
    return False
  body = bytes([len(data)]) + data
  packet = start + body + bytes([calculate_checksum(body)])
  return packet

def sendPacket(conn, packet):
  conn.write(packet)
  response = conn.read(1)
  if response == b'\x06':
    return True
  elif response == b'\x07':
    print('Received NACK')
    if conn.dev:
      leave(1)
    return None
  else:
    print('Warning: Unknown response!')
    if conn.dev:
      leave(1)
    return None

def test():
  p = eraseProgramOnlyPacket()
  e = b'\x07\x0e\x01\x43\xbc'
  if p != e:
    print('ERROR: eraseProgramPacket')
    print(p)
    print(e)
  p = eraseProgramDataPacket()
  e =  b'\x07\x0e\x01\x41\xbe'
  if p != e:
    print('ERROR: eraseProgramDataPacket')
    print(p)
    print(e)
  p = writeProgramMemoryPackets(0x0,
    bytes([0x0, 0xc, 0xe, 0xc, 0xf, 0xe, 0x4f, 0x63]))[0]
  e = b'\x07\x0e\x0c\x57\x00\x00\x00\x00\x0c\x0e\x0c\x0f\x0e\x4f\x63\xa8'
  if p != e:
    print('ERROR: writeProgramMemoryPackets')
    print(p)
    print(e)
  p = writeDataMemoryPackets(0x5,
      bytes([0xa, 0xb, 0xc, 0xd]))
  e = b'\x07\x0e\x08\x45\x00\x00\x05\x0a\x0b\x0c\x0d\x80'
  if p != e:
    print('ERROR: writeDataMemoryPackets')
    print(p)
    print(e)
  p = jumpUserCodePacket(0)
  e = b'\x07\x0e\x04\x55\x00\x00\x00\xa7'
  if p != e:
    print('ERROR: jumpUserCodePacket')
    print(p)
    print(e)

def bytesToHexString(b, sep=' '):
  return sep.join('{:02X}'.format(x) for x in b)

class DeviceWrapper(object):
  def __init__(self, dev, verbose):
    super(DeviceWrapper, self).__init__()
    self.dev, self.verbose = dev, verbose
  def write(self, data, *args, **kwargs):
    if self.verbose:
      print('WRITE: {}'.format(bytesToHexString(data)))
    if self.dev:
      return self.dev.write(data, *args, **kwargs)
    else:
      return len(data)
  def read(self, size, *args, **kwargs):
    if self.dev:
      data = self.dev.read(size, *args, **kwargs)
    else:
      data = size * b'\xff'
    if self.verbose:
      print('READ: {}'.format(bytesToHexString(data)))
    return data
  def flushOutput(self):
    if self.dev:
      self.dev.flushOutput()

def userinputYesNo(msg):
  user_input = input('{0} [Y/N]? '.format(msg))
  if not len(user_input) or user_input[0].upper() != 'Y':
    return False
  else:
    return True

def leave(reason):
  print('leaving...')
  sys.exit(reason)


if __name__ == '__main__':
  import argparse
  import serial
  import sys
  import tempfile

  parser = argparse.ArgumentParser(description='Analog Device Micro Controller Serial Programmer')
  parser.add_argument('device', metavar='DEVICE', type=str, help='Name of the serial device')
  parser.add_argument('-D', '--epd', dest='eraseProgData', action='store_true', help='erase program and data memory')
  parser.add_argument('-E', '--ep', dest='eraseProg', action='store_true', help='erase program memory')
  parser.add_argument('-F', '--force', dest='force', action='store_true', help='force, no questions asked')
  parser.add_argument('-i', '--interrogate', dest='interrogate', action='store_true', help='interrogate the device')
  parser.add_argument('-v', '--verify', dest='verify', action='store_true', help='verify written data')
  parser.add_argument('-p', '--prog', metavar='PROG', dest='program', type=str, help='flash PROG in the program section')
  parser.add_argument('-d', '--data', metavar='DATA', dest='data', type=str, help='flash DATA in the the data section')
  parser.add_argument('-V', '--verbose', dest='verbose', action='store_true', help='verbose output')
  parser.add_argument('-t', '--test', dest='test', action='store_true', help='test only, no data will be transmitted')
  args = parser.parse_args()

  try:
    dev = serial.Serial(args.device, 9600) if not args.test else None
    conn = DeviceWrapper(dev, args.verbose)
  except serial.SerialException as e:
    print(e.strerror)
    leave(1)

  if args.interrogate:
    answer = interrogate(conn)
    if not answer:
      if not args.test:
        leave(1)
      print('Ignoring failed interrogation (test mode)')
    else:
      print(answer)

  if args.eraseProgData:
    if not args.force and not userinputYesNo('Erasing program and data memory'):
      leave(1)
    print('NOT IMPLEMENTED')

  if args.eraseProg:
    if not args.force and not userinputYesNo('Erasing program memory'):
      leave(1)
    eraseProgramOnly(conn)

  if args.program is not None:
    if not args.eraseProgData and not args.eraseProg:
      print('WARNING: programming can only be done when erasing in the same session.')
      if not args.force and not userinputYesNo('Is program section erased?'):
        leave(1)
    if not args.force and not userinputYesNo('Sending program "{0}".'.format(args.program)):
      leave(1)
    sendProgram(conn, args.program)

  if args.verify:
    if not args.program:
      print('WARNING: verification can only be done when programming in the same session.')
    print('verify: NOT IMPLEMENTED COMPLETELY (DATA + REFACTOR)!')
    verifyProgram(conn, args.program)

  if args.data is not None:
    if not args.eraseProgData:
      if not args.force and not userinputYesNo('Is data section erased?'):
        leave(1)
    if not args.force and not userinputYesNo('Sending data "{0}".'.format(args.program)):
      leave(1)
    print('NOT IMPLEMENTED!')

  if False:
    s = serial.Serial(device)
    ldrInfo = wait_for_loader(s)
    print(ldrInfo)
    #sendPacket(b'E123')
    r = interrogate(s)
    print(r)
  test()
  #print(eraseProgramOnlyPacket())
  #print(eraseProgramDataPacket())
  #print(writeProgramMemoryPacket(0x0, bytes([0x0, 0xc, 0xe, 0xc, 0xf, 0xe, 0x4f, 0x63])))
