TARGET ?= intel64

ifdef PIN_ROOT
	CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
else
	CONFIG_ROOT := ../Config
endif

ifndef DFT_ROOT
	DFT_ROOT := ../libdft64/
endif

include $(CONFIG_ROOT)/makefile.config
include Makefile.rules
include $(TOOLS_ROOT)/Config/makefile.default.rules
