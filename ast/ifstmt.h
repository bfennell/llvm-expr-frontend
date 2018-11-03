/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __IFSTMT_H__
#define __IFSTMT_H__

#include <memory>

#include "llvm.h"
#include "base.h"
#include "block.h"
#include "expr.h"

namespace AST
{
struct IfStatement : public Base {
    IfStatement(std::unique_ptr<Expression> cond_);
    void addThen(std::unique_ptr<Block> block);
    void addElse(std::unique_ptr<Block> block);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    IfStatement(const IfStatement&) = delete;
    IfStatement& operator=(const IfStatement&) = delete;

    std::unique_ptr<Expression> cond_;
    std::unique_ptr<Block> then_;
    std::unique_ptr<Block> else_;
};
}

#endif  // __IFSTMT_H__
