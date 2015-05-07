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
 * test function which drives inputs to the Checker
 * needs two file inputs - first one contains a BLTL property in a single line
 *                       - second one contains the trace; the first line of this
 *                           file should be of the form
 *                            "# <signal 1> <signal 2> ..."
 *                           every other line should be of the form
 *                            "<time-stamp> <value of signal 1> <value of
 *                                signal 2> ..."
 *
 * while the actual Checker code needs the property as a separate file, the trace
 * is something specific to this file and can of course, be generated online
 * (see interface.cpp in this module for more details)
 */

#include "checker.hpp"

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
extern int yyparse (void *YYPARSE_PARAM);
#else
extern int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
extern int yyparse (void);
#else
extern int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

extern FILE *yyin;

int main(int argc, char **argv) {
  FILE *trace;
  vector<string> state_vars;
  int size, length_explored;
  char *line, *temp, *tok;
  double f, ts;
  vector<pair<string, double> > state;
  valType t;
  Checker *c;

  if (argc != 3) {
    fprintf(stderr, "ERROR: Format is %s <property-file> <trace-file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  yyin = fopen(argv[1], "r");
  if (!yyin) {
    fprintf(stderr, "ERROR: Property file does not exist\n");
    exit(EXIT_FAILURE);
  }
  yyparse();
  fclose(yyin);

  c = new Checker();
  trace = fopen(argv[2], "r");
  line = (char *)calloc(sizeof(char), 100);
  if (fgets(line, 100, trace) == NULL) {
    cout << "empty file?" << endl;
    exit(EXIT_FAILURE);
  }
  temp = line;
  tok = strtok(temp, " #\t\n");
  if (strcmp(tok, "time") != 0) {
    cout << "incorrect format - first line of trace file" << endl;
    exit(EXIT_FAILURE);
  }
  while ((tok = strtok(NULL, " #\t\n")) != NULL) {
    state_vars.push_back(string(tok));
  }

  length_explored = 0;
  while (1) {
    if (fgets(line, 100, trace) == NULL) {
      cout << "cannot decide : insufficient trace!" << endl;
      break;
    }
    temp = line;
    ts = atof(strtok(temp, " \t\n"));
    for (int i = 0; i < state_vars.size(); i++) {
      f = atof(strtok(NULL, " \t\n"));
      state.push_back(make_pair(state_vars.at(i), f));
    }
    length_explored++;
    #ifdef VERBOSE
    cout << "step " << length_explored << endl;
    #endif
    if ((t = (*c).advance(state, ts)) == SAT) {
      cout << "satisfiable!" << endl;
      break;
    } else if (t == UNSAT) {
      cout << "unsatisfiable!" << endl;
      break;
    }
    state.clear();
  }
  fclose(trace);

  delete c;
  exit(EXIT_SUCCESS);
}
