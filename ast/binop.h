/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __BINOP_H__
#define __BINOP_H__

#include <memory>

#include "llvm.h"
#include "token.h"
#include "expr.h"

namespace AST
{
struct BinaryOperator : public Expression {
    BinaryOperator(const Operator op, std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    BinaryOperator(const BinaryOperator&) = delete;
    BinaryOperator& operator=(const BinaryOperator&) = delete;

    const Operator op_;
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};
}

#endif  // __BINOP_H__
