ifndef ACS_ENV
acs-env-not-defined:
	@echo Set the environment variable by sourcing setup.$$HOSTNAME
endif

TOPDIR		=../../../../
ARCH		=ix86
SOURCES		=$(wildcard *.c *.h)

all: Debug

include $(TOPDIR)/etc/Rules.gnu

Debug Release: dummy
	@MAKEFLAGS=; $(NMAKE) -f mem.mak "CFG=mem - Win32 $@"

clean: dummy
	@$(RM) *~ *.log TAGS

wipe distclean: clean
	$(RM) -r Debug Release
	$(RM) *.aps *.clw *.plg
