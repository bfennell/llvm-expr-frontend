/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <cassert>

#include "core.h"
#include "token.h"
#include "base.h"
#include "expr.h"
#include "uop.h"

using namespace std;
using namespace AST;

UnaryOperator::UnaryOperator(const Operator op, unique_ptr<Expression> expr)
    : op_(op), expr_(move(expr))
{
    nodeType = NodeType::UOP;
}

void UnaryOperator::accept(Visitor* visitor) { visitor->visit(this); }

void UnaryOperator::codegen()
{
    llvm::IRBuilder<>& Builder = *Core::getBuilder();
    llvm::Value* L = llvm::ConstantFP::get(llvm::Type::getDoubleTy(Core::ThisContext), -1);
    expr_->codegen();
    value = Builder.CreateFMul(L, expr_->value, "negtmp");
}
