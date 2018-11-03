/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <string>
#include <vector>

#include "core.h"
#include "symtab.h"
#include "base.h"
#include "funcdecl.h"

using namespace std;
using namespace AST;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction, const string& VarName)
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(Core::ThisContext), 0, VarName.c_str());
}

FunctionDecl::FunctionDecl(const string& name, const vector<string>& args, llvm::Type* retType)
    : name(name), args(args), retType(retType)
{
    nodeType = NodeType::FUNCDECL;
}

void FunctionDecl::accept(Visitor* visitor) { visitor->visit(this); }

void FunctionDecl::codegen()
{
    vector<llvm::Type*> Doubles(args.size(), llvm::Type::getDoubleTy(Core::ThisContext));
    llvm::FunctionType* FT =
        llvm::FunctionType::get(llvm::Type::getDoubleTy(Core::ThisContext), Doubles, false);
    llvm::Function* F =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, Core::getModule());

    if (F->getName() != name) {
        F->eraseFromParent();
        F = Core::getModule()->getFunction(name);

        if (!F->empty()) {
            return;
        }

        if (F->arg_size() != args.size()) {
            return;
        }
    }

    unsigned int idx = 0;
    for (llvm::Function::arg_iterator Arg_It = F->arg_begin(); idx != args.size();
         ++Arg_It, ++idx) {
        Arg_It->setName(args[idx]);
    }

    value = F;
}

/// CreateArgumentAllocas - Create an alloca for each argument and register the
/// argument in the symbol table so that references to it will succeed.
void FunctionDecl::createArgumentAllocas(llvm::Function* F)
{
    SymTab::SymbolTable* symbolTable = SymTab::getSymbolTable();
    llvm::Function::arg_iterator AI = F->arg_begin();
    llvm::IRBuilder<>& Builder = *Core::getBuilder();

    for (unsigned Idx = 0, e = args.size(); Idx != e; ++Idx, ++AI) {
        // Create an alloca for this variable.
        llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(F, args[Idx]);

        // Store the initial value into the alloca.
        Builder.CreateStore(&*AI, Alloca);

        // Add arguments to variable symbol table.
        // varTable[args[Idx]].llvmValue = Alloca;
        symbolTable->addVariable(args[Idx], Alloca);
    }
}
