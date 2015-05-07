// TokenEx.h: interface for the CTokenEx class.
//
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

#if !defined(AFX_TOKENEX_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_)
#define AFX_TOKENEX_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTokenEx  
{
public:
	CTokenEx();
	~CTokenEx();

   
	/*
     @FUNCTION: Splits a Path into 4 parts (Directory, Drive, Filename, Extention).
          NOTE: Supports UNC path names.
       @PARAM1: Whether the Supplied Path (PARAM2) contains a directory name only
                or a file name (Reason: some directories will end with "xxx.xxx"
                which is like a file name).
       @PARAM2: Path to Split.
       @PARAM3: (Referenced) Directory.
       @PARAM4: (Referenced) Drive.
       @PARAM5: (Referenced) Filename.
       @PARAM6: (Referenced) Extention.
	*/
	void SplitPath(BOOL UsingDirsOnly, CString Path, CString& Drive, CString& Dir, CString& FName, CString& Ext);

	/*
     @FUNCTION: Splits a CString into an CStringArray according the Deliminator.
	      NOTE: Supports UNC path names.
       @PARAM1: Source string to be Split.
       @PARAM2: Deliminator.
       @PARAM3: (Referenced) CStringArray to Add to.
    */
	void Split(CString Source, CString Deliminator, CStringArray& AddIt);
  
	/*
     @FUNCTION: Joins a CStringArray to create a CString according the Deliminator.
       @PARAM1: Deliminator.
       @PARAM2: (Referenced) CStringArray to Add to.
    */
	CString Join(CString Deliminator, CStringArray& AddIt);

	/*
     @FUNCTION: Returns the first string found (according to the token given). If the
	            string does not contain a token, then it returns the first param. 
	      NOTE: This uses a REFERENCE as the first param, so everytime it finds
	            a token, the string (& deliminator) are removed from the referenced
	            string.  SEE CODE EXAMPLE!
       @PARAM1: (Referenced) csRefered.
       @PARAM2: Deliminator.
    */
	CString GetString(CString &csRefered, CString Deliminator);

protected:

};

#endif // !defined(AFX_TOKENEX_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_)
