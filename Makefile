# includefile contains LOCALDIR-definition

include ../conf/$(OSTYPE).mk

LIBNAME=puTools
LOCALIDIR=$(LOCALDIR)/include/$(LIBNAME)

INCLUDE=-I../include
DEFINES=

LOCALOPTIONS=
include ../conf/targets.mk
