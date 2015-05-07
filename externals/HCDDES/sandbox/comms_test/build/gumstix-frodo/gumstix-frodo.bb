DESCRIPTION = "Gumstix time-triggered execution framework"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"

PR = "r0"

SRC_URI = " \
   file://app \
   file://app/app.c \
   file://gumstix-frodo.h \
   file://gumstix-frodo.c \
   file://Config.h \
   file://Crc8.h \
   file://Crc8.c \
   file://DumpMem.h \
   file://DumpMem.c \
   file://Log.h \
   file://Log.c \
   file://i2c-api.h \
   file://i2c-api.c \
   file://i2c-dev.h \
   file://i2c-io.h \
   file://i2c-io-api.h \
   file://i2c.h \  
   file://OuterLoop_sl.h \
   file://OuterLoop_sl.c \
  "

CMD_NAME=gumstix-frodo

S = "${WORKDIR}"

do_compile () {
	${CC} -I. -lpthread -o ${CMD_NAME} *.c app/*.c
}

do_install () {
  install -d ${D}${bindir}/
	install -m 0755 ${WORKDIR}/${CMD_NAME} ${D}${bindir}/
}

PACKAGES = "${PN}"
FILES_${PN} = "${bindir}/*"

