// -*-C++-*-
// dynamic.cpp
// Implementation of dynamic classes
/*
CHANGE LOG

  CDynVariableProperties:
	Another constructor added which cclones from an existing CDYnVariableProperties

*/


#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"

#include "core/dynamic.h"
#include "core/manager.h"
#include "core/digraph.h"

#include "BackIface.h"




//-----CDynamicObj

inline CDynamicObj::CDynamicObj() :CCosmic()
{
	type = typeDynamic;
	ASSERT_EX( false, _T("CDynamicObj::CDynamicObj()"), _T("No core") );
};

inline CDynamicObj::CDynamicObj(CStaticObj *core)
	  : CCosmic(), core(core)
{
	type = typeDynamic;
}

inline CDynamicObj::~CDynamicObj() {};
inline const CString& CDynamicObj::GetName() const	{return core->GetName();};
inline CDynamicObj::operator const TCHAR *() const	{return (const TCHAR *)*core;};
inline CStaticObj *CDynamicObj::GetCore()			{return core;};



//--------------CDynElementContainer



inline CDynElementList* CDynElementContainer::GetElements(int)				{return &elements;};
inline CDynElementList& CDynElementContainer::GetElements()				{return elements;};
inline CDynElementList& CDynElementContainer::GetRootElements()				{return rootElements;};
inline CDynElementList* CDynElementContainer::GetRootElements(int)			{return &rootElements;};
inline CDynElement* CDynElementContainer::FindElement(long id)				{CDynElement *ret;  return Find(elements, ret, id) ? ret : 0;};
inline CDynElement* CDynElementContainer::FindElement(const TCHAR *name)		{CDynElement *ret;  return Find(elements, ret, name) ? ret : 0;};
inline CDynElement* CDynElementContainer::FindElement(const CElement *core)	{CDynElement *ret;  return Find(elements, ret, core) ? ret : 0;};
inline CDynElement*	CDynElementContainer::RemoveElement(CDynElement *e)		{Remove(rootElements, e);  return Remove(elements, e) ? e : 0;};
inline CDynElement* CDynElementContainer::RemoveElement(long id)				{CDynElement *e;  Remove(rootElements, e, id);  return Remove(elements, e, id) ? e : 0;};
inline CDynElement* CDynElementContainer::RemoveElement(const TCHAR *name)	{CDynElement *e;  Remove(rootElements, e, name);  return Remove(elements, e, name) ? e : 0;};
inline CDynElementContainer::~CDynElementContainer()							{Destroy(elements);};

inline CDynElement* CDynElementContainer::InsertElement(CDynElement *e)
{
  if (e->GetParent() == 0) Insert(rootElements, e);
  return Insert(elements, e) ? e : 0;
}

void CDynElementContainer::Dump(FILE *f)
{
	POSITION pos = rootElements.GetHeadPosition();
	while(pos) rootElements.GetNext(pos)->Dump(f);
}

void CDynElementContainer::Clone()
{
	// create root elements
	CElementList& list = ((CSpace *)GetCore())->GetRootElements();

	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CDynElement *dup = new CDynElement(list.GetNext(pos), this, 0);
		elements.AddTail(dup);
		rootElements.AddTail(dup);
	}
	// invoke clone on root elements
	pos = rootElements.GetHeadPosition();
	while(pos) rootElements.GetNext(pos)->Clone();
}

void CDynElementContainer::Clone(CDynElementContainer *from)
{
  // create root elements
  CDynElementList& list = from->GetRootElements();
  POSITION pos = list.GetHeadPosition();
  while(pos)
  {
    CDynElement *frm = list.GetNext(pos);
    if (!frm->IsAlive()) continue;
    CElement *org = (CElement *)frm->GetCore();
    CDynElement *dup = new CDynElement(org, this, 0);
    elements.AddTail(dup);
    rootElements.AddTail(dup);
    dup->Clone(frm);
  }
}




inline CDynElement::CDynElement(CElement *c, CDynElementContainer *cont, CDynElement *par)
: CDynamicObj(c),  parent(par),  alive(true)
{
	//pointers to containers
	CDynSpace * s = 0;
	CDynCustomDomain * d  = 0;

	//decision
	if (cont->GetCore()->GetType()== typeSpace)
	{
		//cont is a CDynSpace
		s = (CDynSpace*)cont;
		ASSERT(s->GetType() == typeDynSpace);
	}

	if (cont->GetCore()->GetType() == typeCustomDomain)
	{
		//cont is a CDynCustomDomain
		d = (CDynCustomDomain*)cont;
		ASSERT(d->GetType() == typeDynCustomDomain);
	}
	//assertion
	ASSERT( s || d);

	//space or domain belonging
	space = s; domain = d;

	//others
	type = typeDynElement;
	funcType = funcNofunc;
/*	latencyDone = false;
	areaDone = false;
	powerDone = false;
	*/
};



//---------DynSpace


inline CDynSpace * CDynSpace::Make(CSpace *c)				{return new CDynSpace(c);};
inline CDynSpace * CDynSpace::Make(CDynSpace *f)				{return Make((CSpace *)f->GetCore());};
inline CDynSpace::CDynSpace(CSpace *c)	:CDynamicObj(c),alive(true)	{	type = typeDynSpace;};
inline CDynSpace::~CDynSpace()										{};
inline void CDynSpace::SetEncodingLen(int l)						{encodingLen = l;};
inline CBdd CDynSpace::GetEncoding()								{return encoding;};
inline bool CDynSpace::IsAlive() const								{return alive;};
inline CStaticObj * CDynSpace::GetCore()							{return this->core;};


inline CType CDynSpace::GetType()					{return CDynamicObj::GetType();				};
inline  CDynSpace::operator long() const			{return CDynamicObj::operator long();		};
inline  CDynSpace::operator const TCHAR * () const	{return CDynamicObj::operator const TCHAR *();};


double CDynSpace::ComputeSize()
{
  double ret=1;

  POSITION pos = rootElements.GetHeadPosition();
  while(pos) ret *= rootElements.GetNext(pos)->ComputeSize();

  return ret;
}




int CDynSpace::FindEncodingLen()
{
  int ret = 0;
  int encLen = log2(rootElements.GetCount());
  POSITION pos = rootElements.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = rootElements.GetNext(pos);
    e->SetEncodingLen(encLen);
    int r = e->FindEncodingLen();
    ret = max(ret, r);
  }
  ret += encLen;

  return ret;
}

int CDynSpace::SetEncodingValue(int enc, int sb)
{
  encodingVal = enc;
  startVar = sb;
  int ret=encodingLen;
  int count=0;
  POSITION pos = rootElements.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = rootElements.GetNext(pos);
    int r = e->SetEncodingValue(count++, startVar+encodingLen);
    ret = max(r, ret);
  }

  return ret;
}

CBdd CDynSpace::MaptoBdd(CBdd& parentEnc, int props, ...)
{
  va_list ap;
  va_start(ap, props);

  TRACE( _T("CDynSpace::MaptoBdd : space %s : encval %d : startvar %d : encLen %d  \n"),
         GetName(), encodingVal, startVar, encodingLen );

  CBdd baseEnc = CBdd::Encode(encodingVal, startVar, encodingLen);
  encoding = baseEnc && parentEnc;
  CBdd ret = baseEnc;

  if (rootElements.GetCount() > 0)
  {
    CBdd childEnc = CBdd::Zero();
    POSITION pos = rootElements.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = rootElements.GetNext(pos);
      childEnc = e->MaptoBdd_0(encoding, props, ap) || childEnc;
    }
    ret = ret && childEnc;
  }

  va_end(ap);
  return ret;
}

bool CDynSpace::Restrict(CBdd& res)
{
  alive = CBdd::Zero() != (res && encoding);
  POSITION pos = rootElements.GetHeadPosition();
  while(pos) rootElements.GetNext(pos)->Restrict(res);

  return alive;
}


int CDynSpace::FindNonOrthElementCount()
{
  int ret = 0;
  POSITION pos = rootElements.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = rootElements.GetNext(pos);
    int r = e->FindNonOrthElementCount();
    ret = max(ret, r);
  }

  return ret;
}

int CDynSpace::FindPropertyEncodingLen(const TCHAR *n)
{
  int ret = 0;
  POSITION pos = rootElements.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = rootElements.GetNext(pos);
    int r = e->FindPropertyEncodingLen(n);
    ret = max(ret, r);
  }

  return ret;
}

int CDynSpace::SetPropertyEncodingValue(const TCHAR *n, int sv)
{
  int ret=0;
  startPropertyVec[n] = (void *)sv;
  POSITION pos = rootElements.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = rootElements.GetNext(pos);
    int r = e->SetPropertyEncodingValue(n, sv);
    ret = max(r, ret);
  }
  numPropertyVecs[n] = (void *)ret;

  return ret;
}



//---CDynProperty

inline CDynProperty::CDynProperty(CProperty *c, CDynDomain *d, CDynElement *o)
	: CDynamicObj(c), domain(d), owner(o)				{type = typeDynProperty;};
inline CDynProperty::CDynProperty(CProperty *c, CDynElement *o)
	: CDynamicObj(c), owner(o)							{type = typeDynProperty;};
//CDynProperty *CDynProperty::Make(CProperty *core, CDynDomain *domain, CDynElement *owner);
inline CDynProperty::~CDynProperty()								{};
inline CDynDomain* CDynProperty::GetDomain()						{return domain;};
inline CDynElement* CDynProperty::GetOwner()						{return owner;};
inline bool CDynProperty::GetValue(int& r) const					{r = 0;return false;};
inline bool CDynProperty::GetValue(CDynElement*& r) const			{r = 0;return false;};
inline int CDynProperty::SetEncodingValue(int sb, int encLen)		{return 0;};
inline int CDynProperty::SetEncodingValue(int sv)					{return 0;}; // for constant properties
inline CBdd CDynProperty::MaptoBdd(CBdd& parentEncoding)			{return parentEncoding;};
inline CBdd CDynProperty::MapValuetoBdd(CBdd& parentEncoding)		{return parentEncoding;};


inline double CDynProperty::ComputeSize()							{return 1;};
inline int CDynProperty::GetVector()								{return -1;};
inline void CDynProperty::Restrict(CBdd& res)						{};

inline CDynProperty * CDynProperty::RemoveProperty(CDynProperty *e)		{return Remove(properties, e) ? e : 0;};
inline CDynProperty * CDynProperty::InsertProperty(CDynProperty *e)		{return Insert(properties, e) ? e : 0;};
inline CDynPropertyList& CDynProperty::GetProperties()					{return properties;};

CDynProperty *CDynProperty::Make(CProperty *org, CDynDomain *dmn, CDynElement *own)
{
  CDynProperty *dup=0;
  switch((CType)*org)
  {
  case typeConstantProperty:
    dup = new CDynConstantProperty((CConstantProperty *)org, dmn, own);
    break;
  case typeVariableProperty:
	  if(dmn)
		dup = new CDynVariableProperty((CVariableProperty *)org, dmn, own);
	  else
		dup = new CDynVariableProperty((CVariableProperty *)org, own);
    break;
  default:
    //ASSERT_EX( false, _T("CDynProperty::Make"), _T("bad property type") );
	 throw new CDesertException(_T("CDynProperty::Make(): bad property type"));
  }
  return dup;
}

inline int CDynProperty::FindEncodingLen(){  return domain ? log2(domain->GetSize()) : 0;}
inline int CDynProperty::FindDomainSize(){  return domain ? domain->GetSize() : 0;}



//----CDynConstantProperty
inline CDynConstantProperty::CDynConstantProperty(CConstantProperty *c, CDynDomain* d, CDynElement *o): CDynProperty(c, d, o)					{type = typeDynConstantProperty;}
inline CDynConstantProperty::~CDynConstantProperty()					{};
inline bool CDynConstantProperty::GetValue(int& r) const				{return ((CConstantProperty *)core)->GetValue(r);};
inline int CDynConstantProperty::GetVector()							{return vector;};
inline int CDynConstantProperty::SetEncodingValue(int vec)				{vector = vec;return 1;};
inline void CDynConstantProperty::Dump(FILE *f)							{};

CBdd CDynConstantProperty::MaptoBdd(CBdd& parentEnc)
{
  // we probably should store this? -- doesn't matter
  int val=0;
  CConstantProperty *core = (CConstantProperty *)(this->core);
  core->GetValue(val);
  return // CBdd::EncodeProperty((const TCHAR *)*core, vector, val);
  CBdd::EncodeArith(val);
}


//-----CDynVariableProperty


inline CDynVariableProperty::~CDynVariableProperty(){  range.RemoveAll();}

inline int CDynVariableProperty::SetEncodingValue(int sv){  startVar = sv;  return 1;}

inline CDynElementList& CDynVariableProperty::GetRange(){  return range;}

#ifndef DOUBLE_MTBDD
inline CIntegerList& CDynVariableProperty::GetNaturalRange()
#else
inline CDoubleList& CDynVariableProperty::GetNaturalRange()
#endif
{
	//return  ((CVariableProperty *)core)->GetNaturalRange();
	return naturalRange;
}


inline bool CDynVariableProperty::IsParametric(){ return isParametric;}



void CDynVariableProperty::Restrict(CBdd& res)
{
	int  i = 0;
	CList<POSITION, POSITION> erase_list;
	CBdd zero = CBdd::Zero();

	if (type == typeDynCustomDomain)
	{

		if (range.GetCount() > 1)
		{
			POSITION pos = range.GetHeadPosition();
			while (pos)
			{

				POSITION prev_pos = pos;

				CDynElement * rangeElement = range.GetNext(pos);
				CBdd r = CBdd::Encode(i++, startVar, encodingLen);
				if (zero == (r && res)) erase_list.AddTail(prev_pos);
			}

			//remove what is pruned
			pos = erase_list.GetHeadPosition();
			while (pos) range.RemoveAt(erase_list.GetNext(pos));
		}

	}
	else
	{
		if (naturalRange.GetCount() > 1)
		{
			POSITION pos = naturalRange.GetHeadPosition();
			while (pos)
			{
				POSITION prev_pos = pos;		//store position

				int rangeVal = naturalRange.GetNext(pos);
				CBdd r = CBdd::Mtbdd_Ite( CBdd::Encode(i++, startVar, encodingLen), CBdd::EncodeArith(rangeVal));
				if (zero == (r && res)) erase_list.AddTail(prev_pos);
			}

			//remove what is pruned
			pos = erase_list.GetHeadPosition();
			while (pos) naturalRange.RemoveAt(erase_list.GetNext(pos));
		}
	}
}


void CDynVariableProperty::Dump(FILE *f)
{
	if (f && domain->GetType() == typeDynCustomDomain)
	{
		_ftprintf(f, _T("Variableproperty on Custom Domain: %s at address %p core address: %p\n"), static_cast<const TCHAR*>(GetName()), this, core);
		POSITION pos = range.GetHeadPosition();
		while(pos)
		{
			CDynElement * p = range.GetNext(pos);
			_ftprintf(f,_T("%p(%p):"), p, p->GetCore());
		}
		_ftprintf(f, _T("\n"));
	}

	if (f && domain->GetType() == typeDynNaturalDomain)
	{
		_ftprintf(f, _T("Variableproperty on Natural Domain: %s at address %p core address: %p\n"), static_cast<const TCHAR*>(GetName()), this, core);
#ifndef DOUBLE_MTBDD
		CIntegerList &il = GetNaturalRange();
		POSITION pos = il.GetHeadPosition();
		while(pos)
		{
			int i  = il.GetNext(pos);
			_ftprintf(f,_T("%d:"), i);
		}
#else
		CDoubleList &il = GetNaturalRange();
		POSITION pos = il.GetHeadPosition();
		while(pos)
		{
			int i  = il.GetNext(pos);
			_ftprintf(f,_T("%i:"), i);
		}
#endif
		_ftprintf(f, _T("\n"));
	}


}

CDynVariableProperty::
CDynVariableProperty(CVariableProperty *c, CDynDomain* d, CDynElement *o)
  : CDynProperty(c, d, o)
{
	type = typeDynVariableProperty;
	isParametric = false;

	if (d->GetType() == typeDynCustomDomain)
	{
		CElementList& sr = c->GetRange();
		POSITION pos = sr.GetHeadPosition();
		while(pos)
		{
			CElement *sre = sr.GetNext(pos);
			CDynElement *dre = d->FindElement(sre);

			if (!dre)
			 throw new CDesertException(_T("CDynVariableProperty::CDynVariableProperty(): range element not found in dyn domain"));

			//ASSERT_EX(dre, _T("CDynVariableProperty::CDynVariableProperty"), _T("range element not found in dyn domain"));

		 range.AddTail(dre);
		}
	}
	else
	{
#ifndef DOUBLE_MTBDD
		CIntegerList& nr = c->GetNaturalRange();
#else
		CDoubleList& nr = c->GetNaturalRange();
#endif
		POSITION pos = nr.GetHeadPosition();
		while (pos)
			naturalRange.AddTail(nr.GetNext(pos));
	}
}

CDynVariableProperty::
CDynVariableProperty(CVariableProperty *c, CDynElement *o)
	: CDynProperty(c,o)
{
	type = typeDynVariableProperty;
	isParametric = true;
}

int
CDynVariableProperty::
FindEncodingLen()
{
  // if not assigned any resource, then use the entire domain (base class findencodinglen)
  // else only the resources included in the range

	if (domain->GetType() == typeDynCustomDomain)
	{
	  if (range.IsEmpty())
		  return CDynProperty::FindEncodingLen();
	  else
	  	  return range.GetCount() > 1 ? log2(range.GetCount()) : 0;
	}
	else
	{
#ifndef DOUBLE_MTBDD
	  CIntegerList &nRange = GetNaturalRange();
#else
	  CDoubleList &nRange = GetNaturalRange();
#endif
	  if (nRange.IsEmpty())
		  return CDynProperty::FindEncodingLen();
	  else
		  return nRange.GetCount() > 1 ? log2(nRange.GetCount()) : 0 ;
	}
}

int
CDynVariableProperty::
SetEncodingValue(int sb, int encLen)
{
  startVar = sb;
  encodingLen = encLen;
  return encLen;
}

CBdd CDynVariableProperty:: MapValuetoBdd(CBdd& parentEnc)
{
	//ASSERT(((CVariableProperty *)core)->IsNatural() );
	if(!((CVariableProperty *)core)->IsNatural())
		throw new CDesertException(_T("CDynVariableProperty::MapValuetoBdd(): The value is not natural number"));

	CBdd ret;
#ifndef DOUBLE_MTBDD
		int propVal;
	  CIntegerList &vals = GetNaturalRange();
#else
	double propVal;
	  CDoubleList &vals = GetNaturalRange();
#endif

    switch( vals.GetCount() )
	{
		case 1:
		  propVal = vals.GetHead();
		  ret = CBdd::EncodeArith(propVal);
		  break;
		case 0:
		  // FIXME can we use NaN here
		  ret = CBdd::EncodeArith(0);
		  break;
		default:
		{
		  ret = CBdd::Zero();
		  POSITION pos = vals.GetHeadPosition();
		  int n=0;
		  while(pos)
		  {
			propVal = vals.GetNext(pos);
			// conditionalize the property value with the property encoding
			CBdd r = CBdd::Mtbdd_Ite( CBdd::Encode(n++, startVar, encodingLen), CBdd::EncodeArith(propVal));
			ret = ret || r;
		  }

		}
    } // eo switch





    encoding = ret && parentEnc;
	return ret;



};


double CDynVariableProperty::ComputeSize()
{
	if ( ((CVariableProperty *)core)->IsNatural() )
	{
#ifndef DOUBLE_MTBDD
	  CIntegerList &nRange = GetNaturalRange();
#else
	  CDoubleList &nRange = GetNaturalRange();
#endif
		return nRange.IsEmpty() ? domain->GetSize() : nRange.GetCount();
	}
	else
		return range.IsEmpty() ? domain->GetSize() : range.GetCount();
}

void CDynVariableProperty::BuildConfiguration(BackIfaceFunctions::DBConfiguration *cfg, CBdd& enc, long elementID)
{

  if (CBdd::Zero() == (encoding && enc)) return;
  long assignedTo = -1;


	if (domain->GetType() == typeDynCustomDomain)
	{
		//assignedID will be the element ID of the
		// assigned element (custom member)
		switch(range.GetCount() )
		{
			case 0:
			{
				//it means that it's assigned to the all domain
				CDynCustomDomain *d = (CDynCustomDomain *)domain;
				assignedTo = d->GetExternal(enc, startVar, encodingLen);
				//returns element ID!!!
				break;
			}

		case 1:
			//since there is only one element, for this variable,
			//this is not really a var property,
			//so we do not store this in the configuration
			//and we do not verify either

			/*
				the range of a Var Prob. might be pruned, so check on static version
				if that's one too, then do not generate the only one value in the back interface

			*/

			if (((CVariableProperty*)core)->GetRange().GetCount() == 1) return;

			//otherwise, generate
			assignedTo = *(range.GetHead()->GetCore()); //implicit cast to long
		break;

		default:
		{
			POSITION pos = range.GetHeadPosition();
			int i = 0;
			CBdd zero = CBdd::Zero();
			while(pos)
			{
				CDynElement *el = range.GetNext(pos);
				CBdd r = CBdd::Encode(i++, startVar, encodingLen);
				if (zero == (r && enc)) continue;
				//assignedTo = el->GetExternal();
				assignedTo = *(el->GetCore());//implicit cast to long
				break;
			}
		}//eo case default

	  } // end switch range.GetCount()
		ASSERT(assignedTo != -1);

		CAssignment * ass;

		//we should insert all the elements in the list instead of just one
		CAssignmentList * ass_l = NULL;
		//ASSERT does not evaluate in release version!!!
		if (!((CVariableProperty*)core)->range_lmap.Lookup(assignedTo, ass_l))
			throw new CDesertException(_T("CDynVariableProperty::BuildConfiguration(): List lookup failed!"));


		POSITION mPos = ass_l->GetHeadPosition();

		while (mPos)
		{
			ass = ass_l->GetNext(mPos);
			ASSERT(ass);
			cfg->assignments.AddTail((long)(*ass));
		}


	}//eo if (domain->GetType() == typeDynCustomDomain)
	else
	{
		//assignedTo will be a value!!!
		ASSERT(domain->GetType() == typeDynNaturalDomain);

#ifndef DOUBLE_MTBDD
	  CIntegerList &nRange = GetNaturalRange();
#else
	  CDoubleList &nRange = GetNaturalRange();
#endif
		switch (nRange.GetCount())
		{
		case 0:
		/*	{

				CDynNaturalDomain *d = (CDynNaturalDomain *) domain;
				assignedTo = d->GetValue(enc, startVar, encodingLen);
				break;
			}*/

		case 1:
			//see comment in previous switch, case 1

			if (((CVariableProperty*)core)->GetRange().GetCount() == 1) return;

			if(naturalRange.GetCount()>0)
				assignedTo = naturalRange.GetHead();

			//break;
		default:
			{
				POSITION pos = nRange.GetHeadPosition();
				int i = 0;
				CBdd zero = CBdd::Zero();
				while (pos)
				{
#ifndef DOUBLE_MTBDD
					int value = nRange.GetNext(pos);
#else
					double value = nRange.GetNext(pos);
#endif
					//CBdd r = CBdd::Mtbdd_Ite( CBdd::Encode(i++, startVar, encodingLen), CBdd::EncodeArith(value));

					CBdd r = CBdd::Encode(i++, startVar, encodingLen);
				//	TRACE(_T("%s:testing if %d is ok at position %d\n"), value, i-1, core->GetName());
				//	TRACE(_T("value in range[%d]: %d\n"), i, value);
					if (zero == (r && enc)) continue;
				//	TRACE(_T("%s: %d is ok at encoding: %d, in configuration number:%d\n"), (LPCTSTR)core->GetName(), value, i-1, cfg->id);

					//value is present in the configuration
					assignedTo = value;
					CAssignment * ass;

					//we should insert all the elements in the list instead of just one
					CAssignmentList * ass_l = NULL;
					if (!((CVariableProperty*)core)->natural_range_lmap.Lookup(value, ass_l))
						throw new CDesertException(_T("CDynVariableProperty::BuildConfiguration(): List lookup failed!"));

					//ASSERT(ass_l);

					POSITION mPos = ass_l->GetHeadPosition();
					while (mPos)
					{
						ass = ass_l->GetNext(mPos);
						//ASSERT(ass);
						if (!ass)
							throw new CDesertException(_T("CDynVariableProperty::BuildConfiguration(): Got NULL element if the assignent list!"));
						cfg->assignments.AddTail((long)(*ass));
					}

				}//eo while (pos)
			}//eo case default

		}//eo switch (naturalRange.GetCount())


	}//eo else if (domain->GetType() == typeDynCustomDomain)

}

CBdd
CDynVariableProperty::
GetEncoding(CDynElement *rangeElement)
{
  CBdd ret = CBdd::One();

  CDynElement * debug;
//  CString debugname, rangename;
  switch(range.GetCount())
  {
  case 0:
  {                                     // if there are no elements in the range
                                        // then go to the domain
    CDynCustomDomain *d = (CDynCustomDomain *)domain;
    ret = d->GetEncoding(rangeElement, startVar, encodingLen);
    break;
  }

  case 1:

	  debug = range.GetHead();
//	  debugname = debug->GetName();
//	  rangename = rangeElement->GetName();
	  TRACE(_T("rangeElement: %s(%x)\n"), rangeElement->GetName(), rangeElement);
	  TRACE(_T("debug: %s(%x)\n"), debug->GetName(), debug);
    if (rangeElement != range.GetHead())
   //   Error(_T("CDynVariableProperty::GetEncoding"), _T("this node is already assigned to a different resource"));
   ret = CBdd::Zero();
    else
      Warning(_T("CDynVariableProperty::GetEncoding"), _T("assignment constraint is redundant - node already assigned"));
    break;

  default:
  {
    POSITION pos = range.GetHeadPosition();
    int i = 0;
    while(pos)
    {
      CDynElement *el = range.GetNext(pos);
      if (rangeElement == el) break;
      i++;
    }
    if (i>=range.GetCount())
      Error(_T("CDynVariableProperty::GetEncoding"), _T("invalid constraint - this resource can not be assigned to this resource"));
    else
      ret = CBdd::Encode(i, startVar, encodingLen);
  }
  } // end switch

  return ret;
}






//-------------------CDynDomain


inline CDynDomain* CDynDomain::Make(CDynDomain *f)
{
	return f->IsAlive() ? Make((CDomain *)f->GetCore()) : 0;
};

inline CDynDomain::CDynDomain(CDomain *core): CDynamicObj(core)
{
	type = typeDynDomain;
};

inline CDynDomain::~CDynDomain()								{};


inline int CDynDomain::GetSize() const									{Warning( _T("CDynDomain::GetSize"), _T("shoud not be here") );  return 0;}
inline CDynElement *CDynDomain::FindElement(long id)					{return 0;};
inline CDynElement *CDynDomain::FindElement(const TCHAR *name)			{return 0;};
inline CDynElement *CDynDomain::FindElement(const CElement *core)		{return 0;};
inline CDynElement *CDynDomain::RemoveElement(CDynElement *elem)		{return 0;};
inline CDynElement *CDynDomain::RemoveElement(long id)					{return 0;};
inline CDynElement *CDynDomain::RemoveElement(const TCHAR *name)			{return 0;};
inline CDynElement *CDynDomain::InsertElement(CDynElement *elem)		{return 0;};

inline bool CDynDomain::IsAlive() const									{Todo(_T("CDynDomain::IsAlive"), _T("who me??"));  return true;};
inline int CDynDomain::IndexElement(CDynElement *elem)					{return -1;};


CDynDomain * CDynDomain::Make(CDomain *org)
{
  CDynDomain *dup=0;
  switch((CType)*org)
  {
  case typeNaturalDomain:
    dup = new CDynNaturalDomain((CNaturalDomain *)org);
    break;
  case typeCustomDomain:
    dup = new CDynCustomDomain((CCustomDomain *)org);
    break;
  default:
    //ASSERT_EX( false, _T("CDynDomain::Make"), _T("bad property type") );
	  throw new CDesertException(_T("CDynDomain::Make(): bad property type"));
  }
  return dup;
}



//------------------CDynNaturalDomain

inline CDynNaturalDomain::CDynNaturalDomain(CNaturalDomain *c) :CDynDomain(c)
{
	type = typeDynNaturalDomain;  maxVal = c->GetMax();  minVal = c->GetMin();
}

inline CDynNaturalDomain::CDynNaturalDomain(CNaturalDomain *c, int mx, int mn) :CDynDomain(c), maxVal(mx), minVal(mn)
{
	type = typeDynNaturalDomain;
};

inline void CDynNaturalDomain::Clone()										{};
inline void CDynNaturalDomain::Dump(FILE *f)								{};
inline void CDynNaturalDomain::Clone(CDynDomain* from)						{};
inline CDynNaturalDomain::~CDynNaturalDomain()								{};
inline int CDynNaturalDomain::GetSize() const								{return maxVal-minVal;};

long CDynNaturalDomain::GetValue(CBdd& enc, int sv, int encLen)
{
	int i = 0;
	int val = minVal;

	CBdd zero = CBdd::Zero();
	while(val < maxVal)
	{
		CBdd r = CBdd::Mtbdd_Ite( CBdd::Encode(i++, sv, encLen), CBdd::EncodeArith(val));
		if (zero == (r && enc)){val++; continue;}

		return val;
	}
  return -1;
}



//-----------------CDynCustomDomain


CDynCustomDomain::CDynCustomDomain(CCustomDomain *c):CDynDomain(c)
{
	type = typeDynCustomDomain;
};

inline CDynCustomDomain::~CDynCustomDomain(){};

inline int CDynCustomDomain::GetSize() const
{
	// domains can be hierarchical -- so we need to recurse
	// over elements and find the total count
	// this does not deals with alternatives in domains
	int count=0;
	POSITION pos = elements.GetHeadPosition();
	while(pos) count += elements.GetNext(pos)->GetCount();
	return count;
}

inline CDynElement * CDynCustomDomain::FindElement(long i)				{return CDynElementContainer::FindElement(i);}
inline CDynElement * CDynCustomDomain::FindElement(const TCHAR *n)		{return CDynElementContainer::FindElement(n);}
inline CDynElement * CDynCustomDomain::FindElement(const CElement *c)	{return CDynElementContainer::FindElement(c);}
inline CDynElement * CDynCustomDomain::RemoveElement(CDynElement *e)	{return CDynElementContainer::RemoveElement(e);}
inline CDynElement * CDynCustomDomain::RemoveElement(long i)			{return CDynElementContainer::RemoveElement(i);}
inline CDynElement * CDynCustomDomain::RemoveElement(const TCHAR *n)		{return CDynElementContainer::RemoveElement(n);}
inline CDynElement * CDynCustomDomain::InsertElement(CDynElement *e)	{return CDynElementContainer::InsertElement(e);}
inline int CDynCustomDomain::IndexElement(CDynElement *elem)			{return Index(elements, elem);};
inline CStaticObj * CDynCustomDomain::GetCore()							{return this->core;};
inline CType CDynCustomDomain::GetType()								{return CDynamicObj::GetType();				};
inline CDynCustomDomain::operator long() const							{return CDynamicObj::operator long();		};
inline CDynCustomDomain::operator const TCHAR * () const					{return CDynamicObj::operator const TCHAR *();};
inline void CDynCustomDomain::Dump(FILE *f)								{CDynElementContainer::Dump(f);};

void CDynCustomDomain::Clone()
{
	CDynElementContainer::Clone();
}

void CDynCustomDomain::Clone(CDynDomain *from)
{
	//ASSERT(from->GetCore()->GetType() == typeCustomDomain);
	if (from->GetCore()->GetType() != typeCustomDomain)
		throw new CDesertException(_T("CDynCustomDomain::Clone(CDynDomain *from): Expected CustomDomain here!"));
	CDynElementContainer::Clone( (CDynCustomDomain*) from);
}

long CDynCustomDomain::GetExternal(CBdd& enc, int sv, int encLen)
{
  POSITION pos = elements.GetHeadPosition();
  int i = 0;
  CBdd zero = CBdd::Zero();
  while(pos)
  {
    CDynElement *el = elements.GetNext(pos);
    CBdd r = CBdd::Encode(i++, sv, encLen);
    if (zero == (r && enc)) continue;
    //return el->GetExternal();

	return *(el->GetCore());
  }
  return -1;
}




CBdd CDynCustomDomain::GetEncoding(CDynElement *rangeElement, int sv, int encLen)
{
  CBdd ret = CBdd::One();
  int i = 0;
  int n=elements.GetCount();
  POSITION pos = elements.GetHeadPosition();
  while(pos)
  {
    CDynElement *el = elements.GetNext(pos);
    if ( !el->FindRecurse(rangeElement, i) ) continue;
    i--;                                // we want to decrement one because findrecurse
                                        // gives 1-based nums
    ret = CBdd::Encode(i, sv, encLen);
    return ret;
  }
  //ASSERT_EX(false, _T("CDynVariableProperty::GetEncoding"), _T("resource not found in the domain"));

  throw new CDesertException(_T("CDynVariableProperty::GetEncoding(): resource not found in the domain"));
  return CBdd::One();
}

//------------CDynCOnstraintSet



inline CDynConstraintSet::CDynConstraintSet(CConstraintSet *c)  :CDynamicObj(c), alive(true)
{
	type =	typeDynConstraintSet;
};


inline CDynConstraintSet* CDynConstraintSet::Make(CConstraintSet *c)			{return new CDynConstraintSet(c);};


inline CDynConstraintSet* CDynConstraintSet::Make(CDynConstraintSet *f)		{return f->IsAlive() ? Make((CConstraintSet *)f->GetCore()) : 0;};


inline CDynConstraintSet::~CDynConstraintSet()									{Destroy(constraints);};
inline CDynConstraintList& CDynConstraintSet::GetConstraints()					{return constraints;}
inline CDynConstraint * CDynConstraintSet::FindConstraint(long i)				{CDynConstraint *ret;  return Find(constraints, ret, i) ? ret : 0;}
inline CDynConstraint * CDynConstraintSet::FindConstraint(const TCHAR *n)		{CDynConstraint *ret;  return Find(constraints, ret, n) ? ret : 0;}
inline CDynConstraint * CDynConstraintSet::FindConstraint(const CConstraint *c)	{CDynConstraint *ret;  return Find(constraints, ret, c) ? ret : 0;};
inline CDynConstraint * CDynConstraintSet::RemoveConstraint(CDynConstraint *e)	{return Remove(constraints, e) ? e : 0;}
inline CDynConstraint * CDynConstraintSet::RemoveConstraint(long i)				{CDynConstraint *e;  return Remove(constraints, e, i) ? e : 0;}
inline CDynConstraint * CDynConstraintSet::RemoveConstraint(const TCHAR *n)		{CDynConstraint *e;  return Remove(constraints, e, n) ? e : 0;};
inline CDynConstraint * CDynConstraintSet::InsertConstraint(CDynConstraint *e)	{return Insert(constraints, e) ? e : 0;}

inline void CDynConstraintSet::RemoveAll()										{constraints.RemoveAll();};
inline bool CDynConstraintSet ::IsEmpty() const									{return constraints.IsEmpty() ? true : false;};
inline bool CDynConstraintSet::IsAlive() const									{return alive;};


void CDynConstraintSet::Clone()
{
 // ASSERT_EX( core, _T("CDynConstraintSet::Clone"), _T("core is NULL") );

	if (!core) throw new CDesertException(_T("CDynConstraintSet::Clone: core is NULL") );

  //ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );
	DYN_MANAGER_CHECK(_T("CDynConstraintSet::Clone"));
  CConstraintList& list = ((CConstraintSet *)core)->GetConstraints();
  POSITION pos = list.GetHeadPosition();
  while(pos)
  {
    CConstraint *org = list.GetNext(pos);
    CDynElement *ctx = CManager::theInstance->FindElement(org->GetContext());
    if (!ctx) continue;
    CDynConstraint *dup = new CDynConstraint(org, this, ctx);
    constraints.AddTail(dup);
  }
}

void CDynConstraintSet::Clone(CDynConstraintSet *from)
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynConstraint::Clone()"));
  CDynConstraintList& list = from->GetConstraints();
  POSITION pos = list.GetHeadPosition();
  while(pos)
  {
    CDynConstraint *frm = list.GetNext(pos);
    if (!frm->IsAlive()) continue;
    CConstraint *org = (CConstraint *)frm->GetCore();
    CDynElement *ctx = CManager::theInstance->FindElement(org->GetContext());
    if (!ctx) continue;
    CDynConstraint *dup = new CDynConstraint(org, this, ctx);
    dup->wasApplied = frm->wasApplied;
	dup->valid = frm->valid;
    constraints.AddTail(dup);
  }
}

CBdd CDynConstraintSet::MaptoBdd()
{
	CBdd ret = CBdd::One();
	POSITION pos = constraints.GetHeadPosition();
	while(pos)
	{
		CDynConstraint *con = constraints.GetNext(pos);
		if(con->IsValid())
			ret = (con->MaptoBdd()) && ret;
	}
	return ret;
}

CDynElement *CDynConstraintSet::FindAffectedSpace(CDynSpaceList& spaces, bool& propertyDependence)
{
  propertyDependence = false;
  // prepare a dependency list
  CDynamicObjList dependency;
  POSITION pos = constraints.GetHeadPosition();
  while(pos)
  {
    CDynConstraint* cons = constraints.GetNext(pos);
    cons->FillDependency(dependency);
    propertyDependence = propertyDependence || cons->IsPropertyDependent();
  }

  int sc=0;
  pos = dependency.GetHeadPosition();
  while(pos)
  {
    CDynamicObj *obj = dependency.GetNext(pos);
    if (obj->GetType() == typeDynSpace)
    {
      spaces.AddTail((CDynSpace *)obj);
      sc++;
    }
    else
    {
      CDynSpace *spc = ((CDynElement *)obj)->GetSpace();
      spaces.AddTail(spc);
    }
  }

  if (!sc) return (CDynElement *)FindRoot(dependency);
  return 0;
}

bool CDynConstraintSet::Restrict(void)
{
  alive = false;
  POSITION pos = constraints.GetHeadPosition();
  while(pos) alive = constraints.GetNext(pos)->Restrict() || alive;
  return alive;
}



//-------------CDynConstraint
inline CDynConstraint::CDynConstraint(CConstraint *c, CDynConstraintSet *s, CDynElement* e)
	: CDynamicObj(c), set(s), context(e), alive(true), wasApplied(false)
{
	valid = c->IsValid();
	type = typeDynConstraint;
}

inline CDynConstraint::~CDynConstraint()				{};
inline CDynConstraintSet* CDynConstraint::GetSet()		{return set;};


inline CDynElement* CDynConstraint::GetContext()		{return context;};
inline CString& CDynConstraint::GetText()				{return ((CConstraint *)core)->GetText();};
inline ClConstraint* CDynConstraint::GetExpression()	{return ((CConstraint *)core)->GetExpression();};
inline bool CDynConstraint::IsAlive() const				{return alive;};
inline bool CDynConstraint::WasApplied() const			{return wasApplied && valid;};
inline void CDynConstraint::SetApplied()				{wasApplied = true;};
inline bool CDynConstraint::IsValid() const				{return valid;};
inline void CDynConstraint::SetInvalid()				{valid = false;};
inline bool CDynConstraint::IsPropertyDependent() const	{return ((CConstraint *)core)->IsPropertyDependent();};

CBdd CDynConstraint::MaptoBdd()
{

  ClConstraint *expression = ((CConstraint *)core)->GetExpression();
  ClContext clCtx = context;

  // Himanshu: Always return Bdd::One() when a constraint fails
  CBdd ret = CBdd::One();
  try {
	  ret = clCtx.Eval(expression);
  } catch (CDesertException *e) {
	  int debugHere = 1;
	  // Himanshu: Always return Bdd::One() when a constraint fails
	  this->SetInvalid();
	  CString err(_T("Constraint: <"));
	  err.Append(this->GetName());
	  err.Append(_T("> fails to be evaluated.\r"));
	  err.Append(e->GetErrorMessage());
	  e->Delete();

	  throw new CDesertException(err, this->GetName());
  }

  return ret;
}

void CDynConstraint::FillDependency(CDynamicObjList& dep)
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynConstraint::Restrict()"));

  CConstraint *core = (CConstraint *)(this->core);
  if (core->GetConstraintType() ==  typeUniSpace)
  {
    CCosmic *root = core->GetRoot();
    //ASSERT_EX( root, _T("CDynConstraint::FillDependency"), _T("root is null") );
	if (!root)
		throw new CDesertException(_T("CDynConstraint::FillDependency: root is Null"));
    if (root->GetType() == typeElement)
    {
      CDynElement *elem = CManager::theInstance->FindElement((CElement *)root);
      if (dep.Find(elem) || dep.Find(elem->GetSpace())) ;
      else dep.AddTail(elem);
    }
    else
    {
      CDynamicObj *spc = CManager::theInstance->FindSpace((CSpace *)root);
      if (!dep.Find(spc)) dep.AddTail(spc);
    }
  }
  else
  {
    CSpaceList& spaces = core->GetSpaces();
    POSITION pos = spaces.GetHeadPosition();
    while(pos)
    {
      CDynamicObj *spc = CManager::theInstance->FindSpace(spaces.GetNext(pos));
      if (!dep.Find(spc)) dep.AddTail(spc);
    }
    Todo(_T("CDynConstraint::FillDependency"), _T("what about domains in the dependency?"));
  }
}

bool CDynConstraint::Restrict()
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynConstraint::Restrict()"));

  CConstraint *core = (CConstraint *)(this->core);
  CCosmicList& dependency = core->GetDependency();
  CDynSpace *spc=0;
  CDynDomain *dmn=0;
  CDynElement *elm=0;

  POSITION pos = dependency.GetHeadPosition();
  alive=false;
  while(pos && !alive)
  {
    CCosmic *obj = dependency.GetNext(pos);
    switch(obj->GetType())
    {
    case typeSpace:
      spc = CManager::theInstance->FindSpace((CSpace *)obj);
      alive = spc && spc->IsAlive();
      break;

    case typeElement:
      elm = CManager::theInstance->FindElement((CElement *)obj);
      alive = elm && elm->IsAlive();
      break;

    case typeDomain:
      dmn = CManager::theInstance->FindDomain((CDomain *)obj);
      alive = dmn != 0;
      break;
    }
  }
  if (alive)
  {
    CDynElement *el = CManager::theInstance->FindElement(core->GetContext());
    alive = el->IsAlive();
  }

  return alive;
}


//-------------CDynRelation

inline CDynRelation::CDynRelation(CRelation *core, CDynElement *c, CDynElement *s, CDynElement *d)
: CDynamicObj(core), context(c), source(s), destination(d)	{};

inline CDynRelation::~CDynRelation()												{};
inline CDynElement* CDynRelation::GetContext()										{return context;};
inline CDynElement* CDynRelation::GetSource()										{return source;};
inline CDynElement* CDynRelation::GetDestination()									{return destination;};



//------CDynElement::

inline CDynElement::~CDynElement()								{Destroy(properties);  Destroy(relations);Destroy(formulas);};
inline CDynSpace * CDynElement::GetSpace()								{return space;};
inline CDynElement * CDynElement::GetParent()							{return parent;};
inline CDynElementList& CDynElement::GetChildren()						{return children;};
inline CDynRelationList& CDynElement::GetRelations()					{return relations;};
inline CDynPropertyList& CDynElement::GetProperties()					{return properties;};
inline CDynFormulaList& CDynElement::GetFormulas()					{return formulas;};
inline CDecomposition CDynElement::GetDecomposition() const				{return ((CElement *)core)->GetDecomposition();};
inline CDynElement::operator CDecomposition() const						{return (CDecomposition)*((CElement *)core);};
inline long CDynElement::GetExternal() const							{return ((CElement *)core)->GetExternal();};
inline CDynElement * CDynElement::FindChild(long id)						{CDynElement *e;  return Find(children, e, id) ? e : 0;};
inline CDynElement * CDynElement::FindChild(const TCHAR *n)				{CDynElement *e;  return Find(children, e, n) ? e : 0;};
inline CDynElement * CDynElement::FindChild(const CElement *c)			{CDynElement *e;  return Find(children, e, c) ? e : 0;};
inline CDynElement * CDynElement::RemoveChild(CDynElement *e)			{return Remove(children, e) ? e : 0;};
inline CDynElement * CDynElement::RemoveChild(long id)					{CDynElement *e;  return Remove(children, e, id) ? e : 0;};
inline CDynElement * CDynElement::RemoveChild(const TCHAR *n)				{CDynElement *e;  return Remove(children, e, n) ? e : 0;};
inline CDynElement * CDynElement::InsertChild(CDynElement *e)			{return Insert(children, e) ? e : 0;};
inline CDynProperty * CDynElement::FindProperty(long i)					{CDynProperty *e;  return Find(properties, e, id) ? e : 0;};
inline CDynProperty * CDynElement::FindProperty(const TCHAR *n)			{CDynProperty *e;  return Find(properties, e, n) ? e : 0;}
inline CDynFormula * CDynElement::FindFormula(const TCHAR *n)			{CDynFormula *e;  return Find(formulas, e, n) ? e : 0;}
inline CDynProperty * CDynElement::RemoveProperty(CDynProperty *e)		{return Remove(properties, e) ? e : 0;};
inline CDynProperty * CDynElement::RemoveProperty(long i)				{CDynProperty *e;  return Remove(properties, e, i) ? e : 0;};
inline CDynProperty * CDynElement::RemoveProperty(const TCHAR *n)			{CDynProperty *e;  return Remove(properties, e, n) ? e : 0;};
inline CDynProperty * CDynElement::InsertProperty(CDynProperty *e)		{return Insert(properties, e) ? e : 0;};
inline CDynRelation * CDynElement::FindRelation(long i)					{CDynRelation *e;  return Find(relations, e, i) ? e : 0;};
inline CDynRelation * CDynElement::RemoveRelation(CDynRelation *e)		{return Remove(relations, e) ? e : 0;}	;
inline CDynRelation * CDynElement::RemoveRelation(long i)				{CDynRelation *e;  return Remove(relations, e, i) ? e : 0;};
inline CDynRelation * CDynElement::InsertRelation(CDynRelation *e)		{return Insert(relations, e) ? e : 0;};
inline void CDynElement::SetFunction(FunctionType f)					{funcType = f;};
inline void CDynElement::SetFunction(FunctionType f, const CString& pn)	{funcType = f;  funcProperty = pn;};
inline void CDynElement::SetEncodingLen(int l)							{encodingLen = l;};
inline CBdd CDynElement::GetEncoding()									{return encoding;};
inline CBdd CDynElement::GetBaseEncoding()									{return baseEnc;};

inline bool CDynElement::IsAlive() const								{return alive;};
inline CCosmic* CDynElement::Parent()									{return parent;};
inline CCosmic* CDynElement::Space()									{return space;};

inline int CDynElement:: GetCount() const
{
	int count=1;                          // 1 for self
	POSITION pos = children.GetHeadPosition();
	while(pos) count += children.GetNext(pos)->GetCount();
	return count;
};

//Added by Feng
inline CString CDynElement::GetFuncProperty() const						{return funcProperty;};
//

double CDynElement::ComputeSize()
{
  double ret = 0;
  POSITION pos;
 // CDynProperty *property;

  switch((CDecomposition)*this)
  {
  case decompAnd:
    pos = children.GetHeadPosition();
    ret = 1;
    while(pos) ret *= children.GetNext(pos)->ComputeSize();
    break;

  case decompOr:
    pos = children.GetHeadPosition();
    ret = 0;
    while(pos) ret += children.GetNext(pos)->ComputeSize();
    if (ret==0)
    {
      Warning(_T("CDynElement::ComputeSize"), _T("OR decomposed element <%s> has no child"),
              (const TCHAR *)*this);
      ret=1;                            // empty template
    }
    break;

  case decompLeaf:
    // include the resource property in space size
/*    property = FindProperty(_T("resource"));
	  ret = property ? property->ComputeSize() : 1;
	*/
	  CDynVariableProperty * property;
	  POSITION mPos = properties.GetHeadPosition();
	  while (mPos)
	  {
		  property = (CDynVariableProperty *)properties.GetNext(mPos);
		 /* CDynPropertyList &plist = property->GetProperties();
		  if(plist.GetCount()>0) continue;*/
		  if(property->IsParametric()) continue;
		  if (property)
			  if (property->GetType() == typeDynVariableProperty)
				  if (property->GetDomain()->GetType() == typeDynCustomDomain)
					  ret *= property->ComputeSize();


	  }
	  if (ret == 0) ret = 1;
    break;
  }

  return ret;
}

int CDynElement::FindEncodingLen()
{
//  CDynProperty* property;
  POSITION pos;
  int ret = 0, encLen;
  childEncodingLen = 0;
  resourceEncodingLen = 0;
  propertyEncodingLen = 0;

  switch((CDecomposition)*this)
  {
  case decompAnd:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      e->SetEncodingLen(0);
      ret += e->FindEncodingLen();
    }
    childEncodingLen = ret;
    break;

  case decompOr:
    encLen = log2(children.GetCount());
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      e->SetEncodingLen(encLen);
      int r = e->FindEncodingLen();
      ret = max(ret, r);
    }
    childEncodingLen = ret;
    ret += encLen;
    break;

  case decompLeaf:
    // we incorporate the resource property in the leafs encoding
    /*
	property = FindProperty(_T("resource"));
    resourceEncodingLen = ret = property ? property->FindEncodingLen() : 0;
	*/
    {
      // tbd for dynamic variable natural domain properties
      int nbits = 0;
      POSITION pos = properties.GetHeadPosition();
      while(pos)
      {
        CDynProperty *prop = properties.GetNext(pos);
		//CDynPropertyList &proplist = prop->GetProperties();
		//if(proplist.GetCount() > 0)   //this parametric property
		//	continue;
        CProperty *sp = (CProperty *)prop->GetCore();
		if(sp->GetType()==typeVariableProperty)
		{
			if(((CVariableProperty *)sp)->IsParametric()) continue;
		}
		CString spname = prop->GetName();
        if (prop->IsType(typeDynVariableProperty))
		{
			if (sp->IsNatural())
			{
				CDynVariableProperty *dvnp = (CDynVariableProperty *)prop;
#ifndef DOUBLE_MTBDD
				CIntegerList& vals = dvnp->GetNaturalRange();
#else
				CDoubleList& vals = dvnp->GetNaturalRange();
#endif
				if (vals.GetCount() > 1)
					nbits += log2(vals.GetCount());
			}
			else
			{
				resourceEncodingLen += prop->FindEncodingLen();
			}
		}

      }
      propertyEncodingLen = nbits;
      ret += propertyEncodingLen;
	  ret += resourceEncodingLen;
    }
    break;
  }

  return ret;
}

int CDynElement::SetEncodingValue(int enc, int sb)
{
  int ret=encodingLen;
  int count=0, r1=0;
//  CDynProperty *property;
  POSITION pos;

  encodingVal = enc;
  startVar = sb;
  switch((CDecomposition)*this)
  {
  case decompAnd:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      ret += e->SetEncodingValue(0, startVar+ret);
    }
    break;

  case decompOr:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      int r = e->SetEncodingValue(count++, startVar+encodingLen);
      r1 = max(r, r1);
    }
    ret += r1;
    break;

  case decompLeaf:
    // encode the resource property
	  /*
    property = FindProperty(_T("resource"));
    ret += property ? property->SetEncodingValue(startVar+ret, resourceEncodingLen) : 0;
	*/
	{
        POSITION pos = properties.GetHeadPosition();
		while(pos)
		{
			CDynProperty *prop = properties.GetNext(pos);
		//	CDynPropertyList &proplist = prop->GetProperties();
		//if(proplist.GetCount() > 0)   //this parametric property
		//	continue;
			CProperty *sp = (CProperty *)prop->GetCore();
			if(sp->GetType()==typeVariableProperty)
		{
			if(((CVariableProperty *)sp)->IsParametric()) continue;
		}
			if (prop->IsType(typeDynVariableProperty))
			{
				if(!sp->IsNatural())
				{
					ret += prop->SetEncodingValue(startVar+ret, prop->FindEncodingLen());
				}
			}
		}

	}
    {
      // tbd for dynamic variable natural domain properties
     //int nbits = 0;
      POSITION pos = properties.GetHeadPosition();
      while(pos)
      {
        CDynProperty *prop = properties.GetNext(pos);
		//CDynPropertyList &proplist = prop->GetProperties();
		//if(proplist.GetCount() > 0)   //this parametric property
		//	continue;
        CProperty *sp = (CProperty *)prop->GetCore();
		if(sp->GetType()==typeVariableProperty)
		{
			if(((CVariableProperty *)sp)->IsParametric()) continue;
		}
        if (prop->IsType(typeDynVariableProperty))
		{
			if(sp->IsNatural())
			{
			  CDynVariableProperty *dvnp = (CDynVariableProperty *)prop;
#ifndef DOUBLE_MTBDD
				CIntegerList& vals = dvnp->GetNaturalRange();
#else
				CDoubleList& vals = dvnp->GetNaturalRange();
#endif
			  int nbits = 0;
			  if (vals.GetCount() > 1)
				  nbits = log2(vals.GetCount());
			  dvnp->SetEncodingValue(startVar + ret, nbits);
			  ret += nbits;
			}
		}
      }
    }
    break;
  }

  return ret;
}

CBdd CDynElement::MaptoBdd(CBdd& parentEnc, int props, ...)
{
  va_list ap;
  va_start(ap, props);
  CBdd ret = MaptoBdd_0(parentEnc, props, ap);
  va_end(ap);
  return ret;
}

CBdd CDynElement::MaptoBdd_0(CBdd& parentEnc, int props, va_list ap)
{

  TRACE( _T("CDynElement::MaptoBdd_0 : element %s : encval %d : startvar %d : encLen %d  \n"),
         GetName(), encodingVal, startVar, encodingLen );


  // interesting recursion here
  // what goes down is parent's encoding string concatenated with my encoding string
  // what goes up is children's encoding string concatenated with my encoding string
  // parents string concatenated with my string is stored as class data member

  baseEnc = CBdd::Encode(encodingVal, startVar, encodingLen);
  CBdd ret = baseEnc;
  encoding = baseEnc && parentEnc;

  switch((CDecomposition)*this) {
  case decompAnd:
    if (children.GetCount() > 0)
    {
      CBdd childEnc = CBdd::One();
      POSITION pos = children.GetHeadPosition();
      while(pos)
      {
        CDynElement *e = children.GetNext(pos);
        childEnc = e->MaptoBdd_0(encoding, props, ap) && childEnc;
      }
      ret = ret && childEnc;
    }
    break;

  case decompOr:
    if (children.GetCount() > 0)
    {
      CBdd childEnc = CBdd::Zero();
      POSITION pos = children.GetHeadPosition();
      while(pos)
      {
        CDynElement *e = children.GetNext(pos);
        childEnc = e->MaptoBdd_0(encoding, props, ap) || childEnc;
      }
      ret = ret && childEnc;
    }
    break;

  case decompLeaf:
	  //properties are no longer encoded here
	  //they are encoded when evaluating them

    break;
  }

  return ret;
}

void CDynElement::Dump(FILE *f)
{
//	ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );
	DYN_MANAGER_CHECK(_T("CDynElement::Dump()"));

	if (f) _ftprintf(f, _T(" Element: %s at address %p; core address: %p\n"), static_cast<const TCHAR*>(GetName()), this, GetCore());

	POSITION pos = properties.GetHeadPosition();
	while(pos) properties.GetNext(pos)->Dump(f);

	pos = children.GetHeadPosition();
    while(pos) children.GetNext(pos)->Dump(f);
}

void CDynElement::Clone()
{
  //ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );
	DYN_MANAGER_CHECK(_T("CDynElement::Clone()"));
  if (space || domain)
  { // create children
    CElementList& list = ((CElement *)core)->GetChildren();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CDynElement *dup = space ?
        new CDynElement( list.GetNext(pos), space, this ) :
        new CDynElement( list.GetNext(pos), (CDynCustomDomain*)domain, this );
      children.AddTail(dup);

	  // we do not insert elements in domain
      if (space) space->InsertElement(dup);
	  if (domain)
		  domain->InsertElement(dup);

    }
  }
  { // create properties
    CPropertyList& list = ((CElement *)core)->GetProperties();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CProperty *org = list.GetNext(pos);
      CDynDomain *dmn = CManager::theInstance->FindDomain(org->GetDomain());
      CDynProperty *dup = CDynProperty::Make(org, dmn, this);
      properties.AddTail(dup);
    }
  }
  { //create formulas;
	  CFormulaList& list = ((CElement *)core)->GetFormulas();
	  POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CFormula *org = list.GetNext(pos);
	//	CDynDomain *dmn = CManager::theInstance->FindDomain(org->GetDomain());
		CDynFormula *dup = CDynFormula::Make(org, this);
		formulas.AddTail(dup);
	}
  }
  { // create relations
    CRelationList& list = ((CElement *)core)->GetRelations();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CRelation *org = list.GetNext(pos);
      CDynElement *source = CManager::theInstance->FindElement(org->GetSource());
      CDynElement *destination = CManager::theInstance->FindElement(org->GetDestination());
      if (!source || !destination) continue;
      CDynRelation *dup = new CDynRelation(org, this, source, destination);
      relations.AddTail(dup);
    }
  }
  { // invoke clone on children
    POSITION pos = children.GetHeadPosition();
    while(pos) children.GetNext(pos)->Clone();
  }
}

void CDynElement::Clone(CDynElement *from)
{
  //ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );
	DYN_MANAGER_CHECK(_T("CDynElement::Clone(CDynElement *from)"));

  //if (space && !domain)
  if (space || domain)
  { // create children
    CDynElementList& list = from->GetChildren();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CDynElement *frm = list.GetNext(pos);
      if (!frm->IsAlive()) continue;
      CElement *org = (CElement *)frm->GetCore();
	  CDynElement *dup = NULL;

	  if (space)
	  {
		dup = new CDynElement( org, space, this );
		ASSERT(dup);
		children.AddTail(dup);
		space->InsertElement(dup);
	  }

	  if (domain)
	  {
		 ASSERT(domain->GetCore()->GetType() == typeCustomDomain);
		 dup = new CDynElement( org, (CDynCustomDomain*) domain, this );
		 ASSERT(dup);
		 children.AddTail(dup);
		 domain->InsertElement(dup);
	  }

      dup->Clone(frm);
    }
  }
  { // create properties
    CDynPropertyList& list = from->GetProperties();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CDynProperty *frm = list.GetNext(pos);
      CProperty *org = (CProperty *)frm->GetCore();
      CDynDomain *dmn = CManager::theInstance->FindDomain(org->GetDomain());
	  CDynProperty *dup = CDynProperty::Make(org, dmn, this);
      properties.AddTail(dup);
    }
  }
   { //create formulas;
	  CDynFormulaList& list = from->GetFormulas();
	  POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CDynFormula *frm = list.GetNext(pos);
		 CFormula *org = (CFormula *)frm->GetCore();
		CDynFormula *dup = CDynFormula::Make(org, this);
		formulas.AddTail(dup);
	}
  }
  { // create relations
    CDynRelationList& list = from->GetRelations();
    POSITION pos = list.GetHeadPosition();
    while(pos)
    {
      CDynRelation *frm = list.GetNext(pos);
      CRelation *org = (CRelation *)frm->GetCore();
      CDynElement *source = CManager::theInstance->FindElement(org->GetSource());
      CDynElement *destination = CManager::theInstance->FindElement(org->GetDestination());
      if (!source || !destination) continue;
      CDynRelation *dup = new CDynRelation(org, this, source, destination);
      relations.AddTail(dup);
    }
  }
}

bool CDynElement::Restrict(CBdd& res)
{
	alive = CBdd::Zero() != (res && encoding);
/*
	areaDone = false;
	latencyDone = false;
	powerDone = false;
	*/

	POSITION pos;
	switch((CDecomposition)*this)
	{
	case decompAnd:
		pos = children.GetHeadPosition();
		while(pos) children.GetNext(pos)->Restrict(res);
		break;

	case decompOr:
		Info(_T("CDynElement::Restrict"), _T("is possible that parent is alive while none of the children is alive?"));
		pos = children.GetHeadPosition();
		while(pos) children.GetNext(pos)->Restrict(res);
		break;
	case decompLeaf:
		pos = properties.GetHeadPosition();
		while (pos) properties.GetNext(pos)->Restrict(res);
		break;

	}

	TRACE(_T("%s: sv: %d, len:%d, val: %d\n"), (LPCTSTR)this->GetName(), this->startVar, this->encodingLen, this->encodingVal);
	return alive;
}





void CDynElement::BuildConfiguration(BackIfaceFunctions::DBConfiguration *cfg, CBdd& enc)
{
  if (CBdd::Zero() == (enc && encoding)) return;

  if (parent)
  {
	  //if parent is alive(eg. we are not root)
	  if (parent->GetType() == typeDynElement)
	  {
		  //if parent is element
		  if (parent->GetDecomposition() == decompOr)
		  {//if parent is an or decompositioned
			  long dummy;
			  VERIFY(!cfg->alt_assignments.Lookup(*(parent->GetCore()), dummy));
			  cfg->alt_assignments[*(parent->GetCore())] = *core;
		  };//if (parent->GetDecomposition() == decompOr)
	  };//if (parent->GetType() == typeDynElement)
	  //cfg>
  }

  //BackIfaceFunctions::CreateElement(core->GetIfaceId(), core->GetIfaceExternalId(), core->GetName());

  POSITION pos;

  switch((CDecomposition)*this)
  {
	  /*
	  shouldn't be or-decomposed nodes present in the encoding?
	  */
  case decompAnd:
  case decompOr:
    pos = children.GetHeadPosition();
    while(pos)
		children.GetNext(pos)->BuildConfiguration(cfg, enc);
    break;

  case decompLeaf:
/*
    CDynVariableProperty *property = (CDynVariableProperty *)FindProperty(_T("resource"));
    if (property) property->BuildConfiguration(cfg, enc, GetExternal());
    else Error(_T("CDynElement::BuildConfiguration"), _T("property resource not found"));
	*/
	  CDynVariableProperty * property;
	  POSITION mPos = properties.GetHeadPosition();
	  while (mPos)
	  {
		  property = (CDynVariableProperty *)properties.GetNext(mPos);
		  /*CDynPropertyList &plist = property->GetProperties();
		  if(plist.GetCount()>0) continue;*/
		  if(property->IsParametric()) continue;
		  if (property)
			  if (property->GetType() == typeDynVariableProperty)
				  property->BuildConfiguration(cfg, enc, core->GetIfaceExternalId());

	  }
    break;
  }
}

CCosmicList * CDynElement::Children()
{
  CCosmicList *list = (CCosmicList *)&children;
  return list;
}

ClData CDynElement::Eval(const CCosmic *other) const
{
	/*
		ToDo: What if other is just a plain number?
		Hope that will not get here ;)
	*/
  CDynElement *this_noconst = (CDynElement *)this;
  CDynElement *obj=0;
  CBdd ret = CBdd::One();
 CBdd otherret = CBdd::One();
  if (other)
  {
    CDynVariableProperty *prop;
	CDynProperty * temp;


	CDynPropertyList& prop_list = this_noconst->GetProperties();


    switch(other->GetType())
    {
    case typeDynElement:
      obj = (CDynElement *)other;
      break;

    default:
      Todo(_T("CDynElement::Eval"), _T("Eval for unimplemented type %d"), (int)(other->GetType()));
    }

    switch(funcType)
    {
    case funcImplementedBy:
      ret = obj->GetEncoding() && encoding;
   //   ret = ret || !(encoding);
      break;

	case funcPCMNone:
		ret = obj->Eval();
		break;
	case funcPCMNop:
		ret = ((CDynElement *)this)->PCMNop(other);
		break;
    default:
		Warning(_T("CDynElement::Eval"), _T("Function Type %d not implemented"), funcType);
		break;
    }
  }
  return ClData(ret);
}

bool CDynElement::IsNumericFunc() const
{
//	if (funcType==funcPCMNop || funcType==funcPCMNone || funcType==funcImplementedBy)
	if (funcType==funcPCMNop || funcType==funcImplementedBy)
		return false;
	else
		return true;
}

ClData CDynElement::Eval(ClRelExpr::RelOp op, const CCosmic *other) const
{
//	if(funcType==funcPCMNop || funcType==funcPCMNone || funcType==funcImplementedBy)
	if(funcType==funcPCMNop || funcType==funcImplementedBy)
	{
		return this->Eval(other);
	}

	CDynElement *this_noconst = (CDynElement *)this;
	CDynElement *obj=0;
	CBdd leftBDD = CBdd::One();
	CBdd rightBDD = CBdd::One();
	CBdd ret = CBdd::One();

	if (other)
	{
		CDynVariableProperty *prop;
		CDynProperty * temp;

		CDynPropertyList& prop_list = this_noconst->GetProperties();

		switch(other->GetType())
		{
			case typeDynElement:
				obj = (CDynElement *)other;
				break;

			default:
				Todo(_T("CDynElement::Eval"), _T("Eval for unimplemented type %d"), (int)(other->GetType()));
		}



		leftBDD = ((CDynElement *)this)->PCMGen_0(0, funcProperty);
		rightBDD = obj->PCMGen_0(0, obj->GetFuncProperty());
		ret = leftBDD - rightBDD;
		int limit = 0;
		switch (op)
		{
			//equals to relation operator
			case ClRelExpr::eqOp:
			{
				ret = ret.equals_to(limit);
				ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
				return ret;
			}

			//not equals to relation operator
			case ClRelExpr::neOp:
			{
				ret = ret.not_equals_to(limit);
				ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
				return ret;
			}
			//less then or equal relation operator
			case ClRelExpr::leOp:
				limit++;
			//less then relation operator
			case ClRelExpr::ltOp:
			{
				ret = ret.threshold(limit);
				ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
				return ret;
			}

			//greater then or equal relation operator
			case ClRelExpr::geOp:
				limit--;
			//greater then relation operator
			case ClRelExpr::gtOp:
			{
				ret = ret.low_threshold(limit);
				int sz = ret.size();
				ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
				return ret;
			}

			default:
			{
				//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
				throw new CDesertException(_T("unknow error!"));
				return ret;
			}
		};
	}

	return ClData(ret);
}

ClData CDynElement::Eval(ClAddExpr::AddOp op, const CCosmic *other) const
{
	CDynElement *this_noconst = (CDynElement *)this;
	CDynElement *obj=0;
	CBdd leftBDD = CBdd::One();
	CBdd rightBDD = CBdd::One();
	CBdd ret = CBdd::One();

	if (other)
	{
		CDynVariableProperty *prop;
		CDynProperty * temp;

		CDynPropertyList& prop_list = this_noconst->GetProperties();

		switch(other->GetType())
		{
			case typeDynElement:
				obj = (CDynElement *)other;
				break;

			default:
				Todo(_T("CDynElement::Eval"), _T("Eval for unimplemented type %d"), (int)(other->GetType()));
		}

		if(funcType==funcPCMNop || funcType==funcPCMNone)
		{
			return this->Eval(other);
		}

		leftBDD = ((CDynElement *)this)->PCMGen_0(0, funcProperty);
		rightBDD = obj->PCMGen_0(0, obj->GetFuncProperty());

		switch(op)
		{
		case(ClAddExpr::plusOp):
			ret = leftBDD + rightBDD;
			break;
		case(ClAddExpr::minusOp):
			ret = leftBDD - rightBDD;
			break;
		default:
			return (CBdd::Zero());
		}

	}
	return ClData(ret);
}

ClData CDynElement::Eval(ClAddExpr::AddOp op, const CBdd& left) const
{
	CDynElement *this_noconst = (CDynElement *)this;
	CDynElement *obj=0;
	CBdd leftBDD = left;
	CBdd rightBDD = CBdd::One();
	CBdd ret = CBdd::One();


	rightBDD = ((CDynElement *)this)->PCMGen_0(0, funcProperty);

	switch(op)
	{
	case(ClAddExpr::plusOp):
		ret = leftBDD + rightBDD;
		break;
	case(ClAddExpr::minusOp):
		ret = leftBDD - rightBDD;
		break;
	default:
		return (CBdd::Zero());
	}

	return ClData(ret);
}

ClData CDynElement::Eval(ClMulExpr::MulOp op, const CCosmic *other) const
{
	CDynElement *this_noconst = (CDynElement *)this;
	CDynElement *obj=0;
	CBdd leftBDD = CBdd::One();
	CBdd rightBDD = CBdd::One();
	CBdd ret = CBdd::One();

	if (other)
	{
		CDynVariableProperty *prop;
		CDynProperty * temp;

		CDynPropertyList& prop_list = this_noconst->GetProperties();

		switch(other->GetType())
		{
			case typeDynElement:
				obj = (CDynElement *)other;
				break;

			default:
				Todo(_T("CDynElement::Eval"), _T("Eval for unimplemented type %d"), (int)(other->GetType()));
		}

		if(funcType==funcPCMNop || funcType==funcPCMNone)
		{
			return this->Eval(other);
		}

		leftBDD = ((CDynElement *)this)->PCMGen_0(0, funcProperty);
		rightBDD = obj->PCMGen_0(0, obj->GetFuncProperty());

		switch(op)
		{
		case(ClMulExpr::starOp):
			ret = leftBDD * rightBDD;
			break;
		case(ClMulExpr::perOp):
			ret = leftBDD / rightBDD;
			break;
		default:
			return (CBdd::Zero());
		}

	}
	return ClData(ret);
}

#ifndef DOUBLE_MTBDD
ClData CDynElement::Eval(ClRelExpr::RelOp op, int right) const
#else
ClData CDynElement::Eval(ClRelExpr::RelOp op, double right) const
#endif
{
  //ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynElement::Eval(ClRelExpr::RelOp op, int right)"));
	CBdd ret = CBdd::One();

	switch(funcType)
	{
		case funcPCMAdd:
			ret = ((CDynElement *)this)->PCMAdd(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMMul:
			ret = ((CDynElement *)this)->PCMMul(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMMin:
			ret = ((CDynElement *)this)->PCMMin(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMMax:
			ret = ((CDynElement *)this)->PCMMax(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMAmed:
			ret = ((CDynElement *)this)->PCMAmed(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMGmed:
			ret = ((CDynElement *)this)->PCMGmed(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMCust:
			ret = ((CDynElement *)this)->PCMCust(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMNone:
		  //evaluable only on leaf nodes

			ret = ((CDynElement *)this)->PCMNone(right, op); // screwing up with the constant-ness here
			break;

		case funcPCMNop:
			//does no operation
			ret = ((CDynElement *)this)->PCMNop(right, op); // screwing up with the constant-ness here
			break;

		default:
			Warning(_T("CDynElement::Eval"), _T("Eval a Rel Expr of an unimplemented function type %d for property %s"),
				funcType, funcProperty);
			break;
	}

	return ClData(ret);
}

/*


void
CDynElement::
GetVectors(const TCHAR* name, CUIntArray& vecList)
{
  POSITION pos;
  CDynProperty *prop = 0;
  int vec;

  switch((CDecomposition)*this)
  {
  case decompLeaf:
    prop = FindProperty(name);
    vec = prop ? prop->GetVector() : -1;
    if (vec >= 0) AddToArray(vec, vecList);
    break;

  default:
    pos = children.GetHeadPosition();
    while(pos) children.GetNext(pos)->GetVectors(name, vecList);
  }

}
*/
void
CDynElement::
Complement(CDynElementList& list, CDynElementList& complement)
{
  POSITION pos = children.GetHeadPosition();
  while(pos)
  {
    CDynElement *el = children.GetNext(pos);
    if (list.Find(el)) continue;
    complement.AddTail(el);
  }
}



CBdd CDynElement::PCMMin(int limit, ClRelExpr::RelOp op)
{
  CBdd ret = PCMMin_0(limit, funcProperty);


  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
		//ASSERT_EX(false, _T("CDynElement::PCMMin()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMMin():: Operator unimplemented!"));
		return ret;
	}
  };


}

CBdd CDynElement::PCMMax(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMMax_0(limit, funcProperty);


  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
	//	ASSERT_EX(false, _T("CDynElement::PCMMax()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMMax():: Operator unimplemented!"));
		return ret;
	}
  };


}


CBdd CDynElement::PCMAmed(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMAmed_0(limit, funcProperty);


  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
	//	ASSERT_EX(false, _T("CDynElement::PCMAmed()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMAmed():: Operator unimplemented!"));
		return ret;
	}
  };


}

CBdd CDynElement::PCMGmed(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMGmed_0(limit, funcProperty);


  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
		//ASSERT_EX(false, _T("CDynElement::PCMGmed()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMGmed():: Operator unimplemented!"));
		return ret;
	}
  };


}





CBdd CDynElement::PCMMul(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMMul_0(limit, funcProperty);


  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
		//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMMul():: Operator unimplemented!"));
		return ret;
	}
  };


}

CBdd CDynElement::PCMCust(int limit, ClRelExpr::RelOp op)
{

	fnptrPCMCust PCMfn = CManager::theInstance->GetPCMCustomFunction(funcProperty);
	return PCMfn(limit, op, this);
}

fnptrPCMCust CManager::GetPCMCustomFunction(CString &propName)
{

	void * PCMfn;			//returna value

	//Look Up...
	if (fnPCMtable.Lookup(propName, PCMfn)) return (fnptrPCMCust)PCMfn;

	//Not found, find the Dll and the function
	CString dll_name;
	CStringA eval_fn;

#ifdef _DEBUG
	dll_name = _T("PCM_") + propName +_T("D.dll");
#else
	dll_name = _T("PCM_") + propName + _T(".dll");

#endif

	HMODULE dll_module = LoadLibrary(dll_name);

	if (!dll_module)
		throw new CDesertException(CString(_T(" Failed to load: ")) + dll_name);

	eval_fn = "PCM_Eval_" + propName;
	PCMfn = (fnptrPCMCust)GetProcAddress(dll_module, eval_fn);

	if (!PCMfn)
		throw new CDesertException(CString(_T(" Failed to locate function: ")) + CString(eval_fn));

	//cache it

	fnPCMtable[propName] = PCMfn;

	return (fnptrPCMCust)PCMfn;
}


CBdd CDynElement::PCMAdd(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMAdd_0(limit, funcProperty);

  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
		//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMAdd():: Operator unimplemented!"));
		return ret;
	}
  };


}

CBdd CDynElement::PCMNop_0(const CCosmic * other, const TCHAR * propName)
{
	CDynProperty *prop;
	CDynVariableProperty *vprop;
	CDynElement * obj;
	//int propVal = 0;
	POSITION pos;
	CBdd ret;

	switch((CDecomposition)*this)
	{
	case decompAnd:
		ret = CBdd::One();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret && children.GetNext(pos)->PCMNop_0(other, propName);
		break;

	case decompOr:
		ret = CBdd::Zero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret || children.GetNext(pos)->PCMNop_0(other, propName);
		break;

	case decompLeaf:
		prop = FindProperty(propName);

		if (prop)
		{
			if (prop->GetType() == typeDynVariableProperty)
				vprop = (CDynVariableProperty*) prop;
			else vprop = NULL;
		} else vprop = NULL;

		if (other->GetType() != typeDynElement)
			throw new CDesertException(_T("PCMNop_0(): Unimplemented operation, expected other to be of type typeDynElement"));

		obj = (CDynElement*) other;

		ret = vprop ? vprop->GetEncoding(obj) : CBdd::Zero();
		break;
	} // end switch( decomp...

	if (parent && (decompOr == (CDecomposition)(*parent)))
		ret = CBdd::Mtbdd_Ite(baseEnc, ret);


	return ret;

};

CBdd CDynElement::PCMNone(int limit, ClRelExpr::RelOp op)
{
	CBdd ret;
	CDynProperty * prop;


	switch((CDecomposition)*this)
	{
	case decompAnd:
	case decompOr:
		throw new CDesertException(_T("CDynElement::PCMNone():: PCMNone should be evaluated only at leaf nodes!"));
		break;

	case decompLeaf:
		prop = FindProperty(funcProperty);
		ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
		break;
	} // end switch( decomp...

	//if parent is alternative
	if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

	switch (op)
	{
		//equals to relation operator
		case ClRelExpr::eqOp:
		{
			ret = ret.equals_to(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}

		//not equals to relation operator
		case ClRelExpr::neOp:
		{
			ret = ret.not_equals_to(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}
		//less then or equal relation operator
		case ClRelExpr::leOp:
			limit++;
		//less then relation operator
		case ClRelExpr::ltOp:
		{
			ret = ret.threshold(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}

		//greater then or equal relation operator
		case ClRelExpr::geOp:
			limit--;
		//greater then relation operator
		case ClRelExpr::gtOp:
		{
			ret = ret.low_threshold(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}


		default:
		{
			//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
			throw new CDesertException(_T("CDynElement::PCMAdd():: Operator unimplemented!"));
			return ret;
		}
	};






};

CBdd CDynElement::PCMNop(const CCosmic * other)
{
	CBdd ret = PCMNop_0(other, funcProperty);

	//the only op expected here is eqOp, probably we should also support neOp

	ret = ret  && encoding;
	ret = ret || !(encoding);

	return ret;
}

CBdd CDynElement::PCMNop(int limit, ClRelExpr::RelOp op)
{

  CBdd ret = PCMNop_0(limit, funcProperty);

  switch (op)
  {
	//equals to relation operator
	case ClRelExpr::eqOp:
	{
		ret = ret.equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//not equals to relation operator
	case ClRelExpr::neOp:
	{
		ret = ret.not_equals_to(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}
	//less then or equal relation operator
	case ClRelExpr::leOp:
		limit++;
	//less then relation operator
	case ClRelExpr::ltOp:
	{
		ret = ret.threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}

	//greater then or equal relation operator
	case ClRelExpr::geOp:
		limit--;
	//greater then relation operator
	case ClRelExpr::gtOp:
	{
		ret = ret.low_threshold(limit);
		int sz = ret.size();
		ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
		return ret;
	}


	default:
	{
		//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
		throw new CDesertException(_T("CDynElement::PCMNop():: Operator unimplemented!"));
		return ret;
	}
  };


}


CBdd CDynElement::PCMNop_0(int limit, const TCHAR *propName)
{
	CDynProperty *prop;
	int propVal = 0;
	POSITION pos;
	CBdd ret;

	switch((CDecomposition)*this)
	{
	case decompAnd:
		ret = CBdd::One();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret && children.GetNext(pos)->PCMNop_0(limit, propName);
		break;

	case decompOr:
		ret = CBdd::Zero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret || children.GetNext(pos)->PCMNop_0(limit, propName);
		break;

	case decompLeaf:
		prop = FindProperty(propName);
		ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
		break;
	} // end switch( decomp...

	if (parent && (decompOr == (CDecomposition)(*parent)))
		ret = CBdd::Mtbdd_Ite(baseEnc, ret);


	return ret;
};

CBdd
CDynElement::
PCMAdd_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  CDynFormula *formu;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
	 formu = FindFormula(propName);
	if(formu)
	ret = formu->MaptoBdd();
	else
	{
	ret = CBdd::ArithZero();
	pos = children.GetHeadPosition();
	while(pos)
		ret = ret + children.GetNext(pos)->PCMAdd_0(limit, propName);
	}
    break;

  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMAdd_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);


	if(prop)
	{
		ret = prop->MapValuetoBdd( encoding );
	}
	else
	{
		formu = FindFormula(propName);
	//	std::string dname(propName);
		ret = formu ? formu->MaptoBdd() : CBdd::ArithZero();
	}
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	//ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

 // int sz = ret.size();

  return ret;
}

CBdd
CDynElement::
PCMGmed_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  CDynFormula *formu;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
	  formu = FindFormula(propName);
	  if(formu)
		ret = formu->MaptoBdd();
	  else
	  {
		ret = CBdd::ArithZero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret * children.GetNext(pos)->PCMGmed_0(limit, propName);

		ret = ret ^ children.GetCount();//n-th order root
	  }
	break;


  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMGmed_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	if(prop)
		ret = prop->MapValuetoBdd( encoding );
	else
	{
		formu = FindFormula(propName);
		ret = formu ? formu->MaptoBdd() :  CBdd::ArithOne();
	}
	//ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithOne();
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

  int sz = ret.size();
  return ret;
}


CBdd
CDynElement::
PCMAmed_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  CDynFormula *formu;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
	  formu = FindFormula(propName);
	  if(formu)
		ret = formu->MaptoBdd();
	  else
	  {
		ret = CBdd::ArithZero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = ret + children.GetNext(pos)->PCMAmed_0(limit, propName);

		ret = ret / children.GetCount();
	  }
	break;


  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMAmed_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	//ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
	if(prop)
		ret = prop->MapValuetoBdd( encoding );
	else
	{
		formu = FindFormula(propName);
		ret = formu ? formu->MaptoBdd() :  CBdd::ArithZero();
	}
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

  int sz = ret.size();
  return ret;
}

CBdd
CDynElement::
PCMMax_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  CDynFormula *formu;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
	  formu = FindFormula(propName);
	  if(formu)
		ret = formu->MaptoBdd();
	  else
	  {
		ret = CBdd::ArithZero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = CBdd::Max(ret , children.GetNext(pos)->PCMMax_0(limit, propName));
	  }
    break;

  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMMax_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	//ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
	if(prop)
		ret = prop->MapValuetoBdd( encoding );
	else
	{
		formu = FindFormula(propName);
		ret = formu ? formu->MaptoBdd() :  CBdd::ArithZero();
	}
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

  int sz = ret.size();
  return ret;
}


CBdd
CDynElement::
PCMMin_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  CDynFormula *formu;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
	  formu = FindFormula(propName);
	  if(formu)
		ret = formu->MaptoBdd();
	  else
	  {
		ret = CBdd::ArithZero();
		pos = children.GetHeadPosition();
		while(pos)
			ret = CBdd::Min(ret , children.GetNext(pos)->PCMMin_0(limit, propName));
	  }
    break;

  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMMin_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	//ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
	if(prop)
		ret = prop->MapValuetoBdd( encoding );
	else
	{
		formu = FindFormula(propName);
		ret = formu ? formu->MaptoBdd() :  CBdd::ArithZero();
	}
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

  int sz = ret.size();
  return ret;
}


CBdd
CDynElement::
PCMMul_0(int limit, const TCHAR *propName)
{
  CDynProperty *prop;
  int propVal = 0;
  POSITION pos;
  CBdd ret;

  switch((CDecomposition)*this)
  {
  case decompAnd:
    ret = CBdd::ArithZero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret * children.GetNext(pos)->PCMMul_0(limit, propName);
    break;

  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      ret = ret || children.GetNext(pos)->PCMMul_0(limit, propName);
    break;

  case decompLeaf:
    prop = FindProperty(propName);
    //ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithOne();
    break;
  } // end switch( decomp...

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);

  int sz = ret.size();
  return ret;
}


//end of PCMMul_0 implementation

//void
//CDynElement::
//AddToArray(int val, CUIntArray& array) const
//{
//  // check for duplicates
//  UINT *data = array.GetData();
//  int size = array.GetSize();
//  int width = sizeof(UINT);
//
//  // sort the array
//  qsort( data, size, width, compareInt );
//
//  // do a binary search to find the val
//  if (bsearch( &val, data, size, width, compareInt ) == 0)
//    array.Add(val);
//}

bool
CDynElement::
NotRedundant(int *enc)
{
  // suspicious code -- check this for potential problems

  if ( parent && (decompOr == (CDecomposition)*parent) )
  {
    int i,n;
    int val=encodingVal;

    // if the encoding does not match mine then noyb, return true
    i = startVar + encodingLen - 1;
    for (n=0; n<encodingLen; n++, i--, val = val >> 1)
      if ( enc[i] != (val & 0x1) ) return true;

    // the bits in the childEncodingLen after my encoding & my childencoding & my propertyencoding should be zero
    // otherwise this is a redundant encoding
    // if i am a node then i use:
    // a) my own encoding len (decided by parent)
    // b) childEncodinglen (bits used by my children)
    // if a am an or node i also use
    // c) log2(children_count) bits to distinguish between my children
    //

    int inuse = childEncodingLen + resourceEncodingLen + propertyEncodingLen
      + ((decompOr == (CDecomposition)*this) ? log2(children.GetCount()) : 0);
      ;
    i = startVar + encodingLen + inuse;
    int len = parent->childEncodingLen - inuse;
    for (n=0; n<len; n++, i++)
      if ( enc[i] != 0 ) return false;
  }

  if(decompOr == (CDecomposition)*this)
  {
    // return false for non-existant children
    // child distinction starts after start var +  my encoding len (decided by parent)
    int val = 0;
    int i = startVar + encodingLen;
    int lc = log2(children.GetCount());
    for (int n=0; n<lc; n++, i++)
    {
      val = val << 1;
      val |= enc[i];
    }
    if (children.GetCount() <= val) return false;
  }

  if(decompLeaf == (CDecomposition)*this)
  {
    // if it is a primitive, then check the encoding value
    // also check if this value is greater than the number of possible resources
    // that could be assigned to this primitive
    int val = 0;
    int i = startVar + encodingLen;

	POSITION pos = properties.GetHeadPosition();

	//for custom domains
	while (pos)
	{
		CDynProperty *prop = properties.GetNext(pos);
		//CDynPropertyList &proplist = prop->GetProperties();
		//if(proplist.GetCount() > 0)   //this parametric property
		//	continue;
		if (prop->IsType(typeDynVariableProperty))
		{
			CProperty *sp = (CProperty*)prop->GetCore();
			if(sp->GetType()==typeVariableProperty)
		{
			if(((CVariableProperty *)sp)->IsParametric()) continue;
		}
			if (!sp->IsNatural())
			{
				for (int n=0; n < prop->FindEncodingLen(); n++, i++)
				{
					val = val << 1;
					val |= enc[i];
				}
				if (prop->ComputeSize() <= val) return false;
			}
		}
	}
	/*
	// for natural domains
	pos = properties.GetHeadPosition();
	while (pos)
	{
		CDynProperty *property = properties.GetNext(pos);
		if (property->IsType(typeDynVariableProperty))
		{
			CProperty *sp = (CProperty*)property->GetCore();
			if (sp->IsNatural())
			{
				for (int n=0; n < property->FindEncodingLen(); n++, i++)
				{
					val = val << 1;
					val |= enc[i];
				}
				if (property->ComputeSize() <= val) return false;
			}
		}
	}
	*/




    /*
    for (int n=0; n<resourceEncodingLen; n++, i++)
    {
      val = val << 1;
      val |= enc[i];
    }

    CDynProperty *property = FindProperty(_T("resource"));
    if (property && (property->ComputeSize() <= val)) return false;
	*/
  }

  POSITION pos = children.GetHeadPosition();
  while(pos) if (!children.GetNext(pos)->NotRedundant(enc)) return false;

  return true;
}


int CDynElement::FindNonOrthElementCount()
{
  POSITION pos;
  int ret = 0;

  switch((CDecomposition)*this)
  {
  case decompAnd:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      ret += e->FindNonOrthElementCount();
    }
    break;

  case decompOr:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      int r = e->FindNonOrthElementCount();
      ret = max(ret, r);
    }
    break;

  case decompLeaf:
    ret = 1;
    break;
  }

  return ret;
}

int CDynElement::FindPropertyEncodingLen(const TCHAR *n)
{
  if((CDecomposition)*this == decompLeaf)
  {
    CDynProperty *p = FindProperty(n);
    return p ? p->FindEncodingLen() : 0;
  }

  int ret = 0;
  POSITION pos = children.GetHeadPosition();
  while(pos)
  {
    CDynElement *e = children.GetNext(pos);
    int r = e->FindPropertyEncodingLen(n);
    ret = max(ret, r);
  }

  return ret;
}

int CDynElement::SetPropertyEncodingValue(const TCHAR *n, int sv)
{
  int ret=0;
  CDynProperty *p=0;
  POSITION pos;

  startPropertyVec[n] = (void *)sv;

  switch((CDecomposition)*this)
  {
  case decompAnd:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      ret += e->SetPropertyEncodingValue(n, sv+ret);
    }
    break;

  case decompOr:
    pos = children.GetHeadPosition();
    while(pos)
    {
      CDynElement *e = children.GetNext(pos);
      int r = e->SetPropertyEncodingValue(n, sv);
      ret = max(r, ret);
    }
    break;

  case decompLeaf:
    p = FindProperty(n);
    ret = p ? p->SetEncodingValue(sv) : 0;
    break;
  }

  numPropertyVecs[n] = (void *)ret;
  return ret;
}

bool
CDynElement::
FindRecurse(CDynElement *toFind, int& count)
{
  count++;
  if (toFind == this) return true;
  POSITION pos = children.GetHeadPosition();
  while(pos)  if ( children.GetNext(pos)->FindRecurse(toFind, count) ) return true;

  return false;
}

inline CCosmic *CDynElement::Domain(){  return domain;}

CBdd
CDynElement::
PCMGen_0(int limit, const TCHAR *propName)
{
	CBdd ret = CBdd::One();

	switch(funcType)
	{

	case funcPCMAdd:
		ret = ((CDynElement *)this)->PCMAdd_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMul:
		ret = ((CDynElement *)this)->PCMMul_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMin:
		ret = ((CDynElement *)this)->PCMMin_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMax:
		ret = ((CDynElement *)this)->PCMMax_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMAmed:
		ret = ((CDynElement *)this)->PCMAmed_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMGmed:
		ret = ((CDynElement *)this)->PCMGmed_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMCust:
//		ret = ((CDynElement *)this)->PCMCust(right, op); // screwing up with the constant-ness here
		break;

	case funcPCMNone:
	  //evaluable only on leaf nodes
		{
	//	ret = ((CDynElement *)this)->PCMNone(right, op); // screwing up with the constant-ness here
			CDynProperty * prop;
			switch((CDecomposition)*this)
			{
			case decompAnd:
			case decompOr:
				//throw new CDesertException(_T("CDynElement::PCMNone():: PCMNone should be evaluated only at leaf nodes!"));
				break;

			case decompLeaf:
				prop = FindProperty(funcProperty);
				ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::ArithZero();
				break;
			} // end switch( decomp...

			//if parent is alternatives
			if (parent && (decompOr == (CDecomposition)(*parent)))
				ret = CBdd::Mtbdd_Ite(baseEnc, ret);

			break;
		}
	case funcPCMNop:
		{
		//does no operation
		ret = ((CDynElement *)this)->PCMNop_0(limit, funcProperty); // screwing up with the constant-ness here
		break;
		}
	default:
		Warning(_T("CDynElement::Eval"), _T("Eval a Rel Expr of an unimplemented function type %d for property %s"), funcType, funcProperty);
		break;
  }
  return ClData(ret);
}

//Added 11/30/2011
ClData CDynElement::Eval(int limit, const TCHAR *propName, FunctionType fn) const
{
	CBdd ret = CBdd::One();
	CDynFormula *formu = ((CDynElement*)this)->FindFormula(propName);
	if(formu)
		ret = formu->MaptoBdd();
	else
	{
		FunctionType pcm_fn;
		CDynProperty *prop = ((CDynElement*)this)->FindProperty(propName);

		bool processSimpleFormula = false;
		if(prop)
		{
			CProperty *cprop = (CProperty*)(prop->GetCore());
			pcm_fn = cprop->GetFuncType();
			CDynPropertyList &vplist = prop->GetProperties();
			if(!vplist.IsEmpty())
			{
				processSimpleFormula = true;
				ret = ((CDynElement *)this)->PCMGen_SimpleFormula(limit, vplist, pcm_fn);
			}
		}
		else
			pcm_fn = fn;

		if(!processSimpleFormula)
		{
			switch((CDecomposition)*this)
			{
				case decompAnd:
					ret = ((CDynElement *)this)->PCMGen_decompAnd(limit, propName, pcm_fn);
					break;
				case decompOr:
					ret = ((CDynElement *)this)->PCMGen_decompOr(limit, propName, pcm_fn);
					break;
				case decompLeaf:
					ret = ((CDynElement *)this)->PCMGen_decompLeaf(limit, propName, pcm_fn);
					break;
			}
		}
	}

	if (parent && (decompOr == (CDecomposition)(*parent)))
		ret = CBdd::Mtbdd_Ite(baseEnc, ret);

	return ClData(ret);
}

CBdd CDynElement::PCMGen_SimpleFormula(int limit,CDynPropertyList &propList, FunctionType fn)
{
	CBdd ret = CBdd::ArithZero();
	POSITION pos = propList.GetHeadPosition();
	while(pos)
	{
		CDynProperty *prop = propList.GetNext(pos);
		CString propName = prop->GetName();
		CDynElement *elem = prop->GetOwner();

		CProperty *cprop = (CProperty*)(prop->GetCore());
		FunctionType pcm_fn = cprop->GetFuncType();

		switch(fn)
		{
		case funcPCMAdd:
			ret = ret + (CBdd)(elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMMul:
			ret = ret * (CBdd)(elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMMin:
			ret = CBdd::Min(ret , elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMMax:
			ret = CBdd::Max(ret , elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMAmed:
			ret = ret + (CBdd)(elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMGmed:
			ret = ret * (CBdd)(elem->Eval(limit, propName, pcm_fn));
			break;
		case funcPCMNop:
			ret = ret && elem->Eval(limit, propName, pcm_fn);
			break;
		case funcPCMOr:
			ret = ret || elem->Eval(limit, propName, pcm_fn);
			break;
		}
	}

	switch(fn)
	{
	case funcPCMAmed:
		ret = ret / propList.GetCount();
		break;
	case funcPCMGmed:
		ret = ret ^ propList.GetCount();
		break;
	}

	return ret;
}

CBdd CDynElement::PCMGen_decompAnd(int limit, const TCHAR *propName, FunctionType fn)
{
	CBdd ret = CBdd::ArithZero();

	CDynProperty * prop = FindProperty(propName);
	ret = prop ? prop->MapValuetoBdd((CBdd)encoding ) : CBdd::ArithZero();

	if(fn==funcPCMNone || fn==funcPCMOr)
	{
		/*CDynProperty * prop = FindProperty(propName);
		ret = prop ? prop->MapValuetoBdd((CBdd)encoding ) : CBdd::ArithZero();*/
		return ret;
	}

	POSITION pos = children.GetHeadPosition();
	while(pos)
	{
		switch(fn)
		{
		case funcPCMAdd:
			ret = ret + (CBdd)(children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMMul:
			ret = ret * (CBdd)(children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMMin:
			ret = CBdd::Min(ret , children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMMax:
			ret = CBdd::Max(ret , children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMAmed:
			ret = ret + (CBdd)(children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMGmed:
			ret = ret * (CBdd)(children.GetNext(pos)->Eval(limit, propName, fn));
			break;
		case funcPCMNop:
			ret = ret && children.GetNext(pos)->Eval(limit, propName, fn);
			break;
		default:
			break;
		}
	}

	switch(fn)
	{
	case funcPCMAmed:
		ret = ret / children.GetCount();
		break;
	case funcPCMGmed:
		ret = ret ^ children.GetCount();
		break;
	default:
		break;
	}

	return ret;
}

CBdd CDynElement::PCMGen_decompOr(int limit, const TCHAR *propName, FunctionType fn)
{
	CBdd ret = CBdd::ArithZero();

	if(fn==funcPCMNone)
	{
		CDynProperty * prop = FindProperty(propName);
		ret = prop ? prop->MapValuetoBdd((CBdd)encoding ) : CBdd::ArithZero();
		return ret;
	}

	POSITION pos = children.GetHeadPosition();
	while(pos)
	{
		ret = ret || children.GetNext(pos)->Eval(limit, propName, fn);
	}

	return ret;
}

CBdd CDynElement::PCMGen_decompLeaf(int limit, const TCHAR *propName, FunctionType fn)
{
	CBdd ret;
	CDynProperty *prop = FindProperty(propName);


	if(prop)
		ret = prop->MapValuetoBdd( encoding );
	else
	{
		CDynFormula *formu = FindFormula(propName);
		ret = formu ? formu->MaptoBdd() : CBdd::ArithZero();
	}
	return ret;
}

//Added by Feng
ClData CDynElement::Eval() const
{
//	return ((CDynElement *)this)->PCMGen_0(0, funcProperty);
	return ((CDynElement *)this)->Eval(0, funcProperty, funcType);
	CBdd ret = CBdd::One();

	switch(funcType)
	{

	case funcPCMAdd:
		ret = ((CDynElement *)this)->PCMAdd_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMul:
		ret = ((CDynElement *)this)->PCMMul_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMin:
		ret = ((CDynElement *)this)->PCMMin_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMMax:
		ret = ((CDynElement *)this)->PCMMax_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMAmed:
		ret = ((CDynElement *)this)->PCMAmed_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMGmed:
		ret = ((CDynElement *)this)->PCMGmed_0(0, funcProperty); // screwing up with the constant-ness here
		break;

	case funcPCMCust:
//		ret = ((CDynElement *)this)->PCMCust(right, op); // screwing up with the constant-ness here
		break;

	case funcPCMNone:
	  //evaluable only on leaf nodes
		{
	//	ret = ((CDynElement *)this)->PCMNone(right, op); // screwing up with the constant-ness here
			CDynProperty * prop;
			switch((CDecomposition)*this)
			{
			case decompAnd:
			case decompOr:
				//throw new CDesertException(_T("CDynElement::PCMNone():: PCMNone should be evaluated only at leaf nodes!"));
				break;

			case decompLeaf:
				prop = ((CDynElement *)this)->FindProperty(funcProperty);
				ret = prop ? prop->MapValuetoBdd((CBdd)encoding ) : CBdd::ArithZero();
				break;
			} // end switch( decomp...

			//if parent is alternatives
			if (parent && (decompOr == (CDecomposition)(*parent)))
				ret = CBdd::Mtbdd_Ite(baseEnc, ret);

			break;
		}
	case funcPCMNop:
		{
		//does no operation
		ret = ((CDynElement *)this)->PCMNop_0(0, funcProperty); // screwing up with the constant-ness here
		break;
		}
	default:
		Warning(_T("CDynElement::Eval"), _T("Eval a Rel Expr of an unimplemented function type %d for property %s"), funcType, funcProperty);
		break;
  }
  return ClData(ret);
}

//------------CDynFormulaSet
inline CDynFormulaSet::CDynFormulaSet(CFormulaSet *c)  : CDynamicObj(c), alive(true)
{
	type =	typeDynFormulaSet;
};


inline CDynFormulaSet* CDynFormulaSet::Make(CFormulaSet *c)			{return new CDynFormulaSet(c);};


inline CDynFormulaSet* CDynFormulaSet::Make(CDynFormulaSet *f)		{return f->IsAlive() ? Make((CFormulaSet *)f->GetCore()) : 0;};


inline CDynFormulaSet::~CDynFormulaSet()									{Destroy(formulas);};
inline CDynFormulaList& CDynFormulaSet::GetFormulas()					{return formulas;}
inline CDynFormula * CDynFormulaSet::FindFormula(long i)				{CDynFormula *ret;  return Find(formulas, ret, i) ? ret : 0;}
inline CDynFormula * CDynFormulaSet::FindFormula(const TCHAR *n)		{CDynFormula *ret;  return Find(formulas, ret, n) ? ret : 0;}
inline CDynFormula * CDynFormulaSet::FindFormula(const CFormula *c)	{CDynFormula *ret;  return Find(formulas, ret, c) ? ret : 0;};
inline CDynFormula * CDynFormulaSet::RemoveFormula(CDynFormula *e)	{return Remove(formulas, e) ? e : 0;}
inline CDynFormula * CDynFormulaSet::RemoveFormula(long i)				{CDynFormula *e;  return Remove(formulas, e, i) ? e : 0;}
inline CDynFormula * CDynFormulaSet::RemoveFormula(const TCHAR *n)		{CDynFormula *e;  return Remove(formulas, e, n) ? e : 0;};
inline CDynFormula * CDynFormulaSet::InsertFormula(CDynFormula *e)	{return Insert(formulas, e) ? e : 0;}

inline void CDynFormulaSet::RemoveAll()										{formulas.RemoveAll();};
inline bool CDynFormulaSet ::IsEmpty() const									{return formulas.IsEmpty() ? true : false;};
inline bool CDynFormulaSet::IsAlive() const									{return alive;};


void CDynFormulaSet::Clone()
{
 // ASSERT_EX( core, _T("CDynConstraintSet::Clone"), _T("core is NULL") );

	if (!core) throw new CDesertException(_T("CDynFormulaSet::Clone: core is NULL") );

  //ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );
	DYN_MANAGER_CHECK(_T("CDynFormulaSet::Clone"));
  CFormulaList& list = ((CFormulaSet *)core)->GetFormulas();
  POSITION pos = list.GetHeadPosition();
  while(pos)
  {
    CFormula *org = list.GetNext(pos);
    CDynElement *ctx = CManager::theInstance->FindElement(org->GetContext());
    if (!ctx) continue;
    CDynFormula *dup = new CDynFormula(org, this, ctx);
    formulas.AddTail(dup);
  }
}

void CDynFormulaSet::Clone(CDynFormulaSet *from)
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynFormula::Clone()"));
  CDynFormulaList& list = from->GetFormulas();
  POSITION pos = list.GetHeadPosition();
  while(pos)
  {
    CDynFormula *frm = list.GetNext(pos);
    if (!frm->IsAlive()) continue;
    CFormula *org = (CFormula *)frm->GetCore();
    CDynElement *ctx = CManager::theInstance->FindElement(org->GetContext());
    if (!ctx) continue;
    CDynFormula *dup = new CDynFormula(org, this, ctx);
  //  dup->wasApplied = frm->wasApplied;
    formulas.AddTail(dup);
  }
}

CBdd CDynFormulaSet::MaptoBdd()
{
  CBdd ret = CBdd::One();
  POSITION pos = formulas.GetHeadPosition();

  while(pos)   ret = ((formulas.GetNext(pos))->MaptoBdd()) && ret;
  return ret;

}

CDynElement *CDynFormulaSet::FindAffectedSpace(CDynSpaceList& spaces, bool& propertyDependence)
{
  propertyDependence = false;
  // prepare a dependency list
  CDynamicObjList dependency;
  POSITION pos = formulas.GetHeadPosition();
  while(pos)
  {
    CDynFormula* cons = formulas.GetNext(pos);
    cons->FillDependency(dependency);
    propertyDependence = propertyDependence || cons->IsPropertyDependent();
  }

  int sc=0;
  pos = dependency.GetHeadPosition();
  while(pos)
  {
    CDynamicObj *obj = dependency.GetNext(pos);
    if (obj->GetType() == typeDynSpace)
    {
      spaces.AddTail((CDynSpace *)obj);
      sc++;
    }
    else
    {
      CDynSpace *spc = ((CDynElement *)obj)->GetSpace();
      spaces.AddTail(spc);
    }
  }

  if (!sc) return (CDynElement *)FindRoot(dependency);
  return 0;
}

bool CDynFormulaSet::Restrict(void)
{
  alive = false;
  POSITION pos = formulas.GetHeadPosition();
  while(pos) alive = formulas.GetNext(pos)->Restrict() || alive;
  return alive;
}

//-------------CDynFormula
inline CDynFormula::CDynFormula(CFormula *c, CDynFormulaSet *s, CDynElement* e)
  : CDynamicObj(c), context(e), set(s), alive(true){type = typeDynFormula;}

inline CDynFormula::CDynFormula(CFormula *c, CDynElement* e)
  : CDynamicObj(c), context(e), alive(true){type = typeDynFormula;}

inline CDynFormula::~CDynFormula()				{};
inline CDynFormulaSet* CDynFormula::GetSet()		{return set;};


inline CDynElement* CDynFormula::GetContext()		{return context;};
inline CString& CDynFormula::GetText()				{return ((CFormula *)core)->GetText();};
inline ClFormula* CDynFormula::GetExpression()	{return ((CFormula *)core)->GetExpression();};
inline bool CDynFormula::IsAlive() const				{return alive;};
inline bool CDynFormula::IsPropertyDependent() const	{return ((CFormula *)core)->IsPropertyDependent();};

CBdd CDynFormula::MaptoBdd()
{
  ClFormula *expression = ((CFormula *)core)->GetExpression();
  ClContext clCtx = context;

  // Himanshu: Always return Bdd::One() when a constraint fails
  CBdd ret = CBdd::One();
  try {
	  ret = clCtx.Eval(expression);
  } catch (CDesertException *e) {
	  int debugHere = 1;
	  // Himanshu: Always return Bdd::One() when a constraint fails
	  CString err(_T("Formula: <"));
	  err.Append(this->GetName());
	  err.Append(_T("> fails to be evaluated.\r"));
	  err.Append(e->GetErrorMessage());
	  e->Delete();
	  throw new CDesertException(err);
  }

  return ret;
}

void CDynFormula::FillDependency(CDynamicObjList& dep)
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynFormula::Restrict()"));

  CFormula *core = (CFormula *)(this->core);
  if (core->GetConstraintType() ==  typeUniSpace)
  {
    CCosmic *root = core->GetRoot();
    //ASSERT_EX( root, _T("CDynConstraint::FillDependency"), _T("root is null") );
	if (!root)
		throw new CDesertException(_T("CDynFormula::FillDependency: root is Null"));
    if (root->GetType() == typeElement)
    {
      CDynElement *elem = CManager::theInstance->FindElement((CElement *)root);
      if (dep.Find(elem) || dep.Find(elem->GetSpace())) ;
      else dep.AddTail(elem);
    }
    else
    {
      CDynamicObj *spc = CManager::theInstance->FindSpace((CSpace *)root);
      if (!dep.Find(spc)) dep.AddTail(spc);
    }
  }
  else
  {
    CSpaceList& spaces = core->GetSpaces();
    POSITION pos = spaces.GetHeadPosition();
    while(pos)
    {
      CDynamicObj *spc = CManager::theInstance->FindSpace(spaces.GetNext(pos));
      if (!dep.Find(spc)) dep.AddTail(spc);
    }
    Todo(_T("CDynFormula::FillDependency"), _T("what about domains in the dependency?"));
  }
}

bool CDynFormula::Restrict()
{
//  ASSERT_EX( CManager::theInstance, _T("CDynElement::Clone"), _T("manager not instantiated") );

	DYN_MANAGER_CHECK(_T("CDynFormula::Restrict()"));

  CFormula *core = (CFormula *)(this->core);
  CCosmicList& dependency = core->GetDependency();
  CDynSpace *spc=0;
  CDynDomain *dmn=0;
  CDynElement *elm=0;

  POSITION pos = dependency.GetHeadPosition();
  alive=false;
  while(pos && !alive)
  {
    CCosmic *obj = dependency.GetNext(pos);
    switch(obj->GetType())
    {
    case typeSpace:
      spc = CManager::theInstance->FindSpace((CSpace *)obj);
      alive = spc && spc->IsAlive();
      break;

    case typeElement:
      elm = CManager::theInstance->FindElement((CElement *)obj);
      alive = elm && elm->IsAlive();
      break;

    case typeDomain:
      dmn = CManager::theInstance->FindDomain((CDomain *)obj);
      alive = dmn != 0;
      break;
    }
  }
  if (alive)
  {
    CDynElement *el = CManager::theInstance->FindElement(core->GetContext());
    alive = el->IsAlive();
  }

  return alive;
}

CDynFormula *CDynFormula::Make(CFormula *org, CDynElement *own)
{
  CDynFormula *dup=0;
  switch((CType)*org)
  {
  case typeFormula:
    dup = new CDynFormula((CFormula *)org, own);
    break;
  default:
    //ASSERT_EX( false, _T("CDynProperty::Make"), _T("bad property type") );
	 throw new CDesertException(_T("CDynFormula::Make(): bad property type"));
  }
  return dup;
}
