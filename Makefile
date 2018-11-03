# Copyright (C) 2014 Brendan Fennell <bfennell@skynet.ie> 

CXX = clang++
LINK = clang++
RM = rm -f
LEX = flex -o lex.yy.cc
CXXFLAGS = -I. -Wall -Wno-unused-command-line-argument -Wno-deprecated-register `llvm-config --ldflags` -std=c++14 -Weffc++
LDFLAGS = -Wall `llvm-config --ldflags --system-libs --libs core mcjit native` -rdynamic

#CXXFLAGS += -O0 -ggdb3
CXXFLAGS += -O3 -ggdb3

ifndef V
	QUIET_CXX      = @echo '   ' CXX $@;
	QUIET_LINK     = @echo '   ' LINK $@;
	QUIET_LEX      = @echo '   ' LEX $@;
	QUIET_RM       = @echo '   ' RM $@;
	export V
endif

OBJECTS = lex.yy.o \
	core.o \
	token.o \
	scanner.o \
	parser.o \
	driver.o \
	symtab.o \
	lib.o \
	ast/grapher.o \
	ast/type_analysis.o \
	ast/block.o \
	ast/binop.o \
	ast/fconst.o \
	ast/exprstmt.o \
	ast/ifstmt.o \
	ast/forstmt.o \
	ast/retstmt.o \
	ast/funccall.o \
	ast/funcdecl.o \
	ast/funcdef.o \
	ast/iconst.o \
	ast/program.o \
	ast/uop.o \
	ast/vardecl.o \
	ast/varname.o

.DEFAULT: expr

all: expr

expr: $(OBJECTS) scanner.h
	$(QUIET_LINK)$(CXX) $(OBJECTS) $(LDFLAGS) -o expr

$(OBJECTS): %.o: %.cc
	$(QUIET_CXX)$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.cc: expr.lxx
	$(QUIET_LEX)$(LEX) expr.lxx

.PHONY: test
test: expr
	@echo ========= compile test/fib.expr =========
	./expr -dump test/fib.expr > test/fib.s
	opt -S -O3 test/fib.s > test/fib_opt.s
	clang -x ir -c -o test/fib.o test/fib_opt.s
	clang -c -o test/lib.o test/lib.c
	clang -o test/fib test/fib.o test/lib.o -lm
	file test/fib
	@echo
	@echo ========= compile test/t3.expr =========
	./expr -dump test/t3.expr > test/t3.s
	opt -S -O3 test/t3.s > test/t3_opt.s
	clang -x ir -c -o test/t3.o test/t3_opt.s
	clang -c -o test/lib.o test/lib.c
	clang -o test/t3 test/t3.o test/lib.o -lm
	file test/t3
	@echo
	@echo ========= execute =JIT= test/t3.expr =========
	./expr -exec test/fib.expr > test/test.out
	./expr -exec test/t3.expr >> test/test.out
	@echo
	@echo ========= execute =GRAPH= test/t3.expr =========
	./expr -graph test/t3.expr > test/t3.dot && dot -Tpdf test/t3.dot -o test/t3.pdf
	cat test/t3.dot >> test/test.out
	@echo
	diff test/expected.out test/test.out
	@echo

.PHONY: check
check: expr
	make clean && scan-build make V=1 -j all
	valgrind --leak-check=full ./expr test/t3.expr

.PHONY: clean
clean:
	$(QUIET_RM)$(RM) $(OBJECTS) expr lex.yy.cc test/fib test/t3
