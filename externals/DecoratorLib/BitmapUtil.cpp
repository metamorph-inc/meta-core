//################################################################################################
//
// Mga Decorator Base
//	BitmapUtil.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "BitmapUtil.h"

#include "DecoratorUtil.h"
#include "DecoratorLibResource.h"
#include "DecoratorDefs.h"
#include <io.h>

namespace DecoratorSDK
{

//################################################################################################
//
// CLASS : RefCoord
//
//################################################################################################

RefCoord::RefCoord()
	: m_eType( CRP_BEGIN ), m_lPos( 0 )
{
}

RefCoord::RefCoord( ECoordRefPoint eType, long lPos )
	: m_eType( eType ), m_lPos( lPos )
{
}

RefCoord::RefCoord( const RefCoord& rc )
	: m_eType( rc.m_eType ), m_lPos( rc.m_lPos )
{
}

RefCoord& RefCoord::operator=( const RefCoord& rc )
{
	if ( this != &rc ) {
		m_eType = rc.m_eType;
		m_lPos = rc.m_lPos;
	}
	return *this;
}

long RefCoord::getPos() const
{
	return m_lPos;
}

ECoordRefPoint RefCoord::getRef() const
{
	return m_eType;
}

long RefCoord::calcPos( long lLength )
{
	long lResult = 0;
	switch ( m_eType ) {
		case CRP_BEGIN :
			lResult = m_lPos;
			break;
		case CRP_END :
			lResult = lLength - m_lPos;
			break;
		case CRP_CENTER :
			lResult = lLength / 2 + m_lPos;
			break;
	}
	lResult = ( lResult < 0 ) ? 0 : lResult;
	return ( lResult > lLength ) ? lLength : lResult;
}


//################################################################################################
//
// CLASS : RefPoint
//
//################################################################################################

RefPoint::RefPoint()
{
}

RefPoint::RefPoint( const RefCoord& rcX, const RefCoord& rcY )
	: m_rcX( rcX ), m_rcY( rcY )
{
}

RefPoint::RefPoint( const RefPoint& rp )
	: m_rcX( rp.m_rcX ), m_rcY( rp.m_rcY )
{
}

RefPoint& RefPoint::operator=( const RefPoint rp )
{
	if ( this != &rp ) {
		m_rcX = rp.m_rcX;
		m_rcY = rp.m_rcY;
	}
	return *this;
}

RefCoord RefPoint::getX() const
{
	return m_rcX;
}

RefCoord RefPoint::getY() const
{
	return m_rcY;
}

CPoint RefPoint::calcPoint( long lWidth, long lHeight )
{
	return CPoint( m_rcX.calcPos( lWidth ), m_rcY.calcPos( lHeight ) );
}

//################################################################################################
//
// CLASS : BitmapBase
//
//################################################################################################

BitmapBase::BitmapBase( const CString& strName )
	: m_lWidth( 0 ), m_lHeight( 0 ), m_bHasTransparentColor( false ),
	m_crTransparentColor( 0x0 ), m_bHasBackgroundColor( false ), m_crBackgroundColor( 0x0 )
{
	setName( strName );
}

BitmapBase::BitmapBase( const CString& strName, COLORREF crColor, bool bIsTransparent )
	: m_lWidth( 0 ), m_lHeight( 0 ), m_bHasTransparentColor( bIsTransparent ),
	m_crTransparentColor( ( bIsTransparent ) ? crColor : 0x0 ), m_bHasBackgroundColor( ! bIsTransparent), m_crBackgroundColor( ( ! bIsTransparent ) ? crColor : 0x0  )
{
	setName( strName );
}

BitmapBase::BitmapBase( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor )
	: m_lWidth( 0 ), m_lHeight( 0 ), m_bHasTransparentColor( true ),
	m_crTransparentColor( crTransparentColor ), m_bHasBackgroundColor( true ), m_crBackgroundColor( crBackgroundColor )
{
	setName( strName );
}

BitmapBase::BitmapBase( const CString& strName, COLORREF crTransparentColor, COLORREF crGrayedColor, bool isMasked )
	: m_lWidth( 0 ), m_lHeight( 0 ), m_bHasTransparentColor( true ),
	m_crTransparentColor( crTransparentColor ), m_bHasBackgroundColor( false ), m_bHasGrayedColor( true ),
	m_crGrayedColor( crGrayedColor )
{
	setName( strName );
}

BitmapBase::~BitmapBase()
{
}

CString BitmapBase::getName() const
{
	return m_strName;
}

long BitmapBase::getWidth() const
{
	return m_lWidth;
}

long BitmapBase::getHeight() const
{
	return m_lHeight;
}

bool BitmapBase::hasTransparentColor() const
{
	return m_bHasTransparentColor;
}

bool BitmapBase::hasBackgroundColor() const
{
	return m_bHasBackgroundColor;
}

COLORREF BitmapBase::getTransparentColor() const
{
	return m_crTransparentColor;
}

COLORREF BitmapBase::getBackgroundColor() const
{
	return m_crBackgroundColor;
}

void BitmapBase::draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& cRect, const TileVector& vecTiles,
					   DWORD dwModifierFlags ) const
{
	for ( unsigned long i = 0 ; i < vecTiles.size() ; i++ ) {

		CPoint srcTopLeft = vecTiles[ i ].getPartTopLeft().calcPoint( m_lWidth, m_lHeight );
		CPoint srcBottomRight = vecTiles[ i ].getPartBottomRight().calcPoint( m_lWidth, m_lHeight );
		long lSrcWidth = srcBottomRight.x - srcTopLeft.x;
		long lSrcHeight = srcBottomRight.y - srcTopLeft.y;

		CPoint dstTopLeft = vecTiles[ i ].getTopLeft().calcPoint( cRect.Width(), cRect.Height() ) + cRect.TopLeft();
		CPoint dstBottomRight = vecTiles[ i ].getBottomRight().calcPoint( cRect.Width(), cRect.Height() ) + cRect.TopLeft();
		long lDstWidth = dstBottomRight.x - dstTopLeft.x;
		long lDstHeight = dstBottomRight.y - dstTopLeft.y;

		long lHNumber = 1;
		long lHMod = 0;
		if ( vecTiles[ i ].getHorizontalFill() == FT_TILE ) {
			lHMod = lDstWidth % lSrcWidth;
			lHNumber = lDstWidth / lSrcWidth + ( ( ! lHMod ) ? 0 : 1 );
		}

		long lVNumber = 1;
		long lVMod = 0;
		if ( vecTiles[ i ].getVerticalFill() == FT_TILE ) {
			lVMod = lDstHeight % lSrcHeight;
			lVNumber = lDstHeight / lSrcHeight + ( ( ! lVMod ) ? 0 : 1 );
		}

		for ( long h = 0 ; h < lHNumber ; h++ ) {

			long lSrcRight = srcBottomRight.x;
			long lDstLeft = dstTopLeft.x;
			long lDstRight = 0;
			switch ( vecTiles[ i ].getHorizontalFill() ) {
				case FT_NONE :
					lDstRight = dstTopLeft.x + lSrcWidth;
					break;
				case FT_STRETCH :
					lDstRight = dstBottomRight.x;
					break;
				case FT_TILE :
					if ( h == lHNumber - 1 && ! lHMod )
						lSrcRight = srcTopLeft.x + lHMod;
					lDstLeft += h * lSrcWidth;
					lDstRight = ( h == lHNumber - 1 && ! lHMod ) ? dstBottomRight.x : lDstLeft + lSrcWidth;
					break;
			}

			for ( long v = 0 ; v < lVNumber ; v++ ) {

				long lSrcBottom = srcBottomRight.y;
				long lDstTop = dstTopLeft.y;
				long lDstBottom = 0;
				switch ( vecTiles[ i ].getVerticalFill() ) {
					case FT_NONE :
						lDstBottom = dstTopLeft.y + lSrcHeight;
						break;
					case FT_STRETCH :
						lDstBottom = dstBottomRight.y;
						break;
					case FT_TILE :
						if ( v == lVNumber - 1 && ! lVMod )
							lSrcBottom = srcTopLeft.y + lVMod;
						lDstTop += v * lSrcHeight;
						lDstBottom = ( v == lVNumber - 1 && ! lVMod ) ? dstBottomRight.y : lDstTop + lSrcHeight;
						break;
				}

				draw( gdip, pDC, CRect( srcTopLeft.x, srcTopLeft.y, lSrcRight, lSrcBottom ),
					  CRect( lDstLeft, lDstTop, lDstRight, lDstBottom ), SRCCOPY, dwModifierFlags );
			}
		}
	}
	if ( m_bHasBackgroundColor ) {
		if (gdip != NULL) {
			// TODO: fillrect with background color and SRCAND effect
		} else {
			CDC dcMemory;
			dcMemory.CreateCompatibleDC( pDC );
			CBitmap bmp;
			bmp.CreateCompatibleBitmap( pDC, cRect.Width(), cRect.Height() );
			dcMemory.SelectObject( &bmp );
			dcMemory.FillSolidRect( CRect( 0, 0, cRect.Width(), cRect.Height() ), m_crBackgroundColor );
			CPen pen;
			pen.CreatePen( PS_SOLID, 6, m_crBackgroundColor );
			dcMemory.SelectObject( &pen );
			dcMemory.MoveTo( 0 , 0 );
			dcMemory.LineTo( 60, 60 );
			pDC->StretchBlt( cRect.left, cRect.top, cRect.Width(), cRect.Height(), &dcMemory, 0, 0, cRect.Width(), cRect.Height(), SRCAND );
		}
	}
}

void BitmapBase::setSize( long lWidth, long lHeight )
{
	m_lWidth = lWidth;
	m_lHeight = lHeight;
}

void BitmapBase::setName( const CString& strName )
{
	int iPos = strName.ReverseFind( '\\' );
	int iPos2 = strName.ReverseFind( '/' );
	iPos = max( iPos, iPos2 );
	m_strName = ( iPos == -1 ) ? strName : strName.Right( strName.GetLength() - iPos - 1 );
}

Gdiplus::ColorMatrix BitmapBase::GetGreyFadeMatrix(COLORREF greyColor) const
{
	float fadeFactor = 0.3f * GetRValue(greyColor) + 0.59f * GetGValue(greyColor) + 0.11f * GetBValue(greyColor);
	Gdiplus::ColorMatrix greyFadeColorMatrix = {
		0.3f / fadeFactor,	0.0f,				0.0f,				0.0f,			0.0f,
		0.0f,				0.59f / fadeFactor,	0.0f,				0.0f,			0.0f,
		0.0f,				0.0f,				0.11f / fadeFactor,	0.0f,			0.0f,
		0.0f,				0.0f,				0.0f,				1.0f,			0.0f,
		0.0f,				0.0f,				0.0f,				0.0f,			1.0f
	};
	return greyFadeColorMatrix;
}


//################################################################################################
//
// CLASS : BitmapDIB
//
//################################################################################################

BitmapDIB::BitmapDIB( const CString& strName )
	: BitmapBase( strName ), m_hDIB( NULL )
{
	load( strName );
}

BitmapDIB::BitmapDIB( const CString& strName, COLORREF crColor, bool bIsTransparent )
	: BitmapBase( strName , crColor, bIsTransparent ), m_hDIB( NULL )
{
	load( strName );
}

BitmapDIB::BitmapDIB( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor )
	: BitmapBase( strName, crTransparentColor, crBackgroundColor ), m_hDIB( NULL )
{
	load( strName );
}

BitmapDIB::~BitmapDIB()
{
	if ( isInitialized() ) {
		::GlobalUnlock( (HGLOBAL) m_hDIB );
		::GlobalFree( (HGLOBAL) m_hDIB );
	}
}

void BitmapDIB::draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
					  DWORD dwOpCode, DWORD dwModifierFlags ) const
{
	if ( isInitialized() ) {
		CRect srcRectCopy = srcRect;
		CRect dstRectCopy = dstRect;
		if (gdip != NULL) {
			LPSTR lpDIBHdr = (LPSTR)GlobalLock(m_hDIB);
			LPSTR lpDIBBits = FindDIBBits(lpDIBHdr);
			Gdiplus::Bitmap bitmap((LPBITMAPINFO)lpDIBHdr, lpDIBBits);

			bool bGray = (dwModifierFlags & MF_GREYED) != 0 && m_bHasGrayedColor;
			Gdiplus::ImageAttributes imgAttribs;
			if ((dwModifierFlags & MF_TRANSPARENT) != 0 && m_bHasTransparentColor) {
				Gdiplus::Color transparentColor(GetRValue(m_crTransparentColor), GetGValue(m_crTransparentColor), GetBValue(m_crTransparentColor));
				imgAttribs.SetColorKey(transparentColor, transparentColor);
			}
			if (bGray) {
				Gdiplus::ColorMatrix greyFadeColorMatrix = GetGreyFadeMatrix(m_crGrayedColor);
				imgAttribs.SetColorMatrix(&greyFadeColorMatrix);
			}
			Gdiplus::Rect destRect(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
			gdip->DrawImage(&bitmap, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height(),
							Gdiplus::UnitPixel, &imgAttribs);
			GlobalUnlock(m_hDIB);
		} else {
			::PaintDIB( pDC->m_hDC, &dstRectCopy, m_hDIB, &srcRectCopy, NULL );
		}
	}
}

HBITMAP BitmapDIB::DIBToBitmap(HDIB hDIB, HPALETTE hPal)
{
	LPSTR lpDIBHdr, lpDIBBits;	// pointer to DIB header, pointer to DIB bits
	HBITMAP hBitmap;			// handle to device-dependent bitmap
	HDC hDC;					// handle to DC
	HPALETTE hOldPal = NULL;	// handle to a palette

	if (!hDIB)
		return NULL;

	lpDIBHdr = (LPSTR)GlobalLock(hDIB);

	lpDIBBits = FindDIBBits(lpDIBHdr);

	hDC = GetDC(NULL);
	if (!hDC)
	{
		GlobalUnlock(hDIB);
		return NULL;
	}

	if (hPal)
		hOldPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	hBitmap = CreateDIBitmap(hDC, (LPBITMAPINFOHEADER)lpDIBHdr, CBM_INIT,
							 lpDIBBits, (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS);

	if (hOldPal)
		SelectPalette(hDC, hOldPal, FALSE);

	ReleaseDC(NULL, hDC);
	GlobalUnlock(hDIB);

	return hBitmap;
}

void BitmapDIB::load( const CString& strName )
{
	if ( ! m_hDIB ) {
		if ( ! getFacilities().arePathesValid() )
			return;
		std::vector<CString> vecPathes = getFacilities().getPathes();

		CString strFName = strName;

		CFile fDIB;
		CFileException fexDIB;
		for ( unsigned int i = 0 ; i < vecPathes.size() ; i++ ) {
			if ( fDIB.Open( vecPathes[ i ] + strFName, CFile::modeRead | CFile::shareDenyWrite, &fexDIB ) ) {
				TRY {
					m_hDIB = ::ReadDIBFile( fDIB );
					fDIB.Close();
					if ( m_hDIB ) {
						LPSTR lpDIB = (LPSTR) ::GlobalLock( (HGLOBAL) m_hDIB );
						setSize( ::DIBWidth( lpDIB ), ::DIBHeight( lpDIB) );
						::GlobalUnlock( (HGLOBAL) m_hDIB );
						return;
					}
				}
				CATCH (CFileException, eLoad) {
					fDIB.Abort();
					m_hDIB = NULL;
					AfxMessageBox( "Error Reading Bitmap File: " + vecPathes[ i ] + strFName, MB_OK | MB_ICONSTOP );
					return;
				} END_CATCH
			}
		}
	}
}

bool BitmapDIB::isInitialized() const
{
	return m_hDIB != NULL;
}

//################################################################################################
//
// CLASS : BitmapMasked
//
//################################################################################################

BitmapMasked::BitmapMasked( const CString& strName, COLORREF crTransparentColor, COLORREF crGrayColor ):
	BitmapBase	( strName, crTransparentColor, crGrayColor, true ),
	m_pBits		(NULL),
	m_pBMI		(NULL),
	m_pPalette	(NULL)
{
	CFile bitmapFile(strName, CFile::modeRead);
	Read( bitmapFile );

	setSize( getWidth(), getHeight());
}

BitmapMasked::BitmapMasked( UINT nResID, COLORREF crTransparentColor, COLORREF crGrayColor ):
	BitmapBase	( "" , crTransparentColor, crGrayColor, true ),
	m_pBits		(NULL),
	m_pBMI		(NULL),
	m_pPalette	(NULL)
{
	TCHAR chBuffer[10];
	_ultot( nResID, chBuffer, 10 );
	setName(chBuffer);
	ReadFromResource( nResID );

	setSize( getWidth(), getHeight());
}

BitmapMasked::~BitmapMasked()
{
	Free();
}

bool BitmapMasked::isInitialized() const
{
	return m_pBMI != NULL;
}

void BitmapMasked::draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						 DWORD dwOpCode, DWORD dwModifierFlags ) const
{
	if ( isInitialized() ) {
		bool bGray = (dwModifierFlags & MF_GREYED) != 0 && m_bHasGrayedColor;
		if ((dwModifierFlags & MF_TRANSPARENT) != 0 && m_bHasTransparentColor)
			drawTransparent (gdip, pDC, srcRect, dstRect, m_crTransparentColor, bGray, m_crGrayedColor);
		else
			draw (gdip, pDC, srcRect, dstRect, bGray, m_crGrayedColor);
	}
}

long BitmapMasked::getWidth() const
{
	if (!m_pBMI)
		return 0;
	
	/* return the DIB width */
	return m_pBMI->bmiHeader.biWidth;
}

long BitmapMasked::getHeight() const
{
	if (!m_pBMI)
		return 0;
	
	/* return the DIB height */
	return m_pBMI->bmiHeader.biHeight;
}

DWORD BitmapMasked::Read(CFile& file, BOOL bFromResource)
{
	DWORD dwReadBytes = 0;
	DWORD dwLength = (DWORD) file.GetLength();
	ASSERT ( dwLength > 0 );
	
	// Ensures no memory leaks will occur
	Free();
	
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	
	if( !bFromResource )
	{
		// Go read the DIB file header and check if it's valid.
		if( (dwReadBytes = file.Read((LPSTR)&bmfHeader, BMIF_SIZE)) != BMIF_SIZE)
			return 0;
		if(bmfHeader.bfType != DIB_HEADER_MARKER)
			return 0;
	}
	
	// Read DIB header.
	if( file.Read( &bmiHeader, BMIH_SIZE ) != BMIH_SIZE )
		return 0;
	dwReadBytes += BMIH_SIZE;
	
	DWORD dwPalSize = NumColors( bmiHeader ) * sizeof RGBQUAD;
	m_pBMI = (LPBITMAPINFO) new BYTE[BMIH_SIZE + dwPalSize];
	if (m_pBMI == NULL)
		return 0;
	memcpy( m_pBMI, &bmiHeader, BMIH_SIZE );
	// read palette data
	if( file.Read( m_pBMI->bmiColors, dwPalSize ) != dwPalSize )
		return 0;
	dwReadBytes += dwPalSize;
	CreatePalette();
	
	// Go read the bits.
	m_pBits = new BYTE[ dwLength - dwReadBytes ];
	if (m_pBits == NULL)
		return 0;
	
	if (file.Read( m_pBits, dwLength - dwReadBytes ) != (dwLength - dwReadBytes))
	{
		delete[] m_pBMI;
		m_pBMI = NULL;
		
		delete[] m_pBits;
		m_pBits = NULL;
		
		return 0;
	}
	dwReadBytes = dwLength;
	
	return dwReadBytes;
}

DWORD BitmapMasked::ReadFromResource(UINT nResID)
{
	DWORD	dwResult = 0;
	HMODULE hModule = AfxFindResourceHandle(MAKEINTRESOURCE(nResID), RT_BITMAP);
	ASSERT(hModule != NULL);
	// Load	from resource
	HRSRC	hbmres = FindResource(hModule, MAKEINTRESOURCE(nResID), RT_BITMAP );
	ASSERT(hbmres != NULL);
	CMemFile file; 
	HGLOBAL	hGlob;
	
	if (hbmres)
	{
		DWORD	dwResSize = SizeofResource( hModule, hbmres );
		ASSERT( dwResSize > 0 );
		file.Attach( (LPBYTE)LockResource( hGlob = LoadResource(hModule, hbmres) ), dwResSize );
		dwResult = Read(file, TRUE);
		ASSERT( dwResult == dwResSize );
		file.Detach();
		DeleteObject( hGlob );
	}
	
	return dwResult;
}

WORD  BitmapMasked::NumColors( BITMAPINFOHEADER& bmiHeader ) const
{
	if ( bmiHeader.biClrUsed != 0)
		return (WORD)bmiHeader.biClrUsed;

	switch ( bmiHeader.biBitCount )
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		ASSERT(false);
	case 24:
	case 32:
		return 0;
	}
}

void BitmapMasked::Free()
{
	// Make sure all member data that might have been allocated is freed.
	if(m_pBMI)
	{
		delete m_pBMI;
		m_pBMI = NULL;
	}
	
	if(m_pBits)
	{
		delete m_pBits;
		m_pBits = NULL;
	}
	if(m_pPalette)
	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}
}

BOOL  BitmapMasked::CreatePalette()
{
	if ( !isInitialized() )
		return FALSE;

	//get the number of colors in the DIB
	WORD wNumColors = NumColors( m_pBMI->bmiHeader );
	BOOL bResult = TRUE;
	
	if (wNumColors != 0)
	{
		// allocate memory block for logical palette
		LPLOGPALETTE pLogPal = (LPLOGPALETTE) new BYTE[ sizeof(LOGPALETTE) +
			sizeof(PALETTEENTRY)*wNumColors ];
		
		// if not enough memory, clean up and return NULL
		if( pLogPal == 0 )
			return FALSE;
		
		// set version and number of palette entries
		pLogPal->palVersion = PALVERSION;
		pLogPal->palNumEntries = wNumColors;
		
		for (WORD i = 0; i < wNumColors; i++)
		{
			pLogPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		
		// create the palette and get handle to it 
		if (m_pPalette)
		{
			m_pPalette->DeleteObject();
			delete m_pPalette;
		}
		
		m_pPalette = new CPalette;
		bResult = m_pPalette->CreatePalette( pLogPal );
		delete[] pLogPal;
	}
	
	return bResult;
}

void BitmapMasked::draw(Gdiplus::Graphics* gdip, CDC *pDC, const CRect& srcRect, const CRect &dstRect,
						bool bGray, COLORREF grayColor) const
{
	if ( !isInitialized() )
		return;

	if (gdip != NULL) {
		Gdiplus::Bitmap bitmap(m_pBMI, m_pBits);
		Gdiplus::ImageAttributes imgAttribs;
		if (bGray) {
			Gdiplus::ColorMatrix greyFadeColorMatrix = GetGreyFadeMatrix(grayColor);
			imgAttribs.SetColorMatrix(&greyFadeColorMatrix);
		}
		Gdiplus::Rect destRect(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
		gdip->DrawImage(&bitmap, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height(),
						Gdiplus::UnitPixel, &imgAttribs);
	} else {
		CPalette* pOldPal = NULL;        // Previous palette
		
		// Get the DIB's palette, then select it into DC
		if (m_pPalette != NULL)
		{
			// Select as background since we have
			// already realized in forground if needed
			pOldPal = pDC->SelectPalette( m_pPalette, TRUE);
		}
		
		/* Make sure to use the stretching mode best for color pictures */
		pDC->SetStretchBltMode( COLORONCOLOR );
		
		/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
		::StretchDIBits(pDC->m_hDC,		// hDC
			dstRect.left,				// DestX
			dstRect.top,				// DestY
			dstRect.Width(),			// nDestWidth
			dstRect.Height(),			// nDestHeight
			0,							// SrcX
			0,							// SrcY
			getWidth(),					// wSrcWidth
			getHeight(),				// wSrcHeight
			m_pBits,					// lpBits
			m_pBMI,						// lpBitsInfo
			DIB_RGB_COLORS,				// wUsage
			SRCCOPY);					// dwROP
		
		/* Reselect old palette */
		if (pOldPal != NULL)
		{
			pDC->SelectPalette( pOldPal, TRUE);
		}
	}
}

void BitmapMasked::drawTransparent(Gdiplus::Graphics* gdip, CDC *pDC, const CRect& srcRect, const CRect &dstRect,
								   COLORREF clrTransparency, bool bGray, COLORREF grayColor) const
{
	if ( !isInitialized() )
		return;

	if (gdip != NULL) {
		Gdiplus::Bitmap bitmap(m_pBMI, m_pBits);
		Gdiplus::Rect destRect(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
		Gdiplus::ImageAttributes imgAttribs;
		Gdiplus::Color transparentColor(GetRValue(clrTransparency), GetGValue(clrTransparency), GetBValue(clrTransparency));
		imgAttribs.SetColorKey(transparentColor, transparentColor);
		if (bGray) {
			Gdiplus::ColorMatrix greyFadeColorMatrix = GetGreyFadeMatrix(grayColor);
			imgAttribs.SetColorMatrix(&greyFadeColorMatrix);
		}
		gdip->DrawImage(&bitmap, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height(),
						Gdiplus::UnitPixel, &imgAttribs);
	} else {
		//
		// Create a memory DC (dcImage) and select the bitmap into it.
		//
		CDC dcImage;
		dcImage.CreateCompatibleDC (pDC);
		CBitmap bmImage;
		bmImage.CreateCompatibleBitmap(pDC, dstRect.Width(), dstRect.Height());

		CBitmap* pOldBitmapImage = dcImage.SelectObject(&bmImage);


		CPalette* pOldPal = NULL;        // Previous palette
		// Get the DIB's palette, then select it into DC
		if (m_pPalette != NULL)
		{
			// Select as background since we have
			// already realized in forground if needed
			pOldPal = dcImage.SelectPalette( m_pPalette, TRUE);
		}

		/* Make sure to use the stretching mode best for color pictures */
		dcImage.SetStretchBltMode( COLORONCOLOR );

		/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
		::StretchDIBits(dcImage.m_hDC,	// hDC
			0,							// DestX
			0,							// DestY
			dstRect.Width(),			// nDestWidth
			dstRect.Height(),			// nDestHeight
			0,							// SrcX
			0,							// SrcY
			getWidth(),					// wSrcWidth
			getHeight(),				// wSrcHeight
			m_pBits,					// lpBits
			m_pBMI,						// lpBitsInfo
			DIB_RGB_COLORS,				// wUsage
			SRCCOPY);					// dwROP

		/* Reselect old palette */

		if (pOldPal != NULL)
		{
			dcImage.SelectPalette( pOldPal, TRUE);
		}

		//
		// Create a second memory DC (dcAnd) and in it create an AND mask.
		//
		CDC dcAnd;
		dcAnd.CreateCompatibleDC (pDC);

		CBitmap bitmapAnd;
		bitmapAnd.CreateBitmap (dstRect.Width(), dstRect.Height(), 1, 1, NULL);
		CBitmap* pOldBitmapAnd = dcAnd.SelectObject (&bitmapAnd);

		dcImage.SetBkColor (clrTransparency);
		dcAnd.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcImage, 0, 0,
					  SRCCOPY);

		//
		// Create a third memory DC (dcXor) and in it create an XOR mask.
		//
		CDC dcXor;
		dcXor.CreateCompatibleDC (pDC);

		CBitmap bitmapXor;
		bitmapXor.CreateCompatibleBitmap (&dcImage, dstRect.Width(), dstRect.Height());
		CBitmap* pOldBitmapXor = dcXor.SelectObject (&bitmapXor);

		dcXor.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcImage, 0, 0,
					  SRCCOPY);

		dcXor.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcAnd, 0, 0,
					  0x220326);

		//
		// Copy the pixels in the destination rectangle to a temporary
		// memory DC (dcTemp).
		//
		CDC dcTemp;
		dcTemp.CreateCompatibleDC (pDC);

		CBitmap bitmapTemp;
		bitmapTemp.CreateCompatibleBitmap (&dcImage, dstRect.Width(), dstRect.Height());
		CBitmap* pOldBitmapTemp = dcTemp.SelectObject (&bitmapTemp);

		dcTemp.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), pDC, dstRect.left, dstRect.top, SRCCOPY);

		//
		// Generate the final image by applying the AND and XOR masks to
		// the image in the temporary memory DC.
		//
		if (bGray) {
			dcTemp.SetTextColor(grayColor);
			dcTemp.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcAnd, 0, 0, SRCAND);
		}
		else {
			dcTemp.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcAnd, 0, 0, SRCAND);

			dcTemp.BitBlt (0, 0, dstRect.Width(), dstRect.Height(), &dcXor, 0, 0, SRCINVERT);
		}

		//
		// Blit the resulting image to the screen.
		//
		pDC->BitBlt (dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height(), &dcTemp, 0, 0, SRCCOPY);

		//
		// Restore the default bitmaps.
		//
		dcTemp.SelectObject (pOldBitmapTemp);
		dcXor.SelectObject (pOldBitmapXor);
		dcAnd.SelectObject (pOldBitmapAnd);
		dcImage.SelectObject (pOldBitmapImage);
	}
}

//################################################################################################
//
// CLASS : BitmapGen
//
//################################################################################################


BitmapGen::BitmapGen( const CString& strName )
	: BitmapBase( strName ), m_pImage( NULL )
{
	load( strName );
}

BitmapGen::BitmapGen( const CString& strName, COLORREF crColor, bool bIsTransparent )
	: BitmapBase( strName, crColor, bIsTransparent ), m_pImage( NULL )
{
	load( strName );
}

BitmapGen::BitmapGen( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor )
	: BitmapBase( strName, crTransparentColor, crBackgroundColor ), m_pImage( NULL )
{
	load( strName );
}

BitmapGen::~BitmapGen()
{
	if( m_pImage) 
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

void BitmapGen::draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
					  DWORD dwOpCode, DWORD dwModifierFlags ) const
{
	ASSERT(m_pImage);
	Gdiplus::Graphics* gdip2 = gdip;
	if (gdip == NULL)
		gdip2 = getFacilities().getGraphics();
	Gdiplus::Status st = Gdiplus::Win32Error;
	int srcWidth = srcRect.Width();
	int srcHeight = srcRect.Height();
	if (m_pImage->GetType() == Gdiplus::ImageTypeMetafile) {
		srcWidth = m_pImage->GetWidth();
		srcHeight = m_pImage->GetHeight();
	}
	/* Technique is only available in GDI+ 1.1, which is only distributed with Vista :(((
	if (greyScale) {
		// The common method is to get the gray value of a pixel by the following equation
		// GrayValue = 0.3*Red + 0.59*Green + 0.11*Blue
		Gdiplus::ColorMatrix geryScaleColorConversion;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				geryScaleColorConversion[i][j] = 0.0;
		geryScaleColorConversion(0, 0) = 0.3;
		geryScaleColorConversion(1, 0) = 0.3;
		geryScaleColorConversion(2, 0) = 0.3;
		geryScaleColorConversion(0, 1) = 0.59;
		geryScaleColorConversion(1, 1) = 0.59;
		geryScaleColorConversion(2, 1) = 0.59;
		geryScaleColorConversion(0, 2) = 0.11;
		geryScaleColorConversion(1, 2) = 0.11;
		geryScaleColorConversion(2, 2) = 0.11;
		geryScaleColorConversion(3, 3) = 1.0;
		geryScaleColorConversion(4, 4) = 1.0;
		// Affine transformation matrix: http://msdn.microsoft.com/en-us/library/ms536397(VS.85).aspx
		Gdiplus::RectF srcRectF((float)srcRect.left, (float)srcRect.top, (float)srcWidth, (float)srcHeight);
		Gdiplus::Matrix affineTransform((float)dstRect.Width() / (float)srcWidth, 0.0,
										0.0, (float)dstRect.Height() / (float)srcHeight,
										(float)(dstRect.left - srcRect.left), (float)(dstRect.top - srcRect.top));
		// Greyscale technique (instead of colormatrix): decrease saturation
		// http://weseetips.com/2008/05/27/how-to-convert-images-to-grayscale-by-using-hls-colorspace/
		Gdiplus::HueSaturationLightnessParams hslParams;
		hslParams.hueLevel			= 0;
		hslParams.saturationLevel	= -100;
		hslParams.lightnessLevel	= 0;
		Gdiplus::HueSaturationLightness hslEffect;
		hslEffect.SetParameters(&hslParams);

		st = graphics.DrawImage(m_pImage, &srcRectF, &affineTransform, &hslEffect, NULL, Gdiplus::UnitPixel);
//	} else {*/
		
	bool bGray = (dwModifierFlags & MF_GREYED) != 0 && m_bHasGrayedColor;
	Gdiplus::ImageAttributes imgAttribs;
	if ((dwModifierFlags & MF_TRANSPARENT) != 0 && m_bHasTransparentColor) {
		Gdiplus::Color transparentColor(GetRValue(m_crTransparentColor), GetGValue(m_crTransparentColor), GetBValue(m_crTransparentColor));
		imgAttribs.SetColorKey(transparentColor, transparentColor);
	}
	if (bGray) {
		Gdiplus::ColorMatrix greyFadeColorMatrix = GetGreyFadeMatrix(m_crGrayedColor);
		imgAttribs.SetColorMatrix(&greyFadeColorMatrix);
	}
	Gdiplus::Rect destRect(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
	st = gdip2->DrawImage(m_pImage, destRect, srcRect.left, srcRect.top, srcWidth, srcHeight,
						  Gdiplus::UnitPixel, &imgAttribs);
	if (st == Gdiplus::Win32Error) // in case of corrupted image file (although successfully loaded)
		gdip2->FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::Red), dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
}

void BitmapGen::load( const CString& strName )
{
	if ( ! m_pImage ) {
		if ( ! getFacilities().arePathesValid() )
			return;
		std::vector<CString> vecPathes = getFacilities().getPathes();

		bool success = false;
		CString strFName = strName;

		for ( unsigned int i = 0 ; !success && i < vecPathes.size() ; i++ )
		{
			CString imageFileName = vecPathes[ i ] + strFName;
			bool fileExists = (_taccess(imageFileName, 0) == 0);	// Uses GetFileAttributes
			if (fileExists) {
				m_pImage = Gdiplus::Image::FromFile( CStringW( (LPCTSTR) imageFileName));
				if( m_pImage && m_pImage->GetLastStatus() == Gdiplus::Ok)
				{
					UINT widthToSet = m_pImage->GetWidth();
					UINT heightToSet = m_pImage->GetHeight();
					ASSERT( widthToSet > 0);	// valid sizes, otherwise AutoRouter fails
					ASSERT( heightToSet > 0);
					if (m_pImage->GetType() == Gdiplus::ImageTypeMetafile) {
						int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN);
						int primaryScreenHeight = GetSystemMetrics(SM_CYSCREEN);
						if (widthToSet > 0.9 * primaryScreenWidth ||
							heightToSet > 0.9 * primaryScreenHeight)
						{
							// Override if it's too big
							// Note: if we can figure out the actual size from the wmf/emf file, then remove this limitation
							widthToSet = WIDTH_MODEL;
							heightToSet = HEIGHT_MODEL;
						}
#ifdef _DEBUG
/*						// How many properties (pieces of metadata) are in the image?
						UINT count = m_pImage->GetPropertyCount();
						if (count > 0) {
							// Allocate a buffer to receive an array of PROPIDs.
							PROPID* propIDs = new PROPID[count];

							m_pImage->GetPropertyIdList(count, propIDs);

							// List the retrieved IDs.
							for(UINT j = 0; j < count; ++j)
								TRACE1("%x\n", propIDs[j]);

							delete [] propIDs;
						}*/

/*						Gdiplus::RectF boundsRectF;
						Gdiplus::Unit unit = Gdiplus::UnitPixel;
						m_pImage->GetBounds(&boundsRectF, &unit);
						TRACE2("BoundsRectPixel: %f %f\n", boundsRectF.X, boundsRectF.Y);
						unit = Gdiplus::UnitWorld;
						m_pImage->GetBounds(&boundsRectF, &unit);
						TRACE2("BoundsRectWorld: %f %f\n", boundsRectF.X, boundsRectF.Y);
						TRACE2("Resolution: %f %f\n", m_pImage->GetHorizontalResolution(),
													  m_pImage->GetVerticalResolution());

						Gdiplus::Metafile* metafile = (Gdiplus::Metafile*)m_pImage;
						if (metafile != NULL) {
							Gdiplus::MetafileHeader metafileHeader;
							metafile->GetMetafileHeader(&metafileHeader);
							Gdiplus::Rect rect;
							metafileHeader.GetBounds(&rect);
							TRACE("MetaHeaderBoundsRect: %ld %ld %ld %ld\n", rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetBottom());
							TRACE2("MetaHeaderDPI: %f %f\n", metafileHeader.GetDpiX(), metafileHeader.GetDpiY());
						}*/
#endif
					}
					setSize( widthToSet, heightToSet );
					success = true;
				}
			}
		}

		if( !success && m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
	}
}

bool BitmapGen::isInitialized() const
{
	return m_pImage != NULL;
}

//################################################################################################
//
// CLASS : BitmapRES
//
//################################################################################################

BitmapRES::BitmapRES( UINT uiID  )
	: BitmapBase( createResString( uiID ) ), m_pBitmap( NULL )
{
	load( uiID );
}

BitmapRES::BitmapRES( UINT uiID, COLORREF crColor, bool bIsTransparent )
	: BitmapBase( createResString( uiID ), crColor, bIsTransparent ), m_pBitmap( NULL )
{
	load( uiID );
}

BitmapRES::BitmapRES( UINT uiID, COLORREF crTransparentColor, COLORREF crBackgroundColor )
	: BitmapBase( createResString( uiID ), crTransparentColor, crBackgroundColor ), m_pBitmap( NULL )
{
	load( uiID );
}

BitmapRES::~BitmapRES()
{
	delete m_pBitmap;
	m_pBitmap = NULL;
}

void BitmapRES::draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
					  DWORD dwOpCode, DWORD dwModifierFlags ) const
{
	Gdiplus::Graphics* gdip2 = gdip;
	if (gdip == NULL)
		gdip2 = getFacilities().getGraphics();
	bool bGray = (dwModifierFlags & MF_GREYED) != 0 && m_bHasGrayedColor;
	Gdiplus::ImageAttributes imgAttribs;
	if ((dwModifierFlags & MF_TRANSPARENT) != 0 && m_bHasTransparentColor) {
		Gdiplus::Color transparentColor(GetRValue(m_crTransparentColor), GetGValue(m_crTransparentColor), GetBValue(m_crTransparentColor));
		imgAttribs.SetColorKey(transparentColor, transparentColor);
	}
	if (bGray) {
		Gdiplus::ColorMatrix greyFadeColorMatrix = GetGreyFadeMatrix(m_crGrayedColor);
		imgAttribs.SetColorMatrix(&greyFadeColorMatrix);
	}
	Gdiplus::Rect destRect(dstRect.left, dstRect.top, dstRect.Width(), dstRect.Height());
	gdip2->DrawImage(m_pBitmap, destRect, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height(),
					 Gdiplus::UnitPixel, &imgAttribs);
}

// Accessing the current module's HINSTANCE from a static library http://blogs.msdn.com/b/oldnewthing/archive/2004/10/25/247180.aspx
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

void BitmapRES::load( UINT uiID )
{
	// create a GDI+ Bitmap from the resource
	m_pBitmap = new Gdiplus::Bitmap(HINST_THISCOMPONENT, (WCHAR*) MAKEINTRESOURCE( uiID));
	if (m_pBitmap->GetLastStatus() != Gdiplus::Ok) {
		ASSERT(false);
	}

	setSize(m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
}

bool BitmapRES::isInitialized() const
{
	return m_pBitmap != NULL;
}

//################################################################################################
//
// CLASS : BackgroundTile
//
//################################################################################################

BackgroundTile::BackgroundTile()
	: m_rpPartTopLeft( RefPoint() ), m_rpPartBottomRight( RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 0 ) ) ),
	m_rpTopLeft( RefPoint() ), m_rpBottomRight( RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 0 ) ) ),
	m_eFillVertical( FT_NONE ), m_eFillHorizontal( FT_NONE )
{
}

BackgroundTile::BackgroundTile( const RefPoint& rpPTL, const RefPoint& rpPBR, const RefPoint& rpTL, const RefPoint& rpBR, EFillType eFH, EFillType eFV )
	: m_rpPartTopLeft( rpPTL ), m_rpPartBottomRight( rpPBR ), m_rpTopLeft( rpTL ), m_rpBottomRight( rpBR ), m_eFillVertical( eFV ), m_eFillHorizontal( eFH )
{
}

BackgroundTile::BackgroundTile( const BackgroundTile& bt )
	: m_rpPartTopLeft( bt.m_rpPartTopLeft ), m_rpPartBottomRight( bt.m_rpPartBottomRight ), m_rpTopLeft( bt.m_rpTopLeft ),
	m_rpBottomRight( bt.m_rpBottomRight ), m_eFillVertical( bt.m_eFillVertical ), m_eFillHorizontal( bt.m_eFillHorizontal )
{
}

BackgroundTile& BackgroundTile::operator=( const BackgroundTile& bt )
{
	if ( this != &bt ) {
		m_rpPartTopLeft = bt.m_rpPartTopLeft;
		m_rpPartBottomRight = bt.m_rpPartBottomRight;
		m_rpTopLeft = bt.m_rpTopLeft;
		m_rpBottomRight = bt.m_rpBottomRight;
		m_eFillVertical = bt.m_eFillVertical;
		m_eFillHorizontal = bt.m_eFillHorizontal;
	}
	return *this;
}

RefPoint BackgroundTile::getPartTopLeft() const
{
	return m_rpPartTopLeft;
}

RefPoint BackgroundTile::getPartBottomRight() const
{
	return m_rpPartBottomRight;
}

RefPoint BackgroundTile::getTopLeft() const
{
	return m_rpTopLeft;
}

RefPoint BackgroundTile::getBottomRight() const
{
	return m_rpBottomRight;
}

EFillType BackgroundTile::getHorizontalFill() const
{
	return m_eFillHorizontal;
}

EFillType BackgroundTile::getVerticalFill() const
{
	return m_eFillVertical;
}

}; // namespace DecoratorSDK