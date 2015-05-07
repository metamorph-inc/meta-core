#!/bin/bash

# Copy the SL_CodeGen generated files into place, making appropriate
# backups.

cp -f build/robostix-frodo/files/DataHandler_sl.h build/robostix-frodo/files/DataHandler_sl.bak.h
cp -f qint/DataHandler_sl.h build/robostix-frodo/files/DataHandler_sl.h
cp -f build/robostix-frodo/files/DataHandler_sl.c build/robostix-frodo/files/DataHandler_sl.bak.c
cp -f qint/DataHandler_sl.c build/robostix-frodo/files/DataHandler_sl.c

cp -f build/robostix-frodo/files/InnerLoop_sl.h build/robostix-frodo/files/InnerLoop_sl.bak.h
cp -f qint/InnerLoop_sl.h build/robostix-frodo/files/InnerLoop_sl.h
cp -f build/robostix-frodo/files/InnerLoop_sl.c build/robostix-frodo/files/InnerLoop_sl.bak.c
cp -f qint/InnerLoop_sl.c build/robostix-frodo/files/InnerLoop_sl.c

cp -f build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleS.h build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleS.bak.h
cp -f qint/Ssignal1CsingleCsignal2CsingleS.h build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleS.h

cp -f build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS.h build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS.bak.h
cp -f qint/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS.h build/robostix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS.h

cp -f build/gumstix-frodo/files/OuterLoop_sl.h build/gumstix-frodo/files/OuterLoop_sl.bak.h
cp -f qint/OuterLoop_sl.h build/gumstix-frodo/files/OuterLoop_sl.h
cp -f build/gumstix-frodo/files/OuterLoop_sl.c build/gumstix-frodo/files/OuterLoop_sl.bak.c
cp -f qint/OuterLoop_sl.c build/gumstix-frodo/files/OuterLoop_sl.c

cp -f build/gumstix-frodo/files/RefHandler_sl.h build/gumstix-frodo/files/RefHandler_sl.bak.h
cp -f qint/RefHandler_sl.h build/gumstix-frodo/files/RefHandler_sl.h
cp -f build/gumstix-frodo/files/RefHandler_sl.c build/gumstix-frodo/files/RefHandler_sl.bak.c
cp -f qint/RefHandler_sl.c build/gumstix-frodo/files/RefHandler_sl.c

cp -f build/gumstix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS.h build/gumstix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS.bak.h
cp -f qint/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS.h build/gumstix-frodo/files/Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS.h

