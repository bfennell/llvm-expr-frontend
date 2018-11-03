/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __VARDECL_H__
#define __VARDECL_H__

#include <string>
#include <memory>

#include "llvm.h"
#include "base.h"
#include "expr.h"

namespace AST
{
struct VarDecl : public Base {
    VarDecl(const std::string& name, llvm::Type* type, std::unique_ptr<Expression> init);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    VarDecl(const VarDecl&) = delete;
    VarDecl& operator=(const VarDecl&) = delete;

    const std::string name;
    llvm::Type* type;
    std::unique_ptr<Expression> init;
};
}

#endif  // __VARDECL_H__
