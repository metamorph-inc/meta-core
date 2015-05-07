
OBJS = ECSL_DP.o MatLabEngine.o MatLabUdmBlock.o MatLabUdm.o MatLabUdmData.o MatLabUdmMiscSimulink.o \
       MatLabUdmMiscStateflow.o MatLabUdmPort.o MatLabUdmState.o MatLabUdmTransition.o mdl2mga.o stringExt.o UdmEngine.o

ECSL_DP_FILES = ECSL_DP_xsd.h ECSL_DP.h ECSL_DP.cpp

TARGET = mdl2mga.exe
TARGET_DIR = ../bin

DEBUG = -g

INC = -I/home/hnine/System/include -I. -I/usr/local/matlab/extern/include -IUdm -I/usr/include/udm -I../ECSL_DP

LIBINC = -L/usr/lib -L/usr/local/matlab/bin/glnx86

LIBS = -lxerces-c -ludm -leng -lut -lmx -lmat -licuio -licudata -licui18n -lhdf5 -licuuc -lboost_regex -lminizip

$(TARGET_DIR)/$(TARGET) : $(OBJS)
	mkdir -p $(TARGET_DIR)
	g++ $(OBJS) $(LIBINC) $(LIBS) $(DEBUG) -o $@

.cpp.o :
	g++ $(DEFINES) $(INC) -c $(DEBUG) $< -o $@

.h.cpp :
	touch $@

.hpp.cpp :
	touch $@

$(ECSL_DP_FILES) : ../ECSL_DP/ECSL_DP_udm.xml
	Udm -g $<

clean:
	rm -f $(OBJS) $(TARGET_DIR)/$(TARGET) $(ECSL_DP_FILES)

