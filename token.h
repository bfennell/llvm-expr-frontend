/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

#include "token.h"

using namespace std;

enum TokenId {
    TokenId_I_CONSTANT = 256,
    TokenId_F_CONSTANT,
    TokenId_STRING_LITERAL,
    TokenId_RETTYPE,
    TokenId_COLON,
    TokenId_SEMICOLON,
    TokenId_ASSIGN,
    TokenId_OBRACE,
    TokenId_CBRACE,
    TokenId_OCBRACE,
    TokenId_CCBRACE,
    TokenId_MINUS,
    TokenId_PLUS,
    TokenId_MULT,
    TokenId_DIVIDE,
    TokenId_COMMA,
    TokenId_LT,
    TokenId_GT,
    TokenId_FUNCTION,
    TokenId_IF,
    TokenId_ELSE,
    TokenId_FOR,
    TokenId_VAR,
    TokenId_EXTERN,
    TokenId_RETURN,
    TokenId_NAME,
    TokenId_END,
    // invalid value for initialisation
    TokenId_INVALID
};

class Token
{
   public:
    Token();
    Token(const TokenId tokId, const string lex);
    virtual ~Token();
    const string str();
    const TokenId id;
    const string lexeme;
};

typedef Token Operator;

class Integer : public Token
{
   public:
    Integer(const int v, const string s);
    const string str();
    const int value;
};

class Real : public Token
{
   public:
    Real(const double v, const string s);
    const string str();
    const double value;
};

#endif  // __TOKEN_H__
