#pragma once
#include "UIPropEditorBar.h"
#include "Notifier.h"

// UIPropEditor

class UIPropEditor : public CWnd, public Notifier, public Observer
{
	DECLARE_DYNAMIC(UIPropEditor)

public:
	UIPropEditor();
	virtual ~UIPropEditor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL initialize(CWnd* pParentWnd, CRect rect);
	void observe(IMSG* pIMSG);
private:
	static const UINT		BAR_HEIGHT = 25;
	CStatic					caption;
	UIPropEditorBar			rowPortNameBar;		// Editor for the Src port Name
	UIPropEditorBar			rowPortTypeBar;		// Editor for the Src port Type
	UIPropEditorBar			colPortNameBar;		// Editor for the Dst port Name
	UIPropEditorBar			colPortTypeBar;		// Editor for the Dst port Type
	UIPropEditorBar			linkTypeBar;		// Editor for the link Types
	void updateSrcPortInfo(const CString *name, const CString *type);
	void updateDstPortInfo(const CString *name, const CString *type);
};


