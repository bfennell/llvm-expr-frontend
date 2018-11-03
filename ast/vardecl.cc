/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <string>
#include <memory>

#include "core.h"
#include "symtab.h"
#include "base.h"
#include "expr.h"
#include "vardecl.h"

using namespace std;
using namespace AST;

VarDecl::VarDecl(const string& name, llvm::Type* type, unique_ptr<Expression> init_)
    : name(name), type(type), init(move(init_))
{
    nodeType = NodeType::VARDECL;
}

void VarDecl::accept(Visitor* visitor) { visitor->visit(this); }

void VarDecl::codegen()
{
    llvm::IRBuilder<>& Builder = *Core::getBuilder();
    llvm::Function* func = Builder.GetInsertBlock()->getParent();

    init->codegen();
    value = init->value;

    llvm::AllocaInst* alloca = Core::createLocaVar(func, name, type);
    Builder.CreateStore(value, alloca);

    SymTab::getSymbolTable()->addVariable(name, alloca);
}
