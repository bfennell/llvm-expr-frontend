/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <cassert>

#include "core.h"
#include "token.h"
#include "base.h"
#include "expr.h"
#include "iconst.h"

using namespace std;
using namespace AST;

IConstant::IConstant(Integer number) : number_(move(number))
{
    nodeType = NodeType::ICONST;
}

void IConstant::accept(Visitor* visitor) { visitor->visit(this); }

void IConstant::codegen()
{
    value = llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(double(number_.value)));
}
