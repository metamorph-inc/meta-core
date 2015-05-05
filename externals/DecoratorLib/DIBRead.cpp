// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.

// Modified by Akos Ledeczi
// Copyright (C) 1997 Vanderbilt University

#include "stdafx.h"
#include "dibapi.h"

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/*************************************************************************

  Function:  ReadDIBFile (CFile&)

   Purpose:  Reads in the specified DIB file into a global chunk of
			 memory.

   Returns:  A handle to a dib (hDIB) if successful.
			 NULL if an error occurs.

  Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
			 from the end of the BITMAPFILEHEADER structure on is
			 returned in the global memory handle.

*************************************************************************/


HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	dwBitsSize = (DWORD) file.GetLength();

	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return NULL;

	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
		return NULL;

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	if (file.Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != //ReadHuge previously
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
	::GlobalUnlock((HGLOBAL) hDIB);

	return hDIB;
}
