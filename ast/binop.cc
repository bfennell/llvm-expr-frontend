/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "symtab.h"
#include "expr.h"
#include "binop.h"

using namespace std;
using namespace AST;

BinaryOperator::BinaryOperator(const Operator op, unique_ptr<Expression> left,
                               unique_ptr<Expression> right)
    : op_(op), left_(move(left)), right_(move(right))
{
    nodeType = NodeType::BINOP;
}

void BinaryOperator::accept(Visitor *visitor) { visitor->visit(this); }

void BinaryOperator::codegen()
{
    llvm::IRBuilder<> &Builder = *Core::getBuilder();
    // Special case '=' because we don't want to emit the LHS as an expression.
    if (op_.id == TokenId_ASSIGN) {
        // Assignment requires the LHS to be an identifier.
        // This assume we're building without RTTI because LLVM builds that way by
        // default.  If you build LLVM with RTTI this can be changed to a
        // dynamic_cast for automatic error checking.
        VarName *LHSE = static_cast<VarName *>(left_.get());
        //        if (!LHSE) return nullptr;
        // Codegen the RHS.
        right_->codegen();
        value = right_->value;
        //        if (Val == 0) return 0;

        // Look up the name.
        // llvm::Value *Variable = varTable[LHSE->name_].llvmValue;
        llvm::Value *Variable = SymTab::getSymbolTable()->findVariable(LHSE->name_);
        //        if (Variable == 0) return nullptr;

        Builder.CreateStore(value, Variable);
    } else {
        // llvm::Value *L = left_->codegen();
        // llvm::Value *R = right_->codegen();
        left_->codegen();
        right_->codegen();

        // if (L == 0 || R == 0) return 0;

        switch (op_.id) {
            case TokenId_PLUS: {
                value = Builder.CreateFAdd(left_->value, right_->value, "addtmp");
                break;
            }
            case TokenId_MINUS: {
                value = Builder.CreateFSub(left_->value, right_->value, "subtmp");
                break;
            }
            case TokenId_LT: {
                llvm::Value *tmp = Builder.CreateFCmpULT(left_->value, right_->value, "cmptmp");
                // Convert bool 0/1 to double 0.0 or 1.0
                value = Builder.CreateUIToFP(tmp, llvm::Type::getDoubleTy(Core::ThisContext),
                                             "booltmp");
                break;
            }
            case TokenId_GT: {
                llvm::Value *tmp = Builder.CreateFCmpUGT(left_->value, right_->value, "cmptmp");
                // Convert bool 0/1 to double 0.0 or 1.0
                value = Builder.CreateUIToFP(tmp, llvm::Type::getDoubleTy(Core::ThisContext),
                                             "booltmp");
                break;
            }
            case TokenId_DIVIDE: {
                value = Builder.CreateFDiv(left_->value, right_->value, "div");
                break;
            }
            case TokenId_MULT: {
                value = Builder.CreateFMul(left_->value, right_->value, "multmp");
                break;
            }
            default: {
                value = nullptr;
                break;
            }
        }
    }
}
