/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "base.h"
#include "block.h"
#include "expr.h"
#include "ifstmt.h"

using namespace std;
using namespace AST;

IfStatement::IfStatement(unique_ptr<Expression> cond) : cond_(move(cond)), then_(), else_()
{
    nodeType = NodeType::IFSTMT;
}

void IfStatement::addThen(unique_ptr<Block> block) { then_ = move(block); }

void IfStatement::addElse(unique_ptr<Block> block) { else_ = move(block); }

void IfStatement::accept(Visitor* visitor) { visitor->visit(this); }

void IfStatement::codegen()
{
    llvm::Value* cond;
    llvm::IRBuilder<>& Builder = *Core::getBuilder();

    cond_->codegen();
    cond = cond_->value;
    if (cond == 0) {
        return;
    }

    // Convert condition to a bool by comparing equal to 0.0.
    cond = Builder.CreateFCmpONE(
        cond, llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(0.0)), "ifcond");

    llvm::Function* func = Builder.GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(Core::ThisContext, "then", func);
    llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(Core::ThisContext, "else");
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(Core::ThisContext, "ifcont");

    Builder.CreateCondBr(cond, thenBB, elseBB);

    // Emit then value.
    Builder.SetInsertPoint(thenBB);
    then_->codegen();
    llvm::Value* thenv = then_->value;

    Builder.CreateBr(mergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    thenBB = Builder.GetInsertBlock();

    // Emit else block.
    func->getBasicBlockList().push_back(elseBB);

    Builder.SetInsertPoint(elseBB);
    else_->codegen();
    llvm::Value* elsev = else_->value;

    Builder.CreateBr(mergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    elseBB = Builder.GetInsertBlock();

    // Emit merge block.
    func->getBasicBlockList().push_back(mergeBB);

    Builder.SetInsertPoint(mergeBB);
    llvm::PHINode* PN =
        Builder.CreatePHI(llvm::Type::getDoubleTy(Core::ThisContext), 2, "iftmp");

    PN->addIncoming(thenv, thenBB);
    PN->addIncoming(elsev, elseBB);

    value = PN;
}
