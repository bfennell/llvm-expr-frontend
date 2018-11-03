/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __ICONST_H__
#define __ICONST_H__

#include <memory>

#include "llvm.h"
#include "token.h"
#include "base.h"
#include "expr.h"

namespace AST
{
struct IConstant : public Expression {
    IConstant(Integer number);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    IConstant(const IConstant&) = delete;
    IConstant& operator=(const IConstant&) = delete;

    Integer number_;
};
}

#endif  // __ICONST_H__
