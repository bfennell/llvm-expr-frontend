/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "core.h"
#include "symtab.h"
#include "base.h"
#include "block.h"
#include "funcdecl.h"
#include "funcdef.h"

#include <iostream>
using namespace std;
using namespace AST;

FunctionDef::FunctionDef(unique_ptr<FunctionDecl> decl_) : decl(move(decl_)), body()
{
    nodeType = NodeType::FUNCDEF;
    SymTab::getSymbolTable()->addFunction(decl->name, this);
}

FunctionDef* FunctionDef::setBody(unique_ptr<Block> body_)
{
    body = move(body_);
    return this;
}

void FunctionDef::accept(Visitor* visitor) { visitor->visit(this); }

void FunctionDef::codegen()
{
    llvm::Function* func;
    llvm::IRBuilder<>& Builder = *Core::getBuilder();

    SymTab::enterScope();

    decl->codegen();
    func = static_cast<llvm::Function*>(decl->value);

    if (func == 0) {
        return;
    }

    if (body != nullptr) {
        llvm::BasicBlock* BB = llvm::BasicBlock::Create(Core::ThisContext, "entry", func);
        Builder.SetInsertPoint(BB);

        // Add all arguments to the symbol table and create their allocas.
        decl->createArgumentAllocas(func);

        body->codegen();
        llvm::Value* RetVal = body->value;

        if (RetVal) {
            Builder.CreateRet(RetVal);
            llvm::verifyFunction(*func);
            // Optimize the function.
            Core::getFPM()->run(*func);
        } else {
            func->eraseFromParent();
            func = nullptr;
        }
    }
    SymTab::exitScope();
    value = func;
}
