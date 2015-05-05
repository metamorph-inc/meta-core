ifndef ACS_ENV
acs-env-not-defined:
	@echo Set the environment variable by sourcing setup.$$HOSTNAME
endif

TOPDIR		=../../../../
ARCH		=ix86
SOURCES		=$(wildcard *.h)

Debug Release: dummy


include $(TOPDIR)/etc/Rules.gnu


clean wipe distclean: dummy
	@$(RM) *~ *.log TAGS

