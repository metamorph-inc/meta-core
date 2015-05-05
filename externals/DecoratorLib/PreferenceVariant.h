//################################################################################################
//
// Preference Variant type for decorators
//	PreferenceVariant.h
//
//################################################################################################

#ifndef PREFERENCEVARIANTINCLUDED_H
#define PREFERENCEVARIANTINCLUDED_H


#include "StdAfx.h"
#include <vector>
#include <map>


//################################################################################################
//	Predeclarations
//################################################################################################

namespace DecoratorSDK {

enum ELocation;
enum ECoordRefPoint;
class BitmapBase;
class BackgroundTile;
typedef std::vector<BackgroundTile> TileVector;

struct PreferenceVariant;

typedef std::map<CString,PreferenceVariant> PreferenceMap;

//################################################################################################
//
//	STRUCT : PreferenceVariant
//
//################################################################################################

struct PreferenceVariant
{
	typedef enum {
		PVV_Boolean = 0,
		PVV_String,
		PVV_Long,
		PVV_Double,
		PVV_Color,
		PVV_Bitmap,
		PVV_Tiles,
		PVV_Location
	} Type;

	typedef union {
		bool bValue;
		long lValue;
		double dValue;
		CString* pstrValue;
		COLORREF crValue;
		BitmapBase* pBitmap;
		TileVector* pTiles;
		ELocation eLocation;
	} Value;

	Type eType;
	Value uValue;

	PreferenceVariant( bool bValue = false );
	PreferenceVariant( long lvalue );
	PreferenceVariant( double dValue );
	PreferenceVariant( const CString& strValue );
	PreferenceVariant( COLORREF crValue );
	PreferenceVariant( ELocation eLocation );
	PreferenceVariant( BitmapBase* pBitmap );
	PreferenceVariant( TileVector* pTiles );

	PreferenceVariant( const PreferenceVariant& pv );
	PreferenceVariant& operator=( const PreferenceVariant& pv );
	~PreferenceVariant();
};

};	// namespace DecoratorSDK

#endif // PREFERENCEVARIANTINCLUDED_H