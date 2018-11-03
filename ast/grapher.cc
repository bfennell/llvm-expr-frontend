/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "scanner.h"
#include "ast/ast.h"
#include "grapher.h"

using namespace std;

// --- Grapher
AST::Grapher::Grapher() : stack(), id(0) { stack.push_back("Program"); }

AST::Grapher::~Grapher() {}

void AST::Grapher::graph(AST::Program* p)
{
    Grapher g;
    cout << "digraph graphname {" << endl;
    p->accept(&g);
    cout << "}" << endl;
}

const string AST::Grapher::declare(const string name, const string label)
{
    stringstream ss;

    ss << name << id;
    cout << ss.str() << " [label=\"" << label << "\"];\n";
    id++;

    return ss.str();
}

void AST::Grapher::link(const string parent, const string name)
{
    cout << parent << " -> " << name << ";" << endl;
}

void AST::Grapher::visit(AST::Program* p)
{
    string name = "Statement";
    string label = "Statement";

    for (auto& it : p->nodes) {
        // for ( vector<AST::ASTBase*>::iterator it = p->nodes.begin(); it != p->nodes.end(); ++it )
        // {
        name = declare(name, label);
        link(stack.back(), name);
        stack.push_back(name);
        it->accept(this);
        stack.pop_back();
    }
}

void AST::Grapher::visit(ExprStatement* f)
{
    f->expr_->accept(this);
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::IfStatement* f)
{
    f->cond_->accept(this);
    f->then_->accept(this);
    f->else_->accept(this);
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::ForStatement* f)
{
    f->start->accept(this);
    f->end->accept(this);
    f->step->accept(this);
    f->body->accept(this);
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::RetStatement* f)
{
    f->retval_->accept(this);
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::FunctionDecl* f)
{
    string name = "FunctionDecl";
    string label = name + "\\n" + "'(";

    for (auto s : f->args) {
        label += s + ",";
    }
    label += ")'";

    link(stack.back(), declare(name, label));
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::Block* f)
{
    string name = "FunctionBody";
    string label = name + "\\n" + "'" + "<>" + "'";

    name = declare(name, label);
    link(stack.back(), name);
    stack.push_back(name);

    for (auto& a : f->body) {
        a->accept(this);
    }

    stack.pop_back();
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::FunctionDef* f)
{
    string name = "FunctionDef";
    string label = name + "\\n" + "'" + f->decl->name + "'";

    name = declare(name, label);
    link(stack.back(), name);
    stack.push_back(name);
    f->decl->accept(this);
    if (f->body.get() != nullptr) {
        f->body->accept(this);
    }
    stack.pop_back();
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::FunctionCall* f)
{
    string name = "FunctionCall";
    string label = name + "\\n" + "'" + f->def->decl->name + "'";

    name = declare(name, label);
    link(stack.back(), name);
    stack.push_back(name);
    for (auto& a : *f->args) {
        a->accept(this);
    }
    stack.pop_back();
    if (f->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::BinaryOperator* op)
{
    string name = "BinaryOperator";
    string label = name + "\\n" + "'" + op->op_.lexeme + "'";

    name = declare(name, label);
    link(stack.back(), name);
    stack.push_back(name);
    op->left_->accept(this);
    op->right_->accept(this);
    stack.pop_back();
    if (op->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::UnaryOperator* op)
{
    string name = "UnaryOperator";
    string label = name + "\\n" + "'" + op->op_.lexeme + "'";

    name = declare(name, label);
    link(stack.back(), name);
    stack.push_back(name);
    op->expr_->accept(this);
    stack.pop_back();
    if (op->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::VarDecl* v)
{
    v->init->accept(this);
    if (v->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::VarName* v)
{
    string name = "VarName";
    string label = name + "\\n" + "'" + v->name_ + "'";

    link(stack.back(), declare(name, label));
    if (v->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::IConstant* c)
{
    string name = "IConstant";
    string label = name + "\\n" + "'" + c->number_.lexeme + "'";

    link(stack.back(), declare(name, label));
    if (c->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}

void AST::Grapher::visit(AST::FConstant* c)
{
    string name = "FConstant";
    string label = name + "\\n" + "'" + c->number_.lexeme + "'";

    link(stack.back(), declare(name, label));
    if (c->flags & AST::FLAG_UNREACHABLE) {
        cout << "//" << __PRETTY_FUNCTION__ << "() unreachable" << endl;
    }
}
