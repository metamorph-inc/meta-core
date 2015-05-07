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
 * Implements the methods (functions) of the class Checker declared in checker.hpp
 * which basically is everything needed for the online trace model checker
 */

#include "checker.hpp"

Checker::Checker() {
  /* val_init_tree is the value propagated to the root of the very first tree
   * build from the parse tree at the initial step
   */
  val_init_tree = DONT_KNOW;
  curr_list.clear();
  aux_list.clear();
  prev_vect.clear();
  prev_vect_map.clear();
  ts_old = ts_new = 0.0;
}

Checker::~Checker() {
}

/**
 * This is one of the crucial functions which avoids replication of logically
 * exactly same subtrees of the parse tree, except for the bounds. This function
 * "patches" a node corresponding to a temporal operator with other
 * corresponding bounds
 */
void Checker::patch(LogicalRelationalNode *to, LogicalRelationalNode *from, double duration) {
  if (from == NULL) {
    /* nothing to patch */
    return;
  }
  vector<LogicalRelationalNode::LogicalRelational::Bound>::iterator it;
  LogicalRelationalNode::LogicalRelational::Bound new_bound, b;
  int i;
  /* if "from" has been pulled up (by pull_node), follow the "path" to get to it */
  for (i = 0; i < (from->lr).path.size(); i++) {
    if ((from->lr).path.at(i) == 'l') {
      to = (LogicalRelationalNode *)(to->left);
    } else if ((from->lr).path.at(i) == 'r') {
      to = (LogicalRelationalNode *)(to->right);
    }
  }
  /* assert */
  if (to->golden != from->golden) {
    /* oops - something is wrong */
    cout << "patch error : cannot patch" << endl;
    exit(EXIT_FAILURE);
  }
  int zero_type = 0; // the beginning "type" for zero bounds
  switch ((to->lr).op) {
    case NEXT_L:
      if (zero_type == 0) {
        zero_type = 1;
      }
    case FUTURE_L:
      if (zero_type == 0) {
        zero_type = 2;
      }
    case GLOBAL_L:
      if (zero_type == 0) {
        zero_type = 2;
      }
    case UNTIL_L:
      if (zero_type == 0) {
        zero_type = 3;
      }
      for (i = 0; i < (from->lr).vect.size(); i++) {
        b = (from->lr).vect.at(i);
        if ((from->lr).op == NEXT_L) {
          if (b.isValid == true && b.val == DONT_KNOW && b.bound == 1) {
            for (it = (to->lr).vect.begin(); it != (to->lr).vect.end(); it++) {
              if ((*it).bound == 0) {
                /* the same bound is already present in the future tree, "to". The
                 * value must be DONT_KNOW (SAT or UNSAT is only possible with NOOP
                 * (true or false) which is never patched - can insert an additional
                 * check for this), the type depends on the bound (which must have
                 * already been taken care of). So, we just need to add to
                 * the propagateList */
                (*it).propagateList.push_back(make_pair(make_pair(from, b.bound), HISTORY));
                (from->lr).vect.at(i).future = make_pair(to, 0);
                break;
              }
            }
            if (it == (to->lr).vect.end()) {
              /* no match with existing bounds */
              new_bound = LogicalRelationalNode::LogicalRelational::Bound(0, DONT_KNOW, zero_type);
              new_bound.propagateList.push_back(make_pair(make_pair(from, b.bound), HISTORY));
              (to->lr).vect.push_back(new_bound);
              (to->lr).vect_map.insert(make_pair(new_bound.bound, (to->lr).vect.size() - 1));
              (to->lr).numValid++;
              (from->lr).vect.at(i).future = make_pair(to, 0);
            }
          }
        } else {
          if (b.isValid == true && b.val == DONT_KNOW && b.bound >= 0) {
            for (it = (to->lr).vect.begin(); it != (to->lr).vect.end(); it++) {
              if ((*it).bound == b.bound - duration) {
                /* the same bound is already present in the future tree, "to". The
                 * value must be DONT_KNOW (SAT or UNSAT is only possible with NOOP
                 * (true or false) which is never patched - can insert an additional
                 * check for this), the type depends on the bound (which must have
                 * already been taken care of). So, we just need to add to
                 * the propagateList */
                (*it).propagateList.push_back(make_pair(make_pair(from, b.bound), HISTORY));
                (from->lr).vect.at(i).future = make_pair(to, b.bound - duration);
                break;
              }
            }
            if (it == (to->lr).vect.end()) {
              /* no match with existing bounds */
              if (b.bound > duration) {
                new_bound = LogicalRelationalNode::LogicalRelational::Bound(b.bound - duration, DONT_KNOW, 0);
                new_bound.propagateList.push_back(make_pair(make_pair(from, b.bound), HISTORY));
                (to->lr).vect.push_back(new_bound);
                (to->lr).vect_map.insert(make_pair(new_bound.bound, (to->lr).vect.size() - 1));
                (to->lr).numValid++;
                (from->lr).vect.at(i).future = make_pair(to, b.bound - duration);
              } else if (b.bound == duration) {
                new_bound = LogicalRelationalNode::LogicalRelational::Bound(b.bound - duration, DONT_KNOW, zero_type);
                new_bound.propagateList.push_back(make_pair(make_pair(from, b.bound), HISTORY));
                (to->lr).vect.push_back(new_bound);
                (to->lr).vect_map.insert(make_pair(new_bound.bound, (to->lr).vect.size() - 1));
                (to->lr).numValid++;
                (from->lr).vect.at(i).future = make_pair(to, b.bound - duration);
              } else {
                if (b.bound != 0) { // o.w. "from" already knows
                  no_future_list.push_back(make_pair(from, i)); // no future needed for ith Bound of prev_tree
                }
              }
            }
          }
        }
      }
      /* no need to break here */
    case AND_L:
    case OR_L:
      /* nothing to patch */
      patch((LogicalRelationalNode *)(to->left), (LogicalRelationalNode *)(from->left), duration);
      patch((LogicalRelationalNode *)(to->right), (LogicalRelationalNode *)(from->right), duration);
      break;
    default:
      cout << "NOOP_L or relational operator in \"to\" in patch" << endl;
      exit(EXIT_FAILURE);
  }
}

/**
 * copies only the node at src and returns the result leaving the children
 * pointers NULL
 */
Node *Checker::node_copy(Node *src, double ts) {
  if (src == NULL) {
    return NULL;
  }
  int i;
  if (src->op_type == ARITHMETIC) {
    return new ArithmeticNode(ARITHMETIC,
                              NULL,
                              NULL,
                              src->golden,
                              ts,
                              (((ArithmeticNode *)src)->a).op,
                              (((ArithmeticNode *)src)->a).var,
                              (((ArithmeticNode *)src)->a).val);
  } else { // src->op_type == LOGICAL_RELATIONAL
    return new LogicalRelationalNode(LOGICAL_RELATIONAL,
                                     NULL,
                                     NULL,
                                     src->golden,
                                     ts,
                                     (((LogicalRelationalNode *)src)->lr).op,
                                     (((LogicalRelationalNode *)src)->lr).vect,
                                     (((LogicalRelationalNode *)src)->lr).path);
  }
}

/**
 * prev_vect can contain trees whose golden trees are subtrees of golden trees
 * of other trees in prev_vect. so, traverse the tree rooted at n, checking for
 * any such matches and if found, patching and marking them used in prev_vect
 */
void Checker::traverse_patch(LogicalRelationalNode *n, double duration) {
  if (n == NULL) {
    return;
  }
  if (prev_vect_map.find(((LogicalRelationalNode *)(n->golden))) != prev_vect_map.end()) {
    if (prev_vect.at(prev_vect_map[((LogicalRelationalNode *)(n->golden))]).second == false) {
      patch(n, prev_vect.at(prev_vect_map[((LogicalRelationalNode *)(n->golden))]).first, duration);
      prev_vect.at(prev_vect_map[((LogicalRelationalNode *)(n->golden))]).second = true;
    }
  }
  traverse_patch(((LogicalRelationalNode *)(n->left)), duration);
  traverse_patch(((LogicalRelationalNode *)(n->right)), duration);
}

/**
 *  it is known that "tree" has no future - its value depends only on its
 *  children
 */
void Checker::no_future(LogicalRelationalNode *tree, int i) {
  /* it is known to be either F, G or U only and tree != NULL */
  switch ((tree->lr).op) {
    case FUTURE_L:
      switch ((tree->lr).vect.at(i).type) {
        case 0:
          (tree->lr).vect.at(i).type = 2;
          break;
        case 1:
          set_and_propagate(tree, i, UNSAT, 3);
          break;
        case 2:
        case 3:
          cout << "error : finalizing FUTURE can't be in type 2 or 3" << endl;
          exit(EXIT_FAILURE);
          break;
      }
      break;

    case GLOBAL_L:
      switch ((tree->lr).vect.at(i).type) {
        case 0:
          (tree->lr).vect.at(i).type = 2;
          break;
        case 1:
          set_and_propagate(tree, i, SAT, 3);
          break;
        case 2:
        case 3:
          cout << "error : finalizing GLOBAL can't be in type 2 or 3" << endl;
          exit(EXIT_FAILURE);
          break;
      }
      break;

    case UNTIL_L:
      switch ((tree->lr).vect.at(i).type) {
        case 0:
          (tree->lr).vect.at(i).type = 3;
          break;
        case 1:
          (tree->lr).vect.at(i).type = 6;
          break;
        case 2:
          set_and_propagate(tree, i, UNSAT, 8);
          break;
        case 3:
          cout << "error : finalizing UNTIL : can't be in type 3" << endl;
          exit(EXIT_FAILURE);
          break;
        case 4:
          cout << "error : finalizing UNTIL : can't be in type 4" << endl;
          exit(EXIT_FAILURE);
          break;
        case 5:
          set_and_propagate(tree, i, UNSAT, 8);
          break;
        case 6:
        case 7:
        case 8:
          cout << "error : finalizing UNTIL : can't be in type 6, 7 and 8" << endl;
          exit(EXIT_FAILURE);
          break;
      }
      break;
  }
}

/**
 * builds a future tree from prev_tree. duration is used to get the new bounds
 * for future
 */
void Checker::build_from(LogicalRelationalNode *prev_tree, double duration) {
  if (prev_tree == NULL) {
    return;
  }
  vector<LogicalRelationalNode::LogicalRelational::Bound> new_vect;
  vector<LogicalRelationalNode::LogicalRelational::Bound>::iterator it;
  LogicalRelationalNode::LogicalRelational::Bound new_bound, b;
  int zero_type = 0; /* the beginning "type" for "zero" bounds
                      *
                      * if the bound is zero, this is the last step and hence,
                      * has to be treated differently */
  switch ((prev_tree->lr).op) {
    case AND_L:
    case OR_L:
      build_from((LogicalRelationalNode *)(prev_tree->left), duration);
      build_from((LogicalRelationalNode *)(prev_tree->right), duration);
      break;
    case NEXT_L:
      if (zero_type == 0) {
        zero_type = 1;
      }
    case FUTURE_L:
      if (zero_type == 0) {
        zero_type = 2;
      }
    case GLOBAL_L:
      if (zero_type == 0) {
        zero_type = 2;
      }
    case UNTIL_L:
      if (zero_type == 0) {
        zero_type = 3;
      }
      for (int i = 0; i < (prev_tree->lr).vect.size(); i++) {
        b = (prev_tree->lr).vect.at(i);
        if ((prev_tree->lr).op == NEXT_L) {
          if (b.isValid == true && b.val == DONT_KNOW && b.bound == 1) { /* if the Bound needs to be evaluated and 
                                                       * the prev bound is 1 */
            /* do not care about "propagateList" and can't set "future" yet,
             * as the node is not created! */
            new_bound = LogicalRelationalNode::LogicalRelational::Bound(0, DONT_KNOW, zero_type);
            new_bound.propagateList.push_back(make_pair(make_pair(prev_tree, b.bound), HISTORY));
            new_vect.push_back(new_bound);
          }
        } else {
          if (b.isValid == true && b.val == DONT_KNOW && b.bound >= 0) { /* if the Bound needs to be evaluated and 
                                                     * the prev bound is non-negative */
            /* do not care about "propagateList" and can't set "future" yet,
             * as the node is not created! */
            if (b.bound > duration) {
              new_bound = LogicalRelationalNode::LogicalRelational::Bound(b.bound - duration, DONT_KNOW, 0);
              new_bound.propagateList.push_back(make_pair(make_pair(prev_tree, b.bound), HISTORY));
              new_vect.push_back(new_bound);
            } else if (b.bound == duration) {
              new_bound = LogicalRelationalNode::LogicalRelational::Bound(b.bound - duration, DONT_KNOW, zero_type);
              new_bound.propagateList.push_back(make_pair(make_pair(prev_tree, b.bound), HISTORY));
              new_vect.push_back(new_bound);
            } else {
              if (b.bound != 0) { // o.w. prev_tree already knows that, that was its last step!
                no_future_list.push_back(make_pair(prev_tree, i)); // no future needed for ith Bound of prev_tree
              }
            }
          }
        }
      }
      /* new_vect can be empty if all non-negative bounds have already been
       * evaluated */
      if (new_vect.empty() == true) {
        build_from((LogicalRelationalNode *)(prev_tree->left), duration);
        build_from((LogicalRelationalNode *)(prev_tree->right), duration);
        return;
      }
      LogicalRelationalNode *new_tree;
      new_tree = (LogicalRelationalNode *)node_copy(prev_tree->golden, ts_new);
      new_tree->right = nodep_copy(prev_tree->golden->right, ts_new);
      if (new_tree->right != NULL) {
        new_tree->right->parent = new_tree;
        if (new_tree->right->op_type == LOGICAL_RELATIONAL) {
          for (int i = 0; i < (((LogicalRelationalNode *)(new_tree->right))->lr).vect.size(); i++) {
            (((LogicalRelationalNode *)(new_tree->right))->lr).vect.at(i).propagateList.clear();
            (((LogicalRelationalNode *)(new_tree->right))->lr).vect.at(i).propagateList.push_back(
                 make_pair(make_pair(new_tree, -0.1), RIGHT));
          }
        }
      }
      /* copied vect has parent in the propagateList which is not needed now */
      (new_tree->lr).vect.clear();
      (new_tree->lr).vect = new_vect;
      /* the node is done. set the remaining members */
      for (int i = 0; i < (new_tree->lr).vect.size(); i++) {
        if ((new_tree->lr).vect_map.insert(make_pair((new_tree->lr).vect.at(i).bound, i)).second
            == false) {
          cout << "Map creation : same bounds in the list!" << endl;
          exit(EXIT_FAILURE);
        }
      }
      (new_tree->lr).numValid = (new_tree->lr).vect.size();        
      /* set the future pointers for prev_tree */
      if ((prev_tree->lr).op == NEXT_L) {
        for (it = new_vect.begin(); it != new_vect.end(); it++) {
          (prev_tree->lr).vect.at((prev_tree->lr).vect_map[1]).future =
            make_pair(new_tree, (*it).bound);
        }
      } else {
        for (it = new_vect.begin(); it != new_vect.end(); it++) {
          (prev_tree->lr).vect.at((prev_tree->lr).vect_map[(*it).bound + duration]).future =
            make_pair(new_tree, (*it).bound);
        }
      }
      new_tree->left = nodep_copy(prev_tree->golden->left, ts_new);
      if (new_tree->left != NULL) {
        new_tree->left->parent = new_tree;
        if (new_tree->left->op_type == LOGICAL_RELATIONAL) {
          for (int i = 0; i < (((LogicalRelationalNode *)(new_tree->left))->lr).vect.size(); i++) {
            (((LogicalRelationalNode *)(new_tree->left))->lr).vect.at(i).propagateList.clear();
            (((LogicalRelationalNode *)(new_tree->left))->lr).vect.at(i).propagateList.push_back(
                 make_pair(make_pair(new_tree, -0.1), LEFT));
          }
        }
      }
      patch((LogicalRelationalNode *)(new_tree->left), (LogicalRelationalNode *)(prev_tree->left), duration);
      patch((LogicalRelationalNode *)(new_tree->right), (LogicalRelationalNode *)(prev_tree->right), duration);
      prev_vect.at(prev_vect_map[((LogicalRelationalNode *)(prev_tree->golden))]).second = true; /* prev_tree has been used */
      traverse_patch(((LogicalRelationalNode *)(new_tree->left)), duration);
      traverse_patch(((LogicalRelationalNode *)(new_tree->right)), duration);
      curr_list.push_back(make_pair(new_tree,false));
      break;
    default:
      cout << "NOOP_L or relational operator in prev_tree" << endl;
      exit(EXIT_FAILURE);
  }
}

/**
 * wrapper function for build_from to build trees from prev_vect
 */
void Checker::build_tree_list(double duration) {
  vector<pair<LogicalRelationalNode *, bool> >::iterator it;
  if (prev_vect.empty() == true) {
    /* first iteration */
    if (duration > 0) {
      cout << "error : initial timestamp is greater than zero!" << endl;
      exit(EXIT_FAILURE);
    }
    curr_list.push_back(make_pair((LogicalRelationalNode *)nodep_copy(parse_tree, ts_new),false));
  } else {
    for (it = prev_vect.begin(); it != prev_vect.end(); it++) {
      if ((*it).second == false) {
        build_from((*it).first, duration);
      }
    }
  }
}

/**
 * gets value of "var" from the state
 */
double Checker::get_val(string var, vector<pair<string, double> > state) {
  vector<pair<string, double> >::iterator it;
  for (it = state.begin(); it != state.end(); it++) {
    if (var.compare((*it).first) == 0) {
      return (*it).second;
    }
  }
  cout << "get_val : no match found" << endl;
  exit(EXIT_FAILURE);
}

/**
 * evaluates the arithmetic
 */
void Checker::eval_arithmetic(ArithmeticNode *n, vector<pair<string, double> > state) {
  if (n == NULL) {
    return;
  }
  switch ((n->a).op) {
    case MUL_A:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      (n->a).val = (((ArithmeticNode *)(n->left))->a).val * (((ArithmeticNode *)(n->right))->a).val;
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case ADD_A:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      (n->a).val = (((ArithmeticNode *)(n->left))->a).val + (((ArithmeticNode *)(n->right))->a).val;
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case SUB_A:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if (n->left == NULL) {
        (n->a).val = 0 - (((ArithmeticNode *)(n->right))->a).val;
      } else {
        (n->a).val = (((ArithmeticNode *)(n->left))->a).val - (((ArithmeticNode *)(n->right))->a).val;
      }
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case DIV_A:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      (n->a).val = (((ArithmeticNode *)(n->left))->a).val / (((ArithmeticNode *)(n->right))->a).val;
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case NOOP_A:
      if ((n->a).var.empty() == false) {
        (n->a).val = get_val((n->a).var, state);
      }
      break;
  }
}

/**
 * evaluates a logical or relational operator recursively
 * and maintains a list of evaluated nodes to begin propagation
 * (this list is global)
 *
 * assumption : all relational operators should be able to evaluate to values as
 * its children can only be some arithmetic of the state variables
 */
void Checker::eval_tree(LogicalRelationalNode *n, vector<pair<string, double> > state) {
  if (n == NULL) {
    return;
  }
  switch ((n->lr).op) {
    case NEXT_L:
      if ((n->lr).vect.size() == 1 && (n->lr).vect.at(0).bound == 1) {
        /* no need of right child */
        remove(n,
               (LogicalRelationalNode *)(n->right),
               (((LogicalRelationalNode *)(n->right->golden))->lr).vect.at(0).bound,
               false);
        n->right = NULL;
        break;
      }
    case AND_L:
    case OR_L:
    case FUTURE_L:
    case GLOBAL_L:
    case UNTIL_L:
      eval_tree((LogicalRelationalNode *)(n->left), state);
      eval_tree((LogicalRelationalNode *)(n->right), state);
      break;
    case NOOP_L:
      init_propagateList.push_back(n);
      break;
    case EQ_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val == (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case NEQ_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val != (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case LTE_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val <= (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case LT_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val < (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case GTE_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val >= (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
    case GT_R:
      eval_arithmetic((ArithmeticNode *)(n->left), state);
      eval_arithmetic((ArithmeticNode *)(n->right), state);
      if ((((ArithmeticNode *)(n->left))->a).val > (((ArithmeticNode *)(n->right))->a).val) {
        (n->lr).vect.at(0).val = SAT;
      } else {
        (n->lr).vect.at(0).val = UNSAT;
      }
      init_propagateList.push_back(n);
      delete ((ArithmeticNode *)(n->left)); delete ((ArithmeticNode *)(n->right));
      n->left = NULL; n->right = NULL;
      break;
  }
}

/**
 * this has dual functionality depending on the value of isNecessary
 *
 * if true - is useful in pulling the node up. this means that there is no need
 * to propagate to n1 from n2 (corresponding to "bound") but it is "necessary"
 * to evaluate n2
 *
 * if false - this means that there is no need to propagate to n1 from n2
 * (corresponding to "bound") and also, evaluation of n2 is not necessary "if
 * not needed by anyone else"
 */
void Checker::remove(LogicalRelationalNode *n1, LogicalRelationalNode *n2, double bound, bool isNecessary) {
  list<pair<pair<LogicalRelationalNode *, double>, nodeType> >::iterator it;
  list<pair<LogicalRelationalNode *, bool> >::iterator it1;
  for (it = (n2->lr).vect.at((n2->lr).vect_map[bound]).propagateList.begin();
          it != (n2->lr).vect.at((n2->lr).vect_map[bound]).propagateList.end(); it++) {
    if ((*it).first.first == n1) {
      (n2->lr).vect.at((n2->lr).vect_map[bound]).propagateList.erase(it); 
      break;
    }
  }
  if (n1 == n2->parent) { /* parent has just been removed from propagateList
                           * this means, either all the Bounds in parent are
                           * evaluated (o.w., child may still be needed for
                           * unevaluated Bounds and won't be asked to not
                           * propagate to parent) OR this node is pulled up to
                           * its parent, i.e parent is getting deleted anyway.
                           * so, set this node's parent to NULL */
    n2->parent = NULL;
    /* check if n2 is in current step and if so, add to curr_list */
    if (n2->ts == ts_new) {
      aux_list.push_back(make_pair(n2,false));
    }
  }
  if (isNecessary == false) { /* the value of this node is not needed for n1.
                               * in other words, n1 has already been evaluated */
    if ((n2->lr).vect.at((n2->lr).vect_map[bound]).propagateList.empty() == true) {
      /* no need to propagate to anyone */
      switch ((n2->lr).op) {
        case NEXT_L:
        case AND_L:
        case OR_L:
        case FUTURE_L:
        case GLOBAL_L:
        case UNTIL_L:
          if ((n2->lr).vect.at((n2->lr).vect_map[bound]).future.first != NULL) {
            remove(n2,
                   (n2->lr).vect.at((n2->lr).vect_map[bound]).future.first,
                   (n2->lr).vect.at((n2->lr).vect_map[bound]).future.second,
                   false);
            (n2->lr).vect.at((n2->lr).vect_map[bound]).future.first = NULL;
          }
          (n2->lr).vect.at((n2->lr).vect_map[bound]).isValid = false;
          (n2->lr).numValid--;
          if ((n2->lr).numValid == 0) {
            if (n2->left != NULL) {
              remove(n2,
                     (LogicalRelationalNode *)(n2->left),
                     (((LogicalRelationalNode *)(n2->left->golden))->lr).vect.at(0).bound,
                     false);
              n2->left = NULL;
            }
            if (n2->right != NULL) {
              remove(n2,
                     (LogicalRelationalNode *)(n2->right),
                     (((LogicalRelationalNode *)(n2->right->golden))->lr).vect.at(0).bound,
                     false);
              n2->right = NULL;
            }
            /* if n2 is in curr_list or aux_list, remove it */
            if (n2->ts == ts_new) { /* o.w., it won't be in curr_list or aux_list */
              for (it1 = curr_list.begin(); it1 != curr_list.end(); it1++) {
                if ((*it1).first == n2) {
                  (*it1).second = true;
                  break;
                }
              }
              for (it1 = aux_list.begin(); it1 != aux_list.end(); it1++) {
                if ((*it1).first == n2) {
                  (*it1).second = true;
                  break;
                }
              }
            }
            delete n2;
          }
          break;
        default:
          /* this covers the cases of relational operators and NOOP - this means
           * the node is in init_propagateList used in propagate() */
          (n2->lr).vect.at((n2->lr).vect_map[bound]).isValid = false;
          (n2->lr).numValid--;
          if ((n2->lr).numValid == 0) {
            /* this will be the case!
             * search for n2 in init_propagateList and erase that element */
            list<LogicalRelationalNode *>::iterator it;
            for (it = init_propagateList.begin(); it != init_propagateList.end(); it++) {
              if ((*it) == n2) {
                init_propagateList.erase(it);
                break;
              }
            }
            /* if n2 is in curr_list or aux_list, remove it */
            if (n2->ts == ts_new) { /* o.w., it won't be in curr_list or aux_list */
              for (it1 = curr_list.begin(); it1 != curr_list.end(); it1++) {
                if ((*it1).first == n2) {
                  (*it1).second = true;
                  break;
                }
              }
              for (it1 = aux_list.begin(); it1 != aux_list.end(); it1++) {
                if ((*it1).first == n2) {
                  (*it1).second = true;
                  break;
                }
              }
            }
            delete n2;
          }
          break;
      }
    }
  }
}

/**
 * replace old_node by new_node in the propagateList of n corresponding to
 * "bound"
 */
void Checker::change_propagateList(LogicalRelationalNode *n,
                           double bound,
                           LogicalRelationalNode *old_node,
                           LogicalRelationalNode *new_node,
                           nodeType t) {
  list<pair<pair<LogicalRelationalNode *, double>, nodeType> >::iterator it;
  for (it = (n->lr).vect.at((n->lr).vect_map[bound]).propagateList.begin();
          it != (n->lr).vect.at((n->lr).vect_map[bound]).propagateList.end(); it++) {
    if ((*it).first.first == old_node) {
      (*it).first.first = new_node;
      (*it).second = t;
      break;
    }
  }
}

/**
 * pulls the node to the side of n given by t, in place of n
 * 
 * currently, used and implemented only for the case where the operator at n is
 * either AND or OR
 */
void Checker::pull_node(LogicalRelationalNode *n, nodeType t) {
  list<pair<LogicalRelationalNode *, bool> >::iterator it;
  if (t == LEFT) {
    if (n->parent == NULL) {
      /* remove n from the corresponding propagateList in
       * n->left */
      remove(n,
             (LogicalRelationalNode *)(n->left),
             (((LogicalRelationalNode *)(n->left->golden))->lr).vect.at(0).bound,
             true);
                /* golden bound always corresponds to child */
      n->left->parent = NULL;
    } else {
      if (n == n->parent->left) {
        n->parent->left = n->left;
        /* replace n by n->parent in propagateLists of n->left corresponding to
         * the given bound, and change nodeType to LEFT */
        change_propagateList((LogicalRelationalNode *)(n->left),
                              (((LogicalRelationalNode *)(n->left->golden))->lr).vect.at(0).bound,
                              n,
                              (LogicalRelationalNode *)(n->parent),
                              LEFT);
      } else { // n == n->parent->right
        n->parent->right = n->left;
        /* replace n by n->parent in propagateLists of n->left corresponding to
         * the given bound, and change nodeType to RIGHT */
        change_propagateList((LogicalRelationalNode *)(n->left),
                              (((LogicalRelationalNode *)(n->left->golden))->lr).vect.at(0).bound,
                              n,
                              (LogicalRelationalNode *)(n->parent),
                              RIGHT);
      }
      n->left->parent = n->parent;
      (((LogicalRelationalNode *)(n->left))->lr).path.append(1, 'l');
    }
    /* if n is in curr_list or aux_list, remove it */
    if (n->ts == ts_new) { /* o.w., it won't be in curr_list or aux_list */
      for (it = curr_list.begin(); it != curr_list.end(); it++) {
        if ((*it).first == n) {
          (*it).second = true;
          break;
        }
      }
      for (it = aux_list.begin(); it != aux_list.end(); it++) {
        if ((*it).first == n) {
          (*it).second = true;
          break;
        }
      }
    }
    delete n;
  } else if (t == RIGHT) {
    if (n->parent == NULL) {
      /* remove n from the corresponding propagateList in
       * n->right */
      remove(n,
             (LogicalRelationalNode *)(n->right),
             (((LogicalRelationalNode *)(n->right->golden))->lr).vect.at(0).bound,
             true);
                /* golden bound always corresponds to child */
      n->right->parent = NULL;
    } else {
      if (n == n->parent->left) {
        n->parent->left = n->right;
        /* replace n by n->parent in propagateLists of n->right corresponding to
         * the given bound, and change nodeType to LEFT */
        change_propagateList((LogicalRelationalNode *)(n->right),
                              (((LogicalRelationalNode *)(n->right->golden))->lr).vect.at(0).bound,
                              n,
                              (LogicalRelationalNode *)(n->parent),
                              LEFT);
      } else { // n == n->parent->right
        n->parent->right = n->right;
        /* replace n by n->parent in propagateLists of n->right corresponding to
         * the given bound, and change nodeType to RIGHT */
        change_propagateList((LogicalRelationalNode *)(n->right),
                              (((LogicalRelationalNode *)(n->right->golden))->lr).vect.at(0).bound,
                              n,
                              (LogicalRelationalNode *)(n->parent),
                              RIGHT);
      }
      n->right->parent = n->parent;
      (((LogicalRelationalNode *)(n->right))->lr).path.append(1, 'r');
    }
    /* if n is in curr_list or aux_list, remove it */
    if (n->ts == ts_new) { /* o.w., it won't be in curr_list or aux_list */
      for (it = curr_list.begin(); it != curr_list.end(); it++) {
        if ((*it).first == n) {
          (*it).second = true;
          break;
        }
      }
      for (it = aux_list.begin(); it != aux_list.end(); it++) {
        if ((*it).first == n) {
          (*it).second = true;
          break;
        }
      }
    }
    delete n;
  } else { // t == HISTORY
    cout << "error : pulling future node?" << endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * set the value of Bound at index in n to t, change type to "final_type" and
 * propagate the value
 */
void Checker::set_and_propagate(LogicalRelationalNode *n, int index, valType t, int final_type) {
  if (t == DONT_KNOW) {
    cout << "error : setting a value of type DONT_KNOW" << endl;
    exit(EXIT_FAILURE);
  }
  if ((n->lr).vect.at(index).isValid == false) { /* this particular Bound is
                                                  * invalid (i.e. it is not required
                                                  * to set its value or propagate from
                                                  * it) */
    return;
  }
  list<pair<pair<LogicalRelationalNode *, double>, nodeType> >::iterator it;
  list<pair<LogicalRelationalNode *, bool> >::iterator it1;
  (n->lr).vect.at(index).val = t;
  (n->lr).vect.at(index).type = final_type;
  if ((n->lr).vect.at(index).propagateList.empty() == true) {
    /* yay! we reached the end! */
    val_init_tree = t;
  } else {
    for (it = (n->lr).vect.at(index).propagateList.begin();
            it != (n->lr).vect.at(index).propagateList.end(); it++) {
      if ((*it).first.first == ((LogicalRelationalNode *)(n->parent))) {
        n->parent = NULL; /* we do not maintain history pointers, so
                             don't worry about them */
      }
      propagate_value((*it).first.first, (*it).first.second, (*it).second, t);
    }
  }
  /* no need of future */
  if ((n->lr).vect.at(index).future.first != NULL) {
    remove(n,
           (n->lr).vect.at(index).future.first,
           (n->lr).vect.at(index).future.second,
           false);
    (n->lr).vect.at(index).future.first = NULL;
  }
  (n->lr).vect.at(index).propagateList.clear();
  (n->lr).vect.at(index).isValid = false;
  (n->lr).numValid--;
  if ((n->lr).numValid == 0) {
    /* all propagations done. delete this node */
    /* no need of children */
    if (n->left != NULL) {
      remove(n,
             (LogicalRelationalNode *)(n->left),
             (((LogicalRelationalNode *)(n->left->golden))->lr).vect.at(0).bound,
             false);
      n->left = NULL;
    }
    if (n->right != NULL) {
      remove(n,
             (LogicalRelationalNode *)(n->right),
             (((LogicalRelationalNode *)(n->right->golden))->lr).vect.at(0).bound,
             false);
      n->right = NULL;
    }
    /* check if n is in curr_list or aux_list and delete it if present */
    if (n->ts == ts_new) { /* o.w., it won't be in curr_list or aux_list */
      for (it1 = curr_list.begin(); it1 != curr_list.end(); it1++) {
        if ((*it1).first == n) {
          (*it1).second = true;
          break;
        }
      }
      for (it1 = aux_list.begin(); it1 != aux_list.end(); it1++) {
        if ((*it1).first == n) {
          (*it1).second = true;
          break;
        }
      }
    }
    delete n;
  }
}

/**
 * propagating "from_val" from "from" to "n" corresponding to "bound"
 *
 * it can either evaluate to a value, in which case it calls set_and_propagate,
 * or change its internal state "type"
 */
void Checker::propagate_value(LogicalRelationalNode *n, double bound, nodeType from, valType from_val) {
  if (n == NULL) {
    cout << "error : propagating values to NULL" << endl;
    exit(EXIT_FAILURE);
  }
  switch ((n->lr).op) {
    case AND_L:
      if (bound >= 0) {
        cout << "error : non-negative bound propagated to an AND node" << endl;
        exit(EXIT_FAILURE);
      }
      switch ((n->lr).vect.at(0).type) {
        case 0:
          switch (from) {
            case LEFT:
              n->left = NULL;
              switch (from_val) {
                case SAT:
                  pull_node(n, RIGHT);
                  break;
                case UNSAT:
                  set_and_propagate(n, 0, UNSAT, 1);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case RIGHT:
              n->right = NULL;
              switch (from_val) {
                case SAT:
                  pull_node(n, LEFT);
                  break;
                case UNSAT:
                  set_and_propagate(n, 0, UNSAT, 1);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case HISTORY:
              cout << "error : propagating to AND node from future!" << endl;
              exit(EXIT_FAILURE);
              break;
          }
          break;
        default:
          cout << "error : type != 0 for AND node" << endl;
          exit(EXIT_FAILURE);
          break;
      }
      break;

    case OR_L:
      if (bound >= 0) {
        cout << "error : non-negative bound propagated to an OR node" << endl;
        exit(EXIT_FAILURE);
        return;
      }
      switch ((n->lr).vect.at(0).type) {
        case 0:
          switch (from) {
            case LEFT:
              n->left = NULL;
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, 0, SAT, 1);
                  break;
                case UNSAT:
                  pull_node(n, RIGHT);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case RIGHT:
              n->right = NULL;
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, 0, SAT, 1);
                  break;
                case UNSAT:
                  pull_node(n, LEFT);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case HISTORY:
              cout << "error : propagating to OR node from future!" << endl;
              exit(EXIT_FAILURE);
              break;
          }
          break;
        default:
          cout << "error : type != 0 for AND node" << endl;
          exit(EXIT_FAILURE);
          break;
      }
      break;

    case NEXT_L:
      if (bound < 0) { // from right child
        if (from != RIGHT) {
          cout << "error : bound is -0.1 implying a child, but propagated to FUTURE node from a non-right child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        n->right = NULL;
        for (int i = 0; i < (n->lr).vect.size(); i++) {
          switch ((n->lr).vect.at(i).type) {
            case 0:
              /* this is possible if vect[j] for some j has type 1 and needs a
               * right child but vect[i] only needs future
               *
               * do nothing */
              break;
            case 1:
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, i, SAT, 2);
                  break;
                case UNSAT:
                  set_and_propagate(n, i, UNSAT, 2);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 2:
              /* this is possible if vect[i] reduced to a value by knowing
               * its future but other bounds in vect[] may need right
               * child/future
               *
               * do nothing */
              break;
          }
        }
      } else { // from future!
        if (from != HISTORY) {
          cout << "error : bound is non-negative implying future, but propagated to FUTURE node from a child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is the future */
        (n->lr).vect.at((n->lr).vect_map[bound]).future.first = NULL;
        switch ((n->lr).vect.at((n->lr).vect_map[bound]).type) {
          case 0:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 2);
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 2);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          default:
            cout << "error : NEXT node, propagation from future, can't be type > 0" << endl;
            exit(EXIT_FAILURE);
            break;
        }
      }
      break;

    case FUTURE_L:
      if (bound < 0) { // it is for all Bounds : from the right child
        if (from != RIGHT) {
          cout << "error : bound is -0.1 implying a child, but propagated to FUTURE node from a non-right child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is the right child */
        n->right = NULL;
        for (int i = 0; i < (n->lr).vect.size(); i++) {
          switch ((n->lr).vect.at(i).type) {
            case 0:
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, i, SAT, 3);
                  break;
                case UNSAT:
                  (n->lr).vect.at(i).type = 1;
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 1:
              cout << "error : FUTURE node, propagation from the right child, can't be type 1" << endl;
              exit(EXIT_FAILURE);
              break;
            case 2:
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, i, SAT, 3);
                  break;
                case UNSAT:
                  set_and_propagate(n, i, UNSAT, 3);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 3:
              /* this is possible if vect[i] reduced to a value by knowing
               * its future but other bounds in vect[] may need right child
               *
               * do nothing */
              break;
            default:
              cout << "error : type > 3 for FUTURE while propagating from right child!" << endl;
              exit(EXIT_FAILURE);
              break;
          }
        }
      } else { // it is for a specific bound : from future
        if (from != HISTORY) {
          cout << "error : bound is non-negative implying future, but propagated to FUTURE node from a child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is the future */
        (n->lr).vect.at((n->lr).vect_map[bound]).future.first = NULL;
        switch ((n->lr).vect.at((n->lr).vect_map[bound]).type) {
          case 0:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 3);
                break;
              case UNSAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 2;
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 1:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 3);
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 3);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 2:
            cout << "error : FUTURE node, propagation from the future, can't be type 2" << endl;
            exit(EXIT_FAILURE);
            break;
          default:
            cout << "error : type > 2 for future propagation to FUTURE!" << endl;
            exit(EXIT_FAILURE);
            break;
        }
      }
      break;

    case GLOBAL_L:
      if (bound < 0) { // it is for all Bounds : from the right child
        if (from != RIGHT) {
          cout << "error : bound is -0.1 implying a child, but propagated to GLOBAL node from a non-right child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is the right child */
        n->right = NULL;
        for (int i = 0; i < (n->lr).vect.size(); i++) {
          switch ((n->lr).vect.at(i).type) {
            case 0:
              switch (from_val) {
                case SAT:
                  (n->lr).vect.at(i).type = 1;
                  break;
                case UNSAT:
                  set_and_propagate(n, i, UNSAT, 3);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 1:
              cout << "error : GLOBAL node, propagation from the right child, can't be type 1" << endl;
              exit(EXIT_FAILURE);
              break;
            case 2:
              switch (from_val) {
                case SAT:
                  set_and_propagate(n, i, SAT, 3);
                  break;
                case UNSAT:
                  set_and_propagate(n, i, UNSAT, 3);
                  break;
                case DONT_KNOW:
                  cout << "error : propagating \"DONT_KNOW\"!" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 3:
              /* already evaluated */
              break;
            default:
              cout << "error : type > 3 for GLOBAL for propagation from right child!" << endl;
              exit(EXIT_FAILURE);
              break;
          }
        }
      } else { // it is for a specific bound : from future
        if (from != HISTORY) {
          cout << "error : bound is non-negative implying future, but propagated to GLOBAL node from a child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is the future */
        (n->lr).vect.at((n->lr).vect_map[bound]).future.first = NULL;
        switch ((n->lr).vect.at((n->lr).vect_map[bound]).type) {
          case 0:
            switch (from_val) {
              case SAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 2;
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 3);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 1:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 3);
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 3);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 2:
            cout << "error : GLOBAL node, propagation from the future, can't be type 2" << endl;
            exit(EXIT_FAILURE);
            break;
          default:
            cout << "error : type > 2 for future propagation to GLOBAL!" << endl;
            exit(EXIT_FAILURE);
            break;
        }
      }
      break;

    case UNTIL_L:
      if (bound < 0) { // it is for all Bounds : from a child
        if (from == HISTORY) {
          cout << "error : bound is -0.1 but \"from\" is HISTORY" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        for (int i = 0; i < (n->lr).vect.size(); i++) {
          switch ((n->lr).vect.at(i).type) {
            case 0:
              switch (from) {
                case LEFT:
                  n->left = NULL;
                  switch (from_val) {
                    case SAT:
                      (n->lr).vect.at(i).type = 1;
                      break;
                    case UNSAT:
                      (n->lr).vect.at(i).type = 6;
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
                case RIGHT:
                  n->right = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      (n->lr).vect.at(i).type = 2;
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
              }
              break;
            case 1:
              switch (from) {
                case LEFT:
                  cout << "error : propagating to UNTIL from left child in type 1" << endl;
                  exit(EXIT_FAILURE);
                  break;
                case RIGHT:
                  n->right = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      (n->lr).vect.at(i).type = 5;
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
              }
              break;
            case 2:
              switch (from) {
                case LEFT:
                  n->left = NULL;
                  switch (from_val) {
                    case SAT:
                      (n->lr).vect.at(i).type = 5;
                      break;
                    case UNSAT:
                      set_and_propagate(n, i, UNSAT, 8);
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
                case RIGHT:
                  cout << "error : propagating to UNTIL from right child in type 2" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 3:
              switch (from) {
                case LEFT:
                  n->left = NULL;
                  /* ignore */
                  break;
                case RIGHT:
                  n->right = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      set_and_propagate(n, i, UNSAT, 8);
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
              }
              break;
            case 4:
              switch (from) {
                case LEFT:
                  n->left = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      (n->lr).vect.at(i).type = 6;
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
                case RIGHT:
                  n->right = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      (n->lr).vect.at(i).type = 7;
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
              }
              break;
            case 5:
              cout << "propagated to UNTIL from children in type 5" << endl;
              exit(EXIT_FAILURE);
              break;
            case 6:
              switch (from) {
                case LEFT:
                  cout << "error : propagating from left child in type 6" << endl;
                  exit(EXIT_FAILURE);
                  break;
                case RIGHT:
                  n->right = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      set_and_propagate(n, i, UNSAT, 8);
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
              }
              break;
            case 7:
              switch (from) {
                case LEFT:
                  n->left = NULL;
                  switch (from_val) {
                    case SAT:
                      set_and_propagate(n, i, SAT, 8);
                      break;
                    case UNSAT:
                      set_and_propagate(n, i, UNSAT, 8);
                      break;
                    case DONT_KNOW:
                      cout << "error : propagating \"DONT_KNOW\"!" << endl;
                      exit(EXIT_FAILURE);
                      break;
                  }
                  break;
                case RIGHT:
                  cout << "error : propagating to UNTIL from right child in type 7" << endl;
                  exit(EXIT_FAILURE);
                  break;
              }
              break;
            case 8:
              /* already evaluated */
              break;
          }
        }
      } else {
        if (from != HISTORY) {
          cout << "non-negative bound but propagated from a child" << endl;
          exit(EXIT_FAILURE);
          return;
        }
        /* from is HISTORY */
        (n->lr).vect.at((n->lr).vect_map[bound]).future.first = NULL;
        switch ((n->lr).vect.at((n->lr).vect_map[bound]).type) {
          case 0:
            switch (from_val) {
              case SAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 4;
                break;
              case UNSAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 3;
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 1:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 8);
                break;
              case UNSAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 6;
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 2:
            switch (from_val) {
              case SAT:
                (n->lr).vect.at((n->lr).vect_map[bound]).type = 7;
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 8);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 3:
            cout << "error : propagating to UNTIL from future in type 3" << endl;
            exit(EXIT_FAILURE);
            break;
          case 4:
            cout << "error : propagating to UNTIL from future in type 4" << endl;
            exit(EXIT_FAILURE);
            break;
          case 5:
            switch (from_val) {
              case SAT:
                set_and_propagate(n, (n->lr).vect_map[bound], SAT, 8);
                break;
              case UNSAT:
                set_and_propagate(n, (n->lr).vect_map[bound], UNSAT, 8);
                break;
              case DONT_KNOW:
                cout << "error : propagating \"DONT_KNOW\"!" << endl;
                exit(EXIT_FAILURE);
                break;
            }
            break;
          case 6:
            cout << "error : propagating to UNTIL from future in type 6" << endl;
            exit(EXIT_FAILURE);
            break;
          case 7:
            cout << "error : propagating to UNTIL from future in type 7" << endl;
            exit(EXIT_FAILURE);
            break;
          case 8:
            /* already evaluated */
            break;
        }
      }
      break;

    case NOOP_L:
      cout << "error : propagating values to NOOP LogicalRelational node" << endl;
      exit(EXIT_FAILURE);
      break;
  }
}

/**
 * initiates propagation from the values evaluated in this iteration
 */
void Checker::propagate(void) { /* each node in init_propagateList already has values.
                                 * so, just propagate and delete itself */
  list<LogicalRelationalNode *>::iterator it;
  for (it = init_propagateList.begin(); it != init_propagateList.end(); it++) {
    /* note that init_propagateList only contains evaluated RelationalNodes or NOOP LogicalNodes.
     * Thus, there are no history pointers and Bound vector has just one
     * element */
    set_and_propagate((*it), 0, ((*it)->lr).vect.at(0).val, -1);
  }
}

/**
 * parent function which evaluates each tree built at this step and then calls
 * to propagate those values
 */
valType Checker::evaluate(vector<pair<string, double> > state) {
  list<pair<LogicalRelationalNode *, bool> >::iterator it;
  list<pair<LogicalRelationalNode *, int> >::iterator it1;
  for (it = curr_list.begin(); it != curr_list.end(); it++) {
    if ((*it).second == false) {
      eval_tree((*it).first, state);
    }
  }
  for (it1 = no_future_list.begin(); it1 != no_future_list.end(); it1++) {
    no_future((*it1).first, (*it1).second);
  }
  propagate();
  if (val_init_tree == SAT) {
    return SAT;
  } else if (val_init_tree == UNSAT) {
    return UNSAT;
  }
  return DONT_KNOW;
}

/**
 * the interface function to the outside world which advances the checker by one
 * step to consider the next state and timestamp
 */
valType Checker::advance(vector<pair<string, double> > state, double ts) {
  ts_new = ts;
  double duration = ts_new - ts_old;
  valType result;
  list<pair<LogicalRelationalNode *, bool> >::iterator it;
  build_tree_list(duration);
  result = evaluate(state);
  prev_vect.clear();
  prev_vect_map.clear();
  no_future_list.clear();
  init_propagateList.clear();
  for (it = curr_list.begin(); it != curr_list.end(); it++) {
    if ((*it).second == false) {
      prev_vect.push_back(make_pair((*it).first, false));
    }
  }
  curr_list.clear();
  for (it = aux_list.begin(); it != aux_list.end(); it++) {
    if ((*it).second == false) {
      prev_vect.push_back(make_pair((*it).first, false));
    }
  }
  for (int i = 0; i < prev_vect.size(); i++) {
    prev_vect_map.insert(make_pair(((LogicalRelationalNode *)((prev_vect.at(i).first)->golden)), i));
  }
  aux_list.clear();
  ts_old = ts_new;
  return result;
}
