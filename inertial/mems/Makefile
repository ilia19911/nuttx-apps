
include $(APPDIR)/Make.defs

# Hello, World! C++ Example

MAINSRC = mems_main.cxx

# Второй C++ файл
CXXSRCS = protocol.cxx
# helloxx built-in application info

PROGNAME = mems
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(CONFIG_DEFAULT_TASK_STACKSIZE)
MODULE = $(CONFIG_LIB_MEMS)

include $(APPDIR)/Application.mk
