/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <iostream>

#include "parser.h"
#include "symtab.h"
#include "ast/ast.h"

#include "core.h"

using namespace std;

llvm::LLVMContext Core::ThisContext;

static llvm::Module* module = nullptr;
static llvm::IRBuilder<> builder(Core::ThisContext);
static unique_ptr<llvm::legacy::FunctionPassManager> fpm;
static llvm::ExecutionEngine* execEngine = nullptr;
static unique_ptr<Parser> parser;
static string llvmErrStr;

void Core::initCompiler()
{
    SymTab::initSymbolTable();

    parser = make_unique<Parser>();

    // LLVM initialisation
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    unique_ptr<llvm::Module> owner =
        llvm::make_unique<llvm::Module>("expr_mcjit", Core::ThisContext);
    module = owner.get();

    execEngine = llvm::EngineBuilder(move(owner))
                     .setErrorStr(&llvmErrStr)
                     .setMCJITMemoryManager(llvm::make_unique<llvm::SectionMemoryManager>())
                     .create();

    if (!execEngine) {
        cerr << "Could not create ExecutionEngine: " << llvmErrStr.c_str() << endl;
        exit(1);
    }

    fpm = std::make_unique<llvm::legacy::FunctionPassManager>(module);

    // Set up the optimizer pipeline.  Start with registering info about how the
    // target lays out data structures.
    module->setDataLayout(execEngine->getDataLayout());

    fpm->add(llvm::createCostModelAnalysisPass());
    //fpm->add(llvm::createPromoteMemoryToRegisterPass());
    //fpm->add(llvm::createInstructionCombiningPass());
    fpm->add(llvm::createReassociatePass());
    fpm->add(llvm::createNewGVNPass());
#if 0
    fpm->add(llvm::createCFGSimplificationPass());
#endif
    fpm->doInitialization();
}

void Core::closeCompiler() { delete execEngine; }

llvm::Module* Core::getModule() { return module; }

llvm::IRBuilder<>* Core::getBuilder() { return &builder; }

llvm::legacy::FunctionPassManager* Core::getFPM() { return fpm.get(); }

llvm::ExecutionEngine* Core::getExecEngine() { return execEngine; }

Parser* Core::getParser() { return parser.get(); }

llvm::AllocaInst* Core::createLocaVar(llvm::Function* func, const string name, llvm::Type* type)
{
    llvm::IRBuilder<> Builder(&func->getEntryBlock(), func->getEntryBlock().begin());

    return Builder.CreateAlloca(type, 0, name.c_str());
}

llvm::Type* Core::getTypeByName(const string& name)
{
    if (name == "double") {
        return llvm::Type::getDoubleTy(Core::ThisContext);
    } else if (name == "int") {
        return llvm::Type::getInt32Ty(Core::ThisContext);
    }

    cerr << "invalid type name: " << name << endl;
    exit(-1);
    return nullptr;
}

llvm::Value* Core::getConstant(double value)
{
    return llvm::ConstantFP::get(Core::ThisContext, llvm::APFloat(value));
}
