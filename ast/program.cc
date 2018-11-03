/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <vector>
#include <memory>

#include "llvm.h"
#include "base.h"
#include "program.h"

using namespace AST;

Program::Program() : nodes() {}

void Program::add(std::unique_ptr<Base> e) { nodes.push_back(std::move(e)); }

void Program::accept(Visitor* visitor) { visitor->visit(this); }

void Program::codegen()
{
    for (auto& it : nodes) {
        it->codegen();
    }
}
