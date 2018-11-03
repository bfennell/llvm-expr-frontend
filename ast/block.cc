/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "base.h"
#include "block.h"

using namespace std;
using namespace AST;

Block::Block() : body() {}

void Block::add(unique_ptr<Base> code_)
{
    nodeType = NodeType::BLOCK;
    body.push_back(move(code_));
}

void Block::accept(Visitor* visitor) { visitor->visit(this); }

void Block::codegen()
{
    for (auto& e : body) {
        e->codegen();
        value = e->value;
    }
}
