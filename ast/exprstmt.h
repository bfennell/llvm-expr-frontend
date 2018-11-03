/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __EXPRSTMT_H__
#define __EXPRSTMT_H__

#include <memory>

#include "llvm.h"
#include "base.h"
#include "block.h"
#include "expr.h"

namespace AST
{
struct ExprStatement : public Base {
    ExprStatement(std::unique_ptr<Expression> expr);
    void accept(Visitor* visitor) override final;
    void codegen() override final;

    // delete defaults
    ExprStatement(const ExprStatement&) = delete;
    ExprStatement& operator=(const ExprStatement&) = delete;

    std::unique_ptr<Expression> expr_;
};
}

#endif  // __EXPRSTMT_H__
