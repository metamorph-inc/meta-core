#include "stdafx.h"

#include "GridCellAttrName.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellAttrName, CGridCell)

void CGridCellAttrName::SetRealName(CString realname)
{
	RealName = realname;
}

CString CGridCellAttrName::GetRealName()
{
	return RealName;
}
