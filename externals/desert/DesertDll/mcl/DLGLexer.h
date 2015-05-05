#ifndef DLGLexer_h
#define DLGLexer_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-1998 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR12b
 */


#include "DLexerBase.h"

class DLGLexer : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	typedef unsigned char DfaState;

	DLGLexer(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 1)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	ANTLRTokenType act26();
	ANTLRTokenType act27();
	ANTLRTokenType act28();
	ANTLRTokenType act29();
	ANTLRTokenType act30();
	ANTLRTokenType act31();
	ANTLRTokenType act32();
	ANTLRTokenType act33();
	ANTLRTokenType act34();
	ANTLRTokenType act35();
	ANTLRTokenType act36();
	ANTLRTokenType act37();
	ANTLRTokenType act38();
	ANTLRTokenType act39();
	ANTLRTokenType act40();
	ANTLRTokenType act41();
	ANTLRTokenType act42();
	ANTLRTokenType act43();
	ANTLRTokenType act44();
	ANTLRTokenType act45();
	ANTLRTokenType act46();
	ANTLRTokenType act47();
	ANTLRTokenType act48();
	ANTLRTokenType act49();
	ANTLRTokenType act50();
	ANTLRTokenType act51();
	ANTLRTokenType act52();
	ANTLRTokenType act53();
	static DfaState st0[52];
	static DfaState st1[52];
	static DfaState st2[52];
	static DfaState st3[52];
	static DfaState st4[52];
	static DfaState st5[52];
	static DfaState st6[52];
	static DfaState st7[52];
	static DfaState st8[52];
	static DfaState st9[52];
	static DfaState st10[52];
	static DfaState st11[52];
	static DfaState st12[52];
	static DfaState st13[52];
	static DfaState st14[52];
	static DfaState st15[52];
	static DfaState st16[52];
	static DfaState st17[52];
	static DfaState st18[52];
	static DfaState st19[52];
	static DfaState st20[52];
	static DfaState st21[52];
	static DfaState st22[52];
	static DfaState st23[52];
	static DfaState st24[52];
	static DfaState st25[52];
	static DfaState st26[52];
	static DfaState st27[52];
	static DfaState st28[52];
	static DfaState st29[52];
	static DfaState st30[52];
	static DfaState st31[52];
	static DfaState st32[52];
	static DfaState st33[52];
	static DfaState st34[52];
	static DfaState st35[52];
	static DfaState st36[52];
	static DfaState st37[52];
	static DfaState st38[52];
	static DfaState st39[52];
	static DfaState st40[52];
	static DfaState st41[52];
	static DfaState st42[52];
	static DfaState st43[52];
	static DfaState st44[52];
	static DfaState st45[52];
	static DfaState st46[52];
	static DfaState st47[52];
	static DfaState st48[52];
	static DfaState st49[52];
	static DfaState st50[52];
	static DfaState st51[52];
	static DfaState st52[52];
	static DfaState st53[52];
	static DfaState st54[52];
	static DfaState st55[52];
	static DfaState st56[52];
	static DfaState st57[52];
	static DfaState st58[52];
	static DfaState st59[52];
	static DfaState st60[52];
	static DfaState st61[52];
	static DfaState st62[52];
	static DfaState st63[52];
	static DfaState st64[52];
	static DfaState st65[52];
	static DfaState st66[52];
	static DfaState st67[52];
	static DfaState st68[52];
	static DfaState st69[52];
	static DfaState st70[52];
	static DfaState st71[52];
	static DfaState st72[52];
	static DfaState st73[52];
	static DfaState st74[52];
	static DfaState st75[52];
	static DfaState st76[52];
	static DfaState st77[52];
	static DfaState st78[52];
	static DfaState st79[52];
	static DfaState st80[52];
	static DfaState st81[52];
	static DfaState st82[52];
	static DfaState st83[52];
	static DfaState st84[52];
	static DfaState st85[52];
	static DfaState st86[52];
	static DfaState st87[52];
	static DfaState st88[52];
	static DfaState st89[52];
	static DfaState st90[52];
	static DfaState st91[52];
	static DfaState st92[52];
	static DfaState st93[52];
	static DfaState st94[52];
	static DfaState st95[52];
	static DfaState st96[52];
	static DfaState st97[52];
	static DfaState st98[52];
	static DfaState st99[52];
	static DfaState st100[52];
	static DfaState st101[52];
	static DfaState st102[52];
	static DfaState st103[52];
	static DfaState st104[52];
	static DfaState st105[52];
	static DfaState st106[52];
	static DfaState st107[52];
	static DfaState st108[52];
	static DfaState st109[52];
	static DfaState st110[52];
	static DfaState st111[52];
	static DfaState st112[52];
	static DfaState st113[52];
	static DfaState st114[52];
	static DfaState st115[52];
	static DfaState st116[52];
	static DfaState st117[52];
	static DfaState st118[52];
	static DfaState st119[52];
	static DfaState st120[52];
	static DfaState st121[52];
	static DfaState st122[52];
	static DfaState st123[52];
	static DfaState st124[52];
	static DfaState st125[52];
	static DfaState *dfa[126];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[127];
	static DLGChar alternatives[127];
	static ANTLRTokenType (DLGLexer::*actions[54])();
	static unsigned char shift0[257];
	int ZZSHIFT(int c) { return b_class_no[automaton][1+c]; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (DLGLexer::*PtrDLGLexerMemberFunc)();
#endif
