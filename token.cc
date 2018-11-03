/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <string>
#include <sstream>

#include "token.h"

using namespace std;

Token::Token() : id(TokenId_INVALID), lexeme() {}

Token::Token(const TokenId tokId, const string lex) : id(tokId), lexeme(lex) {}

Token::~Token() {}

const string Token::str()
{
    stringstream ss;
    ss << id;
    return ss.str();
}

Integer::Integer(const int v, const string s) : Token(TokenId_I_CONSTANT, s), value(v) {}

const string Integer::str()
{
    stringstream ss;
    ss << value;
    return ss.str();
}

Real::Real(const double v, const string s) : Token(TokenId_F_CONSTANT, s), value(v) {}

const string Real::str()
{
    stringstream ss;
    ss << value;
    return ss.str();
}
