// -*-C++-*-
// bddmain.h
// CBdd class declaration

#ifndef BDDMAIN_H
#define BDDMAIN_H

#include "core/coredefs.h"

#include <sstream> 
#include <string> 
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <deque>

extern "C"
{
  #define __STDC__ 1
  #include "bdd/bdduser.h"
}

#define MAX_PROPERTIES 4                // latency & resource
typedef struct _idx { int val; int index; } CVIndex;

class DESERT_API CBdd
{
public:
  static void Init(int len, int props, ...);
  static void Finit();
  static void ModifyProperty(const TCHAR *name, int X, int Y, int Z);
  static CBdd One(void);
  static CBdd Zero(void);
  static CBdd ArithZero(void);          // mtbdd 0 terminal
  static CBdd ArithOne(void);           // mtbdd 1 terminal
  static CBdd IfThenElse(const CBdd& _if, const CBdd& _then, const CBdd& _else);
  static CBdd Implies(const CBdd& left, const CBdd& right);
  static CBdd Xor(const CBdd& left, const CBdd& right);
  static CBdd Encode(int encVal, int startVar, int encLen);
  static CBdd QuantifyExistentially(CBdd func, int startVar, int encLen);
  static CBdd Encode(int *e, int b, int n);
  static CBdd CBdd::Encode(const boost::dynamic_bitset<>& enc, int begin_var, int num_vars);
  static CBdd Encode(CVIndex e[], int l);
  //static CBdd EncodeProperty(const TCHAR *name, int idx, int val);
#ifndef DOUBLE_MTBDD
  static CBdd EncodeArith(int val);     // return the mtbdd terminal with this value
  static CBdd Sum(const CBdd& f,  const CBdd& g, long limit);		// return the sum with threshold this to operand; do not modify either this or operand
  static CBdd Mul(const CBdd& f, const CBdd& g, long limit);		// return the mul with threshold this to operand; do not modify either this or operand
#else
  static CBdd EncodeArith(double val);     // return the mtbdd terminal with this value
  static CBdd Sum(const CBdd& f,  const CBdd& g, double limit);		// return the sum with threshold this to operand; do not modify either this or operand
  static CBdd Mul(const CBdd& f, const CBdd& g, double limit);		// return the mul with threshold this to operand; do not modify either this or operand
#endif
  CBdd ApplyUnaryMathFunc(bdd_unary_math_function mathFunc) const; // Himanshu: Applies unary math function to the current Bdd terminal
  static CBdd Mtbdd_Ite(const CBdd& f, const CBdd& g);
  static CBdd Min(const CBdd& f, const CBdd& g);
  static CBdd Max(const CBdd& f, const CBdd& g);
 

 
  static CBdd QuantifyPropertyExistentially(const TCHAR *name, CBdd func, int idx);
  static int  Satisfy(CBdd& b, std::deque<boost::dynamic_bitset<>>& encVectors, int(*filter)(void*, int*), void* arg, volatile bool& cancel);
  static void ExpandDontCare(int *enc, int cur, std::deque<boost::dynamic_bitset<>>& encVectors, int(*filter)(void*, int*), void* arg);
  static void ExpandDontCare2(int *enc, std::deque<boost::dynamic_bitset<>>& encVectors, int(*filter)(void*, int*), void* arg, volatile bool& cancel);

public:
  // generate bdd for z_vec = y_vec + x_vec
  //static CBdd EqProperty(const TCHAR *name, int z_idx, int y_idx, int x_idx);
  // generate bdd for z_vec > y_vec + x_vec
  //static CBdd GtProperty(const TCHAR *name, int z_idx, int y_idx, int x_idx);


private:
  //static int FindProperty(const TCHAR *name);
  static bdd_manager  manager;
  static bdd *vars;
  static int length;
  // stuff for encoding properties (cost, latency, resource etc)
 /* static struct property {
    TCHAR *name;			// name of the property
    int start;			// start bdd var
    int X,Y,Z;			// 3-d grid
  } properties[MAX_PROPERTIES];
  static int propertyCount;
	*/
public:
  CBdd();
  CBdd(bdd core);
  CBdd(const CBdd& from);
  ~CBdd();
  CBdd& operator=(const CBdd& from);
  CBdd operator&&(const CBdd& arg) const;
  CBdd operator||(const CBdd& arg) const;
  CBdd operator!() const;
  
  CBdd operator+(const CBdd& arg) const;						// arithmetic sum over mtbdd's
 //added by feng
  CBdd operator-(const CBdd& arg) const;						// arithmetic sub over mtbdd's
  CBdd operator/(const CBdd& arg) const;						// arithmetic divided over mtbdd's
  //
  CBdd operator*(const CBdd& arg) const;						// arithmetic mul over mtbdd's
#ifndef DOUBLE_MTBDD
  CBdd operator/(const long value) const;						// arithmetic divide over mtbdd's to implement amed
  CBdd operator^(const long order) const;						// arithmetic nth order root over mtbdd'sm to implement gmed
  CBdd operator+(const long order) const;	
  CBdd operator-(const long order) const;
  CBdd operator*(const long order) const;
#else
  CBdd operator/(const double value) const;						// arithmetic divide over mtbdd's to implement amed
  CBdd operator^(const double order) const;						// arithmetic nth order root over mtbdd'sm to implement gmed
  CBdd operator+(const double order) const;	
  CBdd operator-(const double order) const;
  CBdd operator*(const double order) const;
#endif
  bool operator!=(const CBdd& from) const;
  bool operator==(const CBdd& from) const;
  
  operator bdd();
  int size();
  double sat_fract();

#ifndef DOUBLE_MTBDD
  CBdd threshold(int limit, bool eq=false);									// upper threshold operation over mtbdd's
  CBdd low_threshold(int limit, bool eq=false);								// lower threshold operation over mtbdd's
  CBdd equals_to(int value);									// equals_to value operation over mtbdds
  CBdd not_equals_to(int value);								// not_equals_to value operation over mtbdds
  CBdd devides_from(const long value);
#else
  CBdd threshold(double limit, bool eq=false);									// upper threshold operation over mtbdd's
  CBdd low_threshold(double limit, bool eq=false);								// lower threshold operation over mtbdd's
  CBdd equals_to(double value);									// equals_to value operation over mtbdds
  CBdd not_equals_to(double value);								// not_equals_to value operation over mtbdds
  CBdd devides_from(const double value);
#endif
  CBdd eq_to(CBdd other);


private:
  bdd core;
public:
  void dump(const TCHAR * comment) 
  {
	  FILE * fn = fopen("c:\\tmp\\bdd_debug.txt", "a");
	  if (fn)
	  {
		  _ftprintf(fn, _T("--------\n%s\n--------------\n"), comment);
		  bdd_print_bdd(manager, core, NULL, NULL, NULL,fn);
		  fclose(fn);
	  }
  }

  
};

//error handling macros
#define MANAGER_CHECK(FUNC_NAME)	if(!manager)	throw new CDesertException(CString(FUNC_NAME) + CString(": Manager is NULL!"))
#define BDD_FAILED(FUNC_NAME)		if(!temp)		throw new CDesertException("Operation returned null.")


// CBdd inline member function implemention


inline CBdd CBdd::One()
{
	MANAGER_CHECK(_T("CBdd::One()"));
	return CBdd(bdd_one(manager));
}

inline CBdd CBdd::Zero()
{
	MANAGER_CHECK(_T("CBdd::Zero()"));
	return CBdd(bdd_zero(manager));
}

inline CBdd CBdd::ArithZero()
{
	MANAGER_CHECK(_T("CBdd::ArithZero()"));
	return CBdd(mtbdd_get_terminal(manager, 0, 0));
}

inline CBdd CBdd::ArithOne()
{
	MANAGER_CHECK(_T("CBdd::ArithOne()"));
#ifndef DOUBLE_MTBDD
	return CBdd(mtbdd_get_terminal(manager, 0, -1));
#else
	return CBdd(mtbdd_get_terminal(manager, 0, 1072693248));
#endif
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::EncodeArith(int val)
{

	MANAGER_CHECK(_T("CBdd::EncodeArith()"));
#if 1 //ndef NEGATIVE_MTBDD
	return (val == 1) ?
		CBdd(mtbdd_get_terminal(manager, 0, -1)) :
		CBdd(mtbdd_get_terminal(manager, 0, val));
#else
	return (val == 1)? CBdd(mtbdd_get_terminal(manager, 0, -1)) :
		( (val < 0) ? CBdd(mtbdd_get_terminal(manager, -val, 0)) : CBdd(mtbdd_get_terminal(manager, 0, val)) );
#endif
}
#else
inline CBdd CBdd::EncodeArith(double val)
{
	MANAGER_CHECK(_T("CBdd::EncodeArith()"));
	/*if(val==1)
		return CBdd(mtbdd_get_terminal(manager, 0, -1));*/
	
	/*ULONGLONG *bits = reinterpret_cast<ULONGLONG*>(&val);
	unsigned long val_u = (unsigned long) (*bits & 0xffffffffL); 
	unsigned long val_l = (unsigned long) (*bits >> 32); */
	//return CBdd(mtbdd_get_terminal(manager, val_u, val_l));
	
	union val_type {
		double d;
		unsigned long ul[2];
	} double_union;

	double_union.d = val;
	return CBdd(mtbdd_get_terminal(manager, double_union.ul[0], double_union.ul[1]));
}
#endif

inline CBdd CBdd::Mtbdd_Ite(const CBdd& b, const CBdd& m)
{
	MANAGER_CHECK(_T("CBdd::Mtbdd_Ite()"));
	return CBdd(mtbdd_ite(manager, b.core, m.core, bdd_zero(manager)));
}

inline CBdd CBdd::IfThenElse(const CBdd& _if, const CBdd& _then, const CBdd& _else)
{
	MANAGER_CHECK(_T("CBdd::IfThenElse()"));
	return CBdd(bdd_ite(manager, _if.core, _then.core, _else.core));
}

inline CBdd CBdd::Implies(const CBdd& l, const CBdd& r)
{
	MANAGER_CHECK(_T("CBdd::Implies()"));
  
	CBdd temp = bdd_not(manager, l.core);
	
	CBdd result = CBdd(bdd_or(manager, 
                     bdd_not(manager, l.core),
					 bdd_and(manager, l.core, r.core)));
	return result;
}

inline CBdd CBdd::Xor(const CBdd& l, const CBdd& r)
{
	MANAGER_CHECK(_T("CBdd::Xor()"));
	return CBdd(bdd_xor(manager, l.core, r.core));
}

inline CBdd::CBdd() : core(0){}

inline CBdd::CBdd(bdd c) : core(c)						{};
inline CBdd::CBdd(const CBdd& from) : core(from.core)	{};
inline CBdd::~CBdd()									{}
inline CBdd &CBdd::operator= (const CBdd& from)			{core = from.core;  return *this;}


inline CBdd CBdd::operator&& (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator&&()"));
	return CBdd( bdd_and(manager, core, arg.core) );
}


extern "C" extern DESERT_API int bdd_and_step_count;
extern "C" extern int mtbdd_sum_step_count;
extern "C" extern int mtbdd_sum_cache_hit;
extern "C" extern int mtbdd_threshold_step_count;


inline CBdd CBdd::operator|| (const CBdd& arg) const
{
	bdd_and_step_count = 0;
	MANAGER_CHECK(_T("CBdd::operator||()"));
	CBdd ret = CBdd( bdd_or(manager, core, arg.core) );
	return ret;
}

inline CBdd CBdd::operator! () const
{
	MANAGER_CHECK(_T("CBdd::operator!()"));
	return CBdd( bdd_not(manager, core) );
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::operator/ (const long value) const
#else
inline CBdd CBdd::operator/ (const double value) const
#endif
{
	MANAGER_CHECK(_T("CBdd::operator/()"));
 
	bdd temp = mtbdd_divby(manager, core, 0, value);
	//BDD_FAILED(_T("CBdd::operator/()"));
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
	return ret;
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::operator^ (const long value) const
#else
inline CBdd CBdd::operator^ (const double value) const
#endif
{
	MANAGER_CHECK(_T("CBdd::operator^()"));
 	bdd temp = mtbdd_rootby(manager, core, 0, value);
	//BDD_FAILED(_T("CBdd::operator^()"));
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
  
	return ret;
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::operator+ (const long value) const
#else
inline CBdd CBdd::operator+ (const double value) const
#endif
{
	MANAGER_CHECK(_T("CBdd::operator+()"));
	bdd temp = mtbdd_sumby(manager, core, 0, value);
//	BDD_FAILED(_T("CBdd::operator+()"));
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
  
	return ret;
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::operator- (const long value) const
#else
inline CBdd CBdd::operator- (const double value) const
#endif
{
	MANAGER_CHECK(_T("CBdd::operator-()"));
 	bdd temp = mtbdd_subby(manager, core, 0, value);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
  
	return ret;
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::operator* (const long value) const
#else
inline CBdd CBdd::operator* (const double value) const
#endif
{
	MANAGER_CHECK(_T("CBdd::operator*()"));
  
	bdd temp = mtbdd_mulby(manager, core, 0, value);
//	BDD_FAILED(_T("CBdd::operator*()"));
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
	return ret;
}

// Himanshu: Applies unary math function to the current Bdd terminal
inline CBdd CBdd::ApplyUnaryMathFunc(bdd_unary_math_function mathFunc) const
{
	MANAGER_CHECK(_T("CBdd::UnaryMathFunc()"));

	bdd temp = mtbdd_apply_unary_math_function(manager, core, mathFunc);
//	BDD_FAILED(_T("CBdd::UnaryMathFunc()"));
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
	CBdd ret = CBdd( temp );
  
	return ret;
}

inline CBdd CBdd::operator+ (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator+()"));
  
	bdd temp = mtbdd_sum(manager, core, arg.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
	return ret;
}

inline CBdd CBdd::operator- (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator-()"));
  
	bdd temp = mtbdd_sub(manager, core, arg.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
	//BDD_FAILED(_T("CBdd::operator-()"));

	CBdd ret = CBdd( temp );
	return ret;
}

inline CBdd CBdd::operator* (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator*()"));
 
	bdd temp = mtbdd_mul(manager, core, arg.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
//	BDD_FAILED(_T("CBdd::operator*()"));
 
	CBdd ret = CBdd( temp );
  
	return ret;
}

inline CBdd CBdd::operator/ (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator/()"));
 
	bdd temp = mtbdd_div(manager, core, arg.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
//	BDD_FAILED(_T("CBdd::operator/()"));
 
	CBdd ret = CBdd( temp );
  
	return ret;
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::Sum(const CBdd& f,  const CBdd& g, long limit)
#else
inline CBdd CBdd::Sum(const CBdd& f,  const CBdd& g, double limit)
#endif
{
	MANAGER_CHECK(_T("CBdd::Sum()"));

	static unsigned long cache_op_code_add = 1;
	static CMap<int, int, unsigned long, unsigned long> op_code_map;

	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	bdd temp = mtbdd_sum_thresh(manager, f.core, g.core, 0, limit, op_code );
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));	
	
	//BDD_FAILED(_T("CBdd::Sum()"));
	CBdd ret = CBdd( temp );
	return ret;

}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::Mul(const CBdd& f, const CBdd& g, long limit)
#else
inline CBdd CBdd::Mul(const CBdd& f, const CBdd& g, double limit)
#endif
{
	MANAGER_CHECK(_T("CBdd::Mul()"));

	static unsigned long cache_op_code_add = 1;
	static CMap<int, int, unsigned long, unsigned long> op_code_map;

	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	bdd temp = mtbdd_mul_thresh(manager, f.core, g.core, 0, limit, op_code);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

//	BDD_FAILED(_T("CBdd::Mul()"));
	CBdd ret = CBdd( temp );
  
	return ret;
}


inline CBdd CBdd::Max(const CBdd& f, const CBdd& g)
{
	MANAGER_CHECK(_T("CBdd::Max()"));
	bdd temp = mtbdd_max(manager, f.core, g.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
	return CBdd(temp);
//	return CBdd( mtbdd_max(manager, f.core, g.core) );
}


inline CBdd CBdd::Min(const CBdd& f, const CBdd& g)
{
	MANAGER_CHECK(_T("CBdd::Min()"));
	bdd temp = mtbdd_min(manager, f.core, g.core);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));
	return CBdd(temp);
//	return CBdd( mtbdd_min(manager, f.core, g.core) );
}


inline bool CBdd::operator!= (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator!=()"));
	return (core != arg.core);
}

inline bool CBdd::operator== (const CBdd& arg) const
{
	MANAGER_CHECK(_T("CBdd::operator==()"));
	return (core == arg.core);
}

inline CBdd::operator bdd()
{

  return core;
}

inline int CBdd::size()
{
	MANAGER_CHECK(_T("CBdd::size()"));
	return bdd_size( manager, core, 0 );
}

inline double CBdd::sat_fract()
{
	MANAGER_CHECK(_T("CBdd::sat_fract()"));
	return bdd_satisfying_fraction( manager, core );
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::threshold(int limit, bool eq)
#else
inline CBdd CBdd::threshold(double limit, bool eq)
#endif
{
	MANAGER_CHECK(_T("CBdd::threshold()"));

	static unsigned long cache_op_code_add = 1;
#ifndef DOUBLE_MTBDD
	static CMap<int, int, unsigned long, unsigned long> op_code_map;
#else
	static CMap<double, double, unsigned long, unsigned long> op_code_map;
#endif
	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	core = mtbdd_threshold( manager, core, 0, limit, op_code, eq?1:0);
	return CBdd(core);
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::low_threshold(int limit, bool eq)
#else
inline CBdd CBdd::low_threshold(double limit,bool eq)
#endif
{
	MANAGER_CHECK(_T("CBdd::low_threshold()"));
 	
	static unsigned long cache_op_code_add = 1;
#ifndef DOUBLE_MTBDD
	static CMap<int, int, unsigned long, unsigned long> op_code_map;
#else
	static CMap<double, double, unsigned long, unsigned long> op_code_map;
#endif

	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	core = mtbdd_low_threshold( manager, core, 0, limit, op_code, eq?1:0);
	
	return CBdd(core);
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::equals_to(int limit)
#else
inline CBdd CBdd::equals_to(double limit)
#endif
{
	MANAGER_CHECK(_T("CBdd::equals_to()"));

	static unsigned long cache_op_code_add = 1;
#ifndef DOUBLE_MTBDD
	static CMap<int, int, unsigned long, unsigned long> op_code_map;
#else
	static CMap<double, double, unsigned long, unsigned long> op_code_map;
#endif
   
	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	core = mtbdd_equals_to( manager, core, 0, limit, op_code);
	return CBdd(core);
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::not_equals_to(int limit)
#else
inline CBdd CBdd::not_equals_to(double limit)
#endif
{
	MANAGER_CHECK(_T("CBdd::not_equals_to()"));

	static unsigned long cache_op_code_add = 1;
#ifndef DOUBLE_MTBDD
	static CMap<int, int, unsigned long, unsigned long> op_code_map;
#else
	static CMap<double, double, unsigned long, unsigned long> op_code_map;
#endif
   
	unsigned long op_code;

	if (!op_code_map.Lookup(limit, op_code))
	{
		op_code = cache_op_code_add++;
		op_code_map[limit] = op_code;
	}

	core = mtbdd_not_equals_to( manager, core, 0, limit, op_code);
	return CBdd(core);
}

#ifndef DOUBLE_MTBDD
inline CBdd CBdd::devides_from(const long value)
#else
inline CBdd CBdd::devides_from(const double value)
#endif
{
	bdd temp = mtbdd_div_from(manager, core, 0, value);
	if(temp==(bdd)0)		
		throw new CDesertException(_T("Operation returned null."));

	CBdd ret = CBdd( temp );
	return ret;
}
#endif // #ifndef BDDMAIN_H
