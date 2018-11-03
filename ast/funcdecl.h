/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __FUNCDECL_H__
#define __FUNCDECL_H__

#include <string>
#include <vector>

#include "llvm.h"
#include "base.h"

namespace AST
{
struct FunctionDecl : public Base {
    FunctionDecl(const std::string& name, const std::vector<std::string>& args,
                 llvm::Type* retType);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    void createArgumentAllocas(llvm::Function* F);
    // delete defaults
    FunctionDecl(const FunctionDecl&) = delete;
    FunctionDecl& operator=(const FunctionDecl&) = delete;

    std::string name;
    std::vector<std::string> args;
    llvm::Type* retType;
};
}

#endif  // __FUNCDECL_H__
