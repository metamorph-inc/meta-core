DESCRIPTION = "Robostix time-triggered execution framework"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"

PR = "r0"

SRC_URI = " \
     file://kernel \
#    file://kernel/list.c \
#    file://kernel/include \
#    file://kernel/include/queue.h \
#    file://kernel/include/task.h \
#    file://kernel/include/list.h \
#    file://kernel/include/croutine.h \
#    file://kernel/include/semphr.h \
#    file://kernel/include/portable.h \
#    file://kernel/include/FreeRTOS.h \
#    file://kernel/include/projdefs.h \
#    file://kernel/croutine.c \
#    file://kernel/ATmega128 \
#    file://kernel/ATmega128/portmacro.h \
#    file://kernel/ATmega128/port.c \
#    file://kernel/tasks.c \
#    file://kernel/mm \
#    file://kernel/mm/heap_3.c \
#    file://kernel/mm/heap_2.c \
#    file://kernel/mm/heap_1.c \
#    file://kernel/queue.c \
    file://Makefile \
    file://i2c.c \
    file://i2c.h \
#    file://uart.c \
#    file://uart.h \
    file://robostix-frodo.h \
    file://led.h \
    file://led.c \
    file://gpio.h \
    file://gpio.c \
    file://adc.h \
    file://adc.c \
    file://peri.h \
    file://uart0.c \
    file://uart0.h \
    file://DataHandling_sl.c \
    file://DataHandling_sl.h \
    file://InnerLoop_sl.c \
    file://InnerLoop_sl.h \
    file://robostix-frodo.c \
    file://FreeRTOSConfig.h \
    file://app \
  "

S = "${WORKDIR}"

do_compile () {
	unset CC CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
	cd ${S}
	make
}

do_install () {
	install -m 0755 -d ${D}/${datadir}/robostix
	install -m 0755 ${S}/robostix-frodo.hex ${D}/${datadir}/robostix
}

pkg_postinst_${PN} () {
	/usr/bin/uisp --erase --upload if=${datadir}/robostix/robostix-frodo.hex
}

PACKAGES = "${PN}"
FILES_${PN} = "${datadir}/robostix/robostix-frodo.hex"
