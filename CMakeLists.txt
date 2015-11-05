project(proj1 C)


#OUTPUT_IHX=output.ihx
set(SOURCES
  adc.c
  dac.c
  led.c
  pwm.c
  clock.c
  lcd.c
  main.c
  serial.c
  watchdog.c
)
#CSRCS=main.c# clock.c lcd.c led.c pwm.c serial.c adc.c dac.c watchdog.c
#ASRCS=
#OBJS=$(CSRCS:.c=.rel) $(ASRCS:.asm=.rel)
set(ARCH mcs51)

#Options for memory model: small, medium, large or huge
set(MEMORY_MODEL small)
#ADuC832

#8051 architecture
set(INTERNAL_XRAMSIZE 2048)
set(HOME_POS 0x0000)
set(ISEG_POS 0x0000)
set(BSEG_POS 0x0000)

set(CMAKE_C_FLAGS "--std-c99 --Werror --model-${MEMORY_MODEL} -m${ARCH} --opt-code-size")

set(CMAKE_EXE_LINKER_FLAGS "-n -mwux -Y -b HOME=${HOME_POS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -b ISEG=${ISEG_POS} -M")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -b BSEG=${BSEG_POS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -k /usr/share/sdcc/lib/small")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -l mcs51 -l libsdcc")
# -l libint -l liblong -l libfloat


add_executable(hello ${SOURCES})
