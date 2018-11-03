/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */


#ifndef __SYM_TAB_H__
#define __SYM_TAB_H__

#include <string>
#include <map>

#include "ast/ast.h"

namespace SymTab
{
    typedef llvm::AllocaInst SymVariable;
typedef void* SymConstant;
typedef AST::FunctionDef SymFunction;
typedef void* SymType;

class SymbolTable
{
   public:
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymTab::SymbolTable&) = delete;
    SymbolTable(SymbolTable* parent);
    ~SymbolTable();
    void addVariable(const std::string name, SymVariable* variable);
    void addConstant(const std::string name, SymConstant* constant);
    void addFunction(const std::string name, SymFunction* function);
    void addType(const std::string name, SymType* type);

    SymConstant* findConstant(const std::string name);
    SymVariable* findVariable(const std::string name);
    SymFunction* findFunction(const std::string name);
    SymType* findType(const std::string name);

    bool exists(const std::string name);

   private:
    SymbolTable* parent;
    std::map<std::string, SymVariable*> variables;
    std::map<std::string, SymConstant*> constants;
    std::map<std::string, SymFunction*> functions;
    std::map<std::string, SymType*> types;
};

void initSymbolTable();
void closeSymbolTable();
void dumpSymbolTable();
SymbolTable* getSymbolTable();
SymbolTable* enterScope();
SymbolTable* exitScope();
}

#endif  // __SYM_TAB_H__
