CC = g++
CFLAGS = -Wall --std=c++11
BUILDDIR = ../obj/codegenerator/

LEXER_C = assignReg.cpp backwardscan.cpp chooseInstr.cpp Codegenerator.cpp
HEADER_PATH = ../include
LEXER_H = $(HEADER_PATH)/Lexer.h  $(HEADER_PATH)/Tag.h $(HEADER_PATH)/Token.h $(HEADER_PATH)/Type.h
TEST_O = ../obj/inter/*.o ../obj/symboltable/PAF.o ../obj/lexer/*.o ../obj/inter/*.o ../obj/parser/*.o ../obj/optimizer/*.o
TEST_O = ../obj/inter/Expr.o ../obj/inter/Stmt.o ../obj/symboltable/PAF.o ../obj/lexer/Lexer.o ../obj/lexer/Type.o ../obj/inter/Quadruple.o ../obj/parser/*.o ../obj/optimizer/*.o
LEXER_TEST = ../compiler.cpp

OBJS = $(patsubst %.cpp,$(BUILDDIR)%.o,$(LEXER_C))

$(BUILDDIR)%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@
all: Makefile $(OBJS)

test: $(OBJS)
	$(CC) $(CFLAGS) $(LEXER_TEST) $(OBJS) $(TEST_O)  -o ../pl0-compiler
clean:
	rm -f $(OBJS) *~ *.o  ../pl0-compiler
depend: $(LEXER_C)
	makedepend $(LEXER_H) $^
