# llvm-expr-frontend
Toy frontend for llvm

## Simple expression language
    program: { func_def | extern_stmt | var_stmt } | END

    func_def: func_decl block

    func_decl: 'function' IDENTIFIER '(' param_list ')' "->" (TYPE | "void")

    TYPE: "double" | "int"

    param_list: IDENTIFIER
              | IDENTIFIER ',' param_list

    block: '{' { expr ';' | if_stmt | for_stmt | var_stmt | ret_stmt | block } '}'

    if_stmt: ifthen_stmt | ifelse_stmt

    ifthen_stmt: "if" "(" expr ")" block

    ifelse_stmt: "if" "(" expr ")" block "else" "(" expr ")" block

    for_stmt: "for" "(" IDENTIFIER "=" expr ";" expr ";" expr ")" block

    extern_stmt: "extern" func_decl ";"

    var_stmt: "var" IDENTIFIER ":" TYPE ";"
            | "var" IDENTIFIER ":" TYPE "=" expr ";"

    ret_stmt: "return" ";"
            | "return" expr ";"

    func_call: IDENTIFIER '(' arg_list ')'

    arg_list: expr
            | expr ',' arg_list

    expr_stmt: expr ';'

    expr: aterm { '<' aterm | '>' aterm }

    aterm: mterm { '+' mterm | '-' mterm }

    mterm: prim { '/' prim | '*' prim }

    prim: NUMBER
         | IDENTIFIER
         | IDENTIFIER '=' expr
         | − prim
         | '(' expr ')'
         | func_call
