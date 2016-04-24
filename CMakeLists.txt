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
cmake_minimum_required(VERSION 3.3)
project(proj1 C ASM)

#OUTPUT_IHX=output.ihx
set(HEADERS
  assert.h
  intlog2.h
  util.h
  ADUC832.h
)

set(SOURCES
  adc.c
  button.c
  pwm.c
  clock.c
  lcd.c
  main.c
  console.c
  printf.c
  rtc.c
  serial.c
  timer.c
  watchdog.c
)

if (NOT CMAKE_SYSTEM_PROCESSOR)
  set(ARCH mcs51)
else()
  set(ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

#Options for memory model: small, medium, large or huge
set(MEMORY_MODEL small)
#ADuC832

#8051 architecture
set(INTERNAL_XRAMSIZE 2048)
set(HOME_POS 0x0000)
set(ISEG_POS 0x0000)
set(BSEG_POS 0x0000)

set(CMAKE_C_FLAGS "--std-c99 --Werror --model-${MEMORY_MODEL} -m${ARCH} --opt-code-size")

set(CMAKE_EXE_LINKER_FLAGS
  "-n -mwux -Y -b HOME=${HOME_POS} -b ISEG=${ISEG_POS} -M \
   -b BSEG=${BSEG_POS} -k /usr/share/sdcc/lib/small \
   -l mcs51 -l libsdcc")
# -l libint -l liblong -l libfloat

set(TARGETNAME proj1)

add_executable("${TARGETNAME}" ${SOURCES} ${HEADERS})
#add_dependencies("${TARGETNAME}.mem" "${TARGETNAME}")
#add_dependencies("${TARGETNAME}.map" "${TARGETNAME}")

target_include_directories("${TARGETNAME}" PRIVATE "${CMAKE_FIND_ROOT_PATH}/include/${ARCH}")

add_custom_target("${TARGETNAME}_pack" ALL
  objcopy -Iihex "${TARGETNAME}.ihx" -Oihex "${TARGETNAME}_pack.ihx"
  DEPENDS "${TARGETNAME}")
