#ifndef SFC_PRINT_H
#define SFC_PRINT_H
#include <set>
#include <map>
#include "SFC.h"

extern void Print(const SFC::Program& this_);

extern void Print(const SFC::Statement& this_, const std::string &tagPrefix);

extern void Print(const SFC::OperationalStatement& this_, const std::string &tagPrefix);
extern void Print(const SFC::Declaration& this_, const std::string &tagPrefix);

extern void Print(const SFC::CompoundStatement& this_, const std::string &tagPrefix);
extern void Print(const SFC::SimpleStatement& this_, const std::string &tagPrefix);

extern void Print(const SFC::StateLabel& this_);
extern void Print(const SFC::Var& this_, const std::string &tagPrefix);

extern void Print(const SFC::Block& this_, const std::string &tagPrefix);
extern void Print(const SFC::ConditionalGroup& this_, const std::string &tagPrefix);

extern void Print(const SFC::Return& this_);
extern void Print(const SFC::FunctionCall& this_);
extern void Print(const SFC::SetState& this_);
extern void Print(const SFC::SetVar& this_);
extern void Print(const SFC::UserCode& this_, bool isCond=false);

extern void Print(const SFC::LocalVar& this_);

extern void Print(const SFC::Function& this_, bool prototype= false);
extern void Print(const SFC::ConditionalBlock& this_);
extern void Print(const SFC::IterativeBlock& this_);

extern void Print(const SFC::Condition& this_);
extern void Print(const SFC::CheckArg& this_);
extern void Print(const SFC::CheckState& this_);

extern void Print(const SFC::Exprs& this_);
extern void Print(const SFC::BinaryExprs& this_);
extern void Print(const SFC::UnaryExprs& this_);
#endif
