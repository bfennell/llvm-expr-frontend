/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <vector>
#include <memory>

#include "llvm.h"
#include "base.h"

namespace AST
{
struct Program : public Base {
    Program();
    void add(std::unique_ptr<Base> e);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    std::vector<std::unique_ptr<Base>> nodes;
    // delete defaults
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
};
}

#endif  // __PROGRAM_H__
