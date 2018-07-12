// DesertTool.h : main header file for the DESERTTOOL application
//

#if !defined(AFX_DESERTTOOL_H__7E9E1941_2B0E_465F_8435_5825E99785B7__INCLUDED_)
#define AFX_DESERTTOOL_H__7E9E1941_2B0E_465F_8435_5825E99785B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDesertToolApp:
// See DesertTool.cpp for the implementation of this class
//

class CDesertToolApp : public CWinApp
{
public:
	CDesertToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesertToolApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	int returnCode;

	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDesertToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CWzdCommandLineInfo : public CCommandLineInfo
{

// Construction
public:
    CWzdCommandLineInfo();

// Attributes
public:
    CString desert_file;
	bool silent;
	bool applyCons;
	CString consList;
	bool outputFileNeedsToBeRead;
	CString desert_output_file;
	bool multiRun;
	CStringList consGroupNames;
	CStringList consGroups;

// Operations
public:
    void ParseParam(const TCHAR* pszParam,BOOL bFlag, BOOL bLast);
// Overrides

// Implementation
public:
    virtual ~CWzdCommandLineInfo();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESERTTOOL_H__7E9E1941_2B0E_465F_8435_5825E99785B7__INCLUDED_)
