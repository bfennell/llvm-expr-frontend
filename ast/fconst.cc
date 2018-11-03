/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <cassert>

#include "core.h"
#include "token.h"
#include "base.h"
#include "fconst.h"

using namespace std;
using namespace AST;

FConstant::FConstant(Real number) : number_(number)
{
    nodeType = NodeType::FCONST;
}

void FConstant::accept(Visitor* visitor) { visitor->visit(this); }

void FConstant::codegen()
{
    value = llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(double(number_.value)));
}
