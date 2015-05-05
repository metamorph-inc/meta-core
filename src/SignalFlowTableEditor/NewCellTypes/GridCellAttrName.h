#include "../GridCtrl_src/GridCell.h"
#include "GMECOM.h"

class CGridCellAttrName : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellAttrName)

public:

	void SetRealName(CString realname);
	CString GetRealName();
protected:
	CString RealName;

};
