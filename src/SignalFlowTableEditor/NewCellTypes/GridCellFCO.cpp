#include "stdafx.h"

#include "GridCellFCO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellFCO, CGridCell)

void CGridCellFCO::SetFCO(IMgaFCO *fco)
{
	m_FCO = fco;
}

IMgaFCO *CGridCellFCO::GetFCO()
{
	return m_FCO;
}
