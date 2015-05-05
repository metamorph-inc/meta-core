//################################################################################################
//
// Mga Decorator Base
//	DecoratorDefs.h
//
//################################################################################################

#ifndef DecoratorDefs_h
#define DecoratorDefs_h

#pragma warning( disable : 4786 )

#include "StdAfx.h"
#include "DecoratorLibResource.h"


//################################################################################################
//
// IMPORTANT :
// this is from GmeStd.h
//
//################################################################################################
#define GME_NORTH					0			// Must match PopupPropDlg radiobutton stuff!
#define GME_NORTHEAST				1
#define GME_EAST					2
#define GME_SOUTHEAST				3
#define GME_SOUTH					4
#define GME_SOUTHWEST				5
#define GME_WEST					6
#define GME_NORTHWEST				7
#define GME_CENTER					8

#define GME_DYN_MENU_MINID			0x1001
#define DECORATOR_CTX_MENU_MINID	0x4001
#define DECORATOR_CTX_MENU_MAXID	0x4101

#ifndef IDC_INPLACETEXTEDIT
#define IDC_INPLACETEXTEDIT			201
#endif

//################################################################################################
//
// IMPORTANT :
// use Compiler Definition OLD_DECORATOR_LOOKANDFEEL to see Previous Decorator Look & Feel
//
//################################################################################################

namespace DecoratorSDK
{
	enum ELocation {
		L_NORTH			= GME_NORTH,
		L_SOUTH			= GME_SOUTH,
		L_WEST			= GME_WEST,
		L_EAST			= GME_EAST,
		L_SOUTHWEST		= GME_SOUTHWEST,
		L_SOUTHEAST		= GME_SOUTHEAST,
		L_NORTHWEST		= GME_NORTHWEST,
		L_NORTHEAST		= GME_NORTHEAST,
		L_CENTER		= GME_CENTER
	};

	enum EPrefStatus {
		PS_HERE			= 0,
		PS_META			= -1,
		PS_INHERITED	= 1,
		PS_UNDEFINED	= 2
	};

	enum ResizeType {
		NotInResize				= 0,
		RightEdgeResize			= 1,	// Horizontal resize
		BottomEdgeResize		= 2,	// Vertical resize
		LeftEdgeResize			= 3,	// Horizontal resize
		TopEdgeResize			= 4,	// Vertical resize
		TopLeftCornerResize		= 5,	// H + V resize
		TopRightCornerResize	= 6,	// H + V resize
		BottomRightCornerResize	= 7,	// H + V resize
		BottomLeftCornerResize	= 8,	// H + V resize
		MoveOperation			= 9
	};

	static const char* PREF_LABELLOCATION 			= "namePosition";
	static const char* PREF_ICON					= "icon";
	static const char* PREF_PORTICON				= "porticon";
	static const char* PREF_SUBTYPEICON				= "subTypeIcon";
	static const char* PREF_INSTANCEICON			= "instanceIcon";
	static const char* PREF_NULLREFICON				= "nullRefIcon";
	static const char* PREF_OVERLAYCOLOR			= "color";
	static const char* PREF_COLOR					= "color";
	static const char* PREF_LABELCOLOR				= "nameColor";
	static const char* PREF_TRANSPARENTCOLOR		= "transparentColor";
	static const char* PREF_GRAYEDOUTCOLOR			= "grayedOutColor";
	static const char* PREF_PORTLABELCOLOR			= "portColor";
	static const char* PREF_PORTLABELINSIDE 		= "portLabelInside";
	static const char* PREF_TYPESHOWN				= "isTypeShown";
	static const char* PREF_TYPEINFOSHOWN			= "isTypeInfoShown";
	static const char* PREF_PORTLABELLENGTH			= "portLabelLength";
	static const char* PREF_BORDERCOLOR				= "borderColor";
	static const char* PREF_FILLCOLOR				= "fillColor";
	static const char* PREF_GRADIENTCOLOR			= "gradientColor";
	static const char* PREF_SHADOWCOLOR				= "shadowColor";

	static const char* PREF_LABELFONT				= "labelFont";
	static const char* PREF_LABELLENGTH				= "labelLength";

	static const char* PREF_ICONDEFAULT				= "iconDefault";
	static const char* PREF_TILESDEFAULT			= "tilesDefault";
	static const char* PREF_TILESUNDEF				= "tilesUndefined";

	static const char* PREF_TILES					= "tiles";
	static const char* PREF_LABEL					= "label";

	static const char* PREF_LABELWRAP				= "nameWrap";
	static const char* PREF_LABELENABLED			= "isNameEnabled";

	static const char* PREF_VIOLATED				= "isViolated";

	static const char* PREF_ITEMEDITABLE			= "itemEditable";
	static const char* PREF_ITEMRESIZABLE			= "itemResizable";
	static const char* PREF_PREFERREDSIZE			= "preferredSize";
	static const char* PREF_ISMASKEDBITMAP			= "isMaskedBitmap";
	static const char* PREF_ISMODELEXPANDED			= "isModelExpanded";
	static const char* PREF_ITEMSHADOWCAST			= "itemShadowCast";
	static const char* PREF_SHADOWTHICKNESS			= "shadowThickness";
	static const char* PREF_SHADOWDIRECTION			= "shadowDirection";
	static const char* PREF_ITEMGRADIENTFILL		= "gradientFill";
	static const char* PREF_GRADIENTDIRECTION		= "gradientDirection";
	static const char* PREF_ROUNDCORNERRECT			= "roundCornerRect";
	static const char* PREF_ROUNDCORNERRADIUS		= "roundCornerRadius";

	static const char* PREF_DECORATOR_MARGINX		= "decoratorMarginX";
	static const char* PREF_DECORATOR_MARGINY		= "decoratorMarginY";
	static const char* PREF_DECORATOR_GAPY			= "decoratorGapY";
	static const char* PREF_DECORATOR_MINATTRSIZE	= "decoratorMinAttrSize";
	static const char* PREF_TEXTOVERRIDE			= "textOverride";
	static const char* PREF_TEXTCOLOROVERRIDE		= "textColorOverride";
	static const char* PREF_MULTILINEINPLACEEDIT	= "multiLineInPlaceEdit";

	static const int FONT_LABEL					= 0;
	static const int FONT_PORT					= 1;
	static const int FONT_TYPE					= 2;
	static const int FONT_PORT_OUTSIDE			= 3;
	static const int FONT_PORTNAME				= 4;
	static const int FONT_CONNLABEL				= 5;
	static const int FONT_ABSTRACT				= 6;

	static const int GAP_LABEL					= 2;
	static const int GAP_PORT					= 3;
	static const int GAP_XMODELPORT				= 4;
	static const int GAP_YMODELPORT				= 8;
	static const int GAP_PORTLABEL				= 20;

	static const COLORREF COLOR_BLACK 			= RGB( 0x00, 0x00, 0x00 );
	static const COLORREF COLOR_GRAY 			= RGB( 0xB0, 0xB0, 0xB0 );
	static const COLORREF COLOR_LIGHTGRAY		= RGB( 0xF3, 0xF3, 0xF3 );
	static const COLORREF COLOR_RED 			= RGB( 0xFF, 0x00, 0x00 );
	static const COLORREF COLOR_BLUE 			= RGB( 0x00, 0x00, 0xFF );

	static const COLORREF COLOR_BKGND			= RGB( 0xff, 0xff, 0xff );
	static const COLORREF COLOR_BORDER			= RGB( 0x00, 0x00, 0x00 );
	static const COLORREF COLOR_MODEL			= RGB( 0xc0, 0xc0, 0xc0 );
	static const COLORREF COLOR_NAME			= RGB( 0x00, 0x00, 0x00 );
	static const COLORREF COLOR_PORTNAME		= RGB( 0x60, 0x60, 0x60 );
	static const COLORREF COLOR_CONNECTION 		= RGB( 0x00, 0x00, 0x00 );
	static const COLORREF COLOR_GRID			= RGB( 0xd0, 0xd0, 0xd0 );
	static const COLORREF COLOR_WHITE			= RGB( 0xff, 0xff, 0xff );
	static const COLORREF COLOR_GREY			= RGB( 0xc0, 0xc0, 0xc0 );
	static const COLORREF COLOR_GRAYED_OUT		= RGB( 0xde, 0xde, 0xde );
	static const COLORREF COLOR_TRANSPARENT		= RGB( 0xff, 0xff, 0xff );

	static const int MAX_LABEL_LENGTH		= 128;
	static const int MAX_TYPE_LENGTH		= 180;
	static const int MAX_PORT_LENGTH		= 3;

	static const int WIDTH_MODEL			= 113;
	static const int HEIGHT_MODEL			= 71;
	static const int WIDTH_PORT				= 10;
	static const int HEIGHT_PORT			= 11;
	static const int WIDTH_BORDERVIOLATION	= 2;
	static const int WIDTH_BORDERMODELBOX	= 7;
	static const int WIDTH_MODELSIDE		= 100;
	static const int GME_3D_BORDER_SIZE		= 3;
	static const double INHERITANCE_RATIO	= 0.86602540378443864676372317075294;

	static const TCHAR* TILE_ATOMDEFAULT		= _T("$TILE$Atom_Default");
	static const TCHAR* TILE_MODELDEFAULT	= _T("$TILE$Model_Default");
	static const TCHAR* TILE_PORTDEFAULT		= _T("$TILE$Port_Default");

	static const TCHAR* CTX_MENU_STR_RENAME				= _T("Rename");
	static const TCHAR* CTX_MENU_STR_RESETSIZE			= _T("Reset to original size");
	static const TCHAR* CTX_MENU_STR_SETSIZE				= _T("Set size");
	static const TCHAR* CTX_MENU_STR_EXPANDMODEL			= _T("Expand model");
	static const TCHAR* CTX_MENU_STR_COLLAPSEMODEL		= _T("Collapse model");
	static const unsigned int CTX_MENU_ID_RENAME		= DECORATOR_CTX_MENU_MINID + 0;
	static const unsigned int CTX_MENU_ID_RESETSIZE		= DECORATOR_CTX_MENU_MINID + 1;
	static const unsigned int CTX_MENU_ID_SETSIZE		= DECORATOR_CTX_MENU_MINID + 2;
	static const unsigned int CTX_MENU_ID_EXPANDMODEL	= DECORATOR_CTX_MENU_MINID + 3;
	static const unsigned int CTX_MENU_ID_COLLAPSEMODEL	= DECORATOR_CTX_MENU_MINID + 4;

	static const TCHAR* STEREOTYPE_LEFTA					= _T("<<");
	static const TCHAR* STEREOTYPE_RIGHTA				= _T(">>");
	static const wchar_t* STEREOTYPE_LEFTB				= L"\u226A ";
	static const wchar_t* STEREOTYPE_RIGHTB				= L" \u226B";
	static const TCHAR* ABSTRACT_ATTR					= _T("IsAbstract");
	static const TCHAR* ATTRIBUTES_ATTR					= _T("Attributes");
	static const TCHAR* ATTRIBUTE_SEP					= _T(" : ");

	static const int DECORATOR_MARGINX					= 6;
	static const int DECORATOR_MARGINY					= 4;
	static const int DECORATOR_GAPY						= 0;
	static const int DECORATOR_MINATTRSIZE				= 13;
	static const int DECORATOR_MINSENSITIVITYDISTANCE	= 2;
	static const int DECORATOR_MAXSENSITIVITYDISTANCE	= 6;

	#define DEC_CONNECTED_PORTS_ONLY_PARAM				_T("ConnectedPortsOnly")

	struct GdipFont
	{
		Gdiplus::Font*	gdipFont;
		int				iSize;
		CString			strName;
		bool			bBoldness;
		bool			bItalics;

		GdipFont( const CString& _strName, int _iSize, bool _bBoldness, bool _bItalics )
			: gdipFont( NULL ), strName( _strName ), iSize( _iSize ), bBoldness( _bBoldness ), bItalics( _bItalics )
		{
		}
	};

	struct SRGB
	{
		int r;
		int g;
		int b;

		SRGB( int _r, int _g, int _b )
			: r( _r ), g( _g ), b( _b )
		{
		}
	};

	struct SHSB
	{
		int h;
		int s;
		int b;

		SHSB( int _h, int _s, int _b )
			: h( _h ), s( _s ), b( _b )
		{
		}
	};

	SRGB 		CRtoRGB( COLORREF rc );
	SHSB 		CRtoHSB( COLORREF rc );
	SRGB 		HSBtoRGB( SHSB hsb );
	COLORREF 	HSBtoCR( SHSB hsb );
	SHSB 		RGBtoHSB( SRGB rgb );
	COLORREF 	RGBtoCR( SRGB rgb );

	CString createResString( int iID );

}; // namespace DecoratorSDK

#endif // DecoratorDefs_h