/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __FORSTMT_H__
#define __FORSTMT_H__

#include <memory>

#include "base.h"
#include "block.h"
#include "expr.h"

namespace AST
{
struct ForStatement : public Base {
    ForStatement(std::unique_ptr<Expression> start, std::unique_ptr<Expression> end,
                 std::unique_ptr<Expression> step, std::string loopVar);
    void addBody(std::unique_ptr<Block> block);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    ForStatement(const ForStatement&) = delete;
    ForStatement& operator=(const ForStatement&) = delete;

    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::unique_ptr<Expression> step;
    std::string loopVar;
    std::unique_ptr<Block> body;
};
}

#endif  // __FORSTMT_H__
