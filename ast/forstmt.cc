/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "symtab.h"
#include "base.h"
#include "block.h"
#include "expr.h"
#include "forstmt.h"

using namespace std;
using namespace AST;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction,
                                                const std::string& VarName)
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(Core::ThisContext), 0, VarName.c_str());
}

ForStatement::ForStatement(unique_ptr<Expression> start_, unique_ptr<Expression> end_,
                           unique_ptr<Expression> step_, std::string loopVar)
    : start(move(start_)), end(move(end_)), step(move(step_)), loopVar(loopVar), body()
{
    nodeType = NodeType::FORSTMT;
}

void ForStatement::addBody(unique_ptr<Block> block) { body = move(block); }

void ForStatement::accept(Visitor* visitor) { visitor->visit(this); }

void ForStatement::codegen()
{
    llvm::IRBuilder<>& Builder = *Core::getBuilder();
    //
    // ...
    //   start = startexpr
    //   goto for.cond
    // for.cond:
    //   endcond = endexptr
    //   br endcond, for.body, for.exit
    // for.body:
    //   ...
    //   bodyexptr
    //   ...
    // br for.cond
    // for.exit:
    // ...

    SymTab::enterScope();

    llvm::Function* func = Builder.GetInsertBlock()->getParent();
    // allocate the for loop control variable
    llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(func, loopVar);
    // add to the symbol table
    SymTab::getSymbolTable()->addVariable(loopVar, Alloca);

    start->codegen();
    llvm::Value* startVal = start->value;
    // if (startVal == 0) {
    //    SymTab::exitScope();
    //    return nullptr;
    //}
    Builder.CreateStore(startVal, Alloca);

    llvm::BasicBlock* for_cond =
        llvm::BasicBlock::Create(Core::ThisContext, "for.cond", func);
    llvm::BasicBlock* for_body =
        llvm::BasicBlock::Create(Core::ThisContext, "for.body", func);
    llvm::BasicBlock* for_inc = llvm::BasicBlock::Create(Core::ThisContext, "for.inc", func);
    llvm::BasicBlock* for_exit =
        llvm::BasicBlock::Create(Core::ThisContext, "for.exit", func);

    // -------------------------
    // for.cond
    // -------------------------
    Builder.CreateBr(for_cond);
    Builder.SetInsertPoint(for_cond);

    end->codegen();
    llvm::Value* endVal = end->value;
    // if (endVal == nullptr) {
    //    SymTab::exitScope();
    //    return nullptr;
    //}

    endVal = Builder.CreateFCmpONE(
        endVal, llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(0.0)), "endcond");
    Builder.CreateCondBr(endVal, for_body, for_exit);

    // -------------------------
    // for.body
    // -------------------------
    Builder.SetInsertPoint(for_body);

    body->codegen();

    // -------------------------
    // for.inc
    // -------------------------
    Builder.CreateBr(for_inc);
    Builder.SetInsertPoint(for_inc);

    llvm::Value* stepVal;
    if (step) {
        step->codegen();
        stepVal = step->value;
        // if (stepVal == nullptr) {
        //    SymTab::exitScope();
        //    return nullptr;
        //}
    } else {
        // If not specified, use 1.0.
        stepVal = llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(1.0));
    }

    llvm::Value* CurVar = Builder.CreateLoad(Alloca, loopVar.c_str());
    llvm::Value* NextVar = Builder.CreateFAdd(CurVar, stepVal, "nextvar");
    Builder.CreateStore(NextVar, Alloca);
    Builder.CreateBr(for_cond);

    // -------------------------
    // for.exit
    // -------------------------
    Builder.SetInsertPoint(for_exit);

    SymTab::exitScope();

    value = llvm::Constant::getNullValue(llvm::Type::getDoubleTy(Core::ThisContext));
}
