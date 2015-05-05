//################################################################################################
//
// Preference Variant type for decorators
//	PreferenceVariant.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "PreferenceVariant.h"


namespace DecoratorSDK {

//################################################################################################
//
//	STRUCT : PreferenceVariant
//
//################################################################################################

PreferenceVariant::PreferenceVariant( bool bValue )
	: eType( PVV_Boolean )
{
	uValue.bValue = bValue;
}

PreferenceVariant::PreferenceVariant( long lValue )
	: eType( PVV_Long )
{
	uValue.lValue = lValue;
}

PreferenceVariant::PreferenceVariant( double dValue )
	: eType( PVV_Double )
{
	uValue.dValue = dValue;
}

PreferenceVariant::PreferenceVariant( const CString& strValue )
	: eType( PVV_String )
{
	uValue.pstrValue = new CString( strValue );
}

PreferenceVariant::PreferenceVariant( COLORREF crValue )
	: eType( PVV_Color )
{
	uValue.crValue = crValue;
}

PreferenceVariant::PreferenceVariant( ELocation eLocation )
	: eType( PVV_Location )
{
	uValue.eLocation = eLocation;
}

PreferenceVariant::PreferenceVariant( BitmapBase* pBitmap )
	: eType( PVV_Bitmap )
{
	uValue.pBitmap = pBitmap;
}

PreferenceVariant::PreferenceVariant( TileVector* pTiles )
	: eType( PVV_Tiles )
{
	uValue.pTiles = pTiles;
}

PreferenceVariant::PreferenceVariant( const PreferenceVariant& pv )
	: eType( pv.eType ), uValue( pv.uValue )
{
	if ( eType == PreferenceVariant::PVV_String )
		uValue.pstrValue = new CString( *uValue.pstrValue );
}

PreferenceVariant& PreferenceVariant::operator=( const PreferenceVariant& pv )
{
	if ( this != &pv ) {
		if ( eType == PreferenceVariant::PVV_String )
			delete uValue.pstrValue;
		eType = pv.eType;
		uValue = pv.uValue;
		if ( eType == PreferenceVariant::PVV_String )
			uValue.pstrValue = new CString( *uValue.pstrValue );
	}
	return *this;
}

PreferenceVariant::~PreferenceVariant()
{
	if ( eType == PreferenceVariant::PVV_String )
		delete uValue.pstrValue;
}

};	// namespace DecoratorSDK
