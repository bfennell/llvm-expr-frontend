/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>
#include <istream>
#include <map>
#include <memory>

#include "token.h"

using namespace std;

class Scanner
{
   public:
    Scanner();
    ~Scanner();

    void open();
    void open(const string filename);

    Token* get();

    int line();
    int col();

   private:
    // hide the copy constructor and assignment operator
    Scanner(const Scanner&);
    Scanner& operator=(const Scanner&);

    int lineno;
    int colno;
    map<const string, unique_ptr<Token>> tokens;
};

#endif  // __SCANNER_H__
