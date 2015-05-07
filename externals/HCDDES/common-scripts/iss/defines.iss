#define APP_VERSION	'v'+GetDateTimeString('y.mm.dd.hhnn', '', '');
#define APP_PATH 	SourcePath + "\..\"

#ifdef REQUIRES_GME
#define REQUIRES_GMEVERSION 	"10.8.18"
#endif

#ifdef REQUIRES_UDM
#define REQUIRES_UDMVERSION "3.2, 3.2.3 or later"
; eg 3.2.4.0: 3.2 is implied by UDMDLL. First short is 4; second is 0
#define UDM_PATCH 00030000
#define UDMDLL "UdmDll_3_2.dll"
#endif

#ifdef REQUIRES_GREAT
#define REQUIRES_GREATVERSION 	"ANY"
#endif
