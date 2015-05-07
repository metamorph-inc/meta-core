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


#include "parser.hpp"

class Checker {
private:
  list<LogicalRelationalNode *> init_propagateList;
  list<pair<LogicalRelationalNode *, bool> > curr_list, aux_list;
  list<pair<LogicalRelationalNode *, int> > no_future_list;
  vector<pair<LogicalRelationalNode *, bool> > prev_vect;
  map<LogicalRelationalNode *, int> prev_vect_map;
  valType val_init_tree;
  double ts_new, ts_old;

  void patch (LogicalRelationalNode *, LogicalRelationalNode *, double);
  Node *node_copy(Node *, double);
  void traverse_patch(LogicalRelationalNode *, double);
  void no_future(LogicalRelationalNode *, int);
  void build_from(LogicalRelationalNode *, double);
  void build_tree_list(double);
  double get_val(string, vector<pair<string, double> >);
  void eval_arithmetic(ArithmeticNode *, vector<pair<string, double> >);
  void eval_tree(LogicalRelationalNode *, vector<pair<string, double> >);
  void remove(LogicalRelationalNode *, LogicalRelationalNode *, double, bool);
  void change_propagateList(LogicalRelationalNode *,
                            double,
                            LogicalRelationalNode *,
                            LogicalRelationalNode *,
                            nodeType);
  void pull_node(LogicalRelationalNode *, nodeType);
  void set_and_propagate(LogicalRelationalNode *, int, valType, int);
  void propagate_value(LogicalRelationalNode *, double, nodeType, valType);
  void propagate(void);
  valType evaluate(vector<pair<string, double> >);
public:
  Checker();
  ~Checker();
  valType advance(vector<pair<string, double> >, double);
};
