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
# the name of the target operating system
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".lib")
set(CMAKE_IMPORT_LIBRARY_PREFIX )
set(CMAKE_IMPORT_LIBRARY_SUFFIX )
set(CMAKE_EXECUTABLE_SUFFIX ".ihx")
set(CMAKE_LINK_LIBRARY_SUFFIX ".lib")
set(CMAKE_DL_LIBS "")

set(CMAKE_C_OUTPUT_EXTENSION ".rel")

if(NOT CMAKE_SYSTEM_PROCESSOR)
  set(CMAKE_SYSTEM_PROCESSOR mcs51)
endif(NOT CMAKE_SYSTEM_PROCESSOR)

if(NOT SDCC_MODEL)
  set(SDCC_MODEL small)
  message(AUTHOR_WARNING "Model set to '${SDCC_MODEL}'")
endif(NOT SDCC_MODEL)
string(TOLOWER ${SDCC_MODEL} SDCC_MODEL)

if(NOT DEFINED CMAKE_C_FLAGS_INIT)
  set(CMAKE_C_FLAGS_INIT "-m${CMAKE_SYSTEM_PROCESSOR} --model-${SDCC_MODEL}")
endif()

if (NOT DEFINED CMAKE_EXE_LINKER_FLAGS_INIT)
  set(CMAKE_EXE_LINKER_FLAGS_INIT "-k /usr/share/sdcc/lib/${SDCC_MODEL} -l ${CMAKE_SYSTEM_PROCESSOR} -l libsdcc")
endif()

#set(CMAKE_LDFLAGS_INIT "-k /usr/share/sdcc/lib/small -l ${CMAKE_SYSTEM_PROCESSOR} -FUCKER")

if(${CMAKE_VERSION} VERSION_GREATER 3.3.9)
  #set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
  message(FATAL_ERROR "CMake 3.4 needs INCLUDES in the COMPILE_OBJECT")
endif()

set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES>  <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_LIBRARIES> -i <TARGET> <OBJECTS>")
