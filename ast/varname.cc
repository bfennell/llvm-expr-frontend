/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <string>
#include <cassert>

#include "core.h"
#include "symtab.h"
#include "expr.h"
#include "varname.h"

using namespace AST;

VarName::VarName(const std::string& name) : name_(name) { nodeType = NodeType::VARNAME; }

VarName::~VarName() {}

void VarName::accept(Visitor* visitor) { visitor->visit(this); }

void VarName::codegen()
{
    llvm::IRBuilder<>& Builder = *Core::getBuilder();
    // llvm::Value* V = varTable[name_].llvmValue;
    llvm::Value* V = SymTab::getSymbolTable()->findVariable(name_);

    if (V == nullptr) return;

    // return V ? V : 0;
    value = Builder.CreateLoad(V, name_.c_str());
}
