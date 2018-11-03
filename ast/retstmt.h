/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __RETSTMT_H__
#define __RETSTMT_H__

#include <memory>

#include "llvm.h"
#include "base.h"
#include "block.h"
#include "expr.h"

namespace AST
{
struct RetStatement : public Base {
    RetStatement(std::unique_ptr<Expression> retval);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    RetStatement(const RetStatement&) = delete;
    RetStatement& operator=(const RetStatement&) = delete;

    std::unique_ptr<Expression> retval_;
};
}

#endif  // __RETSTMT_H__
