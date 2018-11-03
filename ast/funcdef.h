/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __FUNCDEF_H__
#define __FUNCDEF_H__

#include <memory>

#include "llvm.h"
#include "base.h"
#include "block.h"
#include "funcdecl.h"

namespace AST
{
struct FunctionDef : public Base {
    FunctionDef(std::unique_ptr<FunctionDecl> decl);
    FunctionDef* setBody(std::unique_ptr<Block> body_);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    FunctionDef(const FunctionDef&) = delete;
    FunctionDef& operator=(const FunctionDef&) = delete;

    std::unique_ptr<FunctionDecl> decl;
    std::unique_ptr<Block> body;
};
}

#endif  // __FUNCDEF_H__
