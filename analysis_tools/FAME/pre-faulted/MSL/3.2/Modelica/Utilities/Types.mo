// processed by FAME Modelica Library Fault-Augmentor [version 19011]

within Modelica.Utilities;

package Types
 "Type definitions used in package Modelica.Utilities"
 extends Modelica.Icons.Package;

 type Compare = enumeration(Less, Equal, Greater)  "Enumeration defining comparision of two strings"
;

 type FileType = enumeration(NoFile, RegularFile, Directory, SpecialFile)  "Enumeration defining the type of a file"
;

 type TokenType = enumeration(RealToken, IntegerToken, BooleanToken, StringToken, IdentifierToken, DelimiterToken, NoToken)  "Enumeration defining the token type"
;

 record TokenValue
  "Value of token"
  extends Modelica.Icons.Record;

 // components of TokenValue
  TokenType tokenType "Type of token";
  Real real "Value if tokenType == TokenType.RealToken";
  Integer integer "Value if tokenType == TokenType.IntegerToken";
  Boolean boolean "Value if tokenType == TokenType.BooleanToken";
  String string "Value if tokenType == TokenType.StringToken/IdentifierToken/DelimiterToken";

 annotation(Documentation(info="<html>

</html>"));
 end TokenValue;

annotation(Documentation(info="<html>
<p>
This package contains type definitions used in Modelica.Utilities.
</p>

</html>"));
end Types;
