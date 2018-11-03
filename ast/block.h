/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <vector>
#include <memory>

#include "llvm.h"
#include "base.h"

namespace AST
{
  struct Block : public Base {
    Block();
    void add(std::unique_ptr<Base> code);
    void accept(Visitor* visitor) override final;
    void codegen() override final;
    // delete defaults
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    std::vector<std::unique_ptr<Base>> body;
  };
}

#endif  // __BLOCK_H__
