# THIS FILE WAS AUTOMATICALLY GENERATED. DO NOT EDIT.

DEFINES += -DNDEBUG=1 -DTRIMMED=1 -DMOZ_JEMALLOC_HARD_ASSERTS -Dabort=moz_abort -DMOZ_JEMALLOC_IMPL
LOCAL_INCLUDES += -I$(topsrcdir)/memory/build
CSRCS += jemalloc.c
MOZBUILD_CFLAGS += -Wshadow
MOZBUILD_CFLAGS += -Wno-unused
LIBRARY_NAME := mozjemalloc
FORCE_STATIC_LIB := 1
REAL_LIBRARY := libjemalloc.a
