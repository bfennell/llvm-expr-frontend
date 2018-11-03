/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __TYPE_ANALYSIS_H__
#define __TYPE_ANALYSIS_H__

#include <string>
#include <vector>

#include "token.h"
#include "ast.h"
#include "visitor.h"

namespace AST
{
class TypeAnalysis : public Visitor
{
public:
    static void analyise(Program* p);

private:
    TypeAnalysis();
    virtual ~TypeAnalysis();

    // delete defaults
    TypeAnalysis(const TypeAnalysis&) = delete;
    TypeAnalysis& operator=(const TypeAnalysis&) = delete;

    void visit(Program* p);
    void visit(Block* f);
    void visit(ExprStatement* f);
    void visit(IfStatement* f);
    void visit(ForStatement* f);
    void visit(RetStatement* f);
    void visit(FunctionDecl* f);
    void visit(FunctionDef* f);
    void visit(FunctionCall* f);
    void visit(BinaryOperator* op);
    void visit(UnaryOperator* op);
    void visit(VarDecl* v);
    void visit(VarName* v);
    void visit(IConstant* c);
    void visit(FConstant* c);

private:
    FunctionDef* func;
};
}
#endif  // __TYPE_ANALYSIS_H__
