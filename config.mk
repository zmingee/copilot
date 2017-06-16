# copilot version
VERSION = 0.1.0

# Customize below to fit your system

#########
# PATHS #
#########

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
LIBPREFIX = $(PREFIX)/lib/copilot

#############
# TOOLCHAIN #
#############

CC = avr-gcc
LD = avr-ld
STRIP = avr-strip
OBJCOPY = avr-objcopy
SIZE = avr-size
