/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __FUNCCALL_H__
#define __FUNCCALL_H__

#include <vector>
#include <memory>

#include "llvm.h"
#include "base.h"
#include "funcdef.h"
#include "expr.h"

namespace AST
{
struct FunctionCall : public Expression {
    FunctionCall(FunctionDef* def, std::unique_ptr<std::vector<std::unique_ptr<Expression>>> args);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    FunctionCall(const FunctionCall&) = delete;
    FunctionCall& operator=(const FunctionCall&) = delete;

    FunctionDef* def;
    std::unique_ptr<std::vector<std::unique_ptr<Expression>>> args;
};
}

#endif  // __FUNCCALL_H__
