ifndef ACS_ENV
acs-env-not-defined:
	@echo Set the environment variable by sourcing setup.$$HOSTNAME
endif

TOPDIR		=../../../../
ARCH		=ix86
SOURCES		=$(wildcard *.cpp *.h)

all: Debug

include $(TOPDIR)/etc/Rules.gnu

Debug Release: dummy
	@MAKEFLAGS=; $(NMAKE) -f mcl.mak "CFG=mcl - Win32 $@"

clean: dummy
	@$(RM) *~ *.log TAGS

wipe distclean: clean
	$(RM) -r Debug Release
	$(RM) *.aps *.clw *.plg
	$(RM) parser.dlg mcl.cpp
	$(RM) MCLLexer.cpp MCLLexer.h
	$(RM) MCLParser.cpp MCLParser.h
