/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __FCONST_H__
#define __FCONST_H__

#include <memory>

#include "llvm.h"
#include "token.h"
#include "base.h"

namespace AST
{
struct FConstant : public Expression {
    FConstant(Real number);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    FConstant(const FConstant&) = delete;
    FConstant& operator=(const FConstant&) = delete;

    Real number_;
};
}

#endif  // __FCONST_H__
