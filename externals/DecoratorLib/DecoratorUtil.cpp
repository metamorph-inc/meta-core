//################################################################################################
//
// DecoratorLib Utility Facility
//	DecoratorUtil.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "DecoratorUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "DecoratorDefs.h"
#include "GmeApi.h"
#include "BitmapUtil.h"


static long stereotypeCharacterType = 0;

namespace {
	bool isRunningUnderWine() {
		// http://wiki.winehq.org/DeveloperFaq "How can I detect Wine?" "This is a bad idea."
		static bool haveRunWineDetection = false;
		static bool isWine = false;
		if (!haveRunWineDetection) {
			haveRunWineDetection = true;
			HMODULE ntdll = LoadLibrary(_T("ntdll.dll"));
			if (ntdll != 0) {
				isWine = GetProcAddress(ntdll, "wine_get_version") != 0;
				FreeLibrary(ntdll);
			}
		}
		return isWine;
	}

}

namespace DecoratorSDK
{
	Facilities facilities;

	Facilities& getFacilities()
	{
		return facilities;
	}

	bool Convert( const CString& strValue, long& lValue, bool bIsHexa )
	{
		return _stscanf( strValue, ( bIsHexa ) ? _T("%x") : _T("%ld"), &lValue ) == 1;
	}

	bool Convert( const CString& strValue, COLORREF& crValue )
	{
		long lValue;
		if ( ! Convert( strValue, lValue, true ) ) {
			crValue = 0x0;
			return false;
		}
		unsigned int uiR = ( lValue & 0xFF0000 ) >> 16;
		unsigned int uiG = ( lValue  & 0xFF00 ) >> 8;
		unsigned int uiB = ( lValue & 0xFF );
		crValue = RGB( uiR, uiG, uiB );
		return true;
	}

	bool Convert( const CString& strValueIn, ELocation& eValue )
	{
		int iValue = 0;
		if ( _stscanf( strValueIn, _T("%d"), &iValue ) == 1 ) {
			eValue = (ELocation) iValue;
			return true;
		}
		CString strValue = strValueIn;
		strValue.TrimLeft( _T("\t ") );
		strValue.TrimRight( _T("\t ") );
		if ( strValue == _T("N")) {
			eValue = L_NORTH;
			return true;
		}
		if ( strValue == _T("NE")) {
			eValue = L_NORTHEAST;
			return true;
		}
		if ( strValue == _T("E")) {
			eValue = L_EAST;
			return true;
		}
		if ( strValue == _T("SE")) {
			eValue = L_SOUTHEAST;
			return true;
		}
		if ( strValue == _T("S")) {
			eValue = L_SOUTH;
			return true;
		}
		if ( strValue == _T("SW")) {
			eValue = L_SOUTHWEST;
			return true;
		}
		if ( strValue == _T("W")) {
			eValue = L_WEST;
			return true;
		}
		if ( strValue == _T("NW")) {
			eValue = L_NORTHWEST;
			return true;
		}
		if ( strValue ==_T("C")) {
			eValue = L_CENTER;
			return true;
		}
		return false;
	}

//################################################################################################
//
// CLASS : Facilities
//
//################################################################################################

Facilities::Facilities()
	: m_spProject( NULL )
{
	m_nullDC.CreateCompatibleDC(NULL);
	m_gdip = NULL;	// Create the Gdiplus::Graphics object later, cause at this point GdiplusStartup may not be called by GMEApp
	m_eEdgeAntiAlias = Gdiplus::SmoothingModeHighQuality;
	m_eFontAntiAlias = Gdiplus::TextRenderingHintAntiAlias;

	CComPtr<IMgaRegistrar> registrar;
	COMTHROW(registrar.CoCreateInstance(OLESTR("Mga.MgaRegistrar")));
	ASSERT(registrar != NULL);
	edgesmoothmode_enum edgeSmoothMode;
	COMTHROW(registrar->get_EdgeSmoothMode(REGACCESS_USER, &edgeSmoothMode));
	m_eEdgeAntiAlias = (Gdiplus::SmoothingMode)edgeSmoothMode;
	fontsmoothmode_enum fontSmoothMode;
	COMTHROW(registrar->get_FontSmoothMode(REGACCESS_USER, &fontSmoothMode));
	m_eFontAntiAlias = (Gdiplus::TextRenderingHint)fontSmoothMode;

	createFont( FONT_LABEL,			_T("Arial"), FW_NORMAL,		false,	16 );
	createFont( FONT_PORT,			_T("Arial"), FW_BOLD,		false,	12 );
	createFont( FONT_TYPE,			_T("Arial"), FW_NORMAL,		false,	12 );
	createFont( FONT_PORT_OUTSIDE,	_T("Arial"), FW_BOLD,		false,	11 );
	createFont( FONT_PORTNAME,		_T("Arial"), FW_NORMAL,		false,	15 );
	createFont( FONT_CONNLABEL,		_T("Arial"), FW_LIGHT,		false,	12 );
	createFont( FONT_ABSTRACT,		_T("Arial"), FW_NORMAL,		true,	15 );

	TileVector* pTileVector = new TileVector();

	RefPoint rpTL2( RefCoord( CRP_BEGIN, 0 ), RefCoord( CRP_BEGIN, 0 ) );
	RefPoint rpBR2( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 0 ) );
	pTileVector->push_back( BackgroundTile( rpTL2, rpBR2, rpTL2, rpBR2, FT_STRETCH, FT_STRETCH ) );

//	TileVector* pTileVector = new TileVector( 1, BackgroundTile() );
	addTileVector( TILE_ATOMDEFAULT, pTileVector );

	pTileVector = new TileVector();

	RefPoint rpTL( RefCoord( CRP_BEGIN, 0 ), RefCoord( CRP_BEGIN, 0 ) );
	RefPoint rpBR( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_BEGIN, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_NONE ) );
	rpTL = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_BEGIN, 0 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_BEGIN, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_NONE ) );
	rpTL = RefPoint( RefCoord( CRP_BEGIN, 0 ), RefCoord( CRP_END, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_END, 0 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_NONE ) );
	rpTL = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_END, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 0 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_NONE ) );

	rpTL = RefPoint( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_BEGIN, 0 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_BEGIN, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_STRETCH, FT_NONE ) );
	rpTL = RefPoint( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_END, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_END, 0 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_STRETCH, FT_NONE ) );
	rpTL = RefPoint( RefCoord( CRP_BEGIN, 0 ), RefCoord( CRP_BEGIN, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_END, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_STRETCH ) );
	rpTL = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_BEGIN, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_NONE, FT_STRETCH ) );

	rpTL = RefPoint( RefCoord( CRP_BEGIN, 5 ), RefCoord( CRP_BEGIN, 5 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 5 ), RefCoord( CRP_END, 5 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_STRETCH, FT_STRETCH ) );

	addTileVector( TILE_MODELDEFAULT, pTileVector );

	pTileVector = new TileVector();

	rpTL = RefPoint( RefCoord( CRP_BEGIN, 0 ), RefCoord( CRP_BEGIN, 0 ) );
	rpBR = RefPoint( RefCoord( CRP_END, 0 ), RefCoord( CRP_END, 0 ) );
	pTileVector->push_back( BackgroundTile( rpTL, rpBR, rpTL, rpBR, FT_STRETCH, FT_STRETCH ) );

	addTileVector( TILE_PORTDEFAULT, pTileVector );
}

Facilities::~Facilities()
{
	for ( std::map<CString,BitmapBase*>::iterator it = m_mapBitmaps.begin() ; it != m_mapBitmaps.end() ; ++it )
		delete it->second;
	for ( std::map<CString,TileVector*>::iterator it2 = m_mapTileVectors.begin() ; it2 != m_mapTileVectors.end() ; ++it2 )
		delete it2->second;
	for ( std::map<int,GdipFont*>::iterator it3 = m_mapFonts.begin() ; it3 != m_mapFonts.end() ; ++it3 ) {
		delete it3->second->gdipFont;
		delete it3->second;
	}
	for ( std::map<CString,Gdiplus::Pen*>::iterator it4 = m_mapPens.begin() ; it4 != m_mapPens.end() ; ++it4 )
		delete it4->second;
	for ( std::map<CString,Gdiplus::SolidBrush*>::iterator it5 = m_mapBrushes.begin() ; it5 != m_mapBrushes.end() ; ++it5 )
		delete it5->second;

	delete m_gdip;
}

bool Facilities::loadPathes( IMgaProject* pProject, bool bRefresh )
{
	if (m_gdip == NULL) {
		m_gdip = new Gdiplus::Graphics(m_nullDC.m_hDC);
		if (m_gdip == NULL)
			throw hresult_exception(E_NOT_VALID_STATE);
		m_gdip->SetPageUnit(Gdiplus::UnitPixel);
		m_gdip->SetSmoothingMode(m_eEdgeAntiAlias);
		m_gdip->SetTextRenderingHint(m_eFontAntiAlias);
	}

	if (!m_spProject || !m_spProject.IsEqualObject(pProject))
		m_spProject = pProject;
	else if (m_pathUtil.arePathsValid())
		return true;

	return m_pathUtil.loadPaths(pProject, bRefresh);
}

bool Facilities::arePathesValid() const
{
	return m_pathUtil.arePathsValid();
}

std::vector<CString> Facilities::getPathes() const
{
	return m_pathUtil.getPaths();
}

Gdiplus::Graphics* Facilities::getGraphics(void) const
{
	return m_gdip;
}

CDC* Facilities::getCDC(void)
{
	return &m_nullDC;
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, CString& strValue ) const
{
	return getPreference(spFCO, NULL, strName, strValue);
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, long& lValue, bool bInHexa ) const
{
	return getPreference(spFCO, NULL, strName, lValue, bInHexa);
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, COLORREF& crValue ) const
{
	return getPreference(spFCO, NULL, strName, crValue);
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, ELocation& eValue ) const
{
	return getPreference(spFCO, NULL, strName, eValue);
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, const CString& strName, bool& bValue ) const
{
	return getPreference(spFCO, NULL, strName, bValue);
}

bool Facilities::getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, CString& strValue ) const
{
	return getPreference(NULL, spMetaFCO, strName, strValue);
}

bool Facilities::getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, long& lValue, bool bInHexa ) const
{
	return getPreference(NULL, spMetaFCO, strName, lValue, bInHexa);
}

bool Facilities::getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, COLORREF& crValue ) const
{
	return getPreference(NULL, spMetaFCO, strName, crValue);
}

bool Facilities::getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, ELocation& eValue ) const
{
	return getPreference(NULL, spMetaFCO, strName, eValue);
}

bool Facilities::getPreference( CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, bool& bValue ) const
{
	return getPreference(NULL, spMetaFCO, strName, bValue);
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, CString& strValue ) const
{
	CComBSTR bstrPath;
	CopyTo( strName, bstrPath );
	CComBSTR bstrValue;

	try {
		if (spFCO)
			COMTHROW( spFCO->get_RegistryValue( bstrPath, &bstrValue ) );
		else if (spMetaFCO)
			COMTHROW( spMetaFCO->get_RegistryValue( bstrPath, &bstrValue ) );
		else
			; // assertion fails for Annotator.dll ASSERT(false);
	}
	catch (hresult_exception &) {
		bstrValue.Empty();
	}

	CString strValueT = bstrValue;
	if ( ! strValueT.IsEmpty() )
		strValue = strValueT;
	return ! strValueT.IsEmpty();
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, long& lValue, bool bInHexa ) const
{
	CString strValue;
	if ( ! getPreference( spFCO, spMetaFCO, strName, strValue ) )
		return false;
	long lValueT;
	if ( ! Convert( strValue, lValueT, bInHexa ) )
		return false;
	lValue = lValueT;
	return true;
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, COLORREF& crValue ) const
{
	CString strValue;
	if ( ! getPreference( spFCO, spMetaFCO, strName, strValue ) )
		return false;
	COLORREF crValueT;
	if ( ! Convert( strValue, crValueT ) )
		return false;
	crValue = crValueT;
	return true;
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, ELocation& eValue ) const
{
	CString strValue;
	if ( ! getPreference( spFCO, spMetaFCO, strName, strValue ) )
		return false;
		ELocation eValueT;
	if ( ! Convert( strValue, eValueT ) )
		return false;
	eValue = eValueT;
	return true;
}

bool Facilities::getPreference( CComPtr<IMgaFCO> spFCO, CComPtr<IMgaMetaFCO> spMetaFCO, const CString& strName, bool& bValue ) const
{
	CString strValue;
	if ( ! getPreference( spFCO, spMetaFCO, strName, strValue ) )
		return false;
	strValue.MakeLower();
	bValue = strValue == _T("t") || strValue == _T("true") || strValue == _T("1");
	return true;
}

EPrefStatus Facilities::getPreferenceStatus( CComPtr<IMgaFCO> spFCO, const CString& strName ) const
{
	CString strValue;
	if ( ! getPreference( spFCO, strName, strValue ) )
		return PS_UNDEFINED;

	CComPtr<IMgaRegNode> spNode;
	CComBSTR bstrName;
	CopyTo( strName, bstrName );
	COMTHROW( spFCO->get_RegistryNode( bstrName, &spNode ) );

	long lStatus;
	COMTHROW( spNode->get_Status( &lStatus ) );
	switch ( lStatus ) {
		case 0 : return PS_HERE;
		case -1 : return PS_META;
		default : return PS_INHERITED;
	}
}

bool Facilities::getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, CString& strValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );
	CComBSTR bstrValue;

	try {
		COMTHROW(spFCO->get_StrAttrByName(attrname, &bstrValue));
	}
	catch (hresult_exception &) {
		bstrValue.Empty();
	}

	CString strValueT = bstrValue;
	if ( ! strValueT.IsEmpty() )
		strValue = strValueT;
	return ! strValueT.IsEmpty();
}

bool Facilities::getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, bool& bValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );
	VARIANT_BOOL vval;

	try {
		COMTHROW(spFCO->get_BoolAttrByName(attrname, &vval));
	}
	catch (hresult_exception &) {
		return false;
	}

	bValue = (vval == VARIANT_TRUE);
	return true;
}

bool Facilities::getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, long& lValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );

	try {
		COMTHROW(spFCO->get_IntAttrByName(attrname, &lValue));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::getAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, double& fValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );

	try {
		COMTHROW(spFCO->get_FloatAttrByName(attrname, &fValue));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, const CString& strValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );
	CComBSTR bstrValue;
	CopyTo( strValue, bstrValue );

	try {
		COMTHROW(spFCO->put_StrAttrByName(attrname, bstrValue));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, bool bValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );
	VARIANT_BOOL vval = bValue ? VARIANT_TRUE : VARIANT_FALSE;

	try {
		COMTHROW(spFCO->put_BoolAttrByName(attrname, vval));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, long lValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );

	try {
		COMTHROW(spFCO->put_IntAttrByName(attrname, lValue));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::setAttribute( CComPtr<IMgaFCO> spFCO, const CString& strName, double fValue ) const
{
	if (!spFCO) {
		return false;
	}
	CComBSTR attrname;
	CopyTo( strName, attrname );

	try {
		COMTHROW(spFCO->put_FloatAttrByName(attrname, fValue));
	}
	catch (hresult_exception &) {
		return false;
	}

	return true;
}

bool Facilities::getMetaFCO(const CComPtr<IMgaMetaPart> &metaPart, CComPtr<IMgaMetaFCO> &metaFco) const
{
	if (!metaPart) {
		return false;
	}

	metaFco = NULL;
	CComPtr<IMgaMetaRole> metaRole;
	try {
		COMTHROW(metaPart->get_Role(&metaRole));
		VERIFY(metaRole);
		COMTHROW(metaRole->get_Kind(&metaFco));
	}
	catch (hresult_exception &) {
		metaFco = NULL;
	}
	return (metaFco != NULL);
}

BitmapBase* Facilities::getBitmap( const CString& strName, bool bHasTC, COLORREF crTC, bool bHasBC, COLORREF crBC,
								   bool masked, UINT nResID )
{
	CString strID( strName );
	if ( masked ) {
		if (nResID != 0) {
			TCHAR chBuffer[ 10 ];
			_ultot( nResID, chBuffer, 10 );
			strID += chBuffer;
		}
		strID += _T("<Masked>");
	}
	if ( bHasTC ) {
		TCHAR chBuffer[ 10 ];
		_ultot( crTC, chBuffer, 16 );
		strID += _T("<TC:") + CString( chBuffer ) + _T(">");
	}
	if ( bHasBC ) {
		TCHAR chBuffer[ 10 ];
		_ultot( crBC, chBuffer, 16 );
		strID += _T("<BC:") + CString( chBuffer ) + _T(">");
	}

	std::map<CString,BitmapBase*>::iterator it = m_mapBitmaps.find( strID );
	if ( it != m_mapBitmaps.end() )
		return it->second;

	if (masked) {
		BitmapMasked* pBMP = NULL;
		if ( nResID != NULL )
			pBMP = new BitmapMasked( nResID, crTC, crBC );
		else
			pBMP = new BitmapMasked( strName, crTC, crBC );
		m_mapBitmaps.insert( std::map<CString,BitmapBase*>::value_type( strID, pBMP ) );
		if ( pBMP->isInitialized() ) {
			return pBMP;
		}
		return NULL;
	}

	CString strExt, strEx2;
	if ( strName.GetLength() >= 4 ) {
		strExt = strName.Right( 4 ).MakeLower();
	
		if( strName.GetLength() >= 5) // in case of ".jpeg"
			strEx2 = strName.Right(5).MakeLower();
		
		if ( strExt != _T(".bmp") && strExt != _T(".gif") && strExt != _T(".png")
			&& strExt != _T(".jpg") && strExt != _T(".jpe") && strEx2 != _T(".jpeg")
			&& strExt != _T(".res")  && strExt != _T(".wmf") && strExt != _T(".emf"))
			strExt = _T("");
	}

	if( strExt.IsEmpty()) // if no extension then try the different formats
	{
		bool success = false;
		static TCHAR * exts[] = { _T(".bmp"), _T(".gif"), _T(".png"), _T(".jpg"), _T(".jpe"), _T(".jpeg"), _T(".wmf"), _T(".emf") };
		for( int i = 0; !success && i < 6; ++i)
		{
			CString strName2 = strName + exts[i];
			BitmapGen* pBMP = NULL;
			if ( bHasTC )
				pBMP = ( bHasBC ) ? new BitmapGen( strName2, crTC, crBC ) : new BitmapGen( strName2, crTC, true );
			else
				pBMP = ( bHasBC ) ? new BitmapGen( strName2, crBC, false ) : new BitmapGen( strName2);
			if ( pBMP->isInitialized() ) {
				m_mapBitmaps.insert( std::map<CString,BitmapBase*>::value_type( strID, pBMP ) );
				success = true;
				return pBMP;
			}
			delete pBMP;
		}
	}
	else if( strExt != _T(".res")) // if regular file use name as is
	{
		BitmapGen* pBMP = NULL;
		if ( bHasTC )
			pBMP = ( bHasBC ) ? new BitmapGen( strName, crTC, crBC ) : new BitmapGen( strName, crTC, true );
		else
			pBMP = ( bHasBC ) ? new BitmapGen( strName, crBC, false ) : new BitmapGen( strName);
		if ( pBMP->isInitialized() ) {
			m_mapBitmaps.insert( std::map<CString,BitmapBase*>::value_type( strID, pBMP ) );
			return pBMP;
		}
		delete pBMP;
	}

	if ( strExt == _T(".res") || strExt.IsEmpty() ) {
		int iID = _ttoi( strName.Left( strName.GetLength() - ( ( strExt.IsEmpty() ) ? 0 : 4 ) ) );
		if( iID == 0)    // might be true if specified icon filename
			return NULL; // has no extension and not found yet
		BitmapRES* pBMP = NULL;
		if ( bHasTC )
			pBMP = ( bHasBC ) ? new BitmapRES( iID, crTC, crBC ) : new BitmapRES( iID, crTC, true );
		else
			pBMP = ( bHasBC ) ? new BitmapRES( iID, crBC, false ) : new BitmapRES( iID );
		if ( pBMP->isInitialized() ) {
			m_mapBitmaps.insert( std::map<CString,BitmapBase*>::value_type( strID, pBMP ) );
			return pBMP;
		}
		delete pBMP;
	}
	m_mapBitmaps.insert( std::map<CString,BitmapBase*>::value_type( strID, static_cast<BitmapBase*>(NULL) ) );
	return NULL;
}

BitmapBase* Facilities::getBitmap( const CString& strName )
{
	return getBitmap( strName, false, 0x0, false, 0x0 );
}

BitmapBase* Facilities::getBitmapT( const CString& strName, COLORREF crTransparent )
{
	return getBitmap( strName, true, crTransparent, false, 0x0 );
}

BitmapBase* Facilities::getBitmapB( const CString& strName, COLORREF crBackground )
{
	return getBitmap( strName, false, 0x0, true, crBackground );
}

BitmapBase* Facilities::getBitmapTB( const CString& strName, COLORREF crTransparent, COLORREF crBackground )
{
	return getBitmap( strName, true, crTransparent, true, crBackground );
}

BitmapBase* Facilities::getMaskedBitmap( const CString& strName, COLORREF crTransparent, COLORREF crBackground )
{
	return getBitmap( strName, true, crTransparent, true, crBackground, true );
}

BitmapBase* Facilities::getMaskedBitmap( UINT nResID, COLORREF crTransparent, COLORREF crBackground )
{
	return getBitmap( _T(""), true, crTransparent, true, crBackground, true, nResID );
}

void Facilities::addTileVector( const CString& strName, TileVector* vecTiles )
{
	TileVector* vecStored = getTileVector( strName );
	if ( vecStored )
		delete vecStored;
	m_mapTileVectors[ strName ] = vecTiles;
}

TileVector* Facilities::getTileVector( const CString& strName ) const
{
	std::map<CString,TileVector*>::const_iterator it = m_mapTileVectors.find( strName );
	return ( it == m_mapTileVectors.end() ) ? NULL : it->second;
}

void Facilities::createFont( int iFontKey, const CString& strKind, int iBoldness, bool bItalics, int iSize )
{
	GdipFont* pgFont = GetFont( iFontKey );
	if ( pgFont ) {
		delete pgFont->gdipFont;
		delete pgFont;
	}

/*	int fontStyle = Gdiplus::FontStyleRegular;
	if (iBoldness == FW_BOLD)
		fontStyle |= Gdiplus::FontStyleBold;
	if (bItalics)
		fontStyle |= Gdiplus::FontStyleItalic;

	float pixelSize = static_cast<float> (iSize * 72.0 / GetDeviceCaps(GetDC(NULL), LOGPIXELSY));
	m_mapFonts[ iFontKey ] = new GdipFont( strKind, iSize, iBoldness == FW_BOLD, bItalics );
	CA2W wcTxt(strKind);
	m_mapFonts[ iFontKey ]->gdipFont = new Gdiplus::Font( wcTxt, pixelSize, fontStyle, Gdiplus::UnitPixel );*/

	m_mapFonts[ iFontKey ] = new GdipFont( strKind, iSize, iBoldness == FW_BOLD, bItalics );

	CFont* pFont = new CFont();
	pFont->CreateFont( iSize, 0, 0, 0, iBoldness, bItalics, 0, 0, ANSI_CHARSET, OUT_DEVICE_PRECIS,
					   CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_SWISS, strKind );
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	m_mapFonts[ iFontKey ]->gdipFont = new Gdiplus::Font( getCDC()->m_hDC, &logFont );

	pFont->DeleteObject();
	delete pFont;
}

GdipFont* Facilities::GetFont( int iFontKey ) const
{
	std::map<int,GdipFont*>::const_iterator it = m_mapFonts.find( iFontKey );
	return ( it == m_mapFonts.end() ) ? NULL : it->second;
}

Gdiplus::Pen* Facilities::GetPen( COLORREF crColor, int iWidth )
{
	CString chBuffer;
	chBuffer.Format(_T("%x-%ld"), crColor, iWidth);
	std::map<CString,Gdiplus::Pen*>::iterator it = m_mapPens.find(chBuffer);
	if (it != m_mapPens.end())
		return it->second;
	Gdiplus::Pen* pPen = new Gdiplus::Pen(Gdiplus::Color(GetRValue(crColor), GetGValue(crColor), GetBValue(crColor)), static_cast<float> (iWidth));
	m_mapPens.insert( std::map<CString,Gdiplus::Pen*>::value_type(chBuffer, pPen));
	return pPen;
}

Gdiplus::SolidBrush* Facilities::GetBrush( COLORREF crColor )
{
	CString chBuffer;
	chBuffer.Format(_T("%x"), crColor);
	std::map<CString,Gdiplus::SolidBrush*>::iterator it = m_mapBrushes.find(chBuffer);
	if (it != m_mapBrushes.end())
		return it->second;
	Gdiplus::SolidBrush* pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetRValue(crColor), GetGValue(crColor), GetBValue(crColor)));
	m_mapBrushes.insert(std::map<CString,Gdiplus::SolidBrush*>::value_type(chBuffer, pBrush));
	return pBrush;
}

CSize Facilities::MeasureText( Gdiplus::Graphics* gdip, GdipFont* pFont, const CString& strText) const
{
	return MeasureText(gdip, pFont->gdipFont, strText);
}

CSize Facilities::MeasureText( Gdiplus::Graphics* gdip, Gdiplus::Font* pFont, const CString& strText) const
{
	Gdiplus::Graphics* gdip2 = gdip;
	if (gdip == NULL)
		gdip2 = m_gdip;

	Gdiplus::PointF origin(0, 0);
	Gdiplus::RectF rectF;
	CStringW strText2(strText);
	gdip2->MeasureString(strText2, strText2.GetLength(), pFont, origin, &rectF);
	Gdiplus::SizeF sizeF;
	rectF.GetSize(&sizeF);
	CSize size(static_cast<long> (sizeF.Width), static_cast<long> (sizeF.Height));

	return size;
}

void Facilities::DrawString( Gdiplus::Graphics* gdip, const CString& strText, const CRect& crBounds, GdipFont* pFont,
							 COLORREF crColor, int iAlign, int iLength, const CString& strPre, const CString& strPost, bool bPeriods ) const
{
	DrawString(gdip, strText, crBounds, pFont->gdipFont, crColor, iAlign, iLength, strPre, strPost, bPeriods);
}

void Facilities::DrawString( Gdiplus::Graphics* gdip, const CString& strText, const CRect& crBounds, Gdiplus::Font* pFont,
							 COLORREF crColor, int iAlign, int iLength, const CString& strPre, const CString& strPost, bool bPeriods ) const
{
	CString strText2( strPre + strText + strPost );
	if ( iLength != -1 && strText2.GetLength() > iLength )
		strText2 = strPre + strText.Left( iLength ) + ( ( bPeriods ) ? _T("...") : _T("") ) + strPost;

	Gdiplus::StringFormat format;
	Gdiplus::StringAlignment horizontalAlignment;
	if ((iAlign & TA_CENTER) == TA_CENTER) {
		horizontalAlignment = Gdiplus::StringAlignmentCenter;
	} else if ((iAlign & TA_RIGHT) == TA_RIGHT) {
		horizontalAlignment = Gdiplus::StringAlignmentFar;
	} else {	// TA_LEFT
		horizontalAlignment = Gdiplus::StringAlignmentNear;
	}
	format.SetAlignment(horizontalAlignment);
	Gdiplus::StringAlignment verticalAlignment;
	if ((iAlign & TA_BASELINE) == TA_BASELINE) {
		verticalAlignment = Gdiplus::StringAlignmentCenter;
	} else if ((iAlign & TA_BOTTOM) == TA_BOTTOM) {
		verticalAlignment = Gdiplus::StringAlignmentFar;
	} else {	// TA_TOP
		verticalAlignment = Gdiplus::StringAlignmentNear;
	}
	format.SetLineAlignment(verticalAlignment);
	format.SetFormatFlags(format.GetFormatFlags() | Gdiplus::StringFormatFlagsNoWrap);
	format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);

//	HDC hDC = gdip->GetHDC();
	COLORREF textColor = /*GetDeviceCaps(hDC, TECHNOLOGY) == DT_RASPRINTER ? COLOR_BLACK :*/ crColor;
//	gdip->ReleaseHDC(hDC);

	Gdiplus::SolidBrush textBrush(Gdiplus::Color(GetRValue(textColor),
												 GetGValue(textColor),
												 GetBValue(textColor)));

	CStringW wcTxt(strText2);
	if (isRunningUnderWine()) {
		// KMS: there's a bug(?) in Wine that clips text with gdip->DrawString(...RectF...) when there's a nonidentity transform (i.e. scroll then refresh or zoom)
		// FIXME: This drawing is not a perfect replacement
		CSize size = this->MeasureText(gdip, pFont, strText2);
		Gdiplus::PointF pointF(static_cast<float> (crBounds.left),
							 static_cast<float> (crBounds.top));
		if ((iAlign & TA_CENTER) == TA_CENTER) {
			pointF.X += (crBounds.Width() - size.cx) / 2;
		} else if ((iAlign & TA_RIGHT) == TA_RIGHT) {
			pointF.X += crBounds.Width() - size.cx;
		}

		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentNear);
		gdip->DrawString(wcTxt, strText2.GetLength(), pFont, pointF, &format, &textBrush);
	} else {
		Gdiplus::RectF rectF(static_cast<float> (crBounds.left),
							 static_cast<float> (crBounds.top),
							 static_cast<float> (crBounds.Width()),
							 static_cast<float> (crBounds.Height()));
		gdip->DrawString(wcTxt, strText2.GetLength(), pFont, rectF, &format, &textBrush);
	}

}

void Facilities::DrawRect( Gdiplus::Graphics* gdip, const CRect& cRect, COLORREF crColor, int iWidth,
						   bool bRoundRect, int iCornerRadius ) const
{
	Facilities* pThis = (Facilities*) this;
	Gdiplus::Pen* pPen = pThis->GetPen( crColor, iWidth );
	CRect cRect2 = cRect;
	cRect2.DeflateRect( iWidth / 2, iWidth / 2 );

	if (bRoundRect && iCornerRadius > 0) {
		long diameter = 2 * iCornerRadius;
		Gdiplus::GraphicsPath buttonPath;
		buttonPath.AddArc(cRect2.left, cRect2.top, diameter, diameter, 180.0, 90.0);
		buttonPath.AddLine(cRect2.left + iCornerRadius, cRect2.top, cRect2.right - iCornerRadius, cRect2.top);
		buttonPath.AddArc(cRect2.right - diameter, cRect2.top, diameter, diameter, 270.0, 90.0);
		buttonPath.AddLine(cRect2.right, cRect2.top + iCornerRadius, cRect2.right, cRect2.bottom - iCornerRadius);
		buttonPath.AddArc(cRect2.right - diameter, cRect2.bottom - diameter, diameter, diameter, 0.0, 90.0);
		buttonPath.AddLine(cRect2.right - iCornerRadius, cRect2.bottom, cRect2.left + iCornerRadius, cRect2.bottom);
		buttonPath.AddArc(cRect2.left, cRect2.bottom - diameter, diameter, diameter, 90.0, 90.0);
		buttonPath.AddLine(cRect2.left, cRect2.bottom - iCornerRadius, cRect2.left, cRect2.top + iCornerRadius);
		gdip->DrawPath(pPen, &buttonPath);
	} else {
		gdip->DrawRectangle(pPen, cRect2.left, cRect2.top, cRect2.Width(), cRect2.Height());
	}
}

void Facilities::DrawBox( Gdiplus::Graphics* gdip, const CRect& cRect, COLORREF crColor, int iDepth,
						  bool bRoundRect, int iCornerRadius ) const
{
	COLORREF beginColor = ShiftColor(crColor, -40);
	COLORREF endColor = ShiftColor(crColor, 60);

	Gdiplus::Color darkerColor = Gdiplus::Color(GetRValue(beginColor),
												GetGValue(beginColor),
												GetBValue(beginColor));
	Gdiplus::Color lighterColor = Gdiplus::Color(GetRValue(endColor),
												 GetGValue(endColor),
												 GetBValue(endColor));

	if (isRunningUnderWine()) {
		// Wine doesn't work well with either of the two routines below
		Gdiplus::SolidBrush brush(lighterColor);
		gdip->FillRectangle(&brush, cRect.left, cRect.top, cRect.right - cRect.left, cRect.bottom - cRect.top);
	} else if (bRoundRect && iCornerRadius > 0) {
		long diameter = 2 * iCornerRadius;
		Gdiplus::GraphicsPath buttonPath;
		CRect tmpRect = cRect;
		CRect cRect = tmpRect;
		cRect.right -= 1;
		cRect.top -= 1;
		buttonPath.AddArc(cRect.left, cRect.top, diameter, diameter, 180.0, 90.0);
		buttonPath.AddLine(cRect.left + iCornerRadius, cRect.top, cRect.right - iCornerRadius, cRect.top);
		buttonPath.AddArc(cRect.right - diameter, cRect.top, diameter, diameter, 270.0, 90.0);
		buttonPath.AddLine(cRect.right, cRect.top + iCornerRadius, cRect.right, cRect.bottom - iCornerRadius);
		buttonPath.AddArc(cRect.right - diameter, cRect.bottom - diameter, diameter, diameter, 0.0, 90.0);
		buttonPath.AddLine(cRect.right - iCornerRadius, cRect.bottom, cRect.left + iCornerRadius, cRect.bottom);
		buttonPath.AddArc(cRect.left, cRect.bottom - diameter, diameter, diameter, 90.0, 90.0);
		buttonPath.AddLine(cRect.left, cRect.bottom - iCornerRadius, cRect.left, cRect.top + iCornerRadius);

		Gdiplus::PathGradientBrush roundPathGradientBrush(&buttonPath);
		Gdiplus::Color presetColors[] = {
										darkerColor,
										lighterColor,
										lighterColor };
		float bandRatio = static_cast<float> (static_cast<float> (iDepth) / min(cRect.Width(), cRect.Height()));
		if (bandRatio > 1.0)
			bandRatio = 1.0;
		float interpolationPositions[] = {
			0.0f,
			bandRatio,
			1.0f };
		roundPathGradientBrush.SetInterpolationColors(presetColors, interpolationPositions, 3);

		gdip->FillPath(&roundPathGradientBrush, &buttonPath);
	} else {
		int width = cRect.Width();
		int height = cRect.Height();
		if (width > height) {
			//  __________
			// |\        /|
			// | \______/ |
			// | /      \ |
			// |/________\|
			//
			int h2 = height / 2;
			CPoint leftFocus;
			leftFocus.x = cRect.left + h2;
			leftFocus.y = cRect.top + h2;
			CPoint rightFocus;
			rightFocus.x = cRect.right - h2;
			rightFocus.y = leftFocus.y;

			Gdiplus::GraphicsPath doubleTrapezPath;
			doubleTrapezPath.AddLine(cRect.left, cRect.top, cRect.right, cRect.top);
			doubleTrapezPath.AddLine(cRect.right, cRect.top, cRect.right, cRect.top + 1);
			doubleTrapezPath.AddLine(cRect.right, cRect.top + 1, rightFocus.x + 1, rightFocus.y);
			doubleTrapezPath.AddLine(rightFocus.x + 1, rightFocus.y, cRect.right, cRect.bottom - 1);
			doubleTrapezPath.AddLine(cRect.right, cRect.bottom - 1, cRect.right, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.right, cRect.bottom, cRect.left, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.left, cRect.bottom, cRect.left, cRect.bottom - 1);
			doubleTrapezPath.AddLine(cRect.left, cRect.bottom - 1, leftFocus.x - 1, leftFocus.y);
			doubleTrapezPath.AddLine(leftFocus.x - 1, leftFocus.y, cRect.left, cRect.top + 1);
			doubleTrapezPath.AddLine(cRect.left, cRect.top + 1, cRect.left, cRect.top);

			float bandRatio = static_cast<float> (static_cast<float> (iDepth) / cRect.Height());
			if (bandRatio > 1.0)
				bandRatio = 1.0;
			Gdiplus::LinearGradientBrush trapezGradientBrush(Gdiplus::Point(cRect.left, cRect.top),
															 Gdiplus::Point(cRect.left, cRect.bottom),
															 darkerColor,
															 lighterColor);
			float blendFactors[] = {
				0.0f,
				1.0f,
				1.0f,
				0.0f };
			float blendPositions[] = {
				0.0f,
				bandRatio,
				1.0f - bandRatio,
				1.0f };
			trapezGradientBrush.SetBlend(blendFactors, blendPositions, 4);
			gdip->FillPath(&trapezGradientBrush, &doubleTrapezPath);

			float bandRatio2 = static_cast<float> (static_cast<float> (iDepth) / h2);
			if (bandRatio2 > 1.0)
				bandRatio2 = 1.0;
			Gdiplus::GraphicsPath leftTrianglePath;
			leftTrianglePath.AddLine(cRect.left, cRect.top, leftFocus.x, leftFocus.y);
			leftTrianglePath.AddLine(leftFocus.x, leftFocus.y, cRect.left, cRect.bottom);
			leftTrianglePath.AddLine(cRect.left, cRect.bottom, cRect.left, cRect.top);
			Gdiplus::LinearGradientBrush leftTriangleGradientBrush(Gdiplus::Point(cRect.left, leftFocus.y),
																   Gdiplus::Point(leftFocus.x, leftFocus.y),
																   darkerColor,
																   lighterColor);
			float leftBlendFactors[] = {
				0.0f,
				1.0f,
				1.0f };
			float leftBlendPositions[] = {
				0.0f,
				bandRatio2,
				1.0f };
			leftTriangleGradientBrush.SetBlend(leftBlendFactors, leftBlendPositions, 3);
			gdip->FillPath(&leftTriangleGradientBrush, &leftTrianglePath);

			Gdiplus::GraphicsPath rightTrianglePath;
			rightTrianglePath.AddLine(cRect.right, cRect.top, cRect.right, cRect.bottom);
			rightTrianglePath.AddLine(cRect.right, cRect.bottom, rightFocus.x, rightFocus.y);
			rightTrianglePath.AddLine(rightFocus.x, rightFocus.y, cRect.right, cRect.top);
			Gdiplus::LinearGradientBrush rightTriangleGradientBrush(Gdiplus::Point(rightFocus.x, rightFocus.y),
																	Gdiplus::Point(cRect.right, rightFocus.y),
																	lighterColor,
																	darkerColor);
			float rightBlendFactors[] = {
				0.0f,
				0.0f,
				1.0f };
			float rightBlendPositions[] = {
				0.0f,
				1.0f - bandRatio2,
				1.0f };
			rightTriangleGradientBrush.SetBlend(rightBlendFactors, rightBlendPositions, 3);
			gdip->FillPath(&rightTriangleGradientBrush, &rightTrianglePath);
		} else {
			//  ____
			// |\  /|
			// | \/ |
			// | |  |
			// | |  |
			// | /\ |
			// |/__\|
			//
			int w2 = width / 2;
			CPoint topFocus;
			topFocus.x = cRect.left + w2;
			topFocus.y = cRect.top + w2;
			CPoint bottomFocus;
			bottomFocus.x = topFocus.x;
			bottomFocus.y = cRect.bottom - w2;

			Gdiplus::GraphicsPath doubleTrapezPath;
			doubleTrapezPath.AddLine(cRect.left, cRect.top, cRect.left + 1, cRect.top);
			doubleTrapezPath.AddLine(cRect.left + 1, cRect.top, topFocus.x, topFocus.y - 1);
			doubleTrapezPath.AddLine(topFocus.x, topFocus.y - 1, cRect.right - 1, cRect.top);
			doubleTrapezPath.AddLine(cRect.right - 1, cRect.top, cRect.right, cRect.top);
			doubleTrapezPath.AddLine(cRect.right, cRect.top, cRect.right, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.right, cRect.bottom, cRect.right - 1, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.right - 1, cRect.bottom, bottomFocus.x, bottomFocus.y + 1);
			doubleTrapezPath.AddLine(bottomFocus.x, bottomFocus.y + 1, cRect.left + 1, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.left + 1, cRect.bottom, cRect.left, cRect.bottom);
			doubleTrapezPath.AddLine(cRect.left, cRect.bottom, cRect.left, cRect.top);

			float bandRatio = static_cast<float> (static_cast<float> (iDepth) / cRect.Width());
			if (bandRatio > 1.0)
				bandRatio = 1.0;
			Gdiplus::LinearGradientBrush trapezGradientBrush(Gdiplus::Point(cRect.left, cRect.top),
															 Gdiplus::Point(cRect.right, cRect.top),
															 darkerColor,
															 lighterColor);
			float blendFactors[] = {
				0.0f,
				1.0f,
				1.0f,
				0.0f };
			float blendPositions[] = {
				0.0f,
				bandRatio,
				1.0f - bandRatio,
				1.0f };
			trapezGradientBrush.SetBlend(blendFactors, blendPositions, 4);
			gdip->FillPath(&trapezGradientBrush, &doubleTrapezPath);

			float bandRatio2 = static_cast<float> (static_cast<float> (iDepth) / w2);
			if (bandRatio2 > 1.0)
				bandRatio2 = 1.0;
			Gdiplus::GraphicsPath topTrianglePath;
			topTrianglePath.AddLine(cRect.left, cRect.top, cRect.right, cRect.top);
			topTrianglePath.AddLine(cRect.right, cRect.top, topFocus.x, topFocus.y);
			topTrianglePath.AddLine(topFocus.x, topFocus.y, cRect.left, cRect.top);
			Gdiplus::LinearGradientBrush topTriangleGradientBrush(Gdiplus::Point(topFocus.x, cRect.top),
																   Gdiplus::Point(topFocus.x, topFocus.y),
																   darkerColor,
																   lighterColor);
			float topBlendFactors[] = {
				0.0f,
				1.0f,
				1.0f };
			float topBlendPositions[] = {
				0.0f,
				bandRatio2,
				1.0f };
			topTriangleGradientBrush.SetBlend(topBlendFactors, topBlendPositions, 3);
			gdip->FillPath(&topTriangleGradientBrush, &topTrianglePath);

			Gdiplus::GraphicsPath bottomTrianglePath;
			bottomTrianglePath.AddLine(cRect.left, cRect.bottom, bottomFocus.x, bottomFocus.y);
			bottomTrianglePath.AddLine(bottomFocus.x, bottomFocus.y, cRect.right, cRect.bottom);
			bottomTrianglePath.AddLine(cRect.right, cRect.bottom, cRect.left, cRect.bottom);
			Gdiplus::LinearGradientBrush bottomTriangleGradientBrush(Gdiplus::Point(bottomFocus.x, bottomFocus.y),
																	 Gdiplus::Point(bottomFocus.x, cRect.bottom),
																	 lighterColor,
																	 darkerColor);
			float bottomBlendFactors[] = {
				0.0f,
				0.0f,
				1.0f };
			float bottomBlendPositions[] = {
				0.0f,
				1.0f - bandRatio2,
				1.0f };
			bottomTriangleGradientBrush.SetBlend(bottomBlendFactors, bottomBlendPositions, 3);
			gdip->FillPath(&bottomTriangleGradientBrush, &bottomTrianglePath);
		}
	}
}

COLORREF Facilities::ShiftColor( COLORREF crColor, int iShift ) const
{
	int iR = GetRValue( crColor );
	int iG = GetGValue( crColor );
	int iB = GetBValue( crColor );
	iR = min( max( iR + iShift, 0 ), 255 );
	iG = min( max( iG + iShift, 0 ), 255 );
	iB = min( max( iB + iShift, 0 ), 255 );
	return RGB( iR, iG, iB );
}

double Facilities::Deg2Rad( long deg ) const
{
	return (deg * 2 * M_PI / 360.0);
}

std::vector<CString> Facilities::wrapString( const CString& strIn, int iWrap, int iMax ) const
{
	std::vector<CString> vecOut;
	CString str = strIn;
	str.TrimLeft();
	str.TrimRight();
	iWrap = min( iMax, iWrap );
	if ( iWrap == 0 || iWrap == -1 )
		vecOut.push_back( str );
	else {
		while ( ! str.IsEmpty() ) {
			CString str2 = str;
			if ( str.GetLength() > iWrap ) {
				str2 = str.Left( iWrap );
				int iPos = str2.ReverseFind( ' ' );
				if ( iPos == -1 || iPos < str2.GetLength() - 5 ) {
					str2 = str.Right( str.GetLength() - iWrap );
					iPos = str2.Find( ' ' );
					if ( iPos == -1 && str2.GetLength() > 5 || iPos >= 5  ) {
						str2 = str.Left( iWrap );
						str = str.Right( str.GetLength() - iWrap );
					}
					else {
						if ( iPos == -1 )
							iPos = str2.GetLength();
						str2 = str.Left( iWrap + iPos );
						str = str.Right( str.GetLength() - iWrap - iPos - 1 );
					}
				}
				else {
					str2 = str.Left( iPos );
					str = str.Right( str.GetLength() - iPos - 1 );
				}
			}
			else
				str = "";
			str2.TrimRight();
			str.TrimLeft();
			vecOut.push_back( str2 );
		}
	}
	return vecOut;
}

CString Facilities::getStereotyped( const CString& strIn ) const
{
	CString str;
	if (stereotypeCharacterType)
		str = str + STEREOTYPE_LEFTB + strIn + STEREOTYPE_RIGHTB;
	else
		str = str + STEREOTYPE_LEFTA + strIn + STEREOTYPE_RIGHTA;
	return str;
}

}; // namespace DecoratorSDK