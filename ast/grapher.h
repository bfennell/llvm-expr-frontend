/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __GRAPHER_H__
#define __GRAPHER_H__

#include <string>
#include <vector>

#include "token.h"
#include "ast/ast.h"
#include "ast/visitor.h"

namespace AST
{
class Grapher : public Visitor
{
   public:
    Grapher();
    virtual ~Grapher();

    static void graph(Program* p);

   private:
    void visit(Program* p) override final;
    void visit(Block* f) override final;
    void visit(ExprStatement* f) override final;
    void visit(IfStatement* f) override final;
    void visit(ForStatement* f) override final;
    void visit(RetStatement* f) override final;
    void visit(FunctionDecl* f) override final;
    void visit(FunctionDef* f) override final;
    void visit(FunctionCall* f) override final;
    void visit(BinaryOperator* op) override final;
    void visit(UnaryOperator* op) override final;
    void visit(VarDecl* v) override final;
    void visit(VarName* v) override final;
    void visit(IConstant* c) override final;
    void visit(FConstant* c) override final;

    const string declare(const std::string name, const std::string label);
    void link(const std::string parent, const std::string name);
    std::vector<std::string> stack;
    int id;
};
}
#endif  // __GRAPHER_H__
