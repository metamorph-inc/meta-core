//################################################################################################
//
// Mga Decorator Base
//	BitmapUtil.h
//
//################################################################################################

#ifndef BitmapUtil_h
#define BitmapUtil_h

#pragma warning( disable : 4786 )

#include "StdAfx.h"
#include "DIBAPI.h"
#include <vector>

namespace DecoratorSDK
{

//################################################################################################
//
// BITFLAGS : OpCode
//
//################################################################################################

const unsigned int MF_NOOP			= 0;
const unsigned int MF_GREYED		= 1;
const unsigned int MF_TRANSPARENT	= 2;

//################################################################################################
//
// ENUMERATION : ECoordRefPoint
//
//################################################################################################

enum ECoordRefPoint
{
	CRP_BEGIN	= -1 ,
	CRP_CENTER	= 0 ,
	CRP_END	 	= 1
};

//################################################################################################
//
// CLASS : RefCoord
//
//################################################################################################

class RefCoord
{
	private :
		ECoordRefPoint	m_eType;
		long				m_lPos;

	public :
		RefCoord();
		RefCoord( ECoordRefPoint eType, long lPos );
		RefCoord( const RefCoord& rc );
		RefCoord& operator=( const RefCoord& rc );

		ECoordRefPoint getRef() const;
		long getPos() const;

		long calcPos( long lLength );
};

//################################################################################################
//
// CLASS : RefPoint
//
//################################################################################################

class RefPoint
{
	private :
		RefCoord 	m_rcX;
		RefCoord	m_rcY;

	public :
		RefPoint();
		RefPoint( const RefCoord& rcX, const RefCoord& rcY );
		RefPoint( const RefPoint& rp );
		RefPoint& operator=( const RefPoint rp );

		RefCoord getX() const;
		RefCoord getY() const;

		CPoint calcPoint( long lWidth, long lHeight );
};

//################################################################################################
//
// ENUMERATION : EFillType
//
//################################################################################################

enum EFillType
{
	FT_NONE = 0,
	FT_STRETCH,
	FT_TILE
};

//################################################################################################
//
// CLASS : BackgroundTile
//
//################################################################################################

class BackgroundTile
{
	private :
		RefPoint	m_rpPartTopLeft;
		RefPoint	m_rpPartBottomRight;
		RefPoint	m_rpTopLeft;
		RefPoint	m_rpBottomRight;
		EFillType	m_eFillVertical;
		EFillType	m_eFillHorizontal;

	public :
		BackgroundTile();
		BackgroundTile( const RefPoint& rpPTL, const RefPoint& rpPBR, const RefPoint& rpTL, const RefPoint& rpBR, EFillType eFH, EFillType eFV );
		BackgroundTile( const BackgroundTile& bt );
		BackgroundTile& operator=( const BackgroundTile& bt );

		RefPoint getPartTopLeft() const;
		RefPoint getPartBottomRight() const;
		RefPoint getTopLeft() const;
		RefPoint getBottomRight() const;
		EFillType getHorizontalFill() const;
		EFillType getVerticalFill() const;
};

typedef std::vector<BackgroundTile> TileVector;

//################################################################################################
//
// CLASS : BitmapBase
//
//################################################################################################

class BitmapBase
{
	protected :
		bool 		m_bHasTransparentColor;
		COLORREF	m_crTransparentColor;
		bool		m_bHasBackgroundColor;
		COLORREF	m_crBackgroundColor;
		bool		m_bHasGrayedColor;
		COLORREF	m_crGrayedColor;
		long 		m_lWidth;
		long 		m_lHeight;
		CString		m_strName;

	protected :
		BitmapBase( const CString& strName );
		BitmapBase( const CString& strName, COLORREF crColor, bool bIsTransparent );
		BitmapBase( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor );
		BitmapBase( const CString& strName, COLORREF crTransparentColor, COLORREF crGrayedColor, bool isMasked );

	public :
		virtual ~BitmapBase();

		CString getName() const;
		virtual long getWidth() const;
		virtual long getHeight() const;
		virtual bool isInitialized() const = 0;

		bool hasTransparentColor() const;
		bool hasBackgroundColor() const;
		COLORREF getTransparentColor() const;
		COLORREF getBackgroundColor() const;

		virtual void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						   DWORD dwOpCode, DWORD dwModifierFlags = MF_NOOP ) const = 0;
				void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& cRect, const TileVector& vecTiles,
						   DWORD dwModifierFlags = MF_NOOP ) const;

	protected :
		void setSize( long lWidth, long lHeight );
		void setName( const CString& strName );
		Gdiplus::ColorMatrix GetGreyFadeMatrix(COLORREF greyColor) const;
};

//################################################################################################
//
// CLASS : BitmapDIB
//
//################################################################################################

class BitmapDIB
	: public BitmapBase
{
	private :
		HDIB	m_hDIB;

	public :
		BitmapDIB( const CString& strName );
		BitmapDIB( const CString& strName, COLORREF crColor, bool bIsTransparent );
		BitmapDIB( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor );
		virtual ~BitmapDIB();

	public :
		virtual bool isInitialized() const;
		virtual void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						   DWORD dwOpCode, DWORD dwModifierFlags = MF_NOOP ) const;

	private :
		HBITMAP DIBToBitmap(HDIB hDIB, HPALETTE hPal = NULL);
		void load( const CString& strName );
};

//################################################################################################
//
// CLASS : BitmapMasked
//
//################################################################################################

class BitmapMasked
	: public BitmapBase
{
	private :
		LPBYTE			m_pBits;
		LPBITMAPINFO	m_pBMI;
		CPalette*		m_pPalette;

	public :
		BitmapMasked( const CString& strName, COLORREF crTransparentColor, COLORREF crGrayColor );
		BitmapMasked( UINT nResID, COLORREF crTransparentColor, COLORREF crGrayColor );
		virtual ~BitmapMasked();

	public :
		virtual bool isInitialized() const;
		virtual void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						   DWORD dwOpCode, DWORD dwModifierFlags = MF_NOOP ) const;

		virtual long getWidth() const;
		virtual long getHeight() const;
	private :
		DWORD Read(CFile& file, BOOL bFromResource = FALSE );
		DWORD ReadFromResource(UINT nResID);
		WORD NumColors(  BITMAPINFOHEADER& bmiHeader ) const;
		void Free();
		BOOL CreatePalette();
		void drawTransparent (Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect &dstRect,
							  COLORREF clrTransparency, bool bGray, COLORREF grayColor) const;
		void draw (Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect &dstRect,
				   bool bGray, COLORREF grayColor) const;
};

//################################################################################################
//
// CLASS : BitmapGen
//			this class is suitable for all image formats: BMP, GIF, JPEG, PNG, TIFF, EMF
//			however we will use it only for BMP, GIF, JPEG, PNG
//			
//################################################################################################

class BitmapGen
	: public BitmapBase
{
	private :
		Gdiplus::Image*		m_pImage;

	public :
		BitmapGen( const CString& strName );
		BitmapGen( const CString& strName, COLORREF crColor, bool bIsTransparent );
		BitmapGen( const CString& strName, COLORREF crTransparentColor, COLORREF crBackgroundColor );
		~BitmapGen();

	public :
		virtual bool isInitialized() const;
		virtual void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						   DWORD dwOpCode, DWORD dwModifierFlags = MF_NOOP ) const;

	private :
		void load( const CString& strName );
};

//################################################################################################
//
// CLASS : BitmapRES
//
//################################################################################################

class BitmapRES
	: public BitmapBase
{
	private :
		Gdiplus::Bitmap*  m_pBitmap;

	public :
		BitmapRES( UINT uiID );
		BitmapRES( UINT uiID, COLORREF crColor, bool bIsTransparent );
		BitmapRES( UINT uiID, COLORREF crTransparentColor, COLORREF crBackgroundColor );
		~BitmapRES();

	public :
		virtual bool isInitialized() const;
		virtual void draw( Gdiplus::Graphics* gdip, CDC* pDC, const CRect& srcRect, const CRect& dstRect,
						   DWORD dwOpCode, DWORD dwModifierFlags = MF_NOOP ) const;

	private :
		void load( UINT uiID );
};

}; // namespace DecoratorSDK

#endif // BitmapUtil_h