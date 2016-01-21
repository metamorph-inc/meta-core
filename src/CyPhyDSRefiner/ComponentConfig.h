#define RAWCOMPONENT

// COM UUID-s, names and ProgID
#define TYPELIB_UUID "EA572AF5-9B0A-4ED0-B5A9-ED814C5E6552"
#define TYPELIB_NAME "MGA Interpreter TypeLibrary CyPhyDSRefiner)"
#define COCLASS_UUID "080C83C0-5144-477F-B763-8E4A8246DF49"
#define COCLASS_NAME "MGA Interpreter CoClass (CyPhyDSRefiner)"
#define COCLASS_PROGID "MGA.Interpreter.CyPhyDSRefiner"


// This name will appear in the popup window for interpreter selection.
#define COMPONENT_NAME L"CyPhyDSRefiner"


#define TOOLTIP_TEXT "Design Space Refinement Tool"


// This #define determines the interpreter type:
#define GME_INTERPRETER
// The name of the paradigm(s). The GME will use this component
// for this paradigm. Separate the names of paradigms by commas.
#define PARADIGMS "CyPhyML"


#define BON_ICON_SUPPORT


#define REGISTER_SYSTEMWIDE

// Just to please the whims of those Microsoft jerks:
#define COCLASS_UUID_EXPLODED1 0x080C83C0
#define COCLASS_UUID_EXPLODED2  0x5144
#define COCLASS_UUID_EXPLODED3  0x477F
#define COCLASS_UUID_EXPLODED4  0xB7
#define COCLASS_UUID_EXPLODED5  0x63
#define COCLASS_UUID_EXPLODED6  0x8E
#define COCLASS_UUID_EXPLODED7  0x4A
#define COCLASS_UUID_EXPLODED8  0x82
#define COCLASS_UUID_EXPLODED9  0x46
#define COCLASS_UUID_EXPLODED10  0xDF
#define COCLASS_UUID_EXPLODED11  0x49
