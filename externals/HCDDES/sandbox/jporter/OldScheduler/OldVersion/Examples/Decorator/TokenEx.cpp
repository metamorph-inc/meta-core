// TokenEx.cpp: implementation of the CTokenEx class.
//
//	MFC Simple CString Tokenizer (Version 1)
//	
//	Written by Daniel Madden (daniel.madden@compaq.com)
//  Copyright (c) 1999
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TokenEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTokenEx::CTokenEx()
{
}

CTokenEx::~CTokenEx()
{
}

CString CTokenEx::GetString(CString &tmp, CString Search)
{
	int nFound;
	CString csRet;

	
	CString csTmp1 = tmp;
	if (tmp.Left(Search.GetLength()) == Search) {
		do {
			tmp = csTmp1.Mid(Search.GetLength());
			csTmp1 = tmp;
		} while (tmp.Left(Search.GetLength()) == Search);
		tmp = csTmp1.Mid(Search.GetLength() - 1);
	}

	CString csTmp = tmp;
	nFound = tmp.Find(Search,0);
	if (nFound >= 0) {
		csRet = csTmp.Left(nFound);
		tmp = csTmp.Mid(nFound + (Search.GetLength()));
	}
	else {
		csRet = csTmp;
		tmp = "";
	}

	return csRet;
}


CString CTokenEx::Join(CString Deliminator, CStringArray& AddIt)
{
	CString csReturn;
	CString csTmp;

	// Loop through the Array and Append the Deliminator
	for( int iNum = 0; iNum < AddIt.GetSize(); iNum++ ) {
		csTmp += AddIt.GetAt(iNum);
		csTmp += Deliminator;
	}
	csReturn = csTmp.Left(csTmp.GetLength() - 1);
	return csReturn;
}


void CTokenEx::Split(CString Source, CString Deliminator, CStringArray& AddIt)
{
	CString		 newCString;
	CString		 tmpCString;
	CString		 AddCString;

	int pos1 = 0;
	int pos = 0;

	newCString = Source;
	do {
		pos1 = 0;
		pos = newCString.Find(Deliminator, pos1);
		if ( pos != -1 ) {
			CString AddCString = newCString.Left(pos);
			if (!AddCString.IsEmpty())
				AddIt.Add(AddCString);

			tmpCString = newCString.Mid(pos + Deliminator.GetLength());
			newCString = tmpCString;
		}
	} while ( pos != -1 );
	
	if (!newCString.IsEmpty())
		AddIt.Add(newCString);
}


void CTokenEx::SplitPath (BOOL UsingDirsOnly, CString Path, CString& Drive, CString& Dir, CString& FName, CString& Ext)
{

	int nSecond;

	// Look for a UNC Name!
	if (Path.Left(2) == "\\\\") {
		int nFirst = Path.Find("\\",3);
		nSecond = Path.Find("\\",nFirst + 1);
		if (nSecond == -1) {
			Drive = Path;
			Dir = "";
			FName = "";
			Ext = "";
		}
		else if (nSecond > nFirst)
			Drive = Path.Left(nSecond);
	}
	else { // Look for normal Drive Structure
		nSecond = 2;
		Drive = Path.Left(2);
	}

	if (UsingDirsOnly) {
		Dir = Path.Right((Path.GetLength() - nSecond) - 1);
		FName = "";
		Ext = "";
	}
	else {
		int nDirEnd = Path.ReverseFind('\\');
		if (nDirEnd == Path.GetLength()) {
			Dir = "";
			FName = "";
			Ext = "";
		}
		else {

			Dir = Path.Mid(nSecond + 1, (nDirEnd - nSecond) - 1);

			int nFileEnd = Path.ReverseFind('.');
			if (nFileEnd != -1) {
				
				if (nDirEnd > nFileEnd) {
					FName = Path.Right(Path.GetLength() - nDirEnd);
					Ext = "";
				}
				else {
					FName = Path.Mid(nDirEnd + 1, (nFileEnd - nDirEnd) - 1);
					Ext = Path.Right((Path.GetLength() - nFileEnd) - 1);
				}
			}
			else {
				FName = Path.Right((Path.GetLength() - nDirEnd) - 1);
				Ext = "";
			}
		}
	}
}
