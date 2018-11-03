/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __CORE_H__
#define __CORE_H__

#include "llvm.h"
#include "parser.h"

namespace Core
{
void initCompiler();
void closeCompiler();
llvm::Module* getModule();
llvm::IRBuilder<>* getBuilder();
llvm::legacy::FunctionPassManager* getFPM();
llvm::ExecutionEngine* getExecEngine();
Parser* getParser();

llvm::Type* getTypeByName(const string& name);
llvm::AllocaInst* createLocaVar(llvm::Function* func, const string name, llvm::Type* type);
llvm::Value* getConstant(double value);

 extern llvm::LLVMContext ThisContext;
}

#endif  // __CORE_H__
