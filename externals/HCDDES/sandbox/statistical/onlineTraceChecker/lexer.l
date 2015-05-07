/***********************************************************************************************
Copyright (C) 2010 Anvesh Komuravelli, Paolo Zuliani and Edmund M. Clarke.  All rights reserved.
By using this software the USER indicates that he or she has read, understood and will comply
with the following:

1. The USER is hereby granted non-exclusive permission to use, copy and/or
modify this software for internal, non-commercial, research purposes only. Any
distribution, including commercial sale or license, of this software, copies of
the software, its associated documentation and/or modifications of either is
strictly prohibited without the prior consent of the authors. Title to copyright
to this software and its associated documentation shall at all times remain with
the authors. Appropriated copyright notice shall be placed on all software
copies, and a complete copy of this notice shall be included in all copies of
the associated documentation. No right is granted to use in advertising,
publicity or otherwise any trademark, service mark, or the name of the authors.

2. This software and any associated documentation is provided "as is".

THE AUTHORS MAKE NO REPRESENTATIONS OR WARRANTIES, EXPRESSED OR IMPLIED,
INCLUDING THOSE OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT
USE OF THE SOFTWARE, MODIFICATIONS, OR ASSOCIATED DOCUMENTATION WILL NOT
INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER INTELLECTUAL PROPERTY
RIGHTS OF A THIRD PARTY.

The authors shall not be liable under any circumstances for any direct,
indirect, special, incidental, or consequential damages with respect to any
claim by USER or any third party on account of or arising from the use, or
inability to use, this software or its associated documentation, even if the
authors have been advised of the possibility of those damages.
***********************************************************************************************/


/**
  * Lexer for BLTL properties, allowing arbitrary relational expressions on real
  * (actually, rational) variables (aka, the state variables) as the propositions
  *
  * assumes property is input from a file, whose name is provided as a command
  * line, if the lexer is run independently
  *
  * For the syntax of BLTL properties, refer to the parser
  *
  * Tested on Flex version 2.5.35. No guarantees on other versions of Flex or
  * even Lex (but hopefully, it works).
  */


%{
   #include <stdio.h>
   #include <stdlib.h>
   #include "parser.tab.hpp"
%}

ID                    [_[:alpha:]][[:alpha:][:digit:]]*
T_DOUBLE               [[:digit:]]*"."[[:digit:]]+
T_INT                 [[:digit:]]+

%%
                      /* IDENTIFIERS */
"true"                {
                        #ifdef VERBOSE
                          printf("TRUE\n");
                        #endif
                        return TRUE;
                      }
"false"               {
                        #ifdef VERBOSE
                          printf("FALSE\n");
                        #endif
                        return FALSE;
                      }

                      /* TEMPORAL OPERATORS */
"X"/([ \t]*\[)        {
                        #ifdef VERBOSE
                          printf("NEXT\n");
                        #endif
                        return NEXT;
                      }
"F"/([ \t]*\[)        {
                        #ifdef VERBOSE
                          printf("FUTURE\n");
                        #endif
                        return FUTURE;
                      }
"G"/([ \t]*\[)        {
                        #ifdef VERBOSE
                          printf("GLOBAL\n");
                        #endif
                        return GLOBAL;
                      }
"U"/([ \t]*\[)        {
                        #ifdef VERBOSE
                          printf("UNTIL\n");
                        #endif
                        return UNTIL;
                      }

                      /* IDENTIFIERS CONTD. */
{ID}                  {
                        #ifdef VERBOSE
                          printf("ID : %s\n", yytext);
                        #endif
                        return ID;
                      }
{T_DOUBLE}            {
                        #ifdef VERBOSE
                          printf("T_DOUBLE : %s\n", yytext);
                        #endif
                        return T_DOUBLE;
                      }
{T_INT}               {
                        #ifdef VERBOSE
                          printf("T_INT : %s\n", yytext);
                        #endif
                        return T_INT;
                      }

                      /* INDENTATION */
\(                    {
                        #ifdef VERBOSE
                          printf("L_PAREN\n");
                        #endif
                        return L_PAREN;
                      }
\)                    {
                        #ifdef VERBOSE
                          printf("R_PAREN\n");
                        #endif
                        return R_PAREN;
                      }
\[                    {
                        #ifdef VERBOSE
                          printf("L_SQUARE\n");
                        #endif
                        return L_SQUARE;
                      }
\]                    {
                        #ifdef VERBOSE
                          printf("R_SQUARE\n");
                        #endif
                        return R_SQUARE;
                      }
,                     {
                        #ifdef VERBOSE
                          printf("COMMA\n");
                        #endif
                        return COMMA;
                      }

                      /* RELATIONAL OPERATORS */
==                    {
                        #ifdef VERBOSE
                          printf("EQ\n");
                        #endif
                        return EQ;
                      }
~=                    {
                        #ifdef VERBOSE
                          printf("NEQ\n");
                        #endif
                        return NEQ;
                      }
\<=                   {
                        #ifdef VERBOSE
                          printf("LTE\n");
                        #endif
                        return LTE;
                      }
\<                    {
                        #ifdef VERBOSE
                          printf("LT\n");
                        #endif
                        return LT;
                      }
\>=                   {
                        #ifdef VERBOSE
                          printf("GTE\n");
                        #endif
                        return GTE;
                      }
\>                    {
                        #ifdef VERBOSE
                          printf("GT\n");
                        #endif
                        return GT;
                      }

                      /* ARITHMETIC OPERATORS */
\*                    {
                        #ifdef VERBOSE
                          printf("MUL\n");
                        #endif
                        return MUL;
                      }
\+                    {
                        #ifdef VERBOSE
                          printf("ADD\n");
                        #endif
                        return ADD;
                      }
-                     {
                        #ifdef VERBOSE
                          printf("SUB\n");
                        #endif
                        return SUB;
                      }
\/                    {
                        #ifdef VERBOSE
                          printf("DIV\n");
                        #endif
                        return DIV;
                      }

                      /* LOGICAL CONNECTIVES */
\~                    {
                        #ifdef VERBOSE
                          printf("NEG\n");
                        #endif
                        return NEG;
                      }
\&                    {
                        #ifdef VERBOSE
                          printf("AND\n");
                        #endif
                        return AND;
                      }
\|                    {
                        #ifdef VERBOSE
                          printf("OR\n");
                        #endif
                        return OR;
                      }

                      /* IGNORE WHITE SPACES */

[ \t\n]*

                      /* REST EVERYTHING UNKNOWN */

.                     {
                        #ifdef VERBOSE
                          printf("UNKN\n");
                        #endif
                        return UNKN;
                      }

%%
int yywrap(void) {
  return 1;
}

#ifdef IND_LEXER
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("USAGE : %s <input-file>\n", argv[0]);
    exit(1);
  } else {
    yyin = fopen(argv[1],"r");
    int yylex(void);
    while (yylex() != ENDOFFILE) {
      printf("------\n");
    }
    return 0;
  }
}
#endif
