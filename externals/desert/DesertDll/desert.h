// -*-C++-*-
// desert.h : main header file for the DESERT DLL
//

#if !defined(AFX_DESERT_H__1D34B35E_318F_11D3_8B46_00104B2CCBA0__INCLUDED_)
#define AFX_DESERT_H__1D34B35E_318F_11D3_8B46_00104B2CCBA0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDesertApp
// See desert.cpp for the implementation of this class
//

class CDesertApp : public CWinApp
{
public:
	CDesertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesertApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDesertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESERT_H__1D34B35E_318F_11D3_8B46_00104B2CCBA0__INCLUDED_)
