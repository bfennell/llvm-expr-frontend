/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __UOP_H__
#define __UOP_H__

#include <memory>

#include "llvm.h"
#include "token.h"
#include "base.h"
#include "expr.h"

namespace AST
{
struct UnaryOperator : public Expression {
    UnaryOperator(const Operator op, std::unique_ptr<Expression> expr);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    UnaryOperator(const UnaryOperator&) = delete;
    UnaryOperator& operator=(const UnaryOperator&) = delete;

    const Operator op_;
    std::unique_ptr<Expression> expr_;
};
}

#endif  // __UOP_H__
