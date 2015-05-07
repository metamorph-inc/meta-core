#include "unixExt.hpp"

#include <algorithm>

class copyToCString {
private:
  char *_cString;

public:
  copyToCString( char *cString ) : _cString( cString ) { *_cString = '\0'; }
  void operator()( char c ) {
    *_cString = c; ++_cString; *_cString = '\0';
  }
};


char *_itoa( int value, char *str, int radix ) {

  if ( value == 0 ) {
    char *strPtr = str;
    *strPtr = '0'; ++strPtr;
    *strPtr = '\0';
    return str;
  }

  bool negFlag = false;

  if ( value < 0 ) {
    negFlag = true;
    value = -value;
  }

  std::list< char > listConv;

  while( value != 0 ) {
    char digit = value % radix;
    digit += ( digit < 10 ) ? 48 : 55;
    listConv.push_front( digit );
    value /= radix;
  }

  if ( negFlag ) {
    listConv.push_front( '-' );
  }

  std::for_each(  listConv.begin(), listConv.end(), copyToCString( str )  );
  return str;
}


char *_strupr( char * const str ) {

  char *cnvStr = str;
  while( *cnvStr != '\0' ) {
    if (  isalpha( *cnvStr )  ) *cnvStr &= (char)~0x20;
    (void)++cnvStr;
  }
  return str;
}
