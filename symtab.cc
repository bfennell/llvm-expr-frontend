/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include "symtab.h"

#include <iostream>
#include <map>
#include <stack>
#include <cassert>

using namespace std;

static stack<unique_ptr<SymTab::SymbolTable>> symtab_stack;

SymTab::SymbolTable::SymbolTable(SymTab::SymbolTable* parent)
    : parent(parent), variables(), constants(), functions(), types()
{
}

SymTab::SymbolTable::~SymbolTable() {}

void SymTab::SymbolTable::addVariable(const string name, SymTab::SymVariable* variable)
{
    if (variables.count(name) == 0) {
        variables[name] = variable;
    } else {
        cerr << "Error - redefinition of variable '" << name << "'" << endl;
    }
}

void SymTab::SymbolTable::addConstant(const string name, SymTab::SymConstant* constant)
{
    if (constants.count(name) == 0) {
        constants[name] = constant;
    } else {
        cerr << "Error - redefinition of constant '" << name << "'" << endl;
    }
}

void SymTab::SymbolTable::addFunction(const string name, SymTab::SymFunction* function)
{
    if (functions.count(name) == 0) {
        functions[name] = function;
    } else {
        cerr << "Error - redefinition of function '" << name << "'" << endl;
    }
}

void SymTab::SymbolTable::addType(const string name, SymTab::SymType* type)
{
    if (types.count(name) == 0) {
        types[name] = type;
    } else {
        cerr << "Error - redefinition of type '" << name << "'" << endl;
    }
}

SymTab::SymConstant* SymTab::SymbolTable::findConstant(const string name)
{
    SymTab::SymbolTable* table = this;

    do {
        if (table->constants.count(name) != 0) {
            return table->constants[name];
        }
        table = table->parent;
    } while (table != nullptr);

    return nullptr;
}

SymTab::SymVariable* SymTab::SymbolTable::findVariable(const string name)
{
    SymTab::SymbolTable* table = this;

    do {
        if (table->variables.count(name) != 0) {
            return table->variables[name];
        }
        table = table->parent;
    } while (table != nullptr);

    return nullptr;
}

SymTab::SymFunction* SymTab::SymbolTable::findFunction(const string name)
{
    SymTab::SymbolTable* table = this;

    do {
        if (table->functions.count(name) != 0) {
            return table->functions[name];
        }
        table = table->parent;
    } while (table != nullptr);

    return nullptr;
}

SymTab::SymType* SymTab::SymbolTable::findType(const string name)
{
    SymTab::SymbolTable* table = this;

    do {
        if (table->types.count(name) != 0) {
            return table->types[name];
        }
        table = table->parent;
    } while (table != nullptr);

    return nullptr;
}

bool SymTab::SymbolTable::exists(const string name)
{
    if (findVariable(name) != nullptr || findFunction(name) != nullptr ||
        findType(name) != nullptr || findConstant(name) != nullptr) {
        return true;
    }
    return false;
}

void SymTab::initSymbolTable() { symtab_stack.push(make_unique<SymbolTable>(nullptr)); }

void SymTab::closeSymbolTable() {}

void SymTab::dumpSymbolTable() {}

SymTab::SymbolTable* SymTab::getSymbolTable() { return symtab_stack.top().get(); }

SymTab::SymbolTable* SymTab::enterScope()
{
    symtab_stack.push(make_unique<SymbolTable>(symtab_stack.top().get()));
    return getSymbolTable();
}

SymTab::SymbolTable* SymTab::exitScope()
{
    if (symtab_stack.size() == 1) {
        cerr << "Error - cannot pop TopLevel symbol table" << endl;
        assert(false);
    }

    symtab_stack.pop();
    return getSymbolTable();
}
