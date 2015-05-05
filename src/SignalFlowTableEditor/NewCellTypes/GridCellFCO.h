#include "../GridCtrl_src/GridCell.h"
#include "GMECOM.h"
#include "Mga.h"

class CGridCellFCO : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellFCO)

public:

	void SetFCO(IMgaFCO *fco);
	IMgaFCO *GetFCO();
protected:
	CComPtr<IMgaFCO> m_FCO;

};
