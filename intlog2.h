/*
=============================================================================
Copyright (C) 2015-2016, Anonymous Maarten

This file is part of ADuC832 demoproject.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
=============================================================================
*/
#ifndef _INTLOG2_H
#define _INTLOG2_H

#undef __VALUE_TOO_LOW
#undef __VALUE_TOO_BIG
#define _INTLOG2_4294967296_(VAL) -2

#define _INTLOG2_2147483648_(VAL) \
    (((VAL) <= 2147483647ULL) ? 30 : _INTLOG2_4294967296_(VAL))

#define _INTLOG2_1073741824_(VAL) \
    (((VAL) <= 1073741823ULL) ? 29 : _INTLOG2_2147483648_(VAL))

#define _INTLOG2_536870912_(VAL) \
    (((VAL) <= 536870911ULL) ? 28 : _INTLOG2_1073741824_(VAL))

#define _INTLOG2_268435456_(VAL) \
    (((VAL) <= 268435455ULL) ? 27 : _INTLOG2_536870912_(VAL))

#define _INTLOG2_134217728_(VAL) \
    (((VAL) <= 134217727ULL) ? 26 : _INTLOG2_268435456_(VAL))

#define _INTLOG2_67108864_(VAL) \
    (((VAL) <= 67108863ULL) ? 25 : _INTLOG2_134217728_(VAL))

#define _INTLOG2_33554432_(VAL) \
    (((VAL) <= 33554431ULL) ? 24 : _INTLOG2_67108864_(VAL))

#define _INTLOG2_16777216_(VAL) \
    (((VAL) <= 16777215ULL) ? 23 : _INTLOG2_33554432_(VAL))

#define _INTLOG2_8388608_(VAL) \
    (((VAL) <= 8388607ULL) ? 22 : _INTLOG2_16777216_(VAL))

#define _INTLOG2_4194304_(VAL) \
    (((VAL) <= 4194303ULL) ? 21 : _INTLOG2_8388608_(VAL))

#define _INTLOG2_2097152_(VAL) \
    (((VAL) <= 2097151ULL) ? 20 : _INTLOG2_4194304_(VAL))

#define _INTLOG2_1048576_(VAL) \
    (((VAL) <= 1048575ULL) ? 19 : _INTLOG2_2097152_(VAL))

#define _INTLOG2_524288_(VAL) \
    (((VAL) <= 524287ULL) ? 18 : _INTLOG2_1048576_(VAL))

#define _INTLOG2_262144_(VAL) \
    (((VAL) <= 262143ULL) ? 17 : _INTLOG2_524288_(VAL))

#define _INTLOG2_131072_(VAL) \
    (((VAL) <= 131071ULL) ? 16 : _INTLOG2_262144_(VAL))

#define _INTLOG2_65536_(VAL) \
    (((VAL) <= 65535ULL) ? 15 : _INTLOG2_131072_(VAL))

#define _INTLOG2_32768_(VAL) \
    (((VAL) <= 32767ULL) ? 14 : _INTLOG2_65536_(VAL))

#define _INTLOG2_16384_(VAL) \
    (((VAL) <= 16383ULL) ? 13 : _INTLOG2_32768_(VAL))

#define _INTLOG2_8192_(VAL) \
    (((VAL) <= 8191ULL) ? 12 : _INTLOG2_16384_(VAL))

#define _INTLOG2_4096_(VAL) \
    (((VAL) <= 4095ULL) ? 11 : _INTLOG2_8192_(VAL))

#define _INTLOG2_2048_(VAL) \
    (((VAL) <= 2047ULL) ? 10 : _INTLOG2_4096_(VAL))

#define _INTLOG2_1024_(VAL) \
    (((VAL) <= 1023ULL) ? 9 : _INTLOG2_2048_(VAL))

#define _INTLOG2_512_(VAL) \
    (((VAL) <= 511ULL) ? 8 : _INTLOG2_1024_(VAL))

#define _INTLOG2_256_(VAL) \
    (((VAL) <= 255ULL) ? 7 : _INTLOG2_512_(VAL))

#define _INTLOG2_128_(VAL) \
    (((VAL) <= 127ULL) ? 6 : _INTLOG2_256_(VAL))

#define _INTLOG2_64_(VAL) \
    (((VAL) <= 63ULL) ? 5 : _INTLOG2_128_(VAL))

#define _INTLOG2_32_(VAL) \
    (((VAL) <= 31ULL) ? 4 : _INTLOG2_64_(VAL))

#define _INTLOG2_16_(VAL) \
    (((VAL) <= 15ULL) ? 3 : _INTLOG2_32_(VAL))

#define _INTLOG2_8_(VAL) \
    (((VAL) <= 7ULL) ? 2 : _INTLOG2_16_(VAL))

#define _INTLOG2_4_(VAL) \
    (((VAL) <= 3ULL) ? 1 : _INTLOG2_8_(VAL))

#define _INTLOG2_2_(VAL) \
    (((VAL) <= 1ULL) ? 0 : _INTLOG2_4_(VAL))

#define INTLOG2(VAL) \
     (((VAL) < 1) ? -1 : _INTLOG2_2_(VAL))

#endif //_INTLOG2_H