#=============================================================================
#Copyright (C) 2015, Anonymous Maarten
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
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# the name of the target operating system
set(CMAKE_SYSTEM_NAME sdcc)
set(CMAKE_SYSTEM_PROCESSOR mcs51)

set(SDCC_PREFIX "sdcc-")

if(DEFINED SDCC_LOCATION)
  find_program(CMAKE_C_COMPILER
    NAMES "${SDCC_PREFIX}sdcc"
    PATHS "${SDCC_LOCATION}"
  )
else()
  find_program(CMAKE_C_COMPILER
    NAMES "${SDCC_PREFIX}sdcc"
  )
  get_filename_component(SDCC_LOCATION "${CMAKE_C_COMPILER}" PATH)
endif()

macro(FIND_SDCC_PROGRAM TARGET WHAT)
  find_program(${TARGET}
    NAMES "${SDCC_PREFIX}${WHAT}" "${WHAT}"
    PATHS "${SDCC_LOCATION}"
    NODEFAULTPATH
  )
  find_program(${TARGET}
    NAMES "${SDCC_PREFIX}${WHAT}" "${WHAT}"
  )
endmacro(FIND_SDCC_PROGRAM)

#set(ASM_DIALECT "_SDCC")

FIND_SDCC_PROGRAM(CMAKE_ASM_COMPILER "sdas8051")
set(CMAKE_ASM${ASM_DIALECT}_COMPILER ${CMAKE_ASM_COMPILER})
set(CMAKE_ASM_COMPILER_ENV_VAR "ASM${ASM_DIALECT}")
set(CMAKE_ASM${ASM_DIALECT}_COMPILER_ID SDCC)
FIND_SDCC_PROGRAM(SDCCLIB_EXECUTABLE "sdcclib")
FIND_SDCC_PROGRAM(CMAKE_LINKER "sdld")
FIND_SDCC_PROGRAM(CMAKE_AR "sdar")
FIND_SDCC_PROGRAM(CMAKE_NM "sdnm")
FIND_SDCC_PROGRAM(CMAKE_MAKEBIN "makebin")
FIND_SDCC_PROGRAM(CMAKE_RANLIB "sdranlib")
FIND_SDCC_PROGRAM(CMAKE_OBJCOPY "sdobjcopy")
#FIND_SDCC_PROGRAM(CMAKE_STRIP "sdstrip") #non-existant
#FIND_SDCC_PROGRAM(CMAKE_OBJDUMP "sdobjdump") #non-existant

set(SDCC_MODEL small)

set(CMAKE_FIND_ROOT_PATH  /usr/share/sdcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
