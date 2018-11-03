/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <memory>
#include <string>
#include <iostream>
#include <cstdlib>

#include "ast/ast.h"
#include "parser.h"
#include "symtab.h"
#include "core.h"

using namespace std;

static constexpr int ASSO_LEFT_RIGHT = 0;

// ToDo: currently no right-to-left operators
// ToDo: static constexpr int ASSO_RIGHT_LEFT = 1;

static map<TokenId, pair<int, int>> opPrecedence = {
    // 15,14
    // 13
    {TokenId_MULT, {13, ASSO_LEFT_RIGHT}},
    {TokenId_DIVIDE, {13, ASSO_LEFT_RIGHT}},
    // 12
    {TokenId_PLUS, {12, ASSO_LEFT_RIGHT}},
    {TokenId_MINUS, {12, ASSO_LEFT_RIGHT}},
    // 11
    // 10
    {TokenId_LT, {10, ASSO_LEFT_RIGHT}},
    {TokenId_GT, {10, ASSO_LEFT_RIGHT}}
    // 9..1
};

static int getOpPrecedence(const TokenId id)
{
    if (opPrecedence.count(id) == 0) {
        return -1;
    }
    return opPrecedence[id].first;
}

// ToDo: currently no right-to-left operators
// ToDo:
// ToDo:: static int getOpAssociativity (const TokenId id)
// ToDo:: {
// ToDo::     if (opPrecedence.count(id) == 0) {
// ToDo::         return -1;
// ToDo::     }
// ToDo::     return opPrecedence[id].second;
// ToDo:: }

bool Parser::accept(const int tokenId)
{
    if (look->id == tokenId) {
        next();
        return true;
    }
    return false;
}

void Parser::expect(const int tokenId)
{
    if (accept(tokenId)) return;

    cerr << "error: unexpected symbol '" << look->lexeme << "' " << scanner.line() << ":"
         << scanner.col() << endl;

    return;
}

unique_ptr<AST::Expression> Parser::function_call(const string func_name)
{
    auto args = std::make_unique<vector<unique_ptr<AST::Expression>>>();

    SymTab::SymbolTable* symbolTable = SymTab::getSymbolTable();

    AST::FunctionDef* def = symbolTable->findFunction(func_name);
    if (def == nullptr) {
        cerr << "Error: undefined function '" << func_name << "'" << endl;
        assert(false);
    }
    expect(TokenId_OBRACE);

    do {
        args->push_back(expr());
    } while (accept(TokenId_COMMA));
    expect(TokenId_CBRACE);

    if (def->decl->args.size() != args->size()) {
        cerr << "Error: wrong number of parameters for function '" << def->decl->name << "'"
             << endl;
        assert(false);
    }

    unique_ptr<AST::Expression> expression = std::make_unique<AST::FunctionCall>(def, move(args));
    expression->flags |= flags;
    return expression;
}

unique_ptr<AST::Expression> Parser::prim()
{
    Token* current = look;
    unique_ptr<AST::Expression> expression;

    if (accept(TokenId_I_CONSTANT)) {
        expression = std::make_unique<AST::IConstant>(*static_cast<Integer*>(current));
    } else if (accept(TokenId_F_CONSTANT)) {
        expression = std::make_unique<AST::FConstant>(*static_cast<Real*>(current));
    } else if (look->id == TokenId_NAME) {
        accept(TokenId_NAME);
        if (look->id == TokenId_OBRACE) {
            expression = function_call(current->lexeme);
        } else {
            string name = current->lexeme;
            Token* current = look;

            if (accept(TokenId_ASSIGN)) {
                expression = std::make_unique<AST::BinaryOperator>(
                    *current, std::make_unique<AST::VarName>(name), expr());
            } else {
                expression = std::make_unique<AST::VarName>(name);
            }
        }
    } else if (accept(TokenId_MINUS)) {
        expression = std::make_unique<AST::UnaryOperator>(*current, prim());
    } else if (accept(TokenId_OBRACE)) {
        expression = expr();
        expect(TokenId_CBRACE);
    } else {
        cerr << "error: syntax error " << scanner.line() << ":" << scanner.col() << endl;
        exit(-1);
    }

    expression->flags |= flags;
    return expression;
}

//--------------------------------------------------------------------------------------------------
// Operator-precedence parser (https://en.wikipedia.org/wiki/Operator-precedence_parser)
// ==========================
//
// parse_expression ()
//    return parse_expression_1 (parse_primary (), 0)
//
// parse_expression_1 (lhs, min_precedence)
//     lookahead := peek next token
//     while lookahead is a binary operator whose precedence is >= min_precedence
//         op := lookahead
//         advance to next token
//         rhs := parse_primary ()
//         lookahead := peek next token
//         while lookahead is a binary operator whose precedence is greater
//                  than op's, or a right-associative operator
//                  whose precedence is equal to op's
//             rhs := parse_expression_1 (rhs, lookahead's precedence)
//             lookahead := peek next token
//         lhs := the result of applying op with operands lhs and rhs
//     return lhs
unique_ptr<AST::Expression> Parser::binOpRHS(const int minPrec, unique_ptr<AST::Expression> lhs)
{
    unique_ptr<AST::Expression> rhs;

    while (getOpPrecedence(look->id) >= minPrec) {
        Token* op = look;
        next();
        rhs = prim();
        while (getOpPrecedence(look->id) > getOpPrecedence(op->id)) {
            rhs = binOpRHS(getOpPrecedence(look->id), move(rhs));
        }
        lhs = std::make_unique<AST::BinaryOperator>(*op, move(lhs), move(rhs));
        lhs->flags |= flags;
    }
    return lhs;
}

unique_ptr<AST::Expression> Parser::expr()
{
    unique_ptr<AST::Expression> expression = binOpRHS(0 /*minPrec*/, prim());
    expression->flags |= flags;
    return expression;
}

unique_ptr<AST::ExprStatement> Parser::exprstmt()
{
    // expresion..... ';'
    unique_ptr<AST::Expression> expression = expr();
    expect(TokenId_SEMICOLON);
    unique_ptr<AST::ExprStatement> stmt = make_unique<AST::ExprStatement>(move(expression));
    stmt->flags |= flags;
    return stmt;
}

unique_ptr<AST::IfStatement> Parser::ifstmt()
{
    unique_ptr<AST::IfStatement> ife;

    // if ( cond )
    expect(TokenId_IF);
    expect(TokenId_OBRACE);
    ife = std::make_unique<AST::IfStatement>(expr());
    ife->flags |= flags;
    expect(TokenId_CBRACE);

    // ...  { expr... }
    ife->addThen(block());

    // ... else { expr... }
    if (look->id == TokenId_ELSE) {
        expect(TokenId_ELSE);
        ife->addElse(block());
    }

    return ife;
}

unique_ptr<AST::ForStatement> Parser::forstmt()
{
    // for (expr; expr; expr;) { expr... }
    expect(TokenId_FOR);
    expect(TokenId_OBRACE);
    string loopVar(look->lexeme);
    expect(TokenId_NAME);
    expect(TokenId_ASSIGN);
    unique_ptr<AST::Expression> start = expr();
    expect(TokenId_SEMICOLON);
    unique_ptr<AST::Expression> end = expr();
    expect(TokenId_SEMICOLON);
    unique_ptr<AST::Expression> step = expr();
    expect(TokenId_CBRACE);

    unique_ptr<AST::ForStatement> forexpr =
        std::make_unique<AST::ForStatement>(move(start), move(end), move(step), loopVar);

    forexpr->flags |= flags;

    forexpr->addBody(block());

    return forexpr;
}

unique_ptr<AST::RetStatement> Parser::retstmt()
{
    unique_ptr<AST::RetStatement> retstmt;

    // return [expr] ;
    expect(TokenId_RETURN);
    if (look->id == TokenId_SEMICOLON) {
        retstmt = std::make_unique<AST::RetStatement>(nullptr);
        expect(TokenId_SEMICOLON);
    } else {
        retstmt = std::make_unique<AST::RetStatement>(expr());
        expect(TokenId_SEMICOLON);
    }
    retstmt->flags |= flags;
    return retstmt;
}

unique_ptr<AST::VarDecl> Parser::var_decl()
{
    // "var" IDENTIFIER ":" TYPE "=" expr ";"
    expect(TokenId_VAR);

    string name(look->lexeme);
    expect(TokenId_NAME);

    expect(TokenId_COLON);

    string typeName(look->lexeme);
    expect(TokenId_NAME);

    expect(TokenId_ASSIGN);
    unique_ptr<AST::Expression> init = expr();
    expect(TokenId_SEMICOLON);

    unique_ptr<AST::VarDecl> decl =
        std::make_unique<AST::VarDecl>(name, Core::getTypeByName(typeName), move(init));
    decl->flags |= flags;
    return decl;
}

unique_ptr<AST::FunctionDecl> Parser::function_decl()
{
    vector<string> args;

    expect(TokenId_FUNCTION);
    string name(look->lexeme);
    expect(TokenId_NAME);
    expect(TokenId_OBRACE);

    if (!accept(TokenId_CBRACE)) {
        do {
            args.push_back(look->lexeme);
            expect(TokenId_NAME);
        } while (accept(TokenId_COMMA));
        expect(TokenId_CBRACE);
    }

    expect(TokenId_RETTYPE);
    string retType(look->lexeme);
    expect(TokenId_NAME);

    unique_ptr<AST::FunctionDecl> decl =
        std::make_unique<AST::FunctionDecl>(name, args, Core::getTypeByName(retType));
    decl->flags |= flags;
    return decl;
}

unique_ptr<AST::Block> Parser::block()
{
    unique_ptr<AST::Block> body = std::make_unique<AST::Block>();

    // clear unreachability on entering blocks
    flags &= ~AST::FLAG_UNREACHABLE;

    expect(TokenId_OCBRACE);  // '{'
    while (look->id != TokenId_CCBRACE && look->id != TokenId_END) {
        switch (look->id) {
            case TokenId_VAR: {
                body->add(var_decl());
                break;
            }
            case TokenId_IF: {
                body->add(ifstmt());
                break;
            }
            case TokenId_FOR: {
                body->add(forstmt());
                break;
            }
            case TokenId_RETURN: {
                body->add(retstmt());
                flags |= AST::FLAG_UNREACHABLE;
                break;
            }
            default: {
                body->add(exprstmt());
            }
        }
    }
    expect(TokenId_CCBRACE);  // '}'

    // remember if this block contains unreachable statements (due to return)
    body->flags |= flags;

    flags &= ~AST::FLAG_UNREACHABLE;
    return body;
}

unique_ptr<AST::Program> Parser::program()
{
    unique_ptr<AST::Program> program = std::make_unique<AST::Program>();

    while (look->id != TokenId_END) {
        switch (look->id) {
            case TokenId_FUNCTION: {
                unique_ptr<AST::FunctionDef> def =
                    std::make_unique<AST::FunctionDef>(function_decl());
                def->setBody(block());
                program->add(move(def));
                break;
            }
            case TokenId_EXTERN: {
                expect(TokenId_EXTERN);
                program->add(std::make_unique<AST::FunctionDef>(function_decl()));
                expect(TokenId_SEMICOLON);
                break;
            }
            case TokenId_VAR: {
                program->add(unique_ptr<AST::Base>(var_decl()));
                break;
            }
            default: {
                program->add(unique_ptr<AST::Base>(expr()));
                expect(TokenId_SEMICOLON);
            }
        }
    }

    return program;
}

void Parser::open()
{
    scanner.open();
    look = scanner.get();
}

void Parser::open(const string filename)
{
    scanner.open(filename);
    look = scanner.get();
}

unique_ptr<AST::Program> Parser::parse()
{
    unique_ptr<AST::Program> prog = program();

    expect(TokenId_END);

    return prog;
}

void Parser::next() { look = scanner.get(); }
