/* Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "ast/grapher.h"
#include "ast/type_analysis.h"
#include "parser.h"
#include "symtab.h"
#include "core.h"

using namespace std;

int main(int argc, char** argv)
{
    bool graph = false;
    bool dump = false;
    bool exec = false;
    vector<string> params;
    string usage = "usage: expr [-graph|dump|-exec] [file]";

    Core::initCompiler();

    Parser* parser = Core::getParser();

    for (int i = 1; i < argc; ++i) {
        params.push_back(argv[i]);
    }

    if (params.size() == 0)
        parser->open();
    else if (params.size() == 1)
        parser->open(params[0]);
    else if (params.size() == 2) {
        if (params[0] == "-graph")
            graph = true;
        else if (params[0] == "-dump")
            dump = true;
        else if (params[0] == "-exec")
            exec = true;
        else {
            cerr << "unknown parameter '" << params[0] << "'" << endl;
            cerr << usage << endl;
            return -1;
        }
        parser->open(params[1]);
    } else {
        cerr << usage << endl;
        return -1;
    }

    unique_ptr<AST::Program> program = parser->parse();

    AST::TypeAnalysis::analyise(program.get());

    if (graph) {
        AST::Grapher::graph(program.get());
    } else {
        program->codegen();
        if (dump) {
            Core::getModule()->print(llvm::outs(), nullptr);
            //Core::getModule()->dump();
        } else if (exec) {
            Core::getExecEngine()->finalizeObject();
            void* FPtr =
                Core::getExecEngine()->getPointerToFunction(Core::getModule()->getFunction("main"));
            double (*FP)() = (double (*)())(intptr_t)FPtr;
            fprintf(stdout, "\n\nEvaluated to %f\n\n", FP());
        }
    }

    Core::closeCompiler();

    return 0;
}
