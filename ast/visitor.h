/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __VISITOR_H__
#define __VISITOR_H__

namespace AST
{
struct Program;
struct Block;
struct ExprStatement;
struct IfStatement;
struct ForStatement;
struct RetStatement;
struct FunctionDecl;
struct FunctionDef;
struct FunctionCall;
struct BinaryOperator;
struct UnaryOperator;
struct VarDecl;
struct VarName;
struct IConstant;
struct FConstant;

struct Visitor {
    virtual ~Visitor(){};
    virtual void visit(Program* p) = 0;
    virtual void visit(Block* b) = 0;
    virtual void visit(ExprStatement* f) = 0;
    virtual void visit(IfStatement* f) = 0;
    virtual void visit(ForStatement* f) = 0;
    virtual void visit(RetStatement* f) = 0;
    virtual void visit(FunctionDecl* f) = 0;
    virtual void visit(FunctionDef* f) = 0;
    virtual void visit(FunctionCall* f) = 0;
    virtual void visit(BinaryOperator* op) = 0;
    virtual void visit(UnaryOperator* op) = 0;
    virtual void visit(VarDecl* v) = 0;
    virtual void visit(VarName* v) = 0;
    virtual void visit(IConstant* c) = 0;
    virtual void visit(FConstant* c) = 0;
};
}

#endif  // __VISITOR_H__
