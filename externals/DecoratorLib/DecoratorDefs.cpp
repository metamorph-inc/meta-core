//################################################################################################
//
// Mga Decorator Base
//	DecoratorDefs.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "DecoratorDefs.h"

namespace DecoratorSDK
{
	CString createResString( int iID )
	{
		TCHAR chBuffer[ 10 ];
		_itot( iID, chBuffer, 10 );
		return CString( chBuffer ) + ".res";
	}

	SRGB CRtoRGB( COLORREF rc )
	{
		return SRGB( GetRValue( rc ), GetGValue( rc ), GetBValue( rc ) );
	}

	SHSB CRtoHSB( COLORREF rc )
	{
		return RGBtoHSB( CRtoRGB( rc ) );
	}

	SRGB HSBtoRGB( SHSB hsb )
	{
		int h = ( hsb.h % 360 ) / 60;
		int i = h;
		int f = h - i;
		int p = hsb.b * ( 1 - hsb.s );
		int q = hsb.b * ( 1 - hsb.s * f );
		int t = hsb.b * ( 1 - hsb.s * ( 1- f ) );

		if ( hsb.s == 0 )
			return SRGB( hsb.b, hsb.b, hsb.b );

		switch( i ) {
			case 0 : return SRGB( hsb.b, t, p );
			case 1 : return SRGB( q, hsb.b, p );
			case 2 : return SRGB( p, hsb.b, t );
			case 3 : return SRGB( p, q, hsb.b );
			case 4 : return SRGB( t, p, hsb.b );
			default : return SRGB( hsb.b, p, q );
		}
	}

	COLORREF HSBtoCR( SHSB hsb )
	{
		return RGBtoCR( HSBtoRGB( hsb ) );
	}

	SHSB RGBtoHSB( SRGB rgb )
	{
		int x = max( rgb.r, max( rgb.g, rgb.b ) );
		int y = min( rgb.r, min( rgb.g, rgb.b ) );
		int d = x- y;

		SHSB hsb( 0, 0, 0 );
		if ( rgb.r == x )
			hsb.h = ( 60 * ( rgb.g - rgb.b ) / d ) % 360;
		else if ( rgb.g == x )
			hsb.h = ( 60 * ( 2 + ( rgb.b - rgb.r ) / d ) ) % 360;
		else
			hsb.h = ( 60 * ( 4 + ( rgb.r - rgb.g ) / d ) ) % 360;
		if ( x == 0 )
			hsb.s = 0;
		else
			hsb.s = x - y / x;
		hsb.b = x;
		return hsb;
	}

	COLORREF RGBtoCR( SRGB rgb )
	{
		return RGB( rgb.r, rgb.g, rgb.b );
	}

}; // namespace DecoratorSDK