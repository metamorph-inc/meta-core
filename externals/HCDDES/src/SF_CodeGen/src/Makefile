ANTLR_SRC = CExpr2SFC/CExprLexer.hpp CExpr2SFC/CExprLexer.cpp CExpr2SFC/CExprParser.hpp CExpr2SFC/CExprParser.cpp
CEXPR2SFC_SRC = CExpr2SFC/CExpr2SFC.hpp CExpr2SFC/CExpr2SFC.cpp
MAIN_SRC = GenEDP2SFC_main_g.hpp GenEDP2SFC_main_g.cpp
SFCPRINT_SRC = SFCprint.cpp

OBJS = CallGraphPrint.o GenEDP2SFC_unix.o GenEDP2SFC_main_g.o SFCprint.o SFuncWrapperPrint.o SfuncCodeTemplate.o \
       VarResolve.o main.o stringExt.o StateLabelInfo.o \
       Udm/ECSL_DP.o Udm/EDP2SFC.o Udm/LINKS.o Udm/SFC.o \
       CExpr2SFC/CExpr2SFC.o  CExpr2SFC/CExprLexer.o CExpr2SFC/CExprParser.o \
       unixExt.o


TARGET = SF_CodeGen.exe
TARGET_DIR = ../../bin

DEBUG = -g

DEFINES = -DANTLR_AST_INIT

INC = -I. -ICExpr2SFC -IUdm -I../include -I$(HOME)/System/include -I/usr/include/udm

LIBINC = -L/home/hnine/System/lib

LIBS = -ludm -lantlr -lminizip -lxerces-c

$(TARGET_DIR)/$(TARGET) : $(OBJS)
	mkdir -p $(TARGET_DIR)
	g++ $(OBJS) $(LIBINC) $(LIBS) $(DEBUG) -o $@

$(CEXPR2SFC_SRC) : $(ANTLR_SRC)

$(MAIN_SRC) $(SFCPRINT_SRC) $(CEXPR2SFC_SRC) : $(ANTLR_SRC)

.cpp.o :
	g++ $(DEFINES) $(INC) -c $(DEBUG) $< -o $@

.h.cpp :
	touch $@

.hpp.cpp :
	touch $@

GenEDP2SFC_unix.cpp : GenEDP2SFC.h
	touch $@

$(ANTLR_SRC) : CExpr2SFC/CExprParser.g
	CLASSPATH=/home/hnine/System/lib/antlr.jar gij antlr.Tool -o CExpr2SFC/ $<


clean:
	rm -f $(ANTLR_SRC) $(OBJS) $(TARGET_DIR)/$(TARGET)
