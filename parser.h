/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include <memory>

#include "ast/ast.h"
#include "scanner.h"

class Parser
{
   public:
    Parser() : look(0), scanner(), flags(0) {}

    void open();
    void open(const std::string filename);
    unique_ptr<AST::Program> parse();

   private:
    // hide the copy constructor and assignment operator
    Parser(const Parser&);
    Parser& operator=(const Parser&);

    bool accept(const int tokenId);
    void expect(const int tokenId);
    void next();

    std::unique_ptr<AST::Expression> prim();
    std::unique_ptr<AST::Expression> binOpRHS(const int minPrec,
                                              std::unique_ptr<AST::Expression> lhs);
    std::unique_ptr<AST::Expression> expr();
    std::unique_ptr<AST::Expression> function_call(const std::string func_name);
    std::unique_ptr<AST::ExprStatement> exprstmt();
    std::unique_ptr<AST::IfStatement> ifstmt();
    std::unique_ptr<AST::ForStatement> forstmt();
    std::unique_ptr<AST::RetStatement> retstmt();
    std::unique_ptr<AST::VarDecl> var_decl();
    std::unique_ptr<AST::FunctionDecl> function_decl();
    std::unique_ptr<AST::Block> block();
    std::unique_ptr<AST::Program> program();

    Token* look;
    Scanner scanner;
    uint32_t flags;
};

#endif  // __PARSER_H__
