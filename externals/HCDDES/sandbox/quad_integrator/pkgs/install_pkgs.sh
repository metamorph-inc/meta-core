#!/bin/sh

TARGET_ADDR=root@$1

scp gumstix-frodo_1.0-r0_armv5te.ipk $TARGET_ADDR:
if [ $? -ne 0 ] ; then
	echo "Gumstix Package Copy FAILED."
	exit 2
fi

ssh $TARGET_ADDR "ipkg -force-reinstall install gumstix-frodo_1.0-r0_armv5te.ipk"
if [ $? -ne 0 ] ; then
        echo "Gumstix Package Install FAILED."
        exit 3
fi

scp robostix-frodo_1.0-r0_armv5te.ipk $TARGET_ADDR:
if [ $? -ne 0 ] ; then
	echo "Robostix Package copy FAILED."
	exit 4
fi

ssh $TARGET_ADDR "ipkg -force-reinstall install robostix-frodo_1.0-r0_armv5te.ipk"
if [ $? -ne 0 ] ; then
        echo "Robostix Package Install FAILED."
        exit 5
fi

scp run_controller.sh $TARGET_ADDR:
if [ $? -ne 0 ] ; then
	echo "Controller script copy FAILED."
	exit 6
fi

