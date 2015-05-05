
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LATENCY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LATENCY_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifdef LATENCY_EXPORTS
#define LATENCY_API __declspec(dllexport)
#else
#define LATENCY_API __declspec(dllimport)
#endif




LATENCY_API CBdd PCM_Eval_latency_0(int& maxSize, int limit, CDynElement * p_this);
LATENCY_API CBdd PCM_Eval_latency(int limit, ClRelExpr::RelOp op, CDynElement *p_this);
LATENCY_API CBdd DoPathLatency(int& maxSize, int limit,  CDynElement *p_this);
extern int maxLatBddSz = 0;


