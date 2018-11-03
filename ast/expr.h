/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __EXPR_H__
#define __EXPR_H__

#include "llvm.h"
#include "base.h"

namespace AST
{
struct Expression : public Base {
    virtual ~Expression(){};
    virtual void accept(Visitor* visitor) = 0;
    virtual void codegen() = 0;
};
}

#endif  // __EXPR_H__
