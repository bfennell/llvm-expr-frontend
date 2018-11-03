/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "base.h"
#include "block.h"
#include "expr.h"
#include "retstmt.h"

using namespace std;
using namespace AST;

RetStatement::RetStatement(unique_ptr<Expression> retval) : retval_(move(retval))
{
    nodeType = NodeType::RETSTMT;
}

void RetStatement::accept(Visitor* visitor) { visitor->visit(this); }

void RetStatement::codegen()
{
    llvm::IRBuilder<>& Builder = *Core::getBuilder();

    llvm::Function* func = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* retEnterBB =
        llvm::BasicBlock::Create(Core::ThisContext, "ret_enter", func);
    llvm::BasicBlock* retExitBB =
        llvm::BasicBlock::Create(Core::ThisContext, "ret_exit", func);

    retval_->codegen();
    value = retval_->value;

    Builder.CreateBr(retEnterBB);
    Builder.SetInsertPoint(retEnterBB);
    Builder.CreateRet(value);
    Builder.CreateBr(retExitBB);
    Builder.SetInsertPoint(retExitBB);
}
