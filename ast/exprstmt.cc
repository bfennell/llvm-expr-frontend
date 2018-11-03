/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "base.h"
#include "block.h"
#include "expr.h"
#include "exprstmt.h"

using namespace std;
using namespace AST;

ExprStatement::ExprStatement(unique_ptr<Expression> expr) : expr_(move(expr))
{
    nodeType = NodeType::EXPRSTMT;
}

void ExprStatement::accept(Visitor* visitor) { visitor->visit(this); }

void ExprStatement::codegen()
{
    if (expr_ != nullptr) {
        expr_->codegen();
        value = expr_->value;
    }
}
