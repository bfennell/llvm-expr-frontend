/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "scanner.h"
#include "ast.h"
#include "type_analysis.h"

using namespace std;

// --- TypeAnalysis
AST::TypeAnalysis::TypeAnalysis() : func(nullptr)
{  // cout << __PRETTY_FUNCTION__ << endl;
}

AST::TypeAnalysis::~TypeAnalysis() {}

void AST::TypeAnalysis::analyise(AST::Program* p)
{
    TypeAnalysis g;
    // cout << __PRETTY_FUNCTION__ << endl;
    p->accept(&g);
}

void AST::TypeAnalysis::visit(AST::Program* p)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    for (auto& it : p->nodes) {
        it->accept(this);
    }
}

void AST::TypeAnalysis::visit(ExprStatement* f)
{
    if (f->expr_ != nullptr) {
        f->expr_->accept(this);
    }
}

void AST::TypeAnalysis::visit(AST::IfStatement* f)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    f->cond_->accept(this);
    f->then_->accept(this);
    f->else_->accept(this);
}

void AST::TypeAnalysis::visit(AST::ForStatement* f)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    f->start->accept(this);
    f->end->accept(this);
    f->step->accept(this);
    f->body->accept(this);
}

void AST::TypeAnalysis::visit(AST::RetStatement* f)
{  // cout << __PRETTY_FUNCTION__ << endl;
    f->retval_->accept(this);
}

void AST::TypeAnalysis::visit(AST::FunctionDecl* f)
{  // cout << __PRETTY_FUNCTION__ << endl;
}

void AST::TypeAnalysis::visit(AST::Block* f)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    for (auto& a : f->body) {
        a->accept(this);
    }
}

void AST::TypeAnalysis::visit(AST::FunctionDef* f)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    func = f;  // store current function for return type analysis
    f->decl->accept(this);
    if (f->body.get() != nullptr) {
        f->body->accept(this);
    }
    func = nullptr;  // reset current function
}

void AST::TypeAnalysis::visit(AST::FunctionCall* f)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    for (auto& a : *f->args) {
        a->accept(this);
    }
}

void AST::TypeAnalysis::visit(AST::BinaryOperator* op)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    op->left_->accept(this);
    op->right_->accept(this);
}

void AST::TypeAnalysis::visit(AST::UnaryOperator* op)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    op->expr_->accept(this);
}

void AST::TypeAnalysis::visit(AST::VarDecl* v)
{  // cout << __PRETTY_FUNCTION__ << endl;
    v->init->accept(this);
}

void AST::TypeAnalysis::visit(AST::VarName* v)
{  // cout << __PRETTY_FUNCTION__ << endl;
}

void AST::TypeAnalysis::visit(AST::IConstant* c)
{  // cout << __PRETTY_FUNCTION__ << endl;
}

void AST::TypeAnalysis::visit(AST::FConstant* c)
{  // cout << __PRETTY_FUNCTION__ << endl;
}
