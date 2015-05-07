#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "UIGridCtrl.h"
#include "UIConsole.h"
#include "UIPropEditor.h"
#include "ModelHandler.h"

// UI dialog

class UI : public CDialog
{
	DECLARE_DYNAMIC(UI)

public:
	UI(CWnd* pParent = NULL);   // standard constructor
	virtual ~UI();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	static const INT	CAPTION_HEIGHT	=	20;

private:
	// Variables
	INT					FOOTER_START;
	static const INT	MARGIN_SIZE				=	4;
	INT					HEADER_END;

	// Objects
	ModelHandler		modelHandler;
	CString				curDate;
	CButton				m_btnOK;
	UIConsole			outputConsole;
	UIPropEditor		propViewer;
	UIGridCtrl			matrixView;
	// Methods
	void writeConsole(CString str, UINT itemType);
	vector<CString> getCompleteLog();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnGetDefID(WPARAM wp, LPARAM lp);			// to avoid closing DlgBox on pressing enter

public:
	void printErrorLog(CString str);
	void printNormalLog(CString str);
	void printWarningLog(CString str);
	void attachComponent(ModelicaComponent component);
	virtual BOOL DestroyWindow();
};
