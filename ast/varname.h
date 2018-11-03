/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __VARNAME_H__
#define __VARNAME_H__

#include <string>

#include "llvm.h"
#include "expr.h"

namespace AST
{
struct VarName : public Expression {
    VarName(const std::string& name);
    ~VarName();
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    VarName(const VarName&) = delete;
    VarName& operator=(const VarName&) = delete;

    const std::string name_;
};
}

#endif  // __VARNAME_H__
