//################################################################################################
//
// DecoratorLib Utility Facility
//	DecoratorUtil.h
//
//################################################################################################

#ifndef DecoratorUtil_h
#define DecoratorUtil_h

#pragma warning( disable : 4786 )

#include "StdAfx.h"

#include <vector>
#include <map>

#include <GmeApi.h>

#include <DecoratorDefs.h>

#include "PathUtil.h"

namespace DecoratorSDK
{
	class BitmapBase;
	class Facilities;

	class BackgroundTile;
	typedef std::vector<BackgroundTile> TileVector;

	Facilities& getFacilities();

//################################################################################################
//
// CLASS : Facilities
//
//################################################################################################

	class Facilities
	{
		private :
			CComPtr<IMgaProject>				m_spProject;
			PathUtil m_pathUtil;
			CDC									m_nullDC;
			Gdiplus::Graphics*					m_gdip;
			Gdiplus::SmoothingMode				m_eEdgeAntiAlias;		// Edge smoothing mode
			Gdiplus::TextRenderingHint			m_eFontAntiAlias;		// Text renndering hint mode

			std::map<CString,BitmapBase*>			m_mapBitmaps;
			std::map<CString,TileVector*>			m_mapTileVectors;
			std::map<int,GdipFont*>					m_mapFonts;
			std::map<CString,Gdiplus::Pen*>			m_mapPens;
			std::map<CString,Gdiplus::SolidBrush*>	m_mapBrushes;

		public :
			Facilities();
			~Facilities();

			bool loadPathes(IMgaProject* pProject, bool bRefresh = false);
			bool arePathesValid() const;
			std::vector<CString> getPathes(void) const;
			Gdiplus::Graphics* getGraphics(void) const;
			CDC* getCDC(void);

			BitmapBase* getBitmap( const CString& strName );
			BitmapBase* getBitmapT( const CString& strName, COLORREF crTransparent );
			BitmapBase* getBitmapB( const CString& strName, COLORREF crBackground );
			BitmapBase* getBitmapTB( const CString& strName, COLORREF crTransparent, COLORREF crBackground );
			BitmapBase* getMaskedBitmap( const CString& strName, COLORREF crTransparent, COLORREF crBackground );
			BitmapBase* getMaskedBitmap( UINT nResID, COLORREF crTransparent, COLORREF crBackground );

			void addTileVector( const CString& strName, TileVector* vecTiles );
			TileVector* getTileVector( const CString& strName ) const;

			void createFont( int iFontKey, const CString& strKind, int iBoldness, bool bItalics, int iSize );
			GdipFont* GetFont( int iFontKey ) const;
			Gdiplus::Pen* GetPen( COLORREF crColor, int iWidth = 1 );
			Gdiplus::SolidBrush* GetBrush( COLORREF crColor );

			bool getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, CString& strValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, long& lValue, bool bInHexa = false ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, COLORREF& crValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, ELocation& eValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, bool& bValue ) const;
			bool getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, CString& strValue ) const;
			bool getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, long& lValue, bool bInHexa = false ) const;
			bool getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, COLORREF& crValue ) const;
			bool getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, ELocation& eValue ) const;
			bool getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, bool& bValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, CString& strValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, long& lValue, bool bInHexa = false ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, COLORREF& crValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, ELocation& eValue ) const;
			bool getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, bool& bValue ) const;

			EPrefStatus getPreferenceStatus( CComPtr<IMgaFCO> spFCO, const CString& strName ) const;

			bool getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, CString& strValue ) const;
			bool getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, bool& bValue ) const;
			bool getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, long& lValue ) const;
			bool getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, double& fValue ) const;
			bool setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, const CString& strValue ) const;
			bool setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, bool bValue ) const;
			bool setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, long lValue ) const;
			bool setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, double fValue ) const;

			bool getMetaFCO(const CComPtr<IMgaMetaPart> &metaPart, CComPtr<IMgaMetaFCO> &metaFco) const;

			CSize MeasureText( Gdiplus::Graphics* gdip, GdipFont* pFont, const CString& strText) const;
			CSize MeasureText( Gdiplus::Graphics* gdip, Gdiplus::Font* pFont, const CString& strText) const;
			void DrawString( Gdiplus::Graphics* gdip, const CString& strText, const CRect& crBounds, GdipFont* pFont,
							 COLORREF crColor, int iAlign, int iLength = -1, const CString& strPre = "",
							 const CString& strPost = "", bool bPeriods = true ) const;
			void DrawString( Gdiplus::Graphics* gdip, const CString& strText, const CRect& crBounds, Gdiplus::Font* pFont,
							 COLORREF crColor, int iAlign, int iLength = -1, const CString& strPre = "",
							 const CString& strPost = "", bool bPeriods = true ) const;
			void DrawRect( Gdiplus::Graphics* gdip, const CRect& cRect, COLORREF crColor, int iWidth = 1,
						   bool bRoundRect = false, int iCornerRadius = 0 ) const;
			void DrawBox( Gdiplus::Graphics* gdip, const CRect& cRect, COLORREF crColor, int iDepth,
						  bool bRoundRect = false, int iCornerRadius = 0 ) const;
			COLORREF ShiftColor( COLORREF crColor, int iShift ) const;
			double Deg2Rad( long deg ) const;

			std::vector<CString> wrapString( const CString& str, int iWrap, int iMax ) const;
			CString getStereotyped( const CString& str ) const;

		private :
			BitmapBase* getBitmap( const CString& strName, bool bhasTC, COLORREF crTC, bool bhasBC, COLORREF crBC,
								   bool masked = false, UINT nResID = 0 );

	};

}; // namespace DecoratorSDK

#endif // DecoratorUtil_h