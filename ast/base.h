/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#ifndef __BASE_H__
#define __BASE_H__

#include <cstdint>

#include "llvm.h"
#include "visitor.h"

namespace AST
{
enum class NodeType {
    NONE,
    BINOP,
    BLOCK,
    FCONST,
    FUNCCALL,
    FUNCDECL,
    FUNCDEF,
    ICONST,
    EXPRSTMT,
    IFSTMT,
    FORSTMT,
    RETSTMT,
    UOP,
    VARDECL,
    VARNAME
};

// Node flags
constexpr uint32_t FLAG_UNREACHABLE = (1 << 0);

struct Base {
    Base() : nodeType(NodeType::NONE), flags(0), value(nullptr) {}
    virtual ~Base(){};
    virtual void accept(Visitor* visitor) = 0;
    virtual void codegen() = 0;
    bool reachable() const { return (flags & FLAG_UNREACHABLE) == 0; }
    // delete defaults
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;

    NodeType nodeType;
    uint32_t flags;
    llvm::Value* value;
};
}

#endif  // __BASE_H__
