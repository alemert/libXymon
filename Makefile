################################################################################
# makefile for test util
################################################################################

MAKE_INCLUDE_PATH=mk.inc

# ------------------------------------------------------------------------------
# Compiler and BIT
# ------------------------------------------------------------------------------
CC=gcc
BIT = 64

# ------------------------------------------------------------------------------
# debugger
# ------------------------------------------------------------------------------
DBGOPT = -g

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------
SOURCES = sendmsg.c strfunc.c misc.c stackio.c environ.c

# ------------------------------------------------------------------------------
# libraries
# ------------------------------------------------------------------------------
AROWN = libmsgcat.a
#LSOWN = msgcat  
# sogen

# ------------------------------------------------------------------------------
# main source
# ------------------------------------------------------------------------------
MAIN = main.c

# ------------------------------------------------------------------------------
# BINARY
# ------------------------------------------------------------------------------
BINARY = xym

# ------------------------------------------------------------------------------
# libraries dynamic & static
# ------------------------------------------------------------------------------
LIBRARY = libxym.so

ARCHIVE = libxym.so

# ------------------------------------------------------------------------------
# rollout includes
# ------------------------------------------------------------------------------
ROLLOUT_INC = 

# ------------------------------------------------------------------------------
# general includes
# ------------------------------------------------------------------------------
include $(MAKE_INCLUDE_PATH)/general.modules.mk

cleanlocal :

# ------------------------------------------------------------------------------
# tests
# ------------------------------------------------------------------------------
#TEST = t_file_000 t_string_000 t_fork_000
include $(MAKE_INCLUDE_PATH)/test.mk

