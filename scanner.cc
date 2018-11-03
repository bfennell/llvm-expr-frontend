/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "token.h"
#include "scanner.h"

using namespace std;

// Flex defined globals
extern FILE *yyin;
extern char *yytext;
extern int yylex(void);
extern int lex_lineno;
extern int lex_colno;

Scanner::Scanner() : lineno(1), colno(1), tokens() { yyin = 0; }

Scanner::~Scanner()
{
    if (yyin) {
        fclose(yyin);
    }
    yyin = 0;
}

void Scanner::open() { yyin = stdin; }

void Scanner::open(const string filename)
{
    yyin = fopen(filename.c_str(), "r");

    assert(0 != yyin);
}

Token *Scanner::get()
{
    TokenId id = TokenId_END;
    int c;

    if (0 != (c = yylex())) {
        id = static_cast<TokenId>(c);
    }

    string lexeme(yytext);

    // update line number, column number
    lineno = lex_lineno;
    colno = lex_colno;

    if (id == TokenId_I_CONSTANT) {
        if (tokens.count(lexeme)) {
            return tokens[lexeme].get();
        } else {
            int value = strtol(lexeme.c_str(), 0, 10);
            tokens[lexeme] = make_unique<Integer>(value, lexeme);
            return tokens[lexeme].get();
        }
    } else if (id == TokenId_F_CONSTANT) {
        if (tokens.count(lexeme)) {
            return tokens[lexeme].get();
        } else {
            double value = atof(lexeme.c_str());
            tokens[lexeme] = make_unique<Real>(value, lexeme);
            return tokens[lexeme].get();
        }
    } else if ((id >= TokenId_RETTYPE) && (id <= TokenId_NAME)) {
        if (tokens.count(lexeme)) {
            return tokens[lexeme].get();
        } else {
            tokens[lexeme] = make_unique<Token>(id, lexeme);
            return tokens[lexeme].get();
        }
    } else if (id == TokenId_STRING_LITERAL) {
        assert(false);
    } else if (id == TokenId_END) {
        lexeme = "[eof]";
        tokens[lexeme] = make_unique<Token>(id, lexeme);
        return tokens[lexeme].get();
    } else {
        cout << "error: unkown token in input: " << id << endl;
        assert(false);
    }

    return nullptr;
}

int Scanner::line() { return lineno; }
int Scanner::col() { return colno; }
