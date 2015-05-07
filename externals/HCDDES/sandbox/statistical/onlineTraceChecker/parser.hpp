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
 * This file defines the classes used for the different Nodes of the parse tree
 * and the other trees derived from the parse tree used in the algorithm
 */

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;

/* denotes the kind of propagation to the current node
 * LEFT - coming on the left edge
 * RIGHT - coming on the right edge
 * HISTORY - coming on the history edge (from the future node)
 */
typedef enum {LEFT,RIGHT,HISTORY} nodeType;
/* type of boolean value, DONT_KNOW if can't decide */
typedef enum {SAT,UNSAT,DONT_KNOW} valType;
/* type of a Node of a tree */
typedef enum {LOGICAL_RELATIONAL,ARITHMETIC} opType;
/* arithmetic operators allowed */
typedef enum {MUL_A,ADD_A,SUB_A,DIV_A,NOOP_A} opArithmetic;
/* logical and relational operators allowed */
typedef enum {AND_L,OR_L,NEXT_L,FUTURE_L,GLOBAL_L,UNTIL_L,NOOP_L,EQ_R,NEQ_R,LTE_R,LT_R,GTE_R,GT_R} opLogicalRelational;

/* the base Node class */
class Node {
public:
  opType op_type;
  Node *left, *right, *parent, *golden; /* golden is the corresponding node in
                                           the parse_tree */
  double ts; /* the timestamp (of the trace) at which this Node is created */

  Node() {
    left = right = parent = golden = NULL;
    ts = 0.0;
  }
  Node(opType t, Node *l, Node *r) {
    op_type = t;
    left = l;
    right = r;
    if (l != NULL) {
      l->parent = this;
    }
    if (r != NULL) {
      r->parent = this;
    }
    parent = NULL;
    golden = this;
    ts = 0.0;
  }
  Node(opType t, Node *l, Node *r, Node *g) {
    op_type = t;
    left = l;
    right = r;
    if (l != NULL) {
      l->parent = this;
    }
    if (r != NULL) {
      r->parent = this;
    }
    parent = NULL;
    golden = g;
    ts = 0.0;
  }
  Node(opType t, Node *l, Node *r, Node *g, double ts_p) {
    op_type = t;
    left = l;
    right = r;
    if (l != NULL) {
      l->parent = this;
    }
    if (r != NULL) {
      r->parent = this;
    }
    parent = NULL;
    golden = g;
    ts = ts_p;
  }
  ~Node() {}
};

/* derived class for arithmetic operators */
class ArithmeticNode : public Node {
public:
  class Arithmetic {
  public:
    opArithmetic op;
    string var;
    double val;
  };
  Arithmetic a;

  ArithmeticNode() {
    a.var = "";
    a.val = 0.0;
  }
  ArithmeticNode(opType op_type,
                 Node *left,
                 Node *right,
                 opArithmetic op,
                 string var,
                 double val) : Node(op_type, left, right) {
    a.op = op;
    a.var = var;
    a.val = val;
  }
  ArithmeticNode(opType op_type,
                 Node *left,
                 Node *right,
                 Node *golden,
                 double ts,
                 opArithmetic op,
                 string var,
                 double val) : Node(op_type, left, right, golden, ts) {
    a.op = op;
    a.var = var;
    a.val = val;
  }
  ~ArithmeticNode() {}
};

/* derived class for logical and relational operators */
class LogicalRelationalNode : public Node {
public:
  class LogicalRelational {
  public:
    class Bound {
    public:
      double bound; /* the bound value */
      valType val; /* value of the node corresponding to "bound" */
      int type; /* a logical "state" corresponding to the information seen from
                 * the children and future nodes, so far */
      list<pair<pair<LogicalRelationalNode *, double>, nodeType> > propagateList;
                /* list of nodes (and the corresponding bound in the node and
                 * type of "this" node w.r.t the node propagated to) to
                 * propagate the value of this node */
      pair<LogicalRelationalNode *, double> future;
                /* the future node for this bound */
      bool isValid; /* true iff this Bound is necessary; every decrease in
                       numValid is associated with some isValid becoming zero */

      Bound() {
        bound = -0.1;
        val = DONT_KNOW;
        type = 0;
        propagateList.clear();
        future.first = NULL;
        future.second = -0.1;
        isValid = true;
      }
      Bound(double bound_p, valType val_p, int type_p) {
        bound = bound_p;
        val = val_p;
        type = type_p;
        propagateList.clear();
        future.first = NULL;
        future.second = -0.1;
        isValid = true;
      }
    };
    opLogicalRelational op; /* the operator */
    vector<Bound> vect; /* the vector of Bounds (size > 1 due to patching - see patch()) */
    map<double,int> vect_map; /* a hashtable to access Bound corresponding to a 
                              * given "bound" in constant time */
    int numValid; /* if this becomes 0 at any time, this node can be deleted */
    string path; /* used for some efficiency concern */
  };
  LogicalRelational lr;

  LogicalRelationalNode() {
    LogicalRelational::Bound new_bound;
    lr.vect.clear();
    lr.vect.push_back(new_bound);
    lr.vect_map.insert(make_pair(new_bound.bound, 0));
    lr.numValid = 1;
    lr.path = "";
  }
  LogicalRelationalNode(opType op_type,
                        Node *left,
                        Node *right,
                        opLogicalRelational op,
                        double bound,
                        valType t,
                        int type) : Node(op_type, left, right) {
    lr.op = op;
    LogicalRelational::Bound new_bound(bound, t, type);
    lr.vect.clear();
    lr.vect.push_back(new_bound);
    lr.vect_map.insert(make_pair(new_bound.bound, 0));
    lr.numValid = 1;
    lr.path = "";
    /* set propagateList of children */
    if (left != NULL) {
      if (left->op_type == LOGICAL_RELATIONAL) {
        (((LogicalRelationalNode *)left)->lr).vect.at(0).propagateList.clear();
        (((LogicalRelationalNode *)left)->lr).vect.at(0).propagateList.push_back(
             make_pair(make_pair(this, -0.1), LEFT));
      }
    }
    if (right != NULL) {
      if (right->op_type == LOGICAL_RELATIONAL) {
        (((LogicalRelationalNode *)right)->lr).vect.at(0).propagateList.clear();
        (((LogicalRelationalNode *)right)->lr).vect.at(0).propagateList.push_back(
             make_pair(make_pair(this, -0.1), RIGHT));
      }
    }
  }
  LogicalRelationalNode(opType op_type,
                        Node *left,
                        Node *right,
                        Node *golden,
                        double ts,
                        opLogicalRelational op,
                        vector<LogicalRelational::Bound> vect,
                        string path) : Node(op_type, left, right, golden, ts) {
    lr.op = op;
    lr.vect.clear();
    vector<LogicalRelational::Bound>::iterator it;
    for (it = vect.begin(); it != vect.end(); it++) {
      lr.vect.push_back(LogicalRelational::Bound((*it).bound, (*it).val, (*it).type));
    }
    for (int i = 0; i < lr.vect.size(); i++) {
      lr.vect_map.insert(make_pair(lr.vect.at(i).bound, i));
    }
    lr.numValid = lr.vect.size();        
    lr.path = path;
    /* set propagateList of children */
    if (left != NULL) {
      if (left->op_type == LOGICAL_RELATIONAL) {
        for (int i = 0; i < (((LogicalRelationalNode *)left)->lr).vect.size(); i++) {
          (((LogicalRelationalNode *)left)->lr).vect.at(i).propagateList.clear();
          (((LogicalRelationalNode *)left)->lr).vect.at(i).propagateList.push_back(
               make_pair(make_pair(this, -0.1), LEFT));
        }
      }
    }
    if (right != NULL) {
      if (right->op_type == LOGICAL_RELATIONAL) {
        for (int i = 0; i < (((LogicalRelationalNode *)right)->lr).vect.size(); i++) {
          (((LogicalRelationalNode *)right)->lr).vect.at(i).propagateList.clear();
          (((LogicalRelationalNode *)right)->lr).vect.at(i).propagateList.push_back(
               make_pair(make_pair(this, -0.1), RIGHT));
        }
      }
    }
  }
  ~LogicalRelationalNode() {}
};

extern Node *parse_tree; /* the result of parsing */
extern Node *nodep_copy(Node *, double); /* function defined in parser.ypp */
