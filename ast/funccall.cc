/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <vector>
#include <cassert>

#include "core.h"
#include "base.h"
#include "funccall.h"

using namespace std;
using namespace AST;

FunctionCall::FunctionCall(FunctionDef* def, unique_ptr<vector<unique_ptr<Expression>>> args_)
    : def(def), args(move(args_))
{
    nodeType = NodeType::FUNCCALL;
}

void FunctionCall::accept(Visitor* visitor) { visitor->visit(this); }

void FunctionCall::codegen()
{
    llvm::Function* CalleeF = Core::getModule()->getFunction(def->decl->name);
    llvm::IRBuilder<>& Builder = *Core::getBuilder();

    std::vector<llvm::Value*> vargs;

    for (unsigned i = 0, e = (*args).size(); i != e; ++i) {
        (*args)[i]->codegen();
        vargs.push_back((*args)[i]->value);

        //if (vargs.back() == 0) {
        //    return nullptr;
        //}
    }

    value = Builder.CreateCall(CalleeF, vargs, "calltmp");
}
