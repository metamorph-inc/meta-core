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
 * the interface to the external software (C code) which wants to use the online model
 * checker
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

/**
 * assuming the external software is a C code
 */
extern "C" {
  void read_property(char *);
  void init_signals(char **, int);
  valType advance(double *, int);
}
extern FILE *yyin;
vector<string> state_vars;
vector<pair<string, double> > state;
int length_explored;
Checker *c;

/**
 * reads the BLTL property specified in the file whose path is specified in the
 * parameter `file'
 * 
 * ideally, this has to be the first thing to do
 */
void read_property(char *file) {
  yyin = fopen(file, "r");
  if (!yyin) {
    fprintf(stderr, "ERROR: Property file does not exist\n");
    exit(EXIT_FAILURE);
  }
  yyparse();
  fclose(yyin);
  c = NULL;
}

/**
 * register all the signals whose values appear in the trace
 *
 * parameter `list' contains the list of character strings of the
 * alphanumeric and case-sensitive names of the signals, in the exact order as
 * they appear in the trace
 *
 * parameter `len' is the number of signals in the parameter `list'
 *
 * a call to this has to precede a call to `advance' to check the trace
 */
void init_signals(char **list, int len) {
  state_vars.clear();
  for (int i = 0; i < len; i++) {
    state_vars.push_back(string(list[i]));
    #ifdef VERBOSE
    cout << list[i] << " ";
    #endif
  }
  #ifdef VERBOSE
  cout << endl;
  length_explored = 0;
  #endif
  /* create a new checker */
  delete c;
  c = new Checker();
}

/**
 * advances model checking the trace by one step
 *
 * parameter `new_state' is the sequence of double values corresponding to the
 * signals
 *
 * parameter `len' is the length of the sequence `new_state'
 *
 * returns
 *   SAT/UNSAT if the property satisfiability can be decided at this point on
 *             the trace
 *   DONT_KNOW if the trace has to explored further
 */
valType advance(double *new_state, int len) {
  #ifdef VERBOSE
  length_explored++;
  cout << "step " << length_explored << endl;
  #endif
  double ts = new_state[0];
  valType result;
  #ifdef VERBOSE
  cout << "timestamp : " << ts << endl;
  cout << ts << " ";
  #endif
  for (int i = 0; i < len; i++) {
    state.push_back(make_pair(state_vars.at(i), new_state[i+1]));
    #ifdef VERBOSE
    cout << new_state[i+1] << " ";
    #endif
  }
  #ifdef VERBOSE
  cout << endl;
  #endif
  result = c->advance(state, ts);
  state.clear();
  return result;
}
